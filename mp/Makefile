# Builds and runs the validation suite (main.c) against every micro-project
# living under this directory. Drop a new <name>/<name>.c (+ <name>.h) pair
# in here and its sources are picked up automatically - no edits needed.

CC      ?= cc
CFLAGS  ?= -std=c99 -Wall -Wextra -Wconversion -g
LDFLAGS ?=

TARGET := mp_tests

LIB_SRCS := $(wildcard */*.c)
SRCS     := main.c $(LIB_SRCS)
OBJS     := $(SRCS:.c=.o)
DEPS     := $(OBJS:.o=.d)

.PHONY: all run test clean

all: $(TARGET)

run test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I. -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
