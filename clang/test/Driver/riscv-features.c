// RUN: %clang -target riscv32-unknown-elf -### %s -fsyntax-only 2>&1 | FileCheck %s
// RUN: %clang -target riscv64-unknown-elf -### %s -fsyntax-only 2>&1 | FileCheck %s

// CHECK: fno-signed-char

// RUN: %clang -target riscv32-unknown-elf -### %s 2>&1 | FileCheck %s -check-prefix=DEFAULT

// RUN: %clang -target riscv32-unknown-elf -### %s -mrelax 2>&1 | FileCheck %s -check-prefix=RELAX
// RUN: %clang -target riscv32-unknown-elf -### %s -mno-relax 2>&1 | FileCheck %s -check-prefix=NO-RELAX

// RELAX: "-target-feature" "+relax"
// NO-RELAX: "-target-feature" "-relax"
// DEFAULT: "-target-feature" "+relax"
// DEFAULT-NOT: "-target-feature" "-relax"

// RUN: %clang -target riscv32-unknown-elf -### %s -msave-restore 2>&1 | FileCheck %s -check-prefix=SAVE-RESTORE
// RUN: %clang -target riscv32-unknown-elf -### %s -mno-save-restore 2>&1 | FileCheck %s -check-prefix=NO-SAVE-RESTORE

// SAVE-RESTORE: warning: the clang compiler does not support '-msave-restore'
// NO-SAVE-RESTORE-NOT: warning: the clang compiler does not support
// DEFAULT-NOT: warning: the clang compiler does not support

// RUN: %clang -target riscv32-linux -### %s -fsyntax-only 2>&1 \
// RUN:   | FileCheck %s -check-prefix=DEFAULT-LINUX
// RUN: %clang -target riscv64-linux -### %s -fsyntax-only 2>&1 \
// RUN:   | FileCheck %s -check-prefix=DEFAULT-LINUX

// DEFAULT-LINUX: "-target-feature" "+m"
// DEFAULT-LINUX-SAME: "-target-feature" "+a"
// DEFAULT-LINUX-SAME: "-target-feature" "+f"
// DEFAULT-LINUX-SAME: "-target-feature" "+d"
// DEFAULT-LINUX-SAME: "-target-feature" "+c"
