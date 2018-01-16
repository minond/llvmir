#include <stdio.h>
#include <stdlib.h>
#include "emitter.c"

void emit_shift_right() {}

void emit_shift_left() {}

void emit_increment() {}

void emit_decrement() {}

void emit_putchar() {}

void emit_getchar() {}

void emit_loop_start() {}

void emit_loop_end() {}

int main(void) {
  char* code = ",.";

  Environment* env = llvm_env();

  llvm_declare(env, "calloc", i8ptr, 2, i32, i32);
  llvm_declare(env, "free", Void, 1, i8ptr);

  llvm_declare(env, "putchar", i32, 1, i32);
  llvm_declare(env, "getchar", i32, 0);

  llvm_main_start();

  for (int i = 0; code[i] != '\0'; i++) {
    switch (code[i]) {
      case '>': emit_shift_right(); break;
      case '<': emit_shift_left(); break;
      case '+': emit_increment(); break;
      case '-': emit_decrement(); break;
      case '.': emit_putchar(); break;
      case ',': emit_getchar(); break;
      case '[': emit_loop_start();
      case ']': emit_loop_end(); break;
    }
  }

  llvm_ret(i32, "0");
  llvm_main_close();

  llvm_comment("Environment:");
  llvm_env_print(env);

  llvm_env_free(env);

  return 0;
}
