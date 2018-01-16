#include <stdio.h>
#include <stdarg.h>

#define def_llvm_type(label) \
  def_llvm_labeled_type(label, label);

#define def_llvm_labeled_type(label, value) \
  const char* label = #value; \
  const char* label ## ptr = #value "*";

def_llvm_type(i8);
def_llvm_type(i32);
def_llvm_type(i64);
def_llvm_labeled_type(Void, void);

typedef struct FnPrototype FnPrototype;
typedef struct Environment Environment;

struct FnPrototype {
  int arity;
  char* name;
  char* ret;
  char** args;
};

struct Environment {
  int fnc;
  FnPrototype** fns;
};

Environment* llvm_env() {
  Environment* env = malloc(sizeof(Environment));
  env->fnc = 0;
  env->fns = NULL;
  return env;
}

void llvm_env_store_fn(Environment* env, FnPrototype* fn) {
  env->fnc += 1;
  env->fns = realloc(env->fns, sizeof(FnPrototype) * env->fnc);
  env->fns[env->fnc - 1] = fn;
}

void llvm_fn_free(FnPrototype* fn) {
  free(fn->args);
  free(fn);
}

void llvm_env_free(Environment* env) {
  for (int i = 0; i < env->fnc; i++) {
    llvm_fn_free(env->fns[i]);
  }

  free(env->fns);
  free(env);
}

void llvm_comment(const char* msg) {
  printf("; %s\n", msg);
}

void llvm_ret(const char* ret, const char* val) {
  printf("  ret %s %s;\n", ret, val);
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

void llvm_fn_print(FnPrototype* fn) {
  printf("fn %s/%d (", fn->name, fn->arity);

  for (int i = 0; i < fn->arity; i++) {
    if (i != 0) {
      printf(", ");
    }

    printf("%s", fn->args[i]);
  }

  printf(") %s\n", fn->ret);
}

void llvm_env_print(Environment* env) {
  for (int i = 0; i < env->fnc; i++) {
    printf("; - ");
    llvm_fn_print(env->fns[i]);
  }
}
