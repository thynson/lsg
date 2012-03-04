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

lsg_SOURCES=main.cc\
			lsg.hh

# Not using automake default rules for flex and bison source So here we
# should handle clean up and dist-hook.
clean-local:
	$(RM) parser.cc parser.h lexer.cc lexer.h

dist-hook:
	cp parser.y lexer.l $(distdir)

# Although parser.cc directly depend on parser.h, lexer.h and parser.y,
# and lexer.cc directly depend on parser.h lexer.h and lexer.l, writing
#
# 	parser.cc: parser.h lexer.h
# 	...
# 	lexer.cc: parser.h lexer.h
#	...
#
# will cause cyclic dependency. But we could see that for parser.cc and
# lexer.cc, the essential depending target is parser.y and lexer.l.
parser.cc parser.h: parser.y lexer.l
	$(YACC) --defines=parser.h -o parser.cc $<

lexer.cc lexer.h: lexer.l parser.y
	$(LEX) --header-file=lexer.h -o lexer.cc $<
parser.o: parser.cc lexer.h parser.h
lexer.o: lexer.cc lexer.h parser.h

# Add because lexer.cc and parser.cc is not in lsg_SOURCES, put here to
# tell the linker not to leave them alone.
lsg_LDADD=liblsg.la lexer.o parser.o

liblsg_la_SOURCES=dfa_node.cc\
				  dfa_machine.cc\
				  dfa.hh


