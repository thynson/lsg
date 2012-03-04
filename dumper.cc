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


