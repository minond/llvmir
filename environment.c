#import <stdio.h>

Environment_t* llvm_env() {
  Environment_t* env = malloc(sizeof(Environment_t));
  env->fnc = 0;
  env->fns = NULL;
  env->tmpc = 0;
  return env;
}

const char* llvm_env_get_tmp(Environment_t* env) {
  static char buff[50];
  snprintf(buff, sizeof(buff), "%%tmp%i", env->tmpc++);
  return buff;
}

void llvm_env_store_fn(Environment_t* env, FnPrototype_t* fn) {
  env->fnc += 1;
  env->fns = realloc(env->fns, sizeof(FnPrototype_t) * env->fnc);
  env->fns[env->fnc - 1] = fn;
}

void llvm_fn_free(FnPrototype_t* fn) {
  free(fn->args);
  free(fn);
}

void llvm_env_free(Environment_t* env) {
  for (int i = 0; i < env->fnc; i++) {
    llvm_fn_free(env->fns[i]);
  }

  free(env->fns);
  free(env);
}

void llvm_fn_print(FnPrototype_t* fn) {
  printf("fn %s/%d (", fn->name, fn->arity);

  for (int i = 0; i < fn->arity; i++) {
    if (i != 0) {
      printf(", ");
    }

    printf("%s", fn->args[i]);
  }

  printf(") %s\n", fn->ret);
}

void llvm_env_print(Environment_t* env) {
  for (int i = 0; i < env->fnc; i++) {
    printf("; - ");
    llvm_fn_print(env->fns[i]);
  }
}
