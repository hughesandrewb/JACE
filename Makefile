CC = g++
CFLAGS = -Wall -O3 -std=c++17

INCLUDES = -I../

SRCS = $(wildcard src/*/*.cc)
SRCS += src/main.cc

OBJS = $(SRCS:.cc=.o)

MAIN = jace

all: $(MAIN)
	@echo Compiled

$(MAIN): $(OBJS)
	@mkdir -p build
	$(CC) -o build/$(MAIN) $(addprefix bin/,$(notdir $(OBJS)))

.cc.o:
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o bin/$(notdir $@)

clean:
	@rm bin/*.o
