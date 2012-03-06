//
// Copyright (C) 2012 LAN Xingcan
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

#include "dumper.hh"
#include "lsg.hh"

using namespace std;
using namespace lsg;

dumper::dumper(dfa_machine *m)
	: m_machine(m)
{
}

dumper::~dumper()
{
}

void dumper::dump(std::ostream &os)
{

	const vector<dfa_state*> &v = m_machine->get_states();
	vector<dfa_state*>::const_iterator i;

	dump_prolog(os, v.size());

	for (i = v.begin(); i != v.end(); ++i)
	{
		dump_state(os, *i);
	}

	dump_postscript(os);
}

pretty_dumper::pretty_dumper(dfa_machine *m)
	: dumper(m)
{
}

pretty_dumper::~pretty_dumper()
{
}

void pretty_dumper::dump_state(std::ostream &os, dfa_state *s)
{
	os << " State " << s->get_id() << ":\n";

	const map<unsigned, dfa_state*> &m = s->get_transit_table();
	map<unsigned, dfa_state*>::const_iterator i;

	for (i = m.begin(); i != m.end(); ++i)
	{
		unsigned input = i->first;
		dfa_state *target = i->second;

		if (input >= LSG_RULE_ID_START)
		{
			os << "  Satisfy rule " << input - LSG_RULE_ID_START << endl;
			/* Dummy input for rule is always larger then LSG_RULE_ID_START,
			 * so all the possible input have been handled. And the rule id
			 * increases rule by rule, the former rule have priority than
			 * latter, so after dump one satisfied rule, we just break the
			 * loop, inspite of any other rules */
			break;
		}
		else
		{
			os << "  On input " << input << " : goto "
			   << target->get_id() << endl;
		}
	}
}

void pretty_dumper::dump_prolog(std::ostream &os, unsigned state_count)
{
	os << "State: " << state_count << endl
	   << "Transition graph:" << endl;
}

void pretty_dumper::dump_postscript(std::ostream &os)
{

}

c_dumper::c_dumper(dfa_machine *m)
	: dumper(m)
	, m_ctx("LSG_CTX")
	, m_get_input("LSG_GET_INPUT")
	, m_shift_input("LSG_SHIFT_INPUT")
	, m_satisfy_rule("LSG_SATISFY_RULE")
	, m_get_state("LSG_GET_STATE")
	, m_set_state("LSG_SET_STATE")
	, m_again("LSG_AGAIN")
{
}

c_dumper::~c_dumper()
{
}

void c_dumper::set_var_ctx(const std::string &name)
{
	m_ctx = name;
}

void c_dumper::set_func_shift_input(const std::string &name)
{
	m_shift_input = name;
}

void c_dumper::set_func_satisfy_rule(const std::string &name)
{
	m_satisfy_rule = name;
}

void c_dumper::set_func_set_state(const std::string &name)
{
	m_set_state = name;
}

void c_dumper::set_func_get_state(const std::string &name)
{
	m_get_input = name;
}

void c_dumper::set_macro_again(const std::string &name)
{
	m_again = name;
}

void c_dumper::dump_state(ostream &os, dfa_state *s)
{
	os << "\tcase " << s->get_id() << ":\n"
	   << "\t\t{\n";

	const map<unsigned, dfa_state*> &m = s->get_transit_table();
	map<unsigned, dfa_state*>::const_iterator i;

	bool have_next_input = m.begin()->first < LSG_NONE;
	bool have_satisfy_rule = m.end()->first > LSG_NONE;

	if (have_next_input)
	{
		os << "\t\t\tint input = " << m_get_input
		   << "(" << m_ctx << ");\n";

		os << "\t\t\tif (input < 0)\n"
		   << "\t\t\t{\n";

		os << "\t\t\t\t" << m_set_state
		   << "(" << m_ctx << ", state);\n";

		os << "\t\t\t\t" << m_again
		   << "(" << m_ctx << ");\n";

		os << "\t\t\t}\n";

		os << "\t\t\tswitch(input)\n"
		   << "\t\t\t{\n";

		for (i = m.begin(); i != m.end(); ++i)
		{
			if (i->first < LSG_NONE)
			{
				os << "\t\t\tcase " << i->first << ":\n"
				   << "\t\t\t\t" << m_shift_input
				   << "(" << m_ctx << ");\n";

				os << "\t\t\t\tstate = " << i->second->get_id() << ";\n";
				os << "\t\t\t\tbreak;\n";
			}
			else
			{
				// Handle this rule
				os << "\t\t\tdefault:\n"
				   << "\t\t\t\t" << m_satisfy_rule
				   << "(" << m_ctx << ", " << i->first - LSG_RULE_ID_START
				   << ");\n";

				/* Reset state */
				os << "\t\t\t\tstate = 0;\n";
				break;
			}
		}

		if (!have_satisfy_rule)
		{
			// ERROR HERE
			os << "\t\t\tdefault:\n" << "\t\t\t\tstate = 0;\n";

		}
		os << "\t\t\t}\n";
	}
	else
	{
		os << "\t\t\t" << m_satisfy_rule << "("
		   << m_ctx << ", " << m.begin()->first << ");\n";
		os << "\t\t\tstate = 0;\n";
	}

	os << "\t\t}\n"
	   << "\t\tbreak;\n";

}

void c_dumper::dump_prolog(ostream &os, unsigned state_count)
{
	// Get current state
	os << "int state = " << m_get_state
	   << "(" << m_ctx << ");\n";

	os << "for ( ; ; ) \n{\n";


	os << "\tswitch(state)\n"
	   << "\t{\n";
}

void c_dumper::dump_postscript(ostream &os)
{
	os << "\t}\n";

	// End of subblock
	os << "}\n";

}