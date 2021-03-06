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

#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <getopt.h>
#include "lsg.hh"
#include "dumper.hh"
#include "config.h"

using namespace std;
using namespace lsg;

namespace
{
	enum {
		CO_START = 256, // Not in use, just make value of follow flags larger
		CO_INPUT,
		CO_OUTPUT,
		CO_OUTPUT_HEADER,
		CO_LANG,
		CO_VERSION,
		CO_HELP
	};

	string output_filename("");
	string input_filename("");
	string output_language("pretty");
	string output_header("");

	const struct option long_opts[] = {
		{"input", 1, NULL, CO_INPUT},
		{"output", 1, NULL, CO_OUTPUT},
		{"output-header", 1, NULL, CO_OUTPUT_HEADER},
		{"lang", 1, NULL, CO_LANG},
		{"version", 0, NULL, CO_VERSION},
		{"help", 0, NULL, CO_HELP},
		{NULL, 0, NULL, 0}
	};

	void print_usage(const char *argv0)
	{
		// Calculate the base name of this program

		char name[PATH_MAX];
		strncpy(name, argv0, sizeof(name));

		cout << basename(name) << " usage:" << endl
			 << " -i FILE --input=FILE    Read from FILE other than stdin\n"
			 << " -o FILE --output=FILE   Output to FILE other than stdout\n"
			 << " -H HEADER --output-header=HEADER\n"
			 << "                         Export symbol to HEADER, only\n"
			 << "                         available when dump with LANG=c\n"
			 << " -l LANG --output=LANG   Dump in LANG language\n"
			 << "                         LANG can be (pretty, c)\n"
			 << " -v --version            Show version\n"
			 << " -h --help               Print help\n"
			 << endl;

	}

	void print_version(const char *argv0)
	{
		char name[PATH_MAX];
		strncpy(name, argv0, sizeof(name));

		cout << basename(name) << " version: " << VERSION << endl;
		cout << "Copyright (C) 2012 LAN Xingcan\n\n";
		cout <<
	"Permission to use, copy, modify, and/or distribute this software for any\n"
	"purpose with or without fee is hereby granted, provided that the above\n"
	"copyright notice and this permission notice appear in all copies.\n"
	"\n"
	"THE SOFTWARE IS PROVIDED \"AS IS\" AND THE AUTHOR DISCLAIMS ALL WARRANTIES\n"
	"WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\n"
	"MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR\n"
	"ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\n"
	"WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN\n"
	"ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF\n"
	"OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n\n"
		;

	}

	void parse_cmd_opt(int argc, char **argv)
	{
		for ( ; ; )
		{
			int optidx;
			int ret = getopt_long(argc, argv, "i:o:H:l:vh",
								  long_opts, &optidx);

			switch(ret)
			{
			case 'i':
			case CO_INPUT:
				input_filename = optarg;
				break;

			case 'o':
			case CO_OUTPUT:
				output_filename = optarg;
				break;

			case 'H':
			case CO_OUTPUT_HEADER:
				output_header = optarg;
				break;

			case 'l':
			case CO_LANG:
				output_language = optarg;
				break;

			case 'v':
			case CO_VERSION:
				print_version(argv[0]);
				exit(EXIT_SUCCESS);
				break;

			case 'h':
			case CO_HELP:
				print_usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;

			case -1:
				return;

			default:
				cerr << "Unknown option '" << argv[optind] << "'" << endl;
				exit(EXIT_FAILURE);
				break;
			}
		}
	}
}

int main(int argc, char **argv)
{
	FILE *fin = stdin;

	parse_cmd_opt(argc, argv);

	if (input_filename != "")
		fin = fopen(input_filename.c_str(), "r");

	lexer_set_stream(fin);

	dfa_node *root;
	map<string, unsigned> rule_map;

	start_parse(&root, &rule_map);

	dfa_machine m(root);

	dumper *d;

	if (output_language == "pretty")
		d = new pretty_dumper(&m);
	else if (output_language == "c")
	{
		c_dumper *c = new c_dumper(&m, rule_map);
		if (output_header != "")
		{
			fstream fheader(output_header.c_str(), fstream::out);
			c->dump_header(fheader);
		}
		d = c;
	}
	else
	{
		cerr << "FATAL ERROR: Unsupported output language, abort" << endl;
		abort();
	}


	if (output_filename == "")
		d->dump(cout);
	else
	{
		fstream fout(output_filename.c_str(), fstream::out);
		d->dump(fout);
	}

	delete d;
	delete root;

}
