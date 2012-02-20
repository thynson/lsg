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

#include <memory>
#include "dfa.hh"

namespace lsg
{

	using namespace std;

	namespace
	{
		struct cmper;

		typedef dfa_node::leaf_set_t leaf_set_t;

		typedef set<leaf_set_t*, cmper, allocator<leaf_set_t*> >
			const_state_set_t;

		typedef map<leaf_set_t*, dfa_state*, cmper,
				allocator<pair<const leaf_set_t*, dfa_state*> > >
			const_state_map_t;

		struct cmper
		{
			bool operator () (const leaf_set_t *lhs, const leaf_set_t *rhs)
			{
				return *lhs < *rhs;
			}
		};

		// @brief Convert a DFA AST to states set
		dfa_state *convert_from_dfa_tree(const dfa_node *root)
		{
			const_state_map_t marked_states, unmarked_states;
			leaf_set_t *l = new leaf_set_t(root->get_first_nodes());
			unsigned id = 0;
			dfa_state *current = new dfa_state(id++);
			dfa_state *ret = current;

			unmarked_states.insert(make_pair(l, current));

			while (!unmarked_states.empty())
			{
				leaf_set_t *s = unmarked_states.begin()->first;
				current = unmarked_states.begin()->second;

				// Move it to marked current
				unmarked_states.erase(unmarked_states.begin());
				marked_states.insert(make_pair(s, current));

				map<unsigned, leaf_set_t*> m;



				// Merge by input
				for (leaf_set_t::iterator i = s->begin();
					 i != s->end(); ++i)
				{
					const dfa_leaf_node *leaf_node = *i;

					// A leaf node is either a dfa_none_node or dfa_match_node
					// So if it's nullable, it must be a dfa_none_node
					if (leaf_node->is_nullable())
						continue;

					unsigned input = leaf_node->get_input();
					const leaf_set_t &ref = leaf_node->get_follow_node();

					if (m.find(input) == m.end())
					{
						leaf_set_t *follow_list = new leaf_set_t(ref);
						m.insert(make_pair(input, follow_list));
					} else {
						m[input]->insert(ref.begin(), ref.end());
					}
				}

				// For each input, add a transit path for this current
				for (map<unsigned, leaf_set_t*>::iterator i = m.begin();
					 i != m.end(); ++i)
				{
					leaf_set_t *u = i->second;
					const_state_map_t::iterator pos;

					pos = marked_states.find(u);

					if (pos == marked_states.end())
						pos = unmarked_states.find(u);

					if (pos == unmarked_states.end())
					{
						dfa_state *new_state = new dfa_state(id++);
						unmarked_states.insert(make_pair(u, new_state));
						current->add_transite_path(i->first, new_state);
					} else {
						current->add_transite_path(i->first, pos->second);
						delete u;
					}
				}
			}

			return ret;
		}
	}

	dfa_machine::dfa_machine(const dfa_node *root)
		: m_start(convert_from_dfa_tree(root))
	{
	}

	dfa_machine::~dfa_machine()
	{
		delete m_start;
	}

	dfa_state::dfa_state(unsigned id)
		: m_id(id)
		, m_transit_table()
	{
	}

	dfa_state::~dfa_state()
	{
		for (map<unsigned, dfa_state*>::iterator i = m_transit_table.begin();
			 i != m_transit_table.end(); ++i)
		{
			delete i->second;
		}
	}

	bool dfa_state::add_transite_path(unsigned input, dfa_state *state)
	{
		if (m_transit_table.find(input) != m_transit_table.end())
			return false;
		m_transit_table.insert(make_pair(input, state));
		return true;
	}

	const std::map<unsigned, dfa_state*> &dfa_state::get_transit_table() const
	{
		return m_transit_table;
	}

	unsigned dfa_state::get_id() const
	{
		return m_id;
	}

}
