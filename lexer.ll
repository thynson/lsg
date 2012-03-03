
%{
#include "parser.h"
%}

%option nounput
%option noinput

%x LSG_COND_DEF
%x LSG_COND_RGX
%x LSG_COND_REF
%%
<INITIAL,LSG_COND_DEF>define {
	return LSG_TK_DEFINE;
}

<INITIAL,LSG_COND_DEF>export {
	return LSG_TK_EXPORT;
}

<INITIAL,LSG_COND_DEF,LSG_COND_REF>[a-zA-Z_][a-zA-Z_0-9]* {
	return LSG_TK_ID;
}

<INITIAL,LSG_COND_DEF>\" {
	BEGIN(LSG_COND_RGX);
	return LSG_TK_LQ;
}

<INITIAL,LSG_COND_DEF>\n {
	return LSG_TK_LF;
}

<INITIAL,LSG_COND_DEF>[\t ]+ {
	BEGIN(LSG_COND_DEF);
}

<INITIAL,LSG_COND_DEF>\#[^\n]* {
	BEGIN(LSG_COND_DEF);
}

<LSG_COND_RGX>\" {
	BEGIN(LSG_COND_DEF);
	return LSG_TK_RQ;
}

<LSG_COND_RGX>[a-zA-Z0-9_] {
	return LSG_TK_CHAR;
}

<LSG_COND_RGX>\+ {
	return LSG_TK_PLUS;
}

<LSG_COND_RGX>\* {
	return LSG_TK_STAR;
}

<LSG_COND_RGX>\? {
	return LSG_TK_QUES;
}

<LSG_COND_RGX>\| {
	return LSG_TK_OR;
}

<LSG_COND_RGX>\( {
	return LSG_TK_LP;
}

<LSG_COND_RGX,LSG_COND_REF>\) {
	BEGIN(LSG_COND_RGX);
	return LSG_TK_RP;
}

<LSG_COND_RGX>\$\( {
	BEGIN(LSG_COND_REF);
	return LSG_TK_REF;
}

<LSG_COND_RGX>\\[abfnrtv] {
	return LSG_TK_ESCAPE;
}

<LSG_COND_RGX>\\[xX][0-9a-fA-F]{2} {
	return LSG_TK_ESCAPE_HEX;
}

<LSG_COND_RGX>\\0[0-3][0-7]{2} {
	return LSG_TK_ESCAPE_OCT;
}

<LSG_COND_RGX>\\. {
	return LSG_TK_ESCAPE_ALL;
}

%%


int yywrap(void)
{
	return -1;
}
