//
// Copyright (c) 2016 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#include "compiler/translator/RewriteUnaryMinusOperatorFloat.h"

#include "compiler/translator/IntermNode.h"

namespace sh
{

namespace
{

class Traverser : public TIntermTraverser
{
  public:
    static void Apply(TIntermNode *root);

  private:
    Traverser();
    bool visitUnary(Visit visit, TIntermUnary *node) override;
    void nextIteration();

    bool mFound = false;
};

// static
void Traverser::Apply(TIntermNode *root)
{
    Traverser traverser;
    do
    {
        traverser.nextIteration();
        root->traverse(&traverser);
        if (traverser.mFound)
        {
            traverser.updateTree();
        }
    } while (traverser.mFound);
}

Traverser::Traverser() : TIntermTraverser(true, false, false)
{
}

void Traverser::nextIteration()
{
    mFound = false;
}

bool Traverser::visitUnary(Visit visit, TIntermUnary *node)
{
    if (mFound)
    {
        return false;
    }

    // Detect if the current operator is unary minus operator.
    if (node->getOp() != EOpNegative)
    {
        return true;
    }

    // Detect if the current operand is a float variable.
    TIntermTyped *fValue = node->getOperand();
    if (!fValue->getType().isScalarFloat())
    {
        return true;
    }

    // 0.0 - float
    TIntermTyped *zero = TIntermTyped::CreateZero(fValue->getType());
    zero->setLine(fValue->getLine());
    TIntermBinary *sub = new TIntermBinary(EOpSub, zero, fValue);
    sub->setLine(fValue->getLine());

    queueReplacement(node, sub, OriginalNode::IS_DROPPED);

    mFound = true;
    return false;
}

}  // anonymous namespace

void RewriteUnaryMinusOperatorFloat(TIntermNode *root)
{
    Traverser::Apply(root);
}

}  // namespace sh
