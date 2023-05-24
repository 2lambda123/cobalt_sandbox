// FIXME: XFAIL as currently it cannot be built by lit properly.
// XFAIL: *
// RUN: %clangxx_builtins %s %librt -o %t && %run %t
/* ===-- gcc_personality_test.c - Tests __gcc_personality_v0 -------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 * ===----------------------------------------------------------------------===
 */


#include <stdlib.h>
#include <stdio.h>

extern void foo_clean(void* x);
extern void bar_clean(void* x);
extern void register_foo_local(int* x);
extern void register_bar_local(int* x);
extern void done_foo();
extern void done_bar();


/*
 * foo() is called by main() in gcc_personality_test_helper.cxx.
 * done_bar() is implemented in C++ and will throw an exception.
 * main() will catch the exception and verify that the cleanup
 * routines for foo() and bar() were called by the personality
 * function.
 */

void bar() {
    int x __attribute__((cleanup(bar_clean))) = 0;
    register_bar_local(&x);
    done_bar();
}

void foo() {
    int x __attribute__((cleanup(foo_clean))) = 0;
    register_foo_local(&x);
    bar();
    done_foo();
}
