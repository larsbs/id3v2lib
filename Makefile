CC = gcc
CPPFLAGS = -I./include -I./src
CFLAGS = -g -Wall -std=c99

TARGET = lib/libid3v2
SRCS = $(shell find src -type f -name '*.c')
OBJS = $(SRCS:.c=.o)

TEST_SRCS = $(shell find test -type f -name '*.c')
TEST_OBJS = $(TEST_SRCS:.c=.o)

all: build_test

test: build_test
	@echo "\n\n======TEST======\n"
	@cd ./test; ./main_test; cd ..
	@echo "\n\n======TEST======\n\n"

build_static: $(TARGET).a

$(TARGET).a: $(OBJS)
	mkdir -p $(shell dirname $(TARGET))
	ar -rcs $(TARGET).a $(OBJS)

build_test: build_static test/main_test

test/main_test: $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(CPPFLAGS) -L./lib -lid3v2 -o test/main_test

clean:
	rm -rf lib
	rm -rf $(shell find . -type f -name '*.o')
	rm -rf test/main_test

.PHONY: all clean test build_test build_static
