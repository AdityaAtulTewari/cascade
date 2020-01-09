// Copyright 2017-2019 VMware, Inc.
// SPDX-License-Identifier: BSD-2-Clause
//
// The BSD-2 license (the License) set forth below applies to all parts of the
// Cascade project.  You may not use this file except in compliance with the
// License.
//
// BSD-2 License
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "include/cascade_slave.h"
#include "cl/cl.h"
#include "gtest/gtest.h"
#include "test/harness.h"

using namespace cascade;
using namespace cascade::cl;

int main(int argc, char** argv) {
  Simple::read(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);

  CascadeSlave slave;
  slave.set_listeners("/tmp/fpga_socket", 8800);
  slave.run();

  return RUN_ALL_TESTS();
}

TEST(one_to_one, hello_1) {
  run_code("regression/remote", "share/cascade/test/regression/simple/hello_1.v", "Hello World");
}
TEST(one_to_one, pipeline_1) {
  run_code("regression/remote", "share/cascade/test/regression/simple/pipeline_1.v", "0123456789");
}
TEST(one_to_one, pipeline_2) {
  run_code("regression/remote", "share/cascade/test/regression/simple/pipeline_2.v", "0123456789");
}
TEST(one_to_one, io) {
  run_code("regression/remote", "share/cascade/test/regression/simple/io_1.v", "1234512345");
}
TEST(one_to_one, bitcoin) {
  run_code("regression/remote", "share/cascade/test/benchmark/bitcoin/run_4.v", "0000000f 00000093\n");
}
TEST(one_to_one, bubble) {
  run_code("regression/remote", "share/cascade/test/benchmark/mips32/run_bubble_128.v", "1");
}
TEST(one_to_one, regex) {
  run_code("regression/remote", "share/cascade/test/benchmark/regex/run_disjunct_1.v", "424");
}

TEST(many_to_one, bitcoin) {
  run_concurrent("regression/concurrent", "share/cascade/test/benchmark/bitcoin/run_12.v", "00001314 00001398\n", true);
}
TEST(many_to_one, regex) {
  run_concurrent("regression/concurrent", "share/cascade/test/benchmark/regex/run_disjunct_1.v", "424", true);
}
TEST(many_to_one, array) {
  run_concurrent("regression/concurrent", "share/cascade/test/benchmark/array/run_5.v", "1048577\n", true);
}
