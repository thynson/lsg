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
#include "lsg.hh"

namespace lsg
{
	using namespace std;

	typedef dfa_node::leaf_set_t leaf_set_t;

	dfa_node::dfa_node()
	{
	}

	dfa_node::~dfa_node()
	{
	}

	const leaf_set_t &dfa_node::get_first_nodes() const
	{
		return m_first_set;
	}

	const leaf_set_t &dfa_node::get_last_nodes() const
	{
		return m_last_set;
	}

	void dfa_node::add_first_node(const leaf_set_t &s)
	{
		m_first_set.insert(s.begin(), s.end());
	}

	void dfa_node::add_first_node(const dfa_leaf_node *n)
	{
		m_first_set.insert(n);
	}

	void dfa_node::add_last_node(const leaf_set_t &s)
	{
		m_last_set.insert(s.begin(), s.end());
	}

	void dfa_node::add_last_node(const dfa_leaf_node *n)
	{
		m_last_set.insert(n);
	}

	dfa_leaf_node::dfa_leaf_node(unsigned input)
		: m_follow_nodes()
		, m_input(input)
	{
		if (input != 0)
		{
			add_first_node(this);
			add_last_node(this);
		}
	}

	dfa_leaf_node::~dfa_leaf_node()
	{
	}

	const leaf_set_t &dfa_leaf_node::get_follow_node() const
	{
		return m_follow_nodes;
	}

	void dfa_leaf_node::add_follow_node(const leaf_set_t &l)
	{
		m_follow_nodes.insert(l.begin(), l.end());
	}

	bool dfa_leaf_node::is_nullable() const
	{
		return m_input == LSG_NONE;
	}

	unsigned dfa_leaf_node::get_input() const
	{
		return m_input;
	}

	dfa_node *dfa_leaf_node::clone() const
	{
		return new dfa_leaf_node(m_input);
	}

	dfa_cat_node::dfa_cat_node(dfa_node *former, dfa_node *latter)
		: m_former(former)
		, m_latter(latter)
	{
		const leaf_set_t &l = m_former->get_last_nodes();
		const leaf_set_t &r = m_latter->get_first_nodes();

		for (leaf_set_t::iterator i = l.begin();
			 i != l.end(); ++i)
		{
			const_cast<dfa_leaf_node*>((*i))->add_follow_node(r);
		}

		add_first_node(m_former->get_first_nodes());
		add_last_node(m_latter->get_last_nodes());

		if (m_former->is_nullable())
			add_first_node(m_latter->get_first_nodes());

		if (m_latter->is_nullable())
			add_last_node(m_former->get_last_nodes());
	}

	dfa_cat_node::~dfa_cat_node()
	{
		delete m_former;
		delete m_latter;
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
		add_first_node(m_former->get_first_nodes());
		add_first_node(m_latter->get_first_nodes());
		add_last_node(m_former->get_last_nodes());
		add_last_node(m_latter->get_last_nodes());
	}

	dfa_or_node::~dfa_or_node()
	{
		delete m_former;
		delete m_latter;
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
		const leaf_set_t &l = m_sub->get_last_nodes();
		const leaf_set_t &r = m_sub->get_first_nodes();


		for (leaf_set_t::iterator i = l.begin();
			 i != l.end(); ++i)
		{
			const_cast<dfa_leaf_node*>((*i))->add_follow_node(r);
		}

		add_first_node(r);
		add_last_node(l);
	}

	dfa_star_node::~dfa_star_node()
	{
		delete m_sub;
	}

	dfa_node *dfa_star_node::clone() const
	{
		dfa_node *new_sub = m_sub->clone();
		return new dfa_star_node(new_sub);
	}
}

