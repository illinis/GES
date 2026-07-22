# Builds the validation suite (main.c) and the usage reference (usage.c)
# against every micro-project living under this directory. Drop a new
# <name>/<name>.c (+ <name>.h) pair in here and its sources are picked up
# automatically - no edits needed.

CC      ?= cc
CFLAGS  ?= -std=c99 -Wall -Wextra -Wconversion -g
LDFLAGS ?=

TEST_TARGET  := mp_tests
USAGE_TARGET := mp_usage

LIB_SRCS := $(wildcard */*.c)
LIB_OBJS := $(LIB_SRCS:.c=.o)

TEST_OBJS  := main.o $(LIB_OBJS)
USAGE_OBJS := usage.o $(LIB_OBJS)

DEPS := $(sort $(TEST_OBJS:.o=.d) $(USAGE_OBJS:.o=.d))

.PHONY: all run test demo usage clean

all: $(TEST_TARGET) $(USAGE_TARGET)

run test: $(TEST_TARGET)
	./$(TEST_TARGET)

demo usage: $(USAGE_TARGET)
	./$(USAGE_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(LDFLAGS) -o $@

$(USAGE_TARGET): $(USAGE_OBJS)
	$(CC) $(CFLAGS) $(USAGE_OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I. -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(TEST_OBJS) $(USAGE_OBJS) $(DEPS) $(TEST_TARGET) $(USAGE_TARGET)
