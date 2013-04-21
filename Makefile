# This file is part of PIC (libGTK++)
# Copyright © 2012 Rachel Mant (dx-mon@users.sourceforge.net)
#
# PIC is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PIC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

MAKEFLAGS = -w -r -R
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

O = ROM.o IntelHex.o Memory16F88.o 16F88.o

default: all

all: $(O)

tests:
	libTestMake testIntelHex.cpp -oIntelHex.o

check:
	libTest testIntelHex

.cpp.o:
	$(CC) $(CFLAGS)

clean:
	rm -f *.o
