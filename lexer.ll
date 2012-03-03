
%{
#include "parser.h"
%}

%option nounput
%option noinput

%x define
%x regexp
%x refer
%%

<<EOF>> {
	return LSG_EOF;
}

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

<regexp>\\[abfnrtv] {
	return ESCAPE;
}

<regexp>\\[xX][0-9a-fA-F]{2} {
	return ESCAPE_HEX;
}

<regexp>\\0[0-3][0-7]{2} {
	return ESCAPE_OCT;
}

<regexp>\\. {
	return ESCAPE_ALL;
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
