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

#ifndef CASCADE_SRC_TARGET_CORE_DE10_PASS_FINISH_MANGLE_H
#define CASCADE_SRC_TARGET_CORE_DE10_PASS_FINISH_MANGLE_H

#include "src/verilog/ast/visitors/rewriter.h"

namespace cascade {

class TaskMangle;

// Pass 4: 
//
// Now that we're done using system tasks as landmarks and we don't have to
// worry about introducing new conditionals, go ahead and replace everything
// from pass 1

class FinishMangle : public Rewriter {
  public:
    FinishMangle(TaskMangle* tm);
    ~FinishMangle() override = default;

  private:
    TaskMangle* tm_;

    Expression* rewrite(EofExpression* ee) override;
    Statement* rewrite(DisplayStatement* ds) override;
    Statement* rewrite(ErrorStatement* es) override;
    Statement* rewrite(FinishStatement* fs) override;
    Statement* rewrite(GetStatement* gs) override;
    Statement* rewrite(InfoStatement* is) override;
    Statement* rewrite(PutStatement* ps) override;
    Statement* rewrite(RestartStatement* rs) override;
    Statement* rewrite(RetargetStatement* rs) override;
    Statement* rewrite(SaveStatement* ss) override; 
    Statement* rewrite(SeekStatement* ss) override;
    Statement* rewrite(WarningStatement* ws) override;
    Statement* rewrite(WriteStatement* ws) override;
};

} // namespace cascade

#endif
