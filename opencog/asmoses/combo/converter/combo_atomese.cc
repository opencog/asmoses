/** combo_atomese.cc ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Authors: Kasim Ebrahim <se.kasim.ebrahim@gmail.com>
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


#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/atoms/core/NumberNode.h>
#include <opencog/asmoses/combo/combo/vertex.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/asmoses/combo/combo/iostream_combo.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>
#include <opencog/asmoses/utils/valueUtils.h>
#include "combo_atomese.h"

namespace opencog
{
namespace combo
{

using namespace std;
using namespace boost;

ComboToAtomese::ComboToAtomese()
	: _as(nullptr)
{
}

ComboToAtomese::ComboToAtomese(AtomSpacePtr as)
	: _as(as)
{}

ComboToAtomese::ComboToAtomese(type_node output_type)
	: _as(nullptr), _output_type(output_type)
{
}

Handle ComboToAtomese::operator()(const combo_tree &ct,
                                  const string_seq &labels)
{
	Handle handle;
	combo_tree::iterator it = ct.begin();
	id::procedure_type ptype = id::procedure_type::unknown;
	handle = atomese_combo_it(it, ptype, labels);
	return handle;
}

vertex_2_atom::vertex_2_atom(id::procedure_type *parent, AtomSpacePtr as,
                             const string_seq &labels, type_node output_type)
	: _as(as), _labels(labels), _parent(parent), _out_type(output_type)
{}

std::pair<Type, Handle> vertex_2_atom::operator()(const argument &a) const
{
	Handle handle;
	if (*_parent == id::unknown) {
		switch (_out_type){
			case id::contin_type:{
				*_parent = id::schema;
				break;
			}
			default:{
				*_parent = id::predicate;
				break;
			}
		}
	}
	switch (*_parent) {
		case id::predicate: {
			string var_name;
			if (!_labels.empty()) {
				var_name = "$" + _labels[(a.is_negated() ? -a.idx : a.idx) -1];
			} else var_name = "$" + to_string(a.is_negated() ? -a.idx : a.idx);

			Handle tmp = createNode(PREDICATE_NODE, var_name);
			if (a.is_negated()) {
				HandleSeq handle_seq;
				handle_seq.push_back(tmp);
				handle = createLink(handle_seq, NOT_LINK);
			} else {
				handle = tmp;
			}
			break;
		}
		case id::schema: {
			string var_name;
			if (!_labels.empty()) {
				var_name = "$" + _labels[(a.is_negated() ? -a.idx : a.idx) -1];
			} else var_name = "$" + to_string(a.is_negated() ? -a.idx : a.idx);

			Handle tmp = createNode(SCHEMA_NODE, var_name);
			if (a.is_negated()) {
				HandleSeq handle_seq;
				handle_seq.push_back(tmp);
				handle_seq.push_back(createNode(NUMBER_NODE, "-1"));
				handle = createLink(handle_seq, TIMES_LINK);
			} else {
				handle = tmp;
			}
			break;
		}
		default: {
			OC_ASSERT(false, "unsupported procedure type");
		}
	}
	std::string test1 = oc_to_string(handle);

	handle = _as ? _as->add_atom(handle) : handle;

	return std::make_pair(-1, handle);
}

std::pair<Type, Handle> vertex_2_atom::operator()(const builtin &b) const
{
	Type type = -1;
	switch (b) {
		case id::logical_and:
			*_parent = id::predicate;
			type = AND_LINK;
			break;
		case id::logical_or:
			*_parent = id::predicate;
			type = OR_LINK;
			break;
		case id::logical_not:
			*_parent = id::predicate;
			type = NOT_LINK;
			break;
		case id::plus:
			*_parent = id::schema;
			type = PLUS_LINK;
			break;
		case id::times:
			*_parent = id::schema;
			type = TIMES_LINK;
			break;
		case id::div:
			*_parent = id::schema;
			type = DIVIDE_LINK;
			break;
		case id::exp:
			*_parent = id::schema;
			type = EXP_LINK;
			break;
		case id::sin:
			*_parent = id::schema;
			type = SIN_LINK;
			break;
		case id::logical_true:
			*_parent = id::predicate;
			type = TRUE_LINK;
			break;
		case id::logical_false:
			*_parent = id::predicate;
			type = FALSE_LINK;
			break;
		case id::impulse:
			*_parent = id::predicate;
			type = MPULSE_LINK;
			break;
		case id::greater_than_zero:
			*_parent = id::schema;
			type = GREATER_THAN_LINK;
			break;
		default: OC_ASSERT(false, "unsupported");
	}
	return std::make_pair(type, Handle());
}

std::pair<Type, Handle> vertex_2_atom::operator()(const contin_t &c) const
{
	return std::make_pair(-1, Handle(createNumberNode(c)));
}

std::pair<Type, Handle> vertex_2_atom::operator()(const enum_t &e) const
{
	return pair<Type, Handle>();
}

std::pair<combo_tree, string_seq> AtomeseToCombo::operator()(const Handle &h)
{
	combo_tree tr;
	auto begin = tr.begin();
	vector<string> labels = {};
	atom2combo(h, labels, tr, begin);

	return std::make_pair(tr, labels);
}

void AtomeseToCombo::atom2combo(const Handle &h, string_seq &labels,
                                combo_tree &tr, combo_tree::iterator &iter)
{
	const auto prev = iter;
	if (h->get_type() == KNOB_LINK and
	    bool_value_to_bool(h->getOutgoingAtom(3))) {
		atom2combo(h->getOutgoingAtom(1), labels, tr, iter);
		return;
	}
	else if (h->is_link()) {
		link2combo(h, labels, tr, iter);
		for (auto child : h->getOutgoingSet()) {
			atom2combo(child, labels, tr, iter);
			if (h->get_type() == GREATER_THAN_LINK) break;
		}
	} else {
		node2combo(h, labels, tr, iter);
		return;
	}
	iter = prev;
}

void AtomeseToCombo::link2combo(const Handle &h, string_seq &labels,
                                combo_tree &tr, combo_tree::iterator &iter)
{
	const Type t = h->get_type();
	if (PLUS_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::plus) : tr.append_child(iter, id::plus);
		return;
	}
	if (TIMES_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::times) : tr.append_child(iter, id::times);
		return;
	}
	if (DIVIDE_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::div) : tr.append_child(iter, id::div);
		return;
	}
	if (SIN_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::sin) : tr.append_child(iter, id::sin);
		return;
	}
	if (EXP_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::exp) : tr.append_child(iter, id::exp);
		return;
	}
	if (NOT_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::logical_not) : tr.append_child(iter, id::logical_not);
		return;
	}
	if (TRUE_LINK == t) {
		if (tr.empty()) tr.set_head(id::logical_true);
		else tr.append_child(iter, id::logical_true);
		return;
	}
	if (FALSE_LINK == t) {
		if (tr.empty()) tr.set_head(id::logical_false);
		else tr.append_child(iter, id::logical_false);
		return;
	}
	if (AND_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::logical_and) : tr.append_child(iter, id::logical_and);
		return;
	}
	if (OR_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::logical_or) : tr.append_child(iter, id::logical_or);
		return;
	}
	if (MPULSE_LINK == t) {
		iter = tr.empty() ? tr.set_head(id::impulse) : tr.append_child(iter, id::impulse);
		return;
	}
	if (GREATER_THAN_LINK == t) {
		OC_ASSERT(h->getOutgoingSet().size() == 2)
		OC_ASSERT(content_eq(h->getOutgoingSet().at(1), Handle(createNumberNode(0.0))),
		          "Only greater_than_zero is supported in combo!")
		iter = tr.empty() ? tr.set_head(id::greater_than_zero) :
		       tr.append_child(iter, id::greater_than_zero);
		return;
	} else {
		iter = tr.empty() ? tr.set_head(id::null_vertex) : tr.append_child(iter, id::null_vertex);
		return;
	}
}

void AtomeseToCombo::node2combo(const Handle &h, string_seq &labels,
                                combo_tree &tr, combo_tree::iterator &iter)
{
	Type t = h->get_type();

	if (PREDICATE_NODE == t || SCHEMA_NODE == t ||
	    VARIABLE_NODE == t || GLOB_NODE == t) {
		const auto label = parse_combo_variables(h->get_name())[0];
		// The argument idx must be the index of label in labels plus one.
		auto i = std::find(labels.begin(), labels.end(), label) - labels.begin() + 1;
		// If label exists in labels already we dont want to add it.
		if (i > (int)labels.size()) labels.push_back(label);

		if (tr.empty()) tr.set_head(argument(i));
		else tr.append_child(iter, argument(i));

		return;
	}
	if (NUMBER_NODE == t) {
		if (tr.empty()) tr.set_head(NumberNodeCast(h)->get_value());
		else tr.append_child(iter, NumberNodeCast(h)->get_value());

		return;
	}
	if (TYPE_NODE == t) {
		if (tr.empty()) tr.set_head(id::null_vertex);
		else tr.append_child(iter, id::null_vertex);
	} else OC_ASSERT(false, "unsupported type");
}

}  // ~namespace combo

std::string oc_to_string(const std::pair<combo::combo_tree, string_seq>& ctr_labels,
                         const std::string& indent)
{
	std::stringstream ss;

	// Due to what is apparently a gcc bug, we need to pass the combo
	// tree by copy, otherwise, if passed by const reference we get the following error
	//
	// 	error: call of overloaded ‘oc_to_string(const combo_tree&)’ is ambiguous
	//      << oc_to_string(ctr) << std::endl;
	//                         ^
	// In file included from /home/nilg/OpenCog/asmoses/opencog/combo/converter/combo_atomese.cc:24:
	// Handle.h:321:13: note: candidate: ‘std::__cxx11::string opencog::oc_to_string(const HandleCounter&, const string&)’
	//  std::string oc_to_string(const HandleCounter& hc,
	//              ^~~~~~~~~~~~
	// Handle.h:323:13: note: candidate: ‘std::__cxx11::string opencog::oc_to_string(const HandleUCounter&, const string&)’
	//  std::string oc_to_string(const HandleUCounter& huc,
	//              ^~~~~~~~~~~~
	//
	// Copying by const also gives the same error.
	combo::combo_tree ctr = ctr_labels.first;
	ss << indent << "combo tree:" << std::endl;
	std::string header = indent + oc_to_string_indent;
	ss << header << oc_to_string(ctr) << std::endl;
	ss << indent << "labels:";
	for (const std::string& label : ctr_labels.second)
		ss << " " << label;
	return ss.str();
}

}  // ~namespace opencog
