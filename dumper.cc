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
