/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jsalloc.h"

#include "jscntxt.h"

using namespace js;

void *
TempAllocPolicy::onOutOfMemory(void *p, size_t nbytes)
{
    return cx_->runtime()->onOutOfMemory(p, nbytes, cx_);
}

void
TempAllocPolicy::reportAllocOverflow() const
{
    js_ReportAllocationOverflow(cx_);
}
