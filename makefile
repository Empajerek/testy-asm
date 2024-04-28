++ = g++
CC = gcc
AS = nasm
ASFLAGS = -f elf64 -w+all -w+error
CFLAGS = -Wall -Wextra -std=c17 -O3
CPPFLAGS = -Wall -Wextra -std=c++17 -O3
LDFLAGS = -z noexecstack -lgmp

tests = example edge_cases large
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
	@echo "All tests passed"

$(addprefix run-,$(tests)): run-%:
	@echo "Running: $* "
	@./build/$*
	@if [ $$? -ne 0 ]; then \
		echo "Test $* failed, stopping..."; \
		exit 1; \
	fi

clean:
	rm -rf build/*