# SPDX-FileCopyrightText: 2025 Philipp Grassl <philyg@linandot.net>
# SPDX-License-Identifier: MIT

HEADERS = $(wildcard src/*.hpp)

ifeq ($(PREFIX),)
PREFIX := "/usr/local"
endif

all: bin/minte

help:
	@echo "To compile dynamically linked use:  make"
	@echo "To compile statically  linked use:  make CFLAGS=-static"
	@echo "To install to /usr/local/bin  use:  make install"
	@echo "To install to a custom prefix use:  make install PREFIX=[your prefix]"

bin/minte: src/minte.cpp ${HEADERS}
	mkdir -p bin
	$(CXX) $(CFLAGS) -O2 -o bin/minte src/minte.cpp

install: bin/minte
	install -D -m 755 bin/minte ${PREFIX}/bin/minte

clean:
	rm -rf bin/minte

.PHONY: all clean
