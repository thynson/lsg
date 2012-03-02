
%{

extern "C" {

#include "lexer.h"

}
#include "dfa.hh"
#include <stack>
#include <cstdlib>
#include <iostream>
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

int yyerror(const char*);

%}

%token OR LP RP PLUS STAR QUES CHAR
%token DEFINE EXPORT ID LQ RQ LF REF_BEGIN REF_END


%%

lsg: lsg define LF
   | lsg export LF
   | lsg LF /* Empty line */
   | define
   | export

define: DEFINE id regexp_wrap
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

export: EXPORT id regexp_wrap
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
          export_map.insert(make_pair(id, node));
      }

id: ID
  {
      id_stack.push(string(yytext));
  }

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
			dfa_leaf_node *node = new dfa_leaf_node(yytext[0]);
			node_stack.push(node);
		}
	  | LP regexp RP
      | ref_express

/* Change current_id to id_stack */
ref_express: REF_BEGIN id REF_END
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

int main()
{
	yyparse();

    for (map<string, dfa_node*>::iterator i = export_map.begin();
         i != export_map.end(); ++i)
    {
        dfa_machine m(i->second);
        cout << "Rule " << i->first << endl;
        m.dump(cout);
    }
}
