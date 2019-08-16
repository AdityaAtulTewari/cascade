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

#ifndef CASCADE_SRC_TARGET_COMPILER_H
#define CASCADE_SRC_TARGET_COMPILER_H

#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include "common/uuid.h"
#include "runtime/runtime.h"
#include "verilog/ast/ast_fwd.h"
#include "verilog/ast/visitors/visitor.h"

namespace cascade {

class CoreCompiler;
class Engine;
class Interface;

class Compiler {
  public:
    // Constructors:
    Compiler();
    virtual ~Compiler();

    // Configuration Interface:
    //
    // These methods are not thread-safe. Invoking these methods after the
    // first invocation of compile() or to replace a previously registered
    // compiler is undefined.
    Compiler& set(const std::string& id, CoreCompiler* c);
    CoreCompiler* get(const std::string& id);

    // Compilation Interface:
    // 
    // These methods are all thread-safe.
    // 
    // Ignores md and returns an engine backed by a StubCore.  This method does
    // not take ownership of md.  This method is undefined for modules with
    // incompatible __loc annotations.
    Engine* compile(const ModuleDeclaration* md);
    // Dispatches a compile request to the CoreCompiler specified by the
    // __target annotation on md. This method takes ownership of md.
    Engine* compile(const Uuid& uuid, ModuleDeclaration* md);
    // Dispatches an abort request to any in-flight compilations for uuid.
    void abort(const Uuid& uuid);
    // Dispatches an abort request for all uuids which have been observed, and
    // prevents any other compilations from being initiated.  
    void abort_all();

    // Error Reporting Interface:
    //
    // These methods are all thread safe and report errors if any in-flight
    // compilations encountered an error.
    void error(const std::string& s);
    bool error();
    std::string what();

  protected:
    // Scheduling Interface:
    //
    // Schedules a blocking interrupt in a state safe window defined by all
    // registered compilers. This method is thread safe.
    virtual void schedule_state_safe_interrupt(Runtime::Interrupt __int) = 0;

    // Interface Compilation... Interface:
    //
    // Sanity checks the __loc annotation on a module declaration and returns
    // either the interface provided by this compiler or nullptr.
    virtual Interface* get_interface(const std::string& loc) = 0;

  private:
    // Checks whether a module is a stub: a module with no inputs or outputs,
    // and no runtime-visible side-effects.
    class StubCheck : public Visitor {
      public:
        ~StubCheck() override = default;
        bool check(const ModuleDeclaration* md);
      private:
        bool stub_;
        void visit(const InitialConstruct* ic) override;
        void visit(const FinishStatement* fs) override;
        void visit(const RestartStatement* rs) override;
        void visit(const RetargetStatement* rs) override;
        void visit(const SaveStatement* ss) override;
    };    

    // Compilers:
    std::unordered_map<std::string, CoreCompiler*> ccs_;

    // Error State:
    std::mutex lock_;
    std::string what_;

    // Abort and Shutdown state:
    bool shutdown_;
    std::set<Uuid> uuids_;
};

} // namespace cascade

#endif
