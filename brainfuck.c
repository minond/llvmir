#include <stdio.h>
#include <stdlib.h>
#include "emitter.c"

int main(void) {
  Str code = ">>><<<,.";

  Var cells = var(cells);
  Var ptr = var(ptr);

  Environment_t* env = llvm_env();

  llvm_emit_declare(env, "calloc", i8ptr, 2, i64, i64);
  llvm_emit_declare(env, "free", Void, 1, i8ptr);
  llvm_emit_declare(env, "putchar", i32, 1, i32);
  llvm_emit_declare(env, "getchar", i32, 0);

  llvm_emit_main_start();
  llvm_emit_alloc_and_store(ptr, i64, "0");

  llvm_emit_set(cells, NULL);
  llvm_emit_call("calloc", i8ptr, 4, i64, "120000", i64, "0");

  // XXX Test accessing index of %cells and setting its value.

  for (int i = 0; code[i] != '\0'; i++) {
    switch (code[i]) {
    case '>':
      llvm_emit_add(env, ptr, i64, 1);
      break;

    case '<':
      llvm_emit_sub(env, ptr, i64, 1);
      break;

    // case '+': emit_increment(); break;
    // case '-': emit_decrement(); break;
    // case '.': emit_putchar(); break;
    // case ',': emit_getchar(); break;
    // case '[': emit_loop_start();
    // case ']': emit_loop_end(); break;
    }
  }

  llvm_emit_call("free", Void, 2, i8ptr, cells);
  llvm_emit_ret(i32, "0");
  llvm_emit_main_close();

  llvm_emit_comment("Environment:");
  llvm_env_print(env);
  llvm_env_free(env);

  return 0;
}
