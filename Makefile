# Build all parts:      make
# Run all demos:        make run
# Memory/UB checks:     make check
# Clean:                make clean

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -O2
SAN     = -std=c11 -Wall -Wextra -pedantic -g -fsanitize=address,undefined

SRC  = $(wildcard src/*.c)
BIN  = $(patsubst src/%.c,bin/%,$(SRC))

all: $(BIN)

bin/%: src/%.c | bin
	$(CC) $(CFLAGS) $< -o $@

bin:
	mkdir -p bin

run: all
	@for b in $(BIN); do echo "\n========== $$b =========="; ./$$b; done

check: | bin
	@for s in $(SRC); do \
	  b=bin/$$(basename $$s .c)_check; \
	  $(CC) $(SAN) $$s -o $$b && ./$$b > /dev/null && echo "OK  $$s"; \
	done

clean:
	rm -rf bin

.PHONY: all run check clean
