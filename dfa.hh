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

// @groupe dfa

#ifndef __DFA_HH__
#define __DFA_HH__

#include <set>
#include <map>
#include <vector>

namespace lsg
{
	// Forward declaration
	class dfa_node;
	class dfa_leaf_node;

	// @brief Base class of a DFA AST Node
	//
	// This class is the abstract base class for a DFA AST Node, defines common
	// interfaces for generating DFA
	class dfa_node
	{
	public:
		typedef std::set<const dfa_leaf_node*> leaf_set_t;

		// @brief Destructor
		virtual ~dfa_node();

		// @brief Get possible first nodes for this node
		const leaf_set_t &get_first_nodes() const;

		// @brief Get possible last nodes for this node
		const leaf_set_t &get_last_nodes() const;

		// @brief If this node accept empty input
		virtual bool is_nullable() const = 0;

		// @brief Get a copy of itself
		virtual dfa_node *clone() const = 0;
	protected:
		// @brief Constructor
		dfa_node();

		void add_first_node(const dfa_leaf_node *n);
		void add_first_node(const leaf_set_t &s);

		void add_last_node(const dfa_leaf_node *n);
		void add_last_node(const leaf_set_t &s);

	private:
		leaf_set_t m_first_set;
		leaf_set_t m_last_set;
	};

	// @brief Stand for a DFA AST leaf node
	class dfa_leaf_node : public dfa_node
	{
	public:
		dfa_leaf_node(unsigned input);
		virtual ~dfa_leaf_node();

		const leaf_set_t &get_follow_node() const;

		virtual bool is_nullable() const;

		// @brief Add follow node for this node
		// @param l Ths set of additional possible follow nodes
		void add_follow_node(const leaf_set_t &l);

		unsigned get_input() const;

		virtual dfa_node *clone() const;
	private:
		leaf_set_t m_follow_nodes;
		unsigned m_input;
	};

	// @brief Stand for a DFA AST node that catenate its two subnodes
	class dfa_cat_node : public dfa_node
	{
	public:
		dfa_cat_node(dfa_node *former, dfa_node *latter);
		virtual ~dfa_cat_node();

		virtual bool is_nullable() const;
		virtual dfa_node *clone() const;
	private:
		dfa_node *m_former;
		dfa_node *m_latter;
	};

	// @brief Stand for a DFA AST node that accept which one of its subnodes
	//        will accept.
	class dfa_or_node : public dfa_node
	{
	public:
		dfa_or_node(dfa_node *former, dfa_node *latter);
		virtual ~dfa_or_node();

		virtual bool is_nullable() const;
		virtual dfa_node *clone() const;
	private:
		dfa_node *m_former;
		dfa_node *m_latter;
	};

	// @brief Stand for a DFA AST node that accept empty input, or input which
	//        match its subnode one or more times.
	class dfa_star_node : public dfa_node
	{
	public:
		dfa_star_node(dfa_node *sub);
		virtual ~dfa_star_node();

		virtual dfa_node *clone() const;
		virtual bool is_nullable() const
		{
			return true;
		}
	private:
		dfa_node *m_sub;
	};


	class dfa_state
	{
	public:
		dfa_state(unsigned id);
		bool add_transite_path(unsigned input, dfa_state *state);
		const std::map<unsigned, dfa_state*> &get_transit_table() const;
		unsigned get_id() const;
	private:
		unsigned m_id;
		std::map<unsigned, dfa_state*> m_transit_table;
	};

	class dfa_machine
	{
	public:
		dfa_machine(const dfa_node *root);
		~dfa_machine();
	private:
		dfa_state *m_start;
	};
}

#endif
