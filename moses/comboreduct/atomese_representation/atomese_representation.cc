/** atomese_representation.cc ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Author: Yidnekachew Wondimu <searchyidne@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <moses/comboreduct/table/table_io.h>
#include <opencog/util/comprehension.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/truthvalue/SimpleTruthValue.h>
#include "atomese_representation.h"

namespace opencog { namespace atomese {

using namespace std;
using namespace combo;

Handle& create_repr(const vector<string>& labels, HandleSeq rows,
                    type_node type, Handle& repr)
{
	if (type == id::boolean_type)
		repr = createLink(rows, SET_LINK);
	else if (type == id::contin_type)
		repr = createLink(SIMILARITY_LINK,
		                  parse_header(labels),
		                  createLink(rows, SET_LINK));
}

Handle parse_header(const vector<string>& labels)
{
	HandleSeq cols;
	for (string str : labels)
		cols.push_back(createNode(SCHEMA_NODE, str));

	return createLink(cols, LIST_LINK);
}

Handle parse_row(const string& line, const int& row_number,
                 type_node row_type, const vector<string>& labels)
{
	string row_label = "r" + to_string(row_number);

	if (row_type == id::contin_type)
	{
		HandleSeq cols;
		for (const string& str : tokenizeRow<string>(line))
			cols.push_back(createNode(NUMBER_NODE, str));

		return createLink(LIST_LINK,
		                  createNode(NODE, row_label),
		                  createLink(cols, LIST_LINK));
	}
	else if (row_type == id::boolean_type)
	{
		HandleSeq cols;
		int i = 0;

		for (const string& str : tokenizeRow<string>(line))
		{
			string input_label = labels[i];
			Handle h = createLink(EVALUATION_LINK,
			                      createNode(PREDICATE_NODE, input_label),
			                      createNode(NODE, row_label));
			TruthValuePtr tf(SimpleTruthValue::createTV(stoi(str), 1));
			h->setTruthValue(tf);
			cols.push_back(h);
			i++;
		}

		return createLink(cols, LIST_LINK);
	}
}

type_node get_signature(istream& in)
{
	type_tree tt;
	bool has_header, is_sparse;
	const string& target_feature = "";
	const string& timestamp_feature = "";
	vector<string> ignore_features;

	streampos beg = in.tellg();
	inferTableAttributes(in, target_feature, timestamp_feature,
	                     ignore_features, tt, has_header, is_sparse);
	in.seekg(beg);

	// TODO: find a better way of finding the domain (boolean, real).
	// For instance, the inputs could be contins where as the outputs boolean.
	type_node otype = get_type_node(get_signature_output(tt));

	return otype;
}

Handle& get_atomese_representation(istream& in, Handle& repr)
{
	type_node otype = get_signature(in);

	if (otype == id::boolean_type || otype == id::contin_type)
	{
		vector<string> labels;
		HandleSeq rows;
		string line;
		int i = 0;

		while (get_data_line(in, line))
		{
			if (i == 0)
				labels = tokenizeRow<string>(line);
			else
				rows.push_back(parse_row(line, i, otype, labels));
			i++;
		}

		create_repr(labels, rows, otype, repr);
	}

	// TODO: Add other i/o varieties here.
}

Handle load_atomese(const string& file_name)
{
	OC_ASSERT(!file_name.empty(), "the file name is empty");
	ifstream in(file_name.c_str());
	OC_ASSERT(in.is_open(), "Could not open %s", file_name.c_str());

	Handle res;
	get_atomese_representation(in, res);
	return res;
}
}}
