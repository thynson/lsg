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

ACLOCAL_AMFLAGS=-I build-aux/m4

bin_PROGRAMS=lsg

lib_LTLIBRARIES=liblsg.la

lsg_SOURCES=main.cc

parser.cc parser.h: parser.yy
	$(YACC) --defines=parser.h -o parser.cc $<

lexer.cc lexer.h: lexer.ll
	$(LEX) --header-file=lexer.h -o lexer.cc $<


lsg_LDADD=liblsg.la lexer.o parser.o

liblsg_la_SOURCES=dfa_node.cc\
					  dfa_machine.cc

