#
# Copyright (C) 2012 LAN Xingcan
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

CC ?= gcc
CXX ?= g++
AR ?= ar

OBJS=\
	dfa_node.o\
	dfa_machine.o\

all: lsg

libdfa.a: $(OBJS)

lsg: parser.o libdfa.a
	g++ $< libdfa.a -o $@

parser.cc: parser.y
	bison $< -o $@


all: lsg

.PHONY: clean

clean:
	rm libdfa.a $(OBJS) -rf

libdfa.a: $(OBJS)
	$(AR) rc $@ $(OBJS)

.cc.o:
	$(CXX) -Wall -g -c -o $@ $<

.c.o:
	$(CC) -Wall -g -c -o $@ $<
