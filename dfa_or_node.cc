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

#include "dfa.hh"

namespace lsg
{
    using namespace std;

    dfa_or_node::dfa_or_node(dfa_node *former, dfa_node *latter)
        : m_former(former)
        , m_latter(latter)
    {
    }

    dfa_or_node::~dfa_or_node()
    {
    }

    void dfa_or_node::get_first_nodes(list<dfa_leaf_node*> &l)
    {
        m_former->get_first_nodes(l);
		m_latter->get_first_nodes(l);
    }

    void dfa_or_node::get_last_nodes(list<dfa_leaf_node*> &l)
    {
		m_former->get_last_nodes(l);
        m_latter->get_last_nodes(l);
    }

	bool dfa_or_node::is_nullable()
	{
		return m_former->is_nullable() || m_latter->is_nullable();
	}
}
