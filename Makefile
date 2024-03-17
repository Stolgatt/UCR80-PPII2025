CC= gcc
LFLAGS= -lSDL2 -fsanitize=address
CFLAGS= -O3 -Wall -Wextra -g

all: main.out main_test.out

main.out: main.c graphical_engine.c
	$(CC) $(CFLAGS) main.c graphical_engine.c $(LFLAGS) -o main.out
	chmod u+x main.out

main_test.out: main_test.c
	$(CC) $(CFLAGS) main_test.c $(LFLAGS) -o main_test.out
	chmod u+x main_test.out

run: main.out
	./main.out

test: main_test.out
	./main_test.out

clean:
	rm *.out