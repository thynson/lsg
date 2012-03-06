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

#ifndef __DUMPER_HH__
#define __DUMPER_HH__

#include "dfa.hh"
#include <iostream>


namespace lsg
{
	class dumper
	{
	public:
		dumper(dfa_machine *m);
		virtual ~dumper();
		void dump(std::ostream &os);
	private:
		virtual void dump_state(std::ostream &os, dfa_state *s) = 0;
		virtual void dump_prolog(std::ostream &os, unsigned state_count) = 0;
		virtual void dump_postscript(std::ostream &os) = 0;
		dfa_machine *m_machine;
	};

	class pretty_dumper : public dumper
	{
	public:
		pretty_dumper(dfa_machine *m);
		virtual ~pretty_dumper();
	private:
		virtual void dump_state(std::ostream &os, dfa_state *s);
		virtual void dump_prolog(std::ostream &os, unsigned state_count);
		virtual void dump_postscript(std::ostream &os);
	};

	class c_dumper : public dumper
	{
	public:
		c_dumper(dfa_machine *m);
		virtual ~c_dumper();
		void set_var_ctx(const std::string &name);
		void set_func_get_input(const std::string &name);
		void set_func_unget_input(const std::string &name);
		void set_func_satisfy_rule(const std::string &name);
		void set_func_set_state(const std::string &name);
		void set_func_get_state(const std::string &name);
		void set_macro_again(const std::string &name);
	private:
		virtual void dump_state(std::ostream &os, dfa_state *s);
		virtual void dump_prolog(std::ostream &os, unsigned state_count);
		virtual void dump_postscript(std::ostream &os);

		// int
		std::string m_type_return;

		// LSG_TYPE
		std::string m_type_param;

		// LSG_SELF
		std::string m_self;

		// Default "LSG_CTX"
		std::string m_ctx;

		// Default to "LSG_GET_INPUT" (ctx)
		std::string m_get_input;

		// Default to "LSG_UNGET_INPUT"(ctx)
		// Shift the last input
		std::string m_unget_input;

		// Default to "LSG_SATISFY_RULE" (ctx, RULEID)
		std::string m_satisfy_rule;

		// LSG_GET_STATE(ctx)
		std::string m_get_state;

		// LSG_SET_STATE(ctx, state)
		std::string m_set_state;

		// LSG_AGAIN(ctx)
		std::string m_again;
	};

}

#endif
