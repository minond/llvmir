#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "definitions.c"
#include "environment.c"

#define Str const char*

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

Str llvm_ptr(Str type) {
  if (EQ(type, i8))  return i8ptr;
  if (EQ(type, i32)) return i32ptr;
  if (EQ(type, i64)) return i64ptr;
                     return Voidptr;
}

void llvm_emit_comment(Str msg) {
  printf("; %s\n", msg);
}

void llvm_emit_ret(Str ret, Str val) {
  printf("ret %s %s;\n", ret, val);
}

FnPrototype* llvm_emit_fn(Str kind, Str name, Str ret, int arity, va_list args) {
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

void llvm_emit_declare(Environment* env, Str name, Str ret, int arity, ...) {
  va_list args;
  va_start(args, arity);
  FnPrototype* fn = llvm_emit_fn("declare", name, ret, arity, args);
  va_end(args);
  putchar('\n');

  if (env == NULL) {
    llvm_fn_free(fn);
  } else {
    llvm_env_store_fn(env, fn);
  }
}

void llvm_emit_define_start(Str name, Str ret, int arity, ...) {
  va_list args;
  va_start(args, arity);
  printf("\n; Function definition start: %s\n", name);
  llvm_emit_fn("define", name, ret, arity, args);
  va_end(args);
  printf(" {\n");
}

void llvm_emit_define_close(Str name) {
  printf("}\n; Function definition end: %s\n", name);
}

void llvm_emit_main_start() {
  llvm_emit_define_start("main", i32, 0);
}

void llvm_emit_main_close() {
  llvm_emit_define_close("main");
}

void llvm_emit_alloc(Str name, Str type) {
  // %2 = alloca i32, align 4
  printf("%%%s = alloca %s\n", name, type);
}

void llvm_emit_store(Str name, Str type, char* val) {
  // store i32 42, i32* %2, align 4
  printf("store %s %s, %s %%%s\n", type, val, llvm_ptr(type), name);
}

void llvm_emit_alloc_and_store(Str name, Str type, char* val) {
  llvm_emit_alloc(name, type);
  llvm_emit_store(name, type, val);
}

void llvm_emit_set(Str name, Str value) {
  // %3 = X
  if (value == NULL || EQ(value, "")) {
    // Value is coming from another emitter function
    printf("%%%s = ", name);
  } else {
    printf("%%%s = %s\n", name, value);
  }
}

// TODO Get arity from env?
void llvm_emit_call(Str name, Str ret, int arity, ...) {
  va_list args;

  // call i8* @calloc(i64 120000, i64 0)
  printf("call %s @%s(", ret, name);
  va_start(args, arity);

  for (int i = 0; i < arity; i += 2) {
    if (i != 0) {
      printf(", ");
    }

    printf("%s %s", va_arg(args, char*), va_arg(args, char*));
  }

  va_end(args);
  printf(")\n");
}
