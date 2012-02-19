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

    dfa_cat_node::dfa_cat_node(dfa_node *former, dfa_node *latter)
        : m_former(former)
        , m_latter(latter)
    {
        list<dfa_leaf_node*> l, r;
        m_former->get_last_nodes(l);
        m_latter->get_first_nodes(r);

        for (list<dfa_leaf_node*>::iterator i = l.begin();
             i != l.end(); ++i)
        {
            (*i)->add_follow_nodes(r);
        }
    }

    dfa_cat_node::~dfa_cat_node()
    {
    }

    void dfa_cat_node::get_first_nodes(list<dfa_leaf_node*> &l)
    {
        m_former->get_first_nodes(l);
        if (m_former->is_nullable())
            m_latter->get_first_nodes(l);
    }

    void dfa_cat_node::get_last_nodes(list<dfa_leaf_node*> &l)
    {
        m_latter->get_last_nodes(l);
        if (m_latter->is_nullable())
            m_former->get_last_nodes(l);
    }
}
