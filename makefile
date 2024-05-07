++ = g++
CC = gcc
AS = nasm
ASFLAGS = -f elf64 -w+all -w+error
CFLAGS = -Wall -Wextra -c -std=c17 -O3
CPPFLAGS = -Wall -Wextra -c -std=c++17 -O3
LDFLAGS = -z noexecstack

tests = example edge_cases simple time
.PHONY: $(tests) clean all test $(addprefix run-,$(tests))

all: clean $(tests)

build/mdiv.o: mdiv.asm
	$(AS) $(ASFLAGS) -o $@ $<

build/%.o: tests/%.cpp
	$(++) $(CPPFLAGS) -c -o $@ $<

build/%.o: tests/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(tests): %: build/%.o build/mdiv.o
	$(++) $(LDFLAGS) -o build/$@ $^

test: $(addprefix run-,$(tests))
	@echo "All tests passed" | cowsay -f stegosaurus

$(addprefix run-,$(tests)): run-%:
	@echo "Running: $* "
	@./build/$*

clean:
	rm -rf build/*