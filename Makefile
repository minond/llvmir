CC = clang

build: brainfuck.c
	$(CC) -o brainfuck $^

preprocess: emitter.c
	$(CC) -E $^

clean:
	rm brainfuck
