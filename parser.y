
%{
#include "dfa.hh"
#include <stack>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

using namespace lsg;

std::stack<dfa_node*> node_stack;

int yylex();
int yyerror(const char*);

%}

%token OR LP RP PLUS STAR QUES CHAR


%%

regexp: regexp OR branch
	  	{
			dfa_node *rhs = node_stack.top();
			node_stack.pop();
			dfa_node *lhs = node_stack.top();
			node_stack.pop();
			node_stack.push(new dfa_or_node(lhs, rhs));
		}
	  | branch

branch: branch node
	  	{
			dfa_node *rhs = node_stack.top();
			node_stack.pop();
			dfa_node *lhs = node_stack.top();
			node_stack.pop();
			node_stack.push(new dfa_cat_node(lhs, rhs));
		}
	  | node

node: single PLUS
		{
			dfa_node *lhs = node_stack.top();
			dfa_node *rhs = lhs->clone();
			node_stack.pop();
			lhs = new dfa_star_node(lhs);
			dfa_node *branch = new dfa_cat_node(lhs, rhs);
			node_stack.push(branch);
		}
	| single STAR
		{
			dfa_node *sub = node_stack.top();
			node_stack.pop();
			dfa_node *branch = new dfa_star_node(sub);
			node_stack.push(branch);
		}
	| single QUES
		{
			dfa_node *lhs = node_stack.top();
			node_stack.pop();
			dfa_or_node *branch = new dfa_or_node(lhs, new dfa_leaf_node(0));
			node_stack.push(branch);
		}
	| single

single: CHAR
	  	{
			dfa_leaf_node *node = new dfa_leaf_node($1);
			node_stack.push(node);
		}
	  | LP regexp RP

%%


int yylex()
{
	int ch = fgetc(stdin);

	if (isalnum(ch))
	{
		yylval = ch;
		return CHAR;
	}
	else if (ch == '+')
	{
		return PLUS;
	}
	else if (ch == '*')
	{
		return STAR;
	}
	else if (ch == '?')
	{
		return QUES;
	}
	else if (ch == '(')
	{
		return LP;
	}
	else if (ch == ')')
	{
		return RP;
	}
	else if (ch == '|')
	{
		return OR;
	}
	else
		return EOF;
}

int yyerror(const char *)
{
	exit(EXIT_FAILURE);
}

int main()
{
	yyparse();
	dfa_node *root = node_stack.top();
	dfa_machine m(root);
	delete root;
}
