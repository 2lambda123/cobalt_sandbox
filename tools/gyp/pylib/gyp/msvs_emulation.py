# Copyright (c) 2012 Google Inc. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""
This module helps emulate Visual Studio 2008 behavior on top of other
build systems, primarily ninja.
"""

from difflib import get_close_matches
import os
import re
import subprocess
import sys

if sys.platform in ['cygwin']:
  import cygpath

import gyp.MSVSVersion

windows_quoter_regex = re.compile(r'(\\*)"')

def QuoteForRspFile(arg):
  """Quote a command line argument so that it appears as one argument when
  processed via cmd.exe and parsed by CommandLineToArgvW (as is typical for
  Windows programs)."""
  # See http://goo.gl/cuFbX and http://goo.gl/dhPnp including the comment
  # threads. This is actually the quoting rules for CommandLineToArgvW, not
  # for the shell, because the shell doesn't do anything in Windows. This
  # works more or less because most programs (including the compiler, etc.)
  # use that function to handle command line arguments.

  # For a literal quote, CommandLineToArgvW requires 2n+1 backslashes
  # preceding it, and results in n backslashes + the quote. So we substitute
  # in 2* what we match, +1 more, plus the quote.
  arg = windows_quoter_regex.sub(lambda mo: 2 * mo.group(1) + '\\"', arg)

  # %'s also need to be doubled otherwise they're interpreted as batch
  # positional arguments. Also make sure to escape the % so that they're
  # passed literally through escaping so they can be singled to just the
  # original %. Otherwise, trying to pass the literal representation that
  # looks like an environment variable to the shell (e.g. %PATH%) would fail.
  arg = arg.replace('%', '%%')

  # These commands are used in rsp files, so no escaping for the shell (via ^)
  # is necessary.

  # Finally, wrap the whole thing in quotes so that the above quote rule
  # applies and whitespace isn't a word break.
  return '"' + arg + '"'


def EncodeRspFileList(args):
  """Process a list of arguments using QuoteCmdExeArgument."""
  # Note that the first argument is assumed to be the command. Don't add
  # quotes around it because then built-ins like 'echo', etc. won't work.
  # Take care to normpath only the path in the case of 'call ../x.bat' because
  # otherwise the whole thing is incorrectly interpreted as a path and not
  # normalized correctly.
  if not args: return ''
  if args[0].startswith('call '):
    call, program = args[0].split(' ', 1)
    program = call + ' ' + os.path.normpath(program)
  else:
    program = os.path.normpath(args[0])
  return program + ' ' + ' '.join(QuoteForRspFile(arg) for arg in args[1:])


def _GenericRetrieve(root, default, path):
  """Given a list of dictionary keys |path| and a tree of dicts |root|, find
  value at path, or return |default| if any of the path doesn't exist."""
  if not root:
    return default
  if not path:
    return root
  return _GenericRetrieve(root.get(path[0]), default, path[1:])


def _AddPrefix(element, prefix):
  """Add |prefix| to |element| or each subelement if element is iterable."""
  if element is None:
    return element
  # Note, not Iterable because we don't want to handle strings like that.
  if isinstance(element, list) or isinstance(element, tuple):
    return [prefix + e for e in element]
  else:
    return prefix + element


def _CallMap(map, element):
  e = map(element)
  if e is None:
    raise Exception('Invalid element %s' % element)
  return e

def _DoRemapping(element, map):
  """If |element| then remap it through |map|. If |element| is iterable then
  each item will be remapped. Any elements not found will be removed."""
  if map is not None and element is not None:
    if not callable(map):
      map = map.get # Assume it's a dict, otherwise a callable to do the remap.
    if isinstance(element, list) or isinstance(element, tuple):
      element = [_CallMap(map, elem) for elem in element]
    else:
      element = _CallMap(map, element)

  return element


def _AppendOrReturn(append, element):
  """If |append| is None, simply return |element|. If |append| is not None,
  then add |element| to it, adding each item in |element| if it's a list or
  tuple."""
  if append is not None and element is not None:
    if isinstance(element, list) or isinstance(element, tuple):
      append.extend(element)
    else:
      append.append(element)
  else:
    return element


def _FindDirectXInstallation():
  """Try to find an installation location for the DirectX SDK. Check for the
  standard environment variable, and if that doesn't exist, try to find
  via the registry. May return None if not found in either location."""
  # Return previously calculated value, if there is one
  if hasattr(_FindDirectXInstallation, 'dxsdk_dir'):
    return _FindDirectXInstallation.dxsdk_dir

  dxsdk_dir = os.environ.get('DXSDK_DIR')
  if not dxsdk_dir:
    # Setup params to pass to and attempt to launch reg.exe.
    cmd = ['reg.exe', 'query', r'HKLM\Software\Microsoft\DirectX', '/s']
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    for line in p.communicate()[0].splitlines():
      if 'InstallPath' in line:
        dxsdk_dir = line.split('    ')[3] + "\\"

  # Cache return value
  _FindDirectXInstallation.dxsdk_dir = dxsdk_dir
  return dxsdk_dir


class MsvsSettings(object):
  """A class that understands the gyp 'msvs_...' values (especially the
  msvs_settings field). They largely correpond to the VS2008 IDE DOM. This
  class helps map those settings to command line options."""

  def __init__(self, spec, generator_flags):
    self.spec = spec
    self.vs_version = GetVSVersion(generator_flags)
    self.dxsdk_dir = _FindDirectXInstallation()

    # Try to find an installation location for the Windows DDK by checking
    # the WDK_DIR environment variable, may be None.
    self.wdk_dir = os.environ.get('WDK_DIR')

    supported_fields = [
        ('msvs_configuration_attributes', dict),
        ('msvs_settings', dict),
        ('msvs_system_include_dirs', list),
        ('msvs_disabled_warnings', list),
        ('msvs_precompiled_header', str),
        ('msvs_precompiled_source', str),
        ('msvs_configuration_platform', str),
        ('msvs_target_platform', str),
        ]
    validators = {
        'msvs_settings': self._SettingsValidator,
    }
    configs = spec['configurations']
    for field, default in supported_fields:
      setattr(self, field, {})
      for configname, config in configs.iteritems():
        getattr(self, field)[configname] = config.get(field, default())
        if field in validators:
            validators[field](configname)

    self.msvs_cygwin_dirs = spec.get('msvs_cygwin_dirs', ['.'])

  def GetVSMacroEnv(self, base_to_build=None, config=None):
    """Get a dict of variables mapping internal VS macro names to their gyp
    equivalents."""
    vs_path = self.vs_version.Path()
    # Absolute paths in cygwin the path will start with /cygdrive/c/
    # The MS compiler tools need
    # TODO: this is getting generated from the vs install path
    # need to pass this in, or fix earlier in generation
    if sys.platform == 'cygwin':
      vs_path = cygpath.to_nt(vs_path)

    target_platform = 'Win32' if self.GetArch(config) == 'x86' else 'x64'

    replacements = {
        '$(VSInstallDir)': vs_path,
        '$(VCInstallDir)': os.path.join(vs_path, 'VC') + '\\',
        '$(OutDir)\\': base_to_build + '\\' if base_to_build else '',
        '$(IntDir)': '$!INTERMEDIATE_DIR',
        '$(InputPath)': '${source}',
        '$(InputName)': '${root}',
        '$(ProjectName)': self.spec['target_name'],
        '$(PlatformName)': target_platform,
        '$(ProjectDir)\\': '',
    }

    # Chromium uses DXSDK_DIR in include/lib paths, but it may or may not be
    # set. This happens when the SDK is sync'd via src-internal, rather than
    # by typical end-user installation of the SDK. If it's not set, we don't
    # want to leave the unexpanded variable in the path, so simply strip it.
    replacements['$(DXSDK_DIR)'] = self.dxsdk_dir if self.dxsdk_dir else ''
    replacements['$(WDK_DIR)'] = self.wdk_dir if self.wdk_dir else ''
    return replacements

  def ConvertVSMacros(self, s, base_to_build=None, config=None):
    """Convert from VS macro names to something equivalent."""
    env = self.GetVSMacroEnv(base_to_build, config=config)
    return ExpandMacros(s, env)

  def AdjustLibraries(self, libraries):
    """Strip -l from library if it's specified with that."""
    return [lib[2:] if lib.startswith('-l') else lib for lib in libraries]

  def _GetAndMunge(self, field, path, default, prefix, append, map):
    """Retrieve a value from |field| at |path| or return |default|. If
    |append| is specified, and the item is found, it will be appended to that
    object instead of returned. If |map| is specified, results will be
    remapped through |map| before being returned or appended."""
    result = _GenericRetrieve(field, default, path)
    result = _DoRemapping(result, map)
    result = _AddPrefix(result, prefix)
    return _AppendOrReturn(append, result)

  class _GetWrapper(object):
    def __init__(self, parent, field, base_path, append=None):
      self.parent = parent
      self.field = field
      self.base_path = [base_path]
      self.append = append
    def __call__(self, name, map=None, prefix='', default=None):
      return self.parent._GetAndMunge(self.field, self.base_path + [name],
          default=default, prefix=prefix, append=self.append, map=map)

  def GetArch(self, config):
    """Get architecture based on msvs_configuration_platform and
    msvs_target_platform. Returns either 'x86' or 'x64'."""
    configuration_platform = self.msvs_configuration_platform.get(config, '')
    platform = self.msvs_target_platform.get(config, '')
    if not platform: # If no specific override, use the configuration's.
      platform = configuration_platform
    # Map from platform to architecture.
    return {'Win32': 'x86', 'x64': 'x64'}.get(platform, 'x86')

  def _TargetConfig(self, config):
    """Returns the target-specific configuration."""
    # On Cobalt, we're not using any suffix on config names like Win_Debug_x64.
    # Cobalt on Windows is x64 only.
    return config

  def _Setting(self, path, config,
              default=None, prefix='', append=None, map=None):
    """_GetAndMunge for msvs_settings."""
    return self._GetAndMunge(
        self.msvs_settings[config], path, default, prefix, append, map)

  def _SettingsValidator(self, configname):
    """Validate msvs_settings."""
    valid_fields = [
        'VCCLCompilerTool',
        'VCLinkerTool',
        'VCLibrarianTool',
        'VCMIDLTool',
        'VCResourceCompilerTool',
        'VCManifestTool',
    ]
    for field in self.msvs_settings[configname]:
      if field not in valid_fields:
        message = ('Invalid msvs_settings field: "%s", '
                   'config: "%s"' % (field, configname))
        close_match = get_close_matches(field, valid_fields, 1)
        if close_match:
          message += '\nDid you mean %s?' %  tuple(close_match)
        raise Exception(message)

  def _ConfigAttrib(self, path, config,
                   default=None, prefix='', append=None, map=None):
    """_GetAndMunge for msvs_configuration_attributes."""
    return self._GetAndMunge(
        self.msvs_configuration_attributes[config],
        path, default, prefix, append, map)

  def AdjustIncludeDirs(self, include_dirs, config):
    """Updates include_dirs to expand VS specific paths, and adds the system
    include dirs used for platform SDK and similar."""
    config = self._TargetConfig(config)
    includes = include_dirs + self.msvs_system_include_dirs[config]
    includes.extend(self._Setting(
      ('VCCLCompilerTool', 'AdditionalIncludeDirectories'), config, default=[]))
    return [self.ConvertVSMacros(p, config=config) for p in includes]

  def GetComputedDefines(self, config):
    """Returns the set of defines that are injected to the defines list based
    on other VS settings."""
    config = self._TargetConfig(config)
    defines = []
    if self._ConfigAttrib(['CharacterSet'], config) == '1':
      defines.extend(('_UNICODE', 'UNICODE'))
    if self._ConfigAttrib(['CharacterSet'], config) == '2':
      defines.append('_MBCS')
    defines.extend(self._Setting(
        ('VCCLCompilerTool', 'PreprocessorDefinitions'), config, default=[]))
    return defines

  def GetOutputName(self, config, expand_special):
    """Gets the explicitly overridden output name for a target or returns None
    if it's not overridden."""
    config = self._TargetConfig(config)
    type = self.spec['type']
    root = 'VCLibrarianTool' if type == 'static_library' else 'VCLinkerTool'
    # TODO(scottmg): Handle OutputDirectory without OutputFile.
    output_file = self._Setting((root, 'OutputFile'), config)
    if output_file:
      output_file = expand_special(self.ConvertVSMacros(
          output_file, config=config))
    return output_file

  def GetPDBName(self, config, expand_special):
    """Gets the explicitly overridden pdb name for a target or returns None
    if it's not overridden."""
    config = self._TargetConfig(config)
    output_file = self._Setting(('VCLinkerTool', 'ProgramDatabaseFile'), config)
    if output_file:
      output_file = expand_special(self.ConvertVSMacros(
          output_file, config=config))
    return output_file

  def GetCflags(self, config):
    """Returns the flags that need to be added to .c and .cc compilations."""
    config = self._TargetConfig(config)
    cflags = []
    cflags.extend(['/wd' + w for w in self.msvs_disabled_warnings[config]])
    cl = self._GetWrapper(self, self.msvs_settings[config],
                          'VCCLCompilerTool', append=cflags)
    cl('Optimization',
       map={'0': 'd', '1': '1', '2': '2', '3': 'x'}, prefix='/O')
    cl('InlineFunctionExpansion', prefix='/Ob')
    cl('OmitFramePointers', map={'false': '-', 'true': ''}, prefix='/Oy')
    cl('FavorSizeOrSpeed', map={'1': 't', '2': 's'}, prefix='/O')
    cl('WholeProgramOptimization', map={'true': '/GL'})
    cl('WarningLevel', prefix='/W')
    cl('WarnAsError', map={'false': '', 'true': '/WX'})
    cl('DebugInformationFormat',
        map={'1': '7', '3': 'i', '4': 'I'}, prefix='/Z')
    cl('RuntimeTypeInfo', map={'true': '/GR', 'false': '/GR-'})
    cl('EnableFunctionLevelLinking', map={'true': '/Gy', 'false': '/Gy-'})
    cl('MinimalRebuild', map={'true': '/Gm', 'false': '/Gm-'})
    cl('BufferSecurityCheck', map={'true': '/GS', 'false': '/GS-'})
    cl('BasicRuntimeChecks', map={'1': 's', '2': 'u', '3': '1'}, prefix='/RTC')
    cl('RuntimeLibrary',
        map={'0': 'T', '1': 'Td', '2': 'D', '3': 'Dd'}, prefix='/M')
    cl('ExceptionHandling', map={'1': 'sc','2': 'a'}, prefix='/EH')
    cl('EnablePREfast', map={'true': '/analyze'})
    cl('AdditionalOptions', prefix='')
    cflags.extend(['/FI' + f for f in self._Setting(
        ('VCCLCompilerTool', 'ForcedIncludeFiles'), config, default=[])])
    cflags.extend(['/Zc:' + f for f in self._Setting(
        ('VCCLCompilerTool', 'Conformance'), config, default=[])])

    # ninja handles parallelism by itself, don't have the compiler do it too.
    cflags = filter(lambda x: not x.startswith('/MP'), cflags)
    return cflags

  def GetPrecompiledHeader(self, config, gyp_to_build_path):
    """Returns an object that handles the generation of precompiled header
    build steps."""
    config = self._TargetConfig(config)
    return _PchHelper(self, config, gyp_to_build_path)

  def _GetPchFlags(self, config, extension):
    """Get the flags to be added to the cflags for precompiled header support.
    """
    config = self._TargetConfig(config)
    # The PCH is only built once by a particular source file. Usage of PCH must
    # only be for the same language (i.e. C vs. C++), so only include the pch
    # flags when the language matches.
    if self.msvs_precompiled_header[config]:
      source_ext = os.path.splitext(self.msvs_precompiled_source[config])[1]
      if _LanguageMatchesForPch(source_ext, extension):
        pch = os.path.split(self.msvs_precompiled_header[config])[1]
        return ['/Yu' + pch, '/FI' + pch, '/Fp${pchprefix}.' + pch + '.pch']
    return  []

  def UsesComponentExtensions(self, config):
    return self._Setting(
        ('VCCLCompilerTool', 'ComponentExtensions'), config, default=[])

  def GetCflagsC(self, config):
    """Returns the flags that need to be added to .c compilations."""
    config = self._TargetConfig(config)
    return self._GetPchFlags(config, '.c')

  def GetCflagsCC(self, config):
    """Returns the flags that need to be added to .cc compilations."""
    config = self._TargetConfig(config)
    ccflags = []
    cl = self._GetWrapper(self, self.msvs_settings[config],
                          'VCCLCompilerTool', append=ccflags)
    cl('ComponentExtensions', map={'true': '/ZW'})

    if self.UsesComponentExtensions(config):
      # Disable PCH for libs compiled with /ZW, even if it was requested.
      # Causes a fatal compiler error.
      return ['/TP'] + ccflags
    else:
      return ['/TP'] + self._GetPchFlags(config, '.cc') + ccflags

  def _GetAdditionalLibraryDirectories(self, root, config, gyp_to_build_path):
    """Get and normalize the list of paths in AdditionalLibraryDirectories
    setting."""
    config = self._TargetConfig(config)
    libpaths = self._Setting((root, 'AdditionalLibraryDirectories'),
                             config, default=[])
    libpaths = [os.path.normpath(
                    gyp_to_build_path(self.ConvertVSMacros(p, config=config)))
                for p in libpaths]
    return ['/LIBPATH:"' + p + '"' for p in libpaths]

  def GetLibFlags(self, config, gyp_to_build_path):
    """Returns the flags that need to be added to lib commands."""
    config = self._TargetConfig(config)
    libflags = []
    lib = self._GetWrapper(self, self.msvs_settings[config],
                          'VCLibrarianTool', append=libflags)
    libflags.extend(self._GetAdditionalLibraryDirectories(
        'VCLibrarianTool', config, gyp_to_build_path))
    lib('AdditionalOptions')
    return libflags

  def _GetDefFileAsLdflags(self, spec, ldflags, gyp_to_build_path):
    """.def files get implicitly converted to a ModuleDefinitionFile for the
    linker in the VS generator. Emulate that behaviour here."""
    def_file = ''
    if spec['type'] in ('shared_library', 'loadable_module', 'executable'):
      def_files = [s for s in spec.get('sources', []) if s.endswith('.def')]
      if len(def_files) == 1:
        ldflags.append('/DEF:"%s"' % gyp_to_build_path(def_files[0]))
      elif len(def_files) > 1:
        raise Exception("Multiple .def files")

  def GetLdflags(self, config, gyp_to_build_path, expand_special,
                 manifest_base_name, is_executable):
    """Returns the flags that need to be added to link commands, and the
    manifest files."""
    config = self._TargetConfig(config)
    ldflags = []
    ld = self._GetWrapper(self, self.msvs_settings[config],
                          'VCLinkerTool', append=ldflags)
    self._GetDefFileAsLdflags(self.spec, ldflags, gyp_to_build_path)
    ld('GenerateDebugInformation', map={'true': '/DEBUG'})
    ld('TargetMachine', map={'1': 'X86', '17': 'X64'}, prefix='/MACHINE:')
    ldflags.extend(self._GetAdditionalLibraryDirectories(
        'VCLinkerTool', config, gyp_to_build_path))
    ld('DelayLoadDLLs', prefix='/DELAYLOAD:')
    out = self.GetOutputName(config, expand_special)
    if out:
      ldflags.append('/OUT:' + out)
    pdb = self.GetPDBName(config, expand_special)
    if pdb:
      ldflags.append('/PDB:' + pdb)
    ld('AdditionalOptions', prefix='')
    ld('SubSystem', map={'1': 'CONSOLE', '2': 'WINDOWS'}, prefix='/SUBSYSTEM:')
    ld('LinkIncremental', map={'1': ':NO', '2': ''}, prefix='/INCREMENTAL')
    ld('FixedBaseAddress', map={'1': ':NO', '2': ''}, prefix='/FIXED')
    ld('RandomizedBaseAddress',
        map={'1': ':NO', '2': ''}, prefix='/DYNAMICBASE')
    ld('DataExecutionPrevention',
        map={'1': ':NO', '2': ''}, prefix='/NXCOMPAT')
    ld('OptimizeReferences', map={'1': 'NOREF', '2': 'REF'}, prefix='/OPT:')
    ld('EnableCOMDATFolding', map={'1': 'NOICF', '2': 'ICF'}, prefix='/OPT:')
    ld('LinkTimeCodeGeneration', map={'1': '/LTCG'})
    ld('IgnoreDefaultLibraryNames', prefix='/NODEFAULTLIB:')
    ld('ResourceOnlyDLL', map={'true': '/NOENTRY'})
    ld('EntryPointSymbol', prefix='/ENTRY:')
    ld('Profile', map={ 'true': '/PROFILE'})
    # TODO(scottmg): This should sort of be somewhere else (not really a flag).
    ld('AdditionalDependencies', prefix='')
    # TODO(scottmg): These too.
    if not config.startswith('XB1') and not config.startswith('XB360'):
      ldflags.extend(('kernel32.lib', 'user32.lib', 'gdi32.lib', 'winspool.lib',
          'comdlg32.lib', 'advapi32.lib', 'shell32.lib', 'ole32.lib',
          'oleaut32.lib', 'uuid.lib', 'odbc32.lib', 'DelayImp.lib'))

    if not config.startswith('XB360'):
      # If the base address is not specifically controlled, DYNAMICBASE should
      # be on by default.
      base_flags = filter(lambda x: 'DYNAMICBASE' in x or x == '/FIXED',
                          ldflags)
      if not base_flags:
        ldflags.append('/DYNAMICBASE')

      # If the NXCOMPAT flag has not been specified, default to on. Despite the
      # documentation that says this only defaults to on when the subsystem is
      # Vista or greater (which applies to the linker), the IDE defaults it on
      # unless it's explicitly off.
      if not filter(lambda x: 'NXCOMPAT' in x, ldflags):
        ldflags.append('/NXCOMPAT')

    have_def_file = filter(lambda x: x.startswith('/DEF:'), ldflags)
    manifest_flags, intermediate_manifest_file = self._GetLdManifestFlags(
        config, manifest_base_name, is_executable and not have_def_file)
    ldflags.extend(manifest_flags)
    manifest_files = self._GetAdditionalManifestFiles(config, gyp_to_build_path)
    manifest_files.append(intermediate_manifest_file)

    return ldflags, manifest_files

  def _GetLdManifestFlags(self, config, name, allow_isolation):
    """Returns the set of flags that need to be added to the link to generate
    a default manifest, as well as the name of the generated file."""
    # Add manifest flags that mirror the defaults in VS. Chromium dev builds
    # do not currently use any non-default settings, but we could parse
    # VCManifestTool blocks if Chromium or other projects need them in the
    # future. Of particular note, we do not yet support EmbedManifest because
    # it complicates incremental linking.
    output_name = name + '.intermediate.manifest'
    flags = [
      '/MANIFEST',
      '/ManifestFile:' + output_name,
      '''/MANIFESTUAC:"level='asInvoker' uiAccess='false'"'''
    ]
    if allow_isolation:
      flags.append('/ALLOWISOLATION')
    return flags, output_name

  def _GetAdditionalManifestFiles(self, config, gyp_to_build_path):
    """Gets additional manifest files that are added to the default one
    generated by the linker."""
    files = self._Setting(('VCManifestTool', 'AdditionalManifestFiles'), config,
                          default=[])
    if (self._Setting(
        ('VCManifestTool', 'EmbedManifest'), config, default='') == 'true'):
      print 'gyp/msvs_emulation.py: "EmbedManifest: true" not yet supported.'
    if isinstance(files, str):
      files = files.split(';')
    return [os.path.normpath(
                gyp_to_build_path(self.ConvertVSMacros(f, config=config)))
            for f in files]

  def IsUseLibraryDependencyInputs(self, config):
    """Returns whether the target should be linked via Use Library Dependency
    Inputs (using component .objs of a given .lib)."""
    config = self._TargetConfig(config)
    uldi = self._Setting(('VCLinkerTool', 'UseLibraryDependencyInputs'), config)
    return uldi == 'true'

  def GetRcflags(self, config, gyp_to_ninja_path):
    """Returns the flags that need to be added to invocations of the resource
    compiler."""
    config = self._TargetConfig(config)
    rcflags = []
    rc = self._GetWrapper(self, self.msvs_settings[config],
        'VCResourceCompilerTool', append=rcflags)
    rc('AdditionalIncludeDirectories', map=gyp_to_ninja_path, prefix='/I')
    rcflags.append('/I' + gyp_to_ninja_path('.'))
    rc('PreprocessorDefinitions', prefix='/d')
    # /l arg must be in hex without leading '0x'
    rc('Culture', prefix='/l', map=lambda x: hex(int(x))[2:])
    return rcflags

  def BuildCygwinBashCommandLine(self, args, path_to_base):
    """Build a command line that runs args via cygwin bash. We assume that all
    incoming paths are in Windows normpath'd form, so they need to be
    converted to posix style for the part of the command line that's passed to
    bash. We also have to do some Visual Studio macro emulation here because
    various rules use magic VS names for things. Also note that rules that
    contain ninja variables cannot be fixed here (for example ${source}), so
    the outer generator needs to make sure that the paths that are written out
    are in posix style, if the command line will be used here."""
    cygwin_dir = os.path.normpath(
        os.path.join(path_to_base, self.msvs_cygwin_dirs[0]))
    cd = ('cd %s' % path_to_base).replace('\\', '/')
    args = [a.replace('\\', '/').replace('"', '\\"') for a in args]
    args = ["'%s'" % a.replace("'", "'\\''") for a in args]
    bash_cmd = ' '.join(args)
    cmd = (
        'call "%s\\setup_env.bat" && set CYGWIN=nontsec && ' % cygwin_dir +
        'bash -c "%s ; %s"' % (cd, bash_cmd))
    return cmd

  def IsRuleRunUnderCygwin(self, rule):
    """Determine if an action should be run under cygwin. If the variable is
    unset, or set to 1 we use cygwin."""
    return int(rule.get('msvs_cygwin_shell',
                        self.spec.get('msvs_cygwin_shell', 1))) != 0

  def _HasExplicitRuleForExtension(self, spec, extension):
    """Determine if there's an explicit rule for a particular extension."""
    for rule in spec.get('rules', []):
      if rule['extension'] == extension:
        return True
    return False

  def HasExplicitIdlRules(self, spec):
    """Determine if there's an explicit rule for idl files. When there isn't we
    need to generate implicit rules to build MIDL .idl files."""
    return self._HasExplicitRuleForExtension(spec, 'idl')

  def HasExplicitAsmRules(self, spec):
    """Determine if there's an explicit rule for asm files. When there isn't we
    need to generate implicit rules to assemble .asm files."""
    return self._HasExplicitRuleForExtension(spec, 'asm')

  def GetIdlBuildData(self, source, config):
    """Determine the implicit outputs for an idl file. Returns output
    directory, outputs, and variables and flags that are required."""
    config = self._TargetConfig(config)
    midl_get = self._GetWrapper(self, self.msvs_settings[config], 'VCMIDLTool')
    def midl(name, default=None):
      return self.ConvertVSMacros(midl_get(name, default=default),
                                  config=config)
    if config.startswith('xb1'):
      tlb = ''
      header = midl('HeaderFileName', default='${root}.h')
      dlldata = ''
      iid = ''
      proxy = ''
      outdir = midl('OutputDirectory', default='')
    else:
      tlb = midl('TypeLibraryName', default='${root}.tlb')
      header = midl('HeaderFileName', default='${root}.h')
      dlldata = midl('DLLDataFileName', default='dlldata.c')
      iid = midl('InterfaceIdentifierFileName', default='${root}_i.c')
      proxy = midl('ProxyFileName', default='${root}_p.c')
      # Note that .tlb is not included in the outputs as it is not always
      # generated depending on the content of the input idl file.
      outdir = midl('OutputDirectory', default='')
    if config.startswith('xb1'):
      output = [header]
    else:
      output = [header, dlldata, iid, proxy]
    variables = [('tlb', tlb),
                 ('h', header),
                 ('dlldata', dlldata),
                 ('iid', iid),
                 ('proxy', proxy)]
    if config.startswith('xb1'):
      metadata_dir = '"%s%s"' % ('C:\\Program Files (x86)\\Windows Kits\\10\\',
                                 'UnionMetadata')
      flags = ['/env', 'x64', '/W1', '/char', 'signed', '/enum_class',
               '/metadata_dir', metadata_dir, '/notlb', '/winrt']
    else:
      # TODO(scottmg): Are there configuration settings to set these flags?
      flags = ['/char', 'signed', '/env', 'win32', '/Oicf']
    return outdir, output, variables, flags


def _LanguageMatchesForPch(source_ext, pch_source_ext):
  c_exts = ('.c',)
  cc_exts = ('.cc', '.cxx', '.cpp')
  return ((source_ext in c_exts and pch_source_ext in c_exts) or
          (source_ext in cc_exts and pch_source_ext in cc_exts))


class PrecompiledHeader(object):
  """Helper to generate dependencies and build rules to handle generation of
  precompiled headers. Interface matches the GCH handler in xcode_emulation.py.
  """
  def __init__(
      self, settings, config, gyp_to_build_path, gyp_to_unique_output, obj_ext):
    self.settings = settings
    self.config = config
    pch_source = self.settings.msvs_precompiled_source[self.config]
    self.pch_source = gyp_to_build_path(pch_source)
    filename, _ = os.path.splitext(pch_source)
    self.output_obj = gyp_to_unique_output(filename + obj_ext).lower()

  def _PchHeader(self):
    """Get the header that will appear in an #include line for all source
    files."""
    return os.path.split(self.settings.msvs_precompiled_header[self.config])[1]

  def GetObjDependencies(self, sources, objs):
    """Given a list of sources files and the corresponding object files,
    returns a list of the pch files that should be depended upon. The
    additional wrapping in the return value is for interface compatability
    with make.py on Mac, and xcode_emulation.py."""
    if not self._PchHeader():
      return []
    pch_ext = os.path.splitext(self.pch_source)[1]
    for source in sources:
      if _LanguageMatchesForPch(os.path.splitext(source)[1], pch_ext):
        return [(None, None, self.output_obj)]
    return []

  def GetPchBuildCommands(self):
    """Not used on Windows as there are no additional build steps required
    (instead, existing steps are modified in GetFlagsModifications below)."""
    return []

  def GetFlagsModifications(self, input, output, implicit, command,
                            cflags_c, cflags_cc, expand_special):
    """Get the modified cflags and implicit dependencies that should be used
    for the pch compilation step."""

    if self.settings.UsesComponentExtensions(self.config):
      # No-op if component extensions are used.
      return [], output, []

    if input == self.pch_source:
      pch_output = ['/Yc' + self._PchHeader()]
      if command == 'cxx':
        return ([('cflags_cc', map(expand_special, cflags_cc + pch_output))],
                self.output_obj, [])
      elif command == 'cc':
        return ([('cflags_c', map(expand_special, cflags_c + pch_output))],
                self.output_obj, [])
    return [], output, implicit


vs_version = None
def GetVSVersion(generator_flags):
  global vs_version
  if not vs_version:
    vs_version = gyp.MSVSVersion.SelectVisualStudioVersion(
        generator_flags.get('msvs_version', 'auto'))
  return vs_version

def _GetVsvarsSetupArgs(generator_flags, arch):
  vs = GetVSVersion(generator_flags)
  return vs.SetupScript()

def ExpandMacros(string, expansions):
  """Expand $(Variable) per expansions dict. See MsvsSettings.GetVSMacroEnv
  for the canonical way to retrieve a suitable dict."""
  if '$' in string:
    for old, new in expansions.iteritems():
      assert '$(' not in new, new
      string = string.replace(old, new)
  return string

def _ExtractImportantEnvironment(output_of_set):
  """Extracts environment variables required for the toolchain to run from
  a textual dump output by the cmd.exe 'set' command."""
  envvars_to_save = (
      'durangoxdk',
      'goma_.*', # TODO(scottmg): This is ugly, but needed for goma.
      'include',
      'lib',
      'libpath',
      'path',
      'pathext',
      'systemroot',
      'temp',
      'tmp',
      'xedk',
      'cell_.*',
      'sn_.*',
      'sce_.*',
  )
  env = {}
  for line in output_of_set.splitlines():
    for envvar in envvars_to_save:
      if re.match(envvar + '=', line.lower()):
        var, setting = line.split('=', 1)
        if envvar == 'path':
          # Our own rules (for running gyp-win-tool) and other actions in
          # Chromium rely on python being in the path. Add the path to this
          # python here so that if it's not in the path when ninja is run
          # later, python will still be found.
          setting = os.path.dirname(sys.executable) + ';' + setting
        env[var.upper()] = setting
        break
  for required in ('SYSTEMROOT', 'TEMP', 'TMP'):
    if required not in env:
      raise Exception('Environment variable "%s" '
                      'required to be set to valid path' % required)
  return env

def _FormatAsEnvironmentBlock(envvar_dict):
  """Format as an 'environment block' directly suitable for CreateProcess.
  Briefly this is a list of key=value\0, terminated by an additional \0. See
  CreateProcess documentation for more details."""
  block = ''
  nul = '\0'
  for key, value in envvar_dict.iteritems():
    block += key + '=' + value + nul
  block += nul
  return block

def GenerateEnvironmentFiles(toplevel_build_dir, generator_flags, open_out):
  """It's not sufficient to have the absolute path to the compiler, linker,
  etc. on Windows, as those tools rely on .dlls being in the PATH. We also
  need to support both x86 and x64 compilers within the same build (to support
  msvs_target_platform hackery). Different architectures require a different
  compiler binary, and different supporting environment variables (INCLUDE,
  LIB, LIBPATH). So, we extract the environment here, wrap all invocations
  of compiler tools (cl, link, lib, rc, midl, etc.) via win_tool.py which
  sets up the environment, and then we do not prefix the compiler with
  an absolute path, instead preferring something like "cl.exe" in the rule
  which will then run whichever the environment setup has put in the path."""
  vs = GetVSVersion(generator_flags)
  for arch in ('x86', 'x64'):
    args = vs.SetupScript(arch)
    args.extend(('&&', 'set'))
    popen = subprocess.Popen(
        args, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    variables, _ = popen.communicate()
    env = _ExtractImportantEnvironment(variables)
    env_block = _FormatAsEnvironmentBlock(env)
    f = open_out(os.path.join(toplevel_build_dir, 'environment.' + arch), 'wb')
    f.write(env_block)
    f.close()

def GenerateXB1EnvironmentFiles(toplevel_build_dir, generator_flags, open_out):
  """It's not sufficient to have the absolute path to the compiler, linker,
  etc. on Windows, as those tools rely on .dlls being in the PATH. Different
  architectures require a different compiler binary, and different supporting
  environment variables (INCLUDE, LIB, LIBPATH). So, we extract the environment
  here, wrap all invocations of compiler tools (cl, link, lib, rc, midl, etc.)
  via win_tool.py which sets up the environment, and then we do not prefix the
  compiler with an absolute path, instead preferring something like "cl.exe" in
  the rule which will then run whichever the environment setup has put in the
  path."""
  arch = 'x64'

  # Get the dos environment via set:
  # Use cmd /c to execute under native windows command
  args  = 'set'

  popen = subprocess.Popen(
      args, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
  variables, _ = popen.communicate()
  env = _ExtractImportantEnvironment(variables)
  env_block = _FormatAsEnvironmentBlock(env)
  f = open_out(os.path.join(toplevel_build_dir, 'environment.' + arch), 'wb')
  f.write(env_block)
  f.close()

def GenerateXB360EnvironmentFiles(toplevel_build_dir, generator_flags, open_out):
  """It's not sufficient to have the absolute path to the compiler, linker,
  etc. on Windows, as those tools rely on .dlls being in the PATH. Different
  architectures require a different compiler binary, and different supporting
  environment variables (INCLUDE, LIB, LIBPATH). So, we extract the environment
  here, wrap all invocations of compiler tools (cl, link, lib, rc, midl, etc.)
  via win_tool.py which sets up the environment, and then we do not prefix the
  compiler with an absolute path, instead preferring something like "cl.exe" in
  the rule which will then run whichever the environment setup has put in the
  path."""
  # TODO: Switch to 'ppc' and hope havoc is not precipitated.
  arch = 'x86'

  vs_args = [
      os.path.join(
          os.path.dirname(__file__), '..', '..', '..', '..', '..',
          'lbshell', 'build', 'platforms', 'xboxadkvars.cmd'),
  ]
  # Using cygwin python so there is a bit of wrapping done to get the
  # dos environment via set:
  # Use cmd /c to execute under native windows command
  args_cmd  = 'cmd /c '
  # Convert the cygwin path i.e. /cygdrive/c .. to C:\ ..
  args_bat  = '\"`cygpath -d \'' + vs_args[0] + '\'` '
  # Create a list of the remaining arguments to the bat file
  args_args = ' '.join(vs_args[1:])
  args_set  = ' && set\"'

  args = args_cmd + args_bat + args_args + args_set
  popen = subprocess.Popen(
      args, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
  variables, _ = popen.communicate()
  env = _ExtractImportantEnvironment(variables)
  env_block = _FormatAsEnvironmentBlock(env)
  f = open_out(os.path.join(toplevel_build_dir, 'environment.' + arch), 'wb')
  f.write(env_block)
  f.close()

def VerifyMissingSources(sources, build_dir, generator_flags, gyp_to_ninja):
  """Emulate behavior of msvs_error_on_missing_sources present in the msvs
  generator: Check that all regular source files, i.e. not created at run time,
  exist on disk. Missing files cause needless recompilation when building via
  VS, and we want this check to match for people/bots that build using ninja,
  so they're not surprised when the VS build fails."""
  if int(generator_flags.get('msvs_error_on_missing_sources', 0)):
    no_specials = filter(lambda x: '$' not in x, sources)
    relative = [os.path.join(build_dir, gyp_to_ninja(s)) for s in no_specials]
    missing = filter(lambda x: not os.path.exists(x), relative)
    if missing:
      # They'll look like out\Release\..\..\stuff\things.cc, so normalize the
      # path for a slightly less crazy looking output.
      cleaned_up = [os.path.normpath(x) for x in missing]
      raise Exception('Missing input files:\n%s' % '\n'.join(cleaned_up))
