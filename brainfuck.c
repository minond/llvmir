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

  return 0;
}
