HEADERS = engine_common.h graphical_engine.h
SRCS = engine_common.c graphical_engine.c

CC= gcc
FLAGS= -Wall -Wextra -pedantic 
FLAGS += -O3 -fno-omit-frame-pointer
FLAGS += -lm -fsanitize=address
FLAGS += $(shell pkg-config --cflags sdl2)
FLAGS += $(shell pkg-config --libs sdl2)

all: main.out main_test.out

main.out: main.c $(SRCS) $(HEADERS)
	$(CC) $(FLAGS) $(SRCS) main.c -o main.out
	chmod u+x main.out

main_test.out: main_test.c $(SRCS) $(HEADERS)
	$(CC) $(FLAGS) $(SRCS) main_test.c -o main_test.out
	chmod u+x main_test.out

loadpixels.out: loadpixels.c
	$(CC) $(FLAGS) loadpixels.c -o loadpixels.out
	chmod u+x loadpixels.out

pixel: loadpixels.out
	./loadpixels.out

run: main.out
	./main.out

test: main_test.out
	./main_test.out

clean: main.out main_test.out
	rm *.out
