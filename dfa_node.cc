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

	dfa_node::dfa_node()
	{
	}

	dfa_node::~dfa_node()
	{
	}

	dfa_leaf_node::dfa_leaf_node()
	{
	}

	dfa_leaf_node::~dfa_leaf_node()
	{
	}

	void dfa_leaf_node::get_follow_nodes(const_leaf_list_t &l) const
	{
		l.insert(l.end(), m_follow_nodes.begin(), m_follow_nodes.end());
	}

	void dfa_leaf_node::get_follow_nodes(leaf_list_t &l)
	{
		l.insert(l.end(), m_follow_nodes.begin(), m_follow_nodes.end());
	}

	void dfa_leaf_node::get_first_nodes(const_leaf_list_t &l) const
	{
		l.push_back(this);
	}

	void dfa_leaf_node::get_first_nodes(leaf_list_t &l)
	{
		l.push_back(this);
	}

	void dfa_leaf_node::get_last_nodes(const_leaf_list_t &l) const
	{
		l.push_back(this);
	}

	void dfa_leaf_node::get_last_nodes(leaf_list_t &l)
	{
		l.push_back(this);
	}

	void dfa_leaf_node::add_follow_nodes(const leaf_list_t &l)
	{
		m_follow_nodes.insert(m_follow_nodes.end(), l.begin(), l.end());
	}

	dfa_none_node::dfa_none_node()
	{
	}

	dfa_none_node::~dfa_none_node()
	{
	}

	dfa_match_node::dfa_match_node(unsigned char ch)
		: m_ch(ch)
	{
	}

	dfa_match_node::~dfa_match_node()
	{
	}

	dfa_cat_node::dfa_cat_node(dfa_node *former, dfa_node *latter)
		: m_former(former)
		, m_latter(latter)
	{
		leaf_list_t l, r;
		m_former->get_last_nodes(l);
		m_latter->get_first_nodes(r);

		for (leaf_list_t::iterator i = l.begin();
			 i != l.end(); ++i)
		{
			(*i)->add_follow_nodes(r);
		}
	}

	dfa_cat_node::~dfa_cat_node()
	{
	}

	void dfa_cat_node::get_first_nodes(const_leaf_list_t &l) const
	{
		m_former->get_first_nodes(l);
		if (m_former->is_nullable())
			m_latter->get_first_nodes(l);
	}

	void dfa_cat_node::get_first_nodes(leaf_list_t &l)
	{
		m_former->get_first_nodes(l);
		if (m_former->is_nullable())
			m_latter->get_first_nodes(l);
	}

	void dfa_cat_node::get_last_nodes(const_leaf_list_t &l) const
	{
		m_latter->get_last_nodes(l);
		if (m_latter->is_nullable())
			m_former->get_last_nodes(l);
	}

	void dfa_cat_node::get_last_nodes(leaf_list_t &l)
	{
		m_latter->get_last_nodes(l);
		if (m_latter->is_nullable())
			m_former->get_last_nodes(l);
	}

	bool dfa_cat_node::is_nullable() const
	{
		return m_former->is_nullable() && m_latter->is_nullable();
	}

	dfa_node *dfa_cat_node::clone() const
	{
		dfa_node *l, *r;
		l = m_former->clone();
		r = m_latter->clone();
		return new dfa_cat_node(l, r);
	}

	dfa_or_node::dfa_or_node(dfa_node *former, dfa_node *latter)
		: m_former(former)
		, m_latter(latter)
	{
	}

	dfa_or_node::~dfa_or_node()
	{
	}

	void dfa_or_node::get_first_nodes(const_leaf_list_t &l) const
	{
		m_former->get_first_nodes(l);
		m_latter->get_first_nodes(l);
	}

	void dfa_or_node::get_first_nodes(leaf_list_t &l)
	{
		m_former->get_first_nodes(l);
		m_latter->get_first_nodes(l);
	}

	void dfa_or_node::get_last_nodes(const_leaf_list_t &l) const
	{
		m_former->get_last_nodes(l);
		m_latter->get_last_nodes(l);
	}

	void dfa_or_node::get_last_nodes(leaf_list_t &l)
	{
		m_former->get_last_nodes(l);
		m_latter->get_last_nodes(l);
	}

	bool dfa_or_node::is_nullable() const
	{
		return m_former->is_nullable() || m_latter->is_nullable();
	}

	dfa_node *dfa_or_node::clone() const
	{
		dfa_node *l, *r;
		l = m_former->clone();
		r = m_latter->clone();
		return new dfa_or_node(l, r);
	}

	dfa_star_node::dfa_star_node(dfa_node *sub)
		: m_sub(sub)
	{
		leaf_list_t l, r;
		m_sub->get_last_nodes(l);
		m_sub->get_first_nodes(r);

		for (leaf_list_t::iterator i = l.begin();
			 i != l.end(); ++i)
		{
			(*i)->add_follow_nodes(r);
		}
	}

	dfa_star_node::~dfa_star_node()
	{
	}

	void dfa_star_node::get_first_nodes(const_leaf_list_t &l) const
	{
		m_sub->get_first_nodes(l);
	}

	void dfa_star_node::get_first_nodes(leaf_list_t &l)
	{
		m_sub->get_first_nodes(l);
	}

	void dfa_star_node::get_last_nodes(const_leaf_list_t &l) const
	{
		m_sub->get_last_nodes(l);
	}

	void dfa_star_node::get_last_nodes(leaf_list_t &l)
	{
		m_sub->get_last_nodes(l);
	}

	dfa_node *dfa_star_node::clone() const
	{
		dfa_node *new_sub = m_sub->clone();
		return new dfa_star_node(new_sub);
	}
}

