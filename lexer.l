
%{
#include "parser.h"
%}

%x define
%x regexp
%%

<INITIAL,define>define {
	return DEFINE;
}

<INITIAL,define>export {
	return EXPORT;
}

<INITIAL,define>[a-zA-Z_][a-zA-Z_0-9]* {
	return ID;
}

<INITIAL,define>\" {
	BEGIN(regexp);
	return LQ;
}

<INITIAL,define>\n {
	return LF;
}

<INITIAL,define>[\t ]+ {
	BEGIN(define);
}


<regexp>\" {
	BEGIN(define);
	return RQ;
}

<regexp>[a-zA-Z0-9_] {
	return CHAR;
}

<regexp>\+ {
	return PLUS;
}

<regexp>\* {
	return STAR;
}

<regexp>\? {
	return QUES;
}

<regexp>\| {
	return OR;
}

<regexp>\( {
	return LP;
}

<regexp>\) {
	return RP;
}



%%


int yywrap(void)
{
	return -1;
}
