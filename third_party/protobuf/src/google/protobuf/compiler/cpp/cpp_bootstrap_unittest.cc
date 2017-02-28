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

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.
//
// This test insures that google/protobuf/descriptor.pb.{h,cc} match exactly
// what would be generated by the protocol compiler.  These files are not
// generated automatically at build time because they are compiled into the
// protocol compiler itself.  So, if they were auto-generated, you'd have a
// chicken-and-egg problem.
//
// If this test fails, run the script
// "generate_descriptor_proto.sh" and add
// descriptor.pb.{h,cc} to your changelist.

#include <map>

#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/stubs/map_util.h>
#include <google/protobuf/stubs/stl_util.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/stubs/substitute.h>

#include <google/protobuf/testing/file.h>
#include <google/protobuf/testing/file.h>
#include <google/protobuf/testing/googletest.h>
#include <gtest/gtest.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {

namespace {

class MockErrorCollector : public MultiFileErrorCollector {
 public:
  MockErrorCollector() {}
  ~MockErrorCollector() {}

  string text_;

  // implements ErrorCollector ---------------------------------------
  void AddError(const string& filename, int line, int column,
                const string& message) {
    strings::SubstituteAndAppend(&text_, "$0:$1:$2: $3\n",
                                 filename, line, column, message);
  }
};

class MockGeneratorContext : public GeneratorContext {
 public:
  MockGeneratorContext() {}
  ~MockGeneratorContext() {
    STLDeleteValues(&files_);
  }

  void ExpectFileMatches(const string& virtual_filename,
                         const string& physical_filename) {
    string* expected_contents = FindPtrOrNull(files_, virtual_filename);
    ASSERT_TRUE(expected_contents != NULL)
      << "Generator failed to generate file: " << virtual_filename;

    string actual_contents;
    GOOGLE_CHECK_OK(
        File::GetContents(TestSourceDir() + "/" + physical_filename,
                          &actual_contents, true));
    EXPECT_TRUE(actual_contents == *expected_contents)
      << physical_filename << " needs to be regenerated.  Please run "
         "google/protobuf/compiler/release_compiler.sh and "
         "generate_descriptor_proto.sh. Then add this file "
         "to your CL.";
  }

  // implements GeneratorContext --------------------------------------

  virtual io::ZeroCopyOutputStream* Open(const string& filename) {
    string** map_slot = &files_[filename];
    delete *map_slot;
    *map_slot = new string;

    return new io::StringOutputStream(*map_slot);
  }

 private:
  map<string, string*> files_;
};

TEST(BootstrapTest, GeneratedDescriptorMatches) {
  MockErrorCollector error_collector;
  DiskSourceTree source_tree;
  source_tree.MapPath("", TestSourceDir());
  Importer importer(&source_tree, &error_collector);
  const FileDescriptor* proto_file =
    importer.Import("google/protobuf/descriptor.proto");
  const FileDescriptor* plugin_proto_file =
    importer.Import("google/protobuf/compiler/plugin.proto");
  EXPECT_EQ("", error_collector.text_);
  ASSERT_TRUE(proto_file != NULL);
  ASSERT_TRUE(plugin_proto_file != NULL);

  CppGenerator generator;
  MockGeneratorContext context;
  string error;
  string parameter = "dllexport_decl=LIBPROTOBUF_EXPORT";
  ASSERT_TRUE(generator.Generate(proto_file, parameter,
                                 &context, &error));
  parameter = "dllexport_decl=LIBPROTOC_EXPORT";
  ASSERT_TRUE(generator.Generate(plugin_proto_file, parameter,
                                 &context, &error));

  context.ExpectFileMatches("google/protobuf/descriptor.pb.h",
                            "google/protobuf/descriptor.pb.h");
  context.ExpectFileMatches("google/protobuf/descriptor.pb.cc",
                            "google/protobuf/descriptor.pb.cc");
  context.ExpectFileMatches("google/protobuf/compiler/plugin.pb.h",
                            "google/protobuf/compiler/plugin.pb.h");
  context.ExpectFileMatches("google/protobuf/compiler/plugin.pb.cc",
                            "google/protobuf/compiler/plugin.pb.cc");
}

}  // namespace

}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
