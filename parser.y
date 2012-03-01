
%{
#include "dfa.hh"
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <string>
#include <map>

using namespace std;
using namespace lsg;

stack<dfa_node*> node_stack;

/* Never forget a C++ source file will be generated, how ever yylex is
 * implemented in C source file. */
extern "C" int yylex(void);
int yyerror(const char*);

%}

%token OR LP RP PLUS STAR QUES CHAR
%token DEFINE EXPORT ID LQ RQ LF


%%

lsg: lsg define
   | lsg export
   | lsg LF /* Empty line */
   | define
   | export

define: DEFINE ID regexp_wrap LF

export: EXPORT ID regexp_wrap LF

regexp_wrap: LQ regexp RQ

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
