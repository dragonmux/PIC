MAKEFLAGS = -w -R
GCC ?= gcc
ifeq ($(strip $(DEBUG)), 1)
	OPTIM_FLAGS = -ggdb
	GCC_FLAGS = 
else
	GCC_VER = $(shell gcc -dumpversion | cut -d . -f 1)
	ifeq ($(shell if [ $(GCC_VER) -ge 4 ]; then echo 1; else echo 0; fi), 1)
		GCC_FLAGS = -fvisibility=hidden -fvisibility-inlines-hidden
	else
		GCC_FLAGS = 
	endif
	OPTIM_FLAGS = -O2
	DEBUG = 0
endif
CC = $(GCC) $(GCC_FLAGS)
CFLAGS = $(OPTIM_FLAGS) -Wall -c -o $@ $<
LFLAGS = 

O = ROM.o Memory16F88.o 16F88.o

default: all

all: $(O)

.cpp.o:
	$(CC) $(CFLAGS)

clean:
	rm -f *.o

#gcc -m64 -fPIC -DPIC -c -o 16F88.o 16F88.cpp
