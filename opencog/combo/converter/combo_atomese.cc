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
#include <opencog/combo/combo/vertex.h>
#include <opencog/atomspace/AtomSpace.h>
#include "combo_atomese.h"


namespace opencog { namespace combo {

using namespace std;
using namespace boost;

ComboToAtomeseConverter::ComboToAtomeseConverter(AtomSpace *as)
		:_as(as)
{}

Handle ComboToAtomeseConverter::operator()(const combo_tree &ct)
{
	Handle handle;
	combo_tree::iterator it = ct.begin();
	id::procedure_type ptype = id::procedure_type::unknown;
	handle = atomese_combo_it(it, ptype);
	return handle;
}

vertex_2_atom::vertex_2_atom(id::procedure_type* parent, AtomSpace *as)
		: _as(as), _parent(parent)
{}

std::pair<Type, Handle> vertex_2_atom::operator()(const argument &a) const
{
	Handle handle;
	if(*_parent == id::unknown) {
		*_parent = id::predicate;
	}
	switch (*_parent) {
		case id::predicate: {
			Handle tmp = createNode(PREDICATE_NODE,
			                        to_string(a.is_negated() ? -a.idx : a.idx));
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
			Handle tmp = createNode(SCHEMA_NODE, to_string(a.is_negated() ? -a.idx : a.idx));
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
		case id::logical_true:
			*_parent = id::predicate;
			type = TRUE_LINK;
			break;
		case id::logical_false:
			*_parent = id::predicate;
			type = FALSE_LINK;
			break;
		default:
			OC_ASSERT(false, "unsupported");
	}
	return std::make_pair(type, Handle());
}

std::pair<Type, Handle> vertex_2_atom::operator()(const contin_t& c) const
{
	return std::make_pair(-1, Handle(createNumberNode(c)));
}

std::pair<Type, Handle> vertex_2_atom::operator()(const enum_t &e) const
{
	return pair<Type, Handle>();
}

}}  // ~namespaces combo opencog
