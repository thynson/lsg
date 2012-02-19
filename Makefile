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

OBJS=\
	dfa_match_node.o\
	dfa_star_node.o\
	dfa_or_node.o\
	dfa_cat_node.o\
	dfa_leaf_node.o\
	dfa_node.o\
	dfa_none_node.o\

all: libdfa.a

.PHONY: clean

clean:
	rm libdfa.a $(OBJS) -rf

libdfa.a: $(OBJS)
	ar rc $@ $(OBJS)


.cc.o:
	g++ -Wall -g -c -o $@ $<
