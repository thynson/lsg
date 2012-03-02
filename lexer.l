
%{
#include "parser.h"
%}

%x define
%x regexp
%x refer
%%

<INITIAL,define>define {
	return DEFINE;
}

<INITIAL,define>export {
	return EXPORT;
}

<INITIAL,define,refer>[a-zA-Z_][a-zA-Z_0-9]* {
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

<INITIAL,define>\#[^\n]* {
	return COMMENT;
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

<regexp>\$\( {
	BEGIN(refer);
	return REF_BEGIN;
}

<refer>\) {
	BEGIN(regexp);
	return REF_END;
}


%%


int yywrap(void)
{
	return -1;
}
