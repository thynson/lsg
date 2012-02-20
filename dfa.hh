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

#include <map>
#include <set>
#include <list>

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
		typedef std::list<dfa_leaf_node*> leaf_list_t;
		typedef std::list<const dfa_leaf_node*> const_leaf_list_t;

		// @brief Destructor
		virtual ~dfa_node();

		// @brief Get possible first nodes for this node
		// @param l A list where first nodes will be store here
		virtual void get_first_nodes(const_leaf_list_t &l) const = 0;
		virtual void get_first_nodes(leaf_list_t &l) = 0;

		// @brief Get possible last nodes for this node
		// @param l A list where last nodes will be store here
		virtual void get_last_nodes(const_leaf_list_t &l) const = 0;
		virtual void get_last_nodes(leaf_list_t &l) = 0;

		// @brief If this node accept empty input
		virtual bool is_nullable() const = 0;

		// @brief Get a copy of itself
		virtual dfa_node *clone() const = 0;
	protected:
		// @brief Constructor
		dfa_node();
	};

	// @brief Stand for a DFA AST leaf node
	class dfa_leaf_node : public dfa_node
	{
	public:
		dfa_leaf_node(unsigned input);
		virtual ~dfa_leaf_node();
		virtual void get_follow_nodes(const_leaf_list_t &l) const;
		virtual void get_follow_nodes(leaf_list_t &l);

		virtual void get_first_nodes(const_leaf_list_t &l) const;
		virtual void get_first_nodes(leaf_list_t &l);

		virtual void get_last_nodes(const_leaf_list_t &l) const;
		virtual void get_last_nodes(leaf_list_t &l);

		virtual bool is_nullable() const;

		// @brief Add follow node for this node
		// @param l Ths list of additional possible follow nodes
		virtual void add_follow_nodes(const leaf_list_t &l);

		unsigned get_input() const;
	private:
		leaf_list_t m_follow_nodes;
		unsigned m_input;
	};

	// @brief Stand for a DFA AST node that catenate its two subnodes
	class dfa_cat_node : public dfa_node
	{
	public:
		dfa_cat_node(dfa_node *former, dfa_node *latter);
		virtual ~dfa_cat_node();

		virtual void get_first_nodes(const_leaf_list_t &l) const;
		virtual void get_first_nodes(leaf_list_t &l);

		virtual void get_last_nodes(const_leaf_list_t &l) const;
		virtual void get_last_nodes(leaf_list_t &l);

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

		virtual void get_first_nodes(const_leaf_list_t &l) const;
		virtual void get_first_nodes(leaf_list_t &l);

		virtual void get_last_nodes(const_leaf_list_t &l) const;
		virtual void get_last_nodes(leaf_list_t &l);

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

		virtual void get_first_nodes(const_leaf_list_t &l) const;
		virtual void get_first_nodes(leaf_list_t &l);

		virtual void get_last_nodes(const_leaf_list_t &l) const;
		virtual void get_last_nodes(leaf_list_t &l);

		virtual dfa_node *clone() const;
		virtual bool is_nullable() const
		{
			return true;
		}
	private:
		dfa_node *m_sub;
	};
}

#endif
