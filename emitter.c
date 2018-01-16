#include <stdio.h>
#include <stdarg.h>

#define def_llvm_type(label) \
  const char* label = #label; \
  const char* label ## ptr = #label "*";

#define def_llvm_labeled_type(label, value) \
  const char* label = #value; \
  const char* label ## ptr = #value "*";

def_llvm_type(i8);
def_llvm_type(i32);
def_llvm_type(i64);
def_llvm_labeled_type(Void, void);

void llvm_ret(const char* ret, const char* val) {
  printf("  ret %s %s;\n", ret, val);
}

void llvm_fn(const char* kind, const char* name, const char* ret, int arity, va_list args) {
  printf("%s %s @%s(", kind, ret, name);

  for (int i = 0; i < arity; i++) {
    if (i != 0) {
      printf(", ");
    }

    printf("%s", va_arg(args, char*));
  }

  printf(")");
}

void llvm_declare(const char* name, const char* ret, int arity, ...) {
  va_list args;
  va_start(args, arity);
  llvm_fn("declare", name, ret, arity, args);
  va_end(args);
  putchar('\n');
}

void llvm_define_start(const char* name, const char* ret, int arity, ...) {
  va_list args;
  va_start(args, arity);
  printf("\n; Function definition start: %s\n", name);
  llvm_fn("define", name, ret, arity, args);
  va_end(args);
  printf(" {\n");
}

void llvm_define_close(const char* name) {
  printf("}\n; Function definition end: %s\n", name);
}

void llvm_main_start() {
  llvm_define_start("main", i32, 0);
}

void llvm_main_close() {
  llvm_define_close("main");
}
