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
	void dfa_machine::convert(vector<dfa_state*> &states, const dfa_node *root)
	{
		const_state_map_t marked_states, unmarked_states;
		leaf_set_t *l = new leaf_set_t(root->get_first_nodes());
		unsigned id = 0;
		dfa_state *current = new dfa_state(id++);

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

				if (pos == unmarked_states.end()) {
					dfa_state *new_state = new dfa_state(id++);
					unmarked_states.insert(make_pair(u, new_state));
					current->add_transite_path(i->first, new_state);
				} else {
					current->add_transite_path(i->first, pos->second);
					delete u;
				}
			}
		}

		states.clear();

		for (const_state_map_t::iterator i = marked_states.begin();
			 i != marked_states.end(); ++i)
		{
			states.push_back(i->second);
			delete i->first;
		}
	}

}
