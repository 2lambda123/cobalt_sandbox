# -*- coding: utf-8 -*-
"""
    jinja2.bccache
    ~~~~~~~~~~~~~~

    This module implements the bytecode cache system Jinja is optionally
    using.  This is useful if you have very complex template situations and
    the compiliation of all those templates slow down your application too
    much.

    Situations where this is useful are often forking web applications that
    are initialized on the first request.

    :copyright: (c) 2010 by the Jinja Team.
    :license: BSD.
"""
from os import path, listdir, pardir, sep
import sys
import marshal
import tempfile
import fnmatch
from hashlib import sha1
from jinja2.utils import open_if_exists
from jinja2._compat import BytesIO, pickle, PY2, text_type

_REPOSITORY_ROOT = path.abspath(
    path.join(path.dirname(__file__), pardir, pardir))

# marshal works better on 3.x, one hack less required
if not PY2:
    marshal_dump = marshal.dump
    marshal_load = marshal.load
else:

    def marshal_dump(code, f):
        if isinstance(f, file):
            marshal.dump(code, f)
        else:
            f.write(marshal.dumps(code))

    def marshal_load(f):
        if isinstance(f, file):
            return marshal.load(f)
        return marshal.loads(f.read())


bc_version = 2

# magic version used to only change with new jinja versions.  With 2.6
# we change this to also take Python version changes into account.  The
# reason for this is that Python tends to segfault if fed earlier bytecode
# versions because someone thought it would be a good idea to reuse opcodes
# or make Python incompatible with earlier versions.
bc_magic = 'j2'.encode('ascii') + \
    pickle.dumps(bc_version, 2) + \
    pickle.dumps((sys.version_info[0] << 24) | sys.version_info[1])


class Bucket(object):
    """Buckets are used to store the bytecode for one template.  It's created
    and initialized by the bytecode cache and passed to the loading functions.

    The buckets get an internal checksum from the cache assigned and use this
    to automatically reject outdated cache material.  Individual bytecode
    cache subclasses don't have to care about cache invalidation.
    """

    def __init__(self, environment, key, checksum):
        self.environment = environment
        self.key = key
        self.checksum = checksum
        self.reset()

    def reset(self):
        """Resets the bucket (unloads the bytecode)."""
        self.code = None

    def load_bytecode(self, f):
        """Loads bytecode from a file or file like object."""
        # make sure the magic header is correct
        magic = f.read(len(bc_magic))
        if magic != bc_magic:
            self.reset()
            return
        # the source code of the file changed, we need to reload
        checksum = pickle.load(f)
        if self.checksum != checksum:
            self.reset()
            return
        self.code = marshal_load(f)

    def write_bytecode(self, f):
        """Dump the bytecode into the file or file like object passed."""
        if self.code is None:
            raise TypeError('can\'t write empty bucket')
        f.write(bc_magic)
        pickle.dump(self.checksum, f, 2)
        marshal_dump(self.code, f)

    def bytecode_from_string(self, string):
        """Load bytecode from a string."""
        self.load_bytecode(BytesIO(string))

    def bytecode_to_string(self):
        """Return the bytecode as string."""
        out = BytesIO()
        self.write_bytecode(out)
        return out.getvalue()


class BytecodeCache(object):
    """To implement your own bytecode cache you have to subclass this class
    and override :meth:`load_bytecode` and :meth:`dump_bytecode`.  Both of
    these methods are passed a :class:`~jinja2.bccache.Bucket`.

    A very basic bytecode cache that saves the bytecode on the file system::

        from os import path

        class MyCache(BytecodeCache):

            def __init__(self, directory):
                self.directory = directory

            def load_bytecode(self, bucket):
                filename = path.join(self.directory, bucket.key)
                if path.exists(filename):
                    with open(filename, 'rb') as f:
                        bucket.load_bytecode(f)

            def dump_bytecode(self, bucket):
                filename = path.join(self.directory, bucket.key)
                with open(filename, 'wb') as f:
                    bucket.write_bytecode(f)

    A more advanced version of a filesystem based bytecode cache is part of
    Jinja2.
    """

    def load_bytecode(self, bucket):
        """Subclasses have to override this method to load bytecode into a
        bucket.  If they are not able to find code in the cache for the
        bucket, it must not do anything.
        """
        raise NotImplementedError()

    def dump_bytecode(self, bucket):
        """Subclasses have to override this method to write the bytecode
        from a bucket back to the cache.  If it unable to do so it must not
        fail silently but raise an exception.
        """
        raise NotImplementedError()

    def clear(self):
        """Clears the cache.  This method is not used by Jinja2 but should be
        implemented to allow applications to clear the bytecode cache used
        by a particular environment.
        """

    def get_cache_key(self, name, filename=None):
        """Returns the unique hash key for this template name."""
        hash = sha1(name.encode('utf-8'))
        if filename is not None:
            filename = '|' + filename
            if isinstance(filename, text_type):
                filename = filename.encode('utf-8')
            hash.update(filename)
        return hash.hexdigest()

    def get_source_checksum(self, source):
        """Returns a checksum for the source."""
        return sha1(source.encode('utf-8')).hexdigest()

    def get_bucket(self, environment, name, filename, source):
        """Return a cache bucket for the given template.  All arguments are
        mandatory but filename may be `None`.
        """
        # TODO(b/263289873): To get correct GN outputs the file list of cached
        # templates should be printed to a depfile.
        # Use the path to the template relative to the project root to get a
        # consistent hash. To have hashes consistent across platforms we have to
        # replace the path separator.
        rel_path = path.relpath(filename, _REPOSITORY_ROOT)
        key = self.get_cache_key(name, rel_path.replace(sep, '/'))
        checksum = self.get_source_checksum(source)
        bucket = Bucket(environment, key, checksum)
        self.load_bytecode(bucket)
        return bucket

    def set_bucket(self, bucket):
        """Put the bucket into the cache."""
        self.dump_bytecode(bucket)


class FileSystemBytecodeCache(BytecodeCache):
    """A bytecode cache that stores bytecode on the filesystem.  It accepts
    two arguments: The directory where the cache items are stored and a
    pattern string that is used to build the filename.

    If no directory is specified the system temporary items folder is used.

    The pattern can be used to have multiple separate caches operate on the
    same directory.  The default pattern is ``'__jinja2_%s.cache'``.  ``%s``
    is replaced with the cache key.

    >>> bcc = FileSystemBytecodeCache('/tmp/jinja_cache', '%s.cache')

    This bytecode cache supports clearing of the cache using the clear method.
    """

    def __init__(self, directory=None, pattern='__jinja2_%s.cache'):
        if directory is None:
            directory = tempfile.gettempdir()
        self.directory = directory
        self.pattern = pattern

    def _get_cache_filename(self, bucket):
        return path.join(self.directory, self.pattern % bucket.key)

    def load_bytecode(self, bucket):
        f = open_if_exists(self._get_cache_filename(bucket), 'rb')
        if f is not None:
            try:
                bucket.load_bytecode(f)
            finally:
                f.close()

    def dump_bytecode(self, bucket):
        f = open(self._get_cache_filename(bucket), 'wb')
        try:
            bucket.write_bytecode(f)
        finally:
            f.close()

    def clear(self):
        # imported lazily here because google app-engine doesn't support
        # write access on the file system and the function does not exist
        # normally.
        from os import remove
        files = fnmatch.filter(listdir(self.directory), self.pattern % '*')
        for filename in files:
            try:
                remove(path.join(self.directory, filename))
            except OSError:
                pass


class MemcachedBytecodeCache(BytecodeCache):
    """This class implements a bytecode cache that uses a memcache cache for
    storing the information.  It does not enforce a specific memcache library
    (tummy's memcache or cmemcache) but will accept any class that provides
    the minimal interface required.

    Libraries compatible with this class:

    -   `werkzeug <http://werkzeug.pocoo.org/>`_.contrib.cache
    -   `python-memcached <http://www.tummy.com/Community/software/python-memcached/>`_
    -   `cmemcache <http://gijsbert.org/cmemcache/>`_

    (Unfortunately the django cache interface is not compatible because it
    does not support storing binary data, only unicode.  You can however pass
    the underlying cache client to the bytecode cache which is available
    as `django.core.cache.cache._client`.)

    The minimal interface for the client passed to the constructor is this:

    .. class:: MinimalClientInterface

        .. method:: set(key, value[, timeout])

            Stores the bytecode in the cache.  `value` is a string and
            `timeout` the timeout of the key.  If timeout is not provided
            a default timeout or no timeout should be assumed, if it's
            provided it's an integer with the number of seconds the cache
            item should exist.

        .. method:: get(key)

            Returns the value for the cache key.  If the item does not
            exist in the cache the return value must be `None`.

    The other arguments to the constructor are the prefix for all keys that
    is added before the actual cache key and the timeout for the bytecode in
    the cache system.  We recommend a high (or no) timeout.

    This bytecode cache does not support clearing of used items in the cache.
    The clear method is a no-operation function.

    .. versionadded:: 2.7
       Added support for ignoring memcache errors through the
       `ignore_memcache_errors` parameter.
    """

    def __init__(self, client, prefix='jinja2/bytecode/', timeout=None,
                 ignore_memcache_errors=True):
        self.client = client
        self.prefix = prefix
        self.timeout = timeout
        self.ignore_memcache_errors = ignore_memcache_errors

    def load_bytecode(self, bucket):
        try:
            code = self.client.get(self.prefix + bucket.key)
        except Exception:
            if not self.ignore_memcache_errors:
                raise
            code = None
        if code is not None:
            bucket.bytecode_from_string(code)

    def dump_bytecode(self, bucket):
        args = (self.prefix + bucket.key, bucket.bytecode_to_string())
        if self.timeout is not None:
            args += (self.timeout,)
        try:
            self.client.set(*args)
        except Exception:
            if not self.ignore_memcache_errors:
                raise
