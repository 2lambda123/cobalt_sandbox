// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package com.google.protobuf;

import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Collection;

/**
 * A partial implementation of the {@link MessageLite} interface which
 * implements as many methods of that interface as possible in terms of other
 * methods.
 *
 * @author kenton@google.com Kenton Varda
 */
public abstract class AbstractMessageLite<
    MessageType extends AbstractMessageLite<MessageType, BuilderType>,
    BuilderType extends AbstractMessageLite.Builder<MessageType, BuilderType>> 
        implements MessageLite {
  protected int memoizedHashCode = 0;
  
  @Override
  public ByteString toByteString() {
    try {
      final ByteString.CodedBuilder out =
        ByteString.newCodedBuilder(getSerializedSize());
      writeTo(out.getCodedOutput());
      return out.build();
    } catch (IOException e) {
      throw new RuntimeException(
        "Serializing to a ByteString threw an IOException (should " +
        "never happen).", e);
    }
  }

  @Override
  public byte[] toByteArray() {
    try {
      final byte[] result = new byte[getSerializedSize()];
      final CodedOutputStream output = CodedOutputStream.newInstance(result);
      writeTo(output);
      output.checkNoSpaceLeft();
      return result;
    } catch (IOException e) {
      throw new RuntimeException(
        "Serializing to a byte array threw an IOException " +
        "(should never happen).", e);
    }
  }

  @Override
  public void writeTo(final OutputStream output) throws IOException {
    final int bufferSize =
        CodedOutputStream.computePreferredBufferSize(getSerializedSize());
    final CodedOutputStream codedOutput =
        CodedOutputStream.newInstance(output, bufferSize);
    writeTo(codedOutput);
    codedOutput.flush();
  }

  @Override
  public void writeDelimitedTo(final OutputStream output) throws IOException {
    final int serialized = getSerializedSize();
    final int bufferSize = CodedOutputStream.computePreferredBufferSize(
        CodedOutputStream.computeRawVarint32Size(serialized) + serialized);
    final CodedOutputStream codedOutput =
        CodedOutputStream.newInstance(output, bufferSize);
    codedOutput.writeRawVarint32(serialized);
    writeTo(codedOutput);
    codedOutput.flush();
  }


  /**
   * Package private helper method for AbstractParser to create
   * UninitializedMessageException.
   */
  UninitializedMessageException newUninitializedMessageException() {
    return new UninitializedMessageException(this);
  }

  protected static void checkByteStringIsUtf8(ByteString byteString)
      throws IllegalArgumentException {
    if (!byteString.isValidUtf8()) {
      throw new IllegalArgumentException("Byte string is not UTF-8.");
    }
  }

  protected static <T> void addAll(final Iterable<T> values,
      final Collection<? super T> list) {
    Builder.addAll(values, list);
  }
  
  /**
   * A partial implementation of the {@link Message.Builder} interface which
   * implements as many methods of that interface as possible in terms of
   * other methods.
   */
  @SuppressWarnings("unchecked")
  public abstract static class Builder<
      MessageType extends AbstractMessageLite<MessageType, BuilderType>,
      BuilderType extends Builder<MessageType, BuilderType>>
      implements MessageLite.Builder {
    // The compiler produces an error if this is not declared explicitly.
    @Override
    public abstract BuilderType clone();

    @Override
    public BuilderType mergeFrom(final CodedInputStream input) throws IOException {
      return mergeFrom(input, ExtensionRegistryLite.getEmptyRegistry());
    }

    // Re-defined here for return type covariance.
    @Override
    public abstract BuilderType mergeFrom(
        final CodedInputStream input, final ExtensionRegistryLite extensionRegistry)
        throws IOException;

    @Override
    public BuilderType mergeFrom(final ByteString data) throws InvalidProtocolBufferException {
      try {
        final CodedInputStream input = data.newCodedInput();
        mergeFrom(input);
        input.checkLastTagWas(0);
        return (BuilderType) this;
      } catch (InvalidProtocolBufferException e) {
        throw e;
      } catch (IOException e) {
        throw new RuntimeException(
          "Reading from a ByteString threw an IOException (should " +
          "never happen).", e);
      }
    }

    @Override
    public BuilderType mergeFrom(
        final ByteString data, final ExtensionRegistryLite extensionRegistry)
        throws InvalidProtocolBufferException {
      try {
        final CodedInputStream input = data.newCodedInput();
        mergeFrom(input, extensionRegistry);
        input.checkLastTagWas(0);
        return (BuilderType) this;
      } catch (InvalidProtocolBufferException e) {
        throw e;
      } catch (IOException e) {
        throw new RuntimeException(
          "Reading from a ByteString threw an IOException (should " +
          "never happen).", e);
      }
    }

    @Override
    public BuilderType mergeFrom(final byte[] data) throws InvalidProtocolBufferException {
      return mergeFrom(data, 0, data.length);
    }

    @Override
    public BuilderType mergeFrom(final byte[] data, final int off, final int len)
        throws InvalidProtocolBufferException {
      try {
        final CodedInputStream input =
            CodedInputStream.newInstance(data, off, len);
        mergeFrom(input);
        input.checkLastTagWas(0);
        return (BuilderType) this;
      } catch (InvalidProtocolBufferException e) {
        throw e;
      } catch (IOException e) {
        throw new RuntimeException(
          "Reading from a byte array threw an IOException (should " +
          "never happen).", e);
      }
    }

    @Override
    public BuilderType mergeFrom(final byte[] data, final ExtensionRegistryLite extensionRegistry)
        throws InvalidProtocolBufferException {
      return mergeFrom(data, 0, data.length, extensionRegistry);
    }

    @Override
    public BuilderType mergeFrom(
        final byte[] data,
        final int off,
        final int len,
        final ExtensionRegistryLite extensionRegistry)
        throws InvalidProtocolBufferException {
      try {
        final CodedInputStream input =
            CodedInputStream.newInstance(data, off, len);
        mergeFrom(input, extensionRegistry);
        input.checkLastTagWas(0);
        return (BuilderType) this;
      } catch (InvalidProtocolBufferException e) {
        throw e;
      } catch (IOException e) {
        throw new RuntimeException(
          "Reading from a byte array threw an IOException (should " +
          "never happen).", e);
      }
    }

    @Override
    public BuilderType mergeFrom(final InputStream input) throws IOException {
      final CodedInputStream codedInput = CodedInputStream.newInstance(input);
      mergeFrom(codedInput);
      codedInput.checkLastTagWas(0);
      return (BuilderType) this;
    }

    @Override
    public BuilderType mergeFrom(
        final InputStream input, final ExtensionRegistryLite extensionRegistry) throws IOException {
      final CodedInputStream codedInput = CodedInputStream.newInstance(input);
      mergeFrom(codedInput, extensionRegistry);
      codedInput.checkLastTagWas(0);
      return (BuilderType) this;
    }

    /**
     * An InputStream implementations which reads from some other InputStream
     * but is limited to a particular number of bytes.  Used by
     * mergeDelimitedFrom().  This is intentionally package-private so that
     * UnknownFieldSet can share it.
     */
    static final class LimitedInputStream extends FilterInputStream {
      private int limit;

      LimitedInputStream(InputStream in, int limit) {
        super(in);
        this.limit = limit;
      }

      @Override
      public int available() throws IOException {
        return Math.min(super.available(), limit);
      }

      @Override
      public int read() throws IOException {
        if (limit <= 0) {
          return -1;
        }
        final int result = super.read();
        if (result >= 0) {
          --limit;
        }
        return result;
      }

      @Override
      public int read(final byte[] b, final int off, int len)
                      throws IOException {
        if (limit <= 0) {
          return -1;
        }
        len = Math.min(len, limit);
        final int result = super.read(b, off, len);
        if (result >= 0) {
          limit -= result;
        }
        return result;
      }

      @Override
      public long skip(final long n) throws IOException {
        final long result = super.skip(Math.min(n, limit));
        if (result >= 0) {
          limit -= result;
        }
        return result;
      }
    }

    @Override
    public boolean mergeDelimitedFrom(
        final InputStream input, final ExtensionRegistryLite extensionRegistry) throws IOException {
      final int firstByte = input.read();
      if (firstByte == -1) {
        return false;
      }
      final int size = CodedInputStream.readRawVarint32(firstByte, input);
      final InputStream limitedInput = new LimitedInputStream(input, size);
      mergeFrom(limitedInput, extensionRegistry);
      return true;
    }

    @Override
    public boolean mergeDelimitedFrom(final InputStream input) throws IOException {
      return mergeDelimitedFrom(input,
          ExtensionRegistryLite.getEmptyRegistry());
    }
    
    @Override
    @SuppressWarnings("unchecked") // isInstance takes care of this
    public BuilderType mergeFrom(final MessageLite other) {
      if (!getDefaultInstanceForType().getClass().isInstance(other)) {
        throw new IllegalArgumentException(
            "mergeFrom(MessageLite) can only merge messages of the same type.");
      }
        
      return internalMergeFrom((MessageType) other);
    }
    
    protected abstract BuilderType internalMergeFrom(MessageType message);

    /**
     * Construct an UninitializedMessageException reporting missing fields in
     * the given message.
     */
    protected static UninitializedMessageException
        newUninitializedMessageException(MessageLite message) {
      return new UninitializedMessageException(message);
    }

    /**
     * Adds the {@code values} to the {@code list}.  This is a helper method
     * used by generated code.  Users should ignore it.
     *
     * @throws NullPointerException if {@code values} or any of the elements of
     * {@code values} is null. When that happens, some elements of
     * {@code values} may have already been added to the result {@code list}.
     */
    protected static <T> void addAll(final Iterable<T> values,
                                     final Collection<? super T> list) {
      if (values == null) {
        throw new NullPointerException();
      }
      if (values instanceof LazyStringList) {
        // For StringOrByteStringLists, check the underlying elements to avoid
        // forcing conversions of ByteStrings to Strings.
        checkForNullValues(((LazyStringList) values).getUnderlyingElements());
        list.addAll((Collection<T>) values);
      } else if (values instanceof Collection) {
        checkForNullValues(values);
        list.addAll((Collection<T>) values);
      } else {
        for (final T value : values) {
          if (value == null) {
            throw new NullPointerException();
          }
          list.add(value);
        }
      }
    }

    private static void checkForNullValues(final Iterable<?> values) {
      for (final Object value : values) {
        if (value == null) {
          throw new NullPointerException();
        }
      }
    }
  }
}
