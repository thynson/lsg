

#ifndef __LSG_HH__
#define __LSG_HH__

#include "dfa.hh"
#include <cstdio> /* For FILE */

/* Do not put parser.h and lexer.h here, they are generated */

namespace lsg {
	void lexer_set_stream(FILE *stream);
	dfa_node *start_parse();
}

#endif

