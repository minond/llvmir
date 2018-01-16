#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "definitions.c"
#include "environment.c"

#define def_llvm_labeled_type(label, value) \
  const char* label = #value; \
  const char* label ## ptr = #value "*";

#define def_llvm_type(label) \
  def_llvm_labeled_type(label, label);

#define EQ(lhs, rhs) \
  strcmp(lhs, rhs) == 0

def_llvm_type(i8);
def_llvm_type(i32);
def_llvm_type(i64);
def_llvm_labeled_type(Void, void);

const char* llvm_ptr(const char* type) {
  if (EQ(type, i8))  return i8ptr;
  if (EQ(type, i32)) return i32ptr;
  if (EQ(type, i64)) return i64ptr;
                     return Voidptr;
}

void llvm_comment(const char* msg) {
  printf("; %s\n", msg);
}

void llvm_ret(const char* ret, const char* val) {
  printf("ret %s %s;\n", ret, val);
}

FnPrototype* llvm_fn(const char* kind, const char* name, const char* ret, int arity, va_list args) {
  FnPrototype* fn = malloc(sizeof(FnPrototype));

  fn->arity = arity;
  fn->name = (char*) name;
  fn->ret = (char*) ret;
  fn->args = NULL;

  if (arity > 0) {
    fn->args = malloc(sizeof(char*) * arity);
  }

  printf("%s %s @%s(", kind, ret, name);

  for (int i = 0; i < arity; i++) {
    if (i != 0) {
      printf(", ");
    }

    fn->args[i] = va_arg(args, char*);
    printf("%s", fn->args[i]);
  }

  printf(")");

  return fn;
}

void llvm_declare(Environment* env, const char* name, const char* ret, int arity, ...) {
  va_list args;
  va_start(args, arity);
  FnPrototype* fn = llvm_fn("declare", name, ret, arity, args);
  va_end(args);
  putchar('\n');

  if (env == NULL) {
    llvm_fn_free(fn);
  } else {
    llvm_env_store_fn(env, fn);
  }
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

void llvm_alloc(const char* name, const char* type) {
  // %2 = alloca i32, align 4
  printf("%%%s = alloca %s\n", name, type);
}

void llvm_store(const char* name, const char* type, char* val) {
  // store i32 42, i32* %2, align 4
  printf("store %s %s, %s %%%s\n", type, val, llvm_ptr(type), name);
}

void llvm_alloc_and_store(const char* name, const char* type, char* val) {
  llvm_alloc(name, type);
  llvm_store(name, type, val);
}
