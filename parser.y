
%{

#include "lsg.hh"
#include "dfa.hh"
#include "lexer.h"
#include <stack>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cctype>
#include <string>
#include <map>

using namespace std;
using namespace lsg;

stack<dfa_node*> node_stack;
stack<string> id_stack;
map<string, dfa_node*> define_map;
map<string, dfa_node*> export_map;
map<string, unsigned> rule_map;

int rule_id;

int yyerror(const char*);

%}
%token LSG_TK_CHAR LSG_TK_LP LSG_TK_RP
%token LSG_TK_OR LSG_TK_PLUS LSG_TK_STAR LSG_TK_QUES
%token LSG_TK_LQ LSG_TK_RQ
%token LSG_TK_DEFINE LSG_TK_EXPORT
%token LSG_TK_ID
%token LSG_TK_LF
%token LSG_TK_REF
%token LSG_TK_ESCAPE LSG_TK_ESCAPE_HEX LSG_TK_ESCAPE_OCT LSG_TK_ESCAPE_ALL

%%

lsg: lines
{
	map<string, dfa_node*>::iterator i;
	for (i = define_map.begin(); i != define_map.end(); ++i)
	{
		delete i->second;
	}
}

lines: lines define
	| lines export
	| lines LSG_TK_LF /* Empty lines */
	| LSG_TK_LF
	| define
	| export

define: LSG_TK_DEFINE id regexp_wrap LSG_TK_LF
{
	dfa_node *node = node_stack.top();
	string id = id_stack.top();
	id_stack.pop();

	if (export_map.find(id) != export_map.end())
	{
		cerr << "Redefine id `" << id << "`" << endl;
		exit (EXIT_FAILURE);
	}
	else if (define_map.find(yytext) != define_map.end())
	{
		cerr << "Redefine id `" << id << "`" << endl;
		exit (EXIT_FAILURE);
	}

	node_stack.pop();
	define_map.insert(make_pair(id, node));
}

export: LSG_TK_EXPORT id regexp_wrap LSG_TK_LF
{
	dfa_node *node = node_stack.top();
	string id = id_stack.top();
	id_stack.pop();

	if (export_map.find(id) != export_map.end())
	{
		cerr << "Redefine id `" << id << "`" << endl;
		exit (EXIT_FAILURE);
	}
	else if (define_map.find(yytext) != define_map.end())
	{
		cerr << "Redefine id `" << id << "`" << endl;
		exit (EXIT_FAILURE);
	}

	node_stack.pop();

	rule_map.insert(make_pair(id, rule_id++));
	export_map.insert(make_pair(id, node));
}

id: LSG_TK_ID
{
	id_stack.push(string(yytext));
}

regexp_wrap: LSG_TK_LQ regexp LSG_TK_RQ

regexp: regexp LSG_TK_OR branch
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

node: single LSG_TK_PLUS
{
	dfa_node *lhs = node_stack.top();
	dfa_node *rhs = lhs->clone();
	node_stack.pop();
	lhs = new dfa_star_node(lhs);
	dfa_node *branch = new dfa_cat_node(lhs, rhs);
	node_stack.push(branch);
}
	| single LSG_TK_STAR
{
	dfa_node *sub = node_stack.top();
	node_stack.pop();
	dfa_node *branch = new dfa_star_node(sub);
	node_stack.push(branch);
}
	| single LSG_TK_QUES
{
	dfa_node *lhs = node_stack.top();
	node_stack.pop();
	dfa_or_node *branch = new dfa_or_node(lhs, new dfa_leaf_node(LSG_NONE));
	node_stack.push(branch);
}
	| single

single: LSG_TK_CHAR
{
	dfa_leaf_node *node = new dfa_leaf_node(yytext[0]);
	node_stack.push(node);
}
	| LSG_TK_ESCAPE
{
	int input = -1;
	switch(yytext[1])
	{
	case 'a':
		input = '\a';
		break;
	case 'b':
		input = '\b';
		break;
	case 'f':
		input = '\f';
		break;
	case 'n':
		input = '\n';
		break;
	case 'r':
		input = '\r';
		break;
	case 't':
		input = '\t';
		break;
	case 'v':
		input = '\v';
		break;
	}

	if (input > 0)
	{
		dfa_leaf_node *node = new dfa_leaf_node(input);
		node_stack.push(node);
	}
	else
	{
		/* TODO: What to handle with this situation? */
	}
}
	| LSG_TK_ESCAPE_HEX
{
	stringstream s;
	s << hex << '0' << (yytext+1);
	int input;
	s >> input;
	dfa_leaf_node *node = new dfa_leaf_node(input);
	node_stack.push(node);
}
	| LSG_TK_ESCAPE_OCT
{
	stringstream s;
	s << oct << (yytext+1);
	int input;
	s >> input;
	dfa_leaf_node *node = new dfa_leaf_node(input);
	node_stack.push(node);
}
	| LSG_TK_ESCAPE_ALL
{
	int input = yytext[1];
	dfa_leaf_node *node = new dfa_leaf_node(input);
	node_stack.push(node);
}
	| LSG_TK_LP regexp LSG_TK_RP
	| ref_express

/* Change current_id to id_stack */
ref_express: LSG_TK_REF id LSG_TK_RP
{
	dfa_node *top;
	string id = id_stack.top();
	id_stack.pop();
	if (export_map.find(id) != export_map.end()) {
		top = export_map[id]->clone();
	}
	else if (define_map.find(id) != define_map.end())
	{
		top = define_map[id]->clone();
	}
	else
	{
		cerr << "id is not found" << endl;
		exit(EXIT_FAILURE);
	}

	node_stack.push(top);
}

%%

int yyerror(const char *)
{
	exit(EXIT_FAILURE);
}



namespace lsg
{
	dfa_node *start_parse()
	{
		yyparse();
		dfa_node *root = NULL;

		for (map<string, dfa_node*>::iterator i = export_map.begin();
			 i != export_map.end(); ++i)
		{
			int rid = LSG_RULE_ID_START + rule_map[i->first];
			dfa_leaf_node *dummy = new dfa_leaf_node(rid);
			dfa_cat_node *handle = new dfa_cat_node(i->second, dummy);

			if (root == NULL)
			{
				root = handle;
			}
			else
			{
				root = new dfa_or_node(root, handle);
			}
		}
		return root;
	}
}
