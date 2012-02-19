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

	// @brief Base class of a DFA AST Node
	//
	// This class is the abstract base class for a DFA AST Node, defines common
	// interfaces for generating DFA
	class dfa_node
	{
	public:
		dfa_node();
		virtual void get_follow_nodes(std::list<dfa_node*> &l) = 0;
		virtual void get_first_nodes(std::list<dfa_node*> &l) = 0;
		virtual void get_last_nodes(std::list<dfa_node*> &l) = 0;
		virtual bool is_nullable() = 0;
	protected:
		virtual void add_follow_nodes(std::list<dfa_node*> &l) = 0;
	private:
	};
}

#endif
