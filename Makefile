CC = clang
CFLAGS = -std=c11 -Werror -Wall

build: brainfuck.c
	$(CC) $(CFLAGS) -o brainfuck $^

preprocess: emitter.c
	$(CC) -E $^

clean:
	rm brainfuck
