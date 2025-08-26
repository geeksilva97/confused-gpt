# this make file is used to build the project

# compiler
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =
LDLIBS = -lm
# source files
SRCS_C = $(wildcard *.c)
# object files
OBJS_C = $(SRCS_C:.c=.o)
OBJS_CPP = $(SRCS_CPP:.cpp=.o)
# executable name
TARGET = confused_gpt

# default target
all: $(TARGET)
# build target
$(TARGET): $(OBJS_C)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS_C) $(OBJS_CPP) $(TARGET)
# phony targets
.PHONY: all clean
