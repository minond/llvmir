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
