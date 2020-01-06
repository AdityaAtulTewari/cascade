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

#ifndef CASCADE_SRC_COMMON_SYSTEM_H
#define CASCADE_SRC_COMMON_SYSTEM_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include "codegen/cmake.h"

namespace cascade {

// This class provides a thin wrapper aorund some basic *nix functionality and
// environment-specific build-time constants.

struct System {
  // Returns the directory that cascade is installed in
  static std::string src_root();

  // Invokes the execute() command and returns its exit value
  static int execute(const std::string& cmd);
  // Forks a process and returns its pid, setting verbose to false will
  // redirect all output to /dev/null. Unlike execute(), this method will not
  // prevent sigint and sigkill from reaching the main thread
  static pid_t no_block_begin_execute(const std::string& cmd, bool verbose);
  // Blocks until a pid completes execution
  static int no_block_wait_finish(pid_t pid);
  // Convenience method, invokes no_block_begin_execute and then blocks on
  // the result.
  static int no_block_execute(const std::string& cmd, bool verbose);

  // Returns constants which were defined when cmake was invoked
  static std::string c_compiler();
  static std::string cxx_compiler();
};

#ifdef __APPLE__
inline std::string System::src_root() {
  char buffer[1024];
  uint32_t count = 1024;
  const auto res = _NSGetExecutablePath(buffer, &count);
  const auto path = (res == -1) ? "" : std::string(buffer);
  return path.substr(0, path.rfind('/')) + "/../";
}
#elif defined(__linux__)
inline std::string System::src_root() {
  char buffer[1024];
  const auto count = readlink("/proc/self/exe", buffer, 1024);
  const auto path = std::string(buffer, (count > 0) ? count : 0);
  return path.substr(0, path.rfind('/')) + "/../";
}
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__) || defined(__NetBSD__)
inline std::string System::src_root() {
  char buffer[1024];
  const auto count = readlink("/proc/curproc/file", buffer, 1024);
  const auto path = std::string(buffer, (count > 0) ? count : 0);
  return path.substr(0, path.rfind('/')) + "/../";
}
#else
#  error "Unknown OS"
#endif

inline int System::execute(const std::string& cmd) {
  return std::system(cmd.c_str());
}

inline pid_t System::no_block_begin_execute(const std::string& cmd, bool verbose) {
  const auto pid = fork();
  if (pid == 0) {
    if (!verbose) {
      fclose(stdout);
      fclose(stderr); 
    }
    return execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
  } else {
    return pid;
  }
}

inline int System::no_block_wait_finish(pid_t pid) {
  while (true) {
    int status;
    const auto w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
    if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      return WTERMSIG(status);
    } else if (WIFSTOPPED(status)) {
      return WSTOPSIG(status);
    }
  } 
}

inline int System::no_block_execute(const std::string& cmd, bool verbose) {
  return no_block_wait_finish(no_block_begin_execute(cmd, verbose));
}

inline std::string System::c_compiler() {
  return CMAKE_C_COMPILER;
}

inline std::string System::cxx_compiler() {
  return CMAKE_CXX_COMPILER;
}

} // namespace cascade

#endif
