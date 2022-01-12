/** load_table.cc ---
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

#include <opencog/asmoses/data/table/table_io.h>
#include <opencog/util/comprehension.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/core/NumberNode.h>
#include <opencog/atoms/truthvalue/SimpleTruthValue.h>
#include "load_table.h"

namespace opencog {
namespace atomese {

using namespace std;
using namespace combo;

/**
 * Represent feature labels as a list of schema nodes.
 *
 * @param labels feature labels
 * @return
 */
Handle parse_header(const vector<string>& labels)
{
	HandleSeq cols;
	for (string str : labels)
		cols.push_back(createNode(SCHEMA_NODE, str));

	return createLink(cols, LIST_LINK);
}

/**
 * Create a node representing a row having
 * the row number as its value.
 *
 * @param row_num row number
 * @return
 */
Handle mk_row_number_cell(const int row_num)
{
	return createNode(NODE, "r" + to_string(row_num));
}

/**
 * Create a NumberNode given a contin value.
 *
 * @param contin contin value of a cell
 * @return
 */
Handle mk_real_cell(const contin_t contin)
{
	return createNumberNode(contin)->get_handle();
}

/**
 * Represent a real cell as an Execution of the corresponding feature
 * on its row (i.e Schema of feature on Node of row).
 *
 * @param contin contin value of a cell
 * @param row_num row number
 * @param label label of the feature
 * @return
 */
Handle mk_real_cell_exec(const contin_t contin,
                         const int row_num, const string& label)
{
	return createLink(EXECUTION_LINK,
	                  createNode(SCHEMA_NODE, label),
	                  mk_row_number_cell(row_num),
	                  mk_real_cell(contin));
}

/**
 * Create a True / False link based on a given builtin value.
 *
 * @param b builtin value
 * @return
 */
Handle mk_boolean_cell(const builtin b)
{
	return createLink(builtin_to_bool(b) ? TRUE_LINK : FALSE_LINK);
}

/**
 * Represent a real cell as an Evaluation of the corresponding feature
 * on its row (i.e Predicate of feature on Node of row).
 *
 * @param b builtin value
 * @param row_num row number
 * @param label label of the feature
 * @return
 */
Handle mk_boolean_cell_eval(const builtin b,
                            const int row_num, const string& label)
{
	Handle h = createLink(EVALUATION_LINK,
	                      createNode(PREDICATE_NODE, label),
	                      mk_row_number_cell(row_num));
	TruthValuePtr tv(SimpleTruthValue::createTV(builtin_to_bool(b), 1));
	h->setTruthValue(tv);
	return h;
}

/**
 * Represent a boolean cell as an Execution of the corresponding feature
 * on its row (i.e Schema of feature on Node of row).
 *
 * @param b builtin value
 * @param row_num row number
 * @param label label of the feature
 * @return
 */
Handle mk_boolean_cell_exec(const builtin b,
                            const int row_num, const string& label)
{
	return createLink(EXECUTION_LINK,
	                  createNode(SCHEMA_NODE, label),
	                  mk_row_number_cell(row_num),
	                  mk_boolean_cell(b));
}

struct mk_cell_visitor : public boost::static_visitor<Handle>
{
	Handle operator()(const builtin b) const
	{
		return mk_boolean_cell(b);
	}

	Handle operator()(const contin_t contin) const
	{
		return mk_real_cell(contin);
	}

	template<typename T>
	Handle operator()(const T& t) const
	{
		return Handle::UNDEFINED;
	}
};

struct mk_exec_cell_visitor : public boost::static_visitor<Handle>
{
	// TODO: this constructor isn't needed in C++17.
	mk_exec_cell_visitor(const int row_num, const string& label)
		: row_num(row_num), label(label)
	{}

	Handle operator()(const builtin b) const
	{
		return mk_boolean_cell_exec(b, row_num, label);
	}

	Handle operator()(const contin_t contin) const
	{
		return mk_real_cell_exec(contin, row_num, label);
	}

	template<typename T>
	Handle operator()(const T& t) const
	{
		return Handle::UNDEFINED;
	}

	const int row_num;
	const string& label;
};

struct mk_cell_seq_visitor : public boost::static_visitor<HandleSeq>
{
	HandleSeq operator()(const builtin_seq& b) const
	{
		HandleSeq cellSeq;
		for (builtin cell_value : b)
			cellSeq.push_back(mk_boolean_cell(cell_value));

		return cellSeq;
	}

	HandleSeq operator()(const contin_seq& c) const
	{
		HandleSeq cellSeq;
		for (contin_t cell_value : c)
			cellSeq.push_back(mk_real_cell(cell_value));

		return cellSeq;
	}

	template<typename T>
	HandleSeq operator()(const T& t) const
	{
		return {};
	}
};

struct mk_exec_cell_seq_visitor : public boost::static_visitor<HandleSeq>
{
	// TODO: this constructor isn't needed in C++17.
	mk_exec_cell_seq_visitor(const int row_num,
	                         const vector<string>& labels)
		: row_num(row_num), labels(labels)
	{}

	HandleSeq operator()(const builtin_seq& b)
	{
		int cell_number = 0;
		HandleSeq cellSeq;
		for (builtin cell_value : b)
			cellSeq.push_back(mk_boolean_cell_exec(cell_value, row_num,
			                                       labels[cell_number++]));

		return cellSeq;
	}

	HandleSeq operator()(const contin_seq& c)
	{
		int cell_number = 0;
		HandleSeq cellSeq;
		for (contin_t cell_value : c)
			cellSeq.push_back(mk_real_cell_exec(cell_value, row_num,
			                                    labels[cell_number++]));

		return cellSeq;
	}

	template<typename T>
	HandleSeq operator()(const T& t) const
	{
		return {};
	}

	const int row_num;
	const vector<string>& labels;
};

/**
 * Represent a row as a list of cells.
 *
 * @param cells cells in a row
 * @return
 */
Handle mk_row(const HandleSeq& cells)
{
	return createLink(cells, LIST_LINK);
}

/**
 * Represent a row as a list of row number and cells
 *
 * @param cells cells in a row
 * @param row_num row number
 * @return
 */
Handle mk_numbered_row(const HandleSeq& cells, const int row_num)
{
	return createLink(LIST_LINK,
	                  mk_row_number_cell(row_num),
	                  createLink(cells, LIST_LINK));
}

/**
 * Create a table of dependent (output) data.
 *
 * @param oTable output table
 * @return
 */
Handle mk_output_table(const OTable& oTable)
{
	const string& label = oTable.get_label();
	HandleSeq rows;
	int row_num = 0;

	for (const vertex& v : oTable)
	{
		Handle h = createLink(LIST_LINK,
		                      mk_row_number_cell(row_num++),
		                      boost::apply_visitor(mk_cell_visitor(), v));
		rows.push_back(h);
	}

	return createLink(SIMILARITY_LINK,
	                  createNode(SCHEMA_NODE, label),
	                  createLink(rows, SET_LINK));
}

/**
 * Create a table of independent (input) data.
 *
 * @param iTable input table
 * @todo this method doesn't support a table of mixed types. It assumes that
 * 		 all features have similar type with the output feature.
 * @return
 */
Handle mk_input_table(const ITable& iTable)
{
	const vector<string> labels = iTable.get_labels();
	HandleSeq rows;
	int row_num = 0;

	for (const multi_type_seq& m : iTable)
	{
		HandleSeq cells = boost::apply_visitor(mk_cell_seq_visitor(),
		                                       m.get_variant());

		Handle h = createLink(LIST_LINK,
		                      mk_row_number_cell(row_num++),
		                      createLink(cells, LIST_LINK));
		rows.push_back(h);
	}

	return createLink(SIMILARITY_LINK, parse_header(labels),
	                  createLink(rows, SET_LINK));
}

/**
 * Represent the table using either similarity or list link.
 *
 * @param table
 * @param similarity use similarity link if true
 * @return
 */
HandleSeq mk_full_table(const Table& table, const bool similarity=false)
{
	HandleSeq rows;
	int row_num = 0;

	for (const multi_type_seq& m : table.itable)
	{
		HandleSeq cells = boost::apply_visitor(mk_cell_seq_visitor(),
		                                       m.get_variant());

		cells.insert(cells.begin(),
		             boost::apply_visitor(mk_cell_visitor(),
		                                  table.otable[row_num]));

		rows.push_back(
			similarity ? mk_numbered_row(cells, row_num)
			           : mk_row(cells));
		row_num++;
	}

	return rows;
}

/**
 * Represent a boolean table using EvaluationLink.
 *
 * @param table
 * @return
 */
Handle mk_unfolded_boolean_table_eval(const Table& table)
{
	HandleSeq rows;
	int row_num = 0;
	const vector<string>& labels = table.itable.get_labels();

	for (const multi_type_seq& m : table.itable)
	{
		HandleSeq cells;
		int cell_number = 0;

		cells.push_back(mk_boolean_cell_eval(
			boost::get<builtin>(table.otable[row_num]),
			row_num,
			table.otable.get_label()));

		for (builtin cell_value : m.get_seq<builtin>())
			cells.push_back(
				mk_boolean_cell_eval(cell_value, row_num,
				                     labels[cell_number++]));

		rows.push_back(mk_row(cells));
		row_num++;
	}

	return createLink(rows, SET_LINK);
}

/**
 * Represent a table without using any compression.
 *
 * @param table
 * @return
 */
Handle mk_unfolded_table(const Table& table)
{
	HandleSeq rows;
	int row_num = 0;
	const vector<string>& labels = table.itable.get_labels();

	for (const multi_type_seq& m : table.itable)
	{
		mk_exec_cell_seq_visitor cseqv{row_num, labels};
		HandleSeq cells = boost::apply_visitor(cseqv, m.get_variant());

		mk_exec_cell_visitor cv{row_num, table.otable.get_label()};
		cells.insert(cells.begin(),
		             boost::apply_visitor(cv, table.otable[row_num]));

		rows.push_back(mk_row(cells));
		row_num++;
	}

	return createLink(rows, SET_LINK);

}

Handle load_atomese_io(const string& file_name,
                       const std::string& target_feature,
                       const std::string& timestamp_feature,
                       const string_seq& ignore_features)
{
	const Table& t = loadTable(file_name, target_feature,
	                           timestamp_feature, ignore_features);

	return createLink(LIST_LINK,
	                  mk_output_table(t.otable),
	                  mk_input_table(t.itable));
}

Handle load_atomese_compact(const string& file_name,
                            const std::string& target_feature,
                            const std::string& timestamp_feature,
                            const string_seq& ignore_features)
{
	const Table& t = loadTable(file_name, target_feature,
	                           timestamp_feature, ignore_features);

	HandleSeq seq = mk_full_table(t, false);
	seq.insert(seq.begin(), parse_header(t.get_labels()));

	return createLink(EVALUATION_LINK,
	                  createNode(PREDICATE_NODE, "AS-MOSES:table"),
	                  createLink(seq, LIST_LINK));
}

Handle load_atomese_similarity(const string& file_name,
                               const std::string& target_feature,
                               const std::string& timestamp_feature,
                               const string_seq& ignore_features)
{
	const Table& t = loadTable(file_name, target_feature,
	                           timestamp_feature, ignore_features);

	return createLink(SIMILARITY_LINK,
	                  parse_header(t.get_labels()),
	                  createLink(mk_full_table(t, true), SET_LINK));
}

Handle load_atomese_unfolded(const string& file_name,
                             const std::string& target_feature,
                             const std::string& timestamp_feature,
                             const string_seq& ignore_features,
                             const bool use_eval)
{
	const Table& t = loadTable(file_name, target_feature,
	                           timestamp_feature, ignore_features);

	if (t.otable.get_type() == id::boolean_type && use_eval)
		return mk_unfolded_boolean_table_eval(t);
	else
		return mk_unfolded_table(t);
}
}
}
