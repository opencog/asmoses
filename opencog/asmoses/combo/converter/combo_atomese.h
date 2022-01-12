/** combo_atomese.h ---
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


#ifndef MOSES_COMBO_ATOMESE_H
#define MOSES_COMBO_ATOMESE_H

#include <opencog/util/empty_string.h>

#include <opencog/atoms/base/Handle.h>
#include <opencog/asmoses/combo/combo/vertex.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>

namespace opencog
{
namespace combo
{

namespace id
{
//    Atomese procedure types
enum __attribute__((packed)) procedure_type
{
	unknown = 0,
	predicate,
	schema
};

}

struct vertex_2_atom : boost::static_visitor<std::pair<Type, Handle>>
{
public:
	vertex_2_atom(id::procedure_type *parent, AtomSpacePtr as=nullptr,
	              const string_seq &labels={},
	              type_node output_type=id::boolean_type);
	std::pair<Type, Handle> operator()(const argument &a) const;

	std::pair<Type, Handle> operator()(const builtin &b) const;

	std::pair<Type, Handle> operator()(const enum_t &e) const;

	std::pair<Type, Handle> operator()(const contin_t &c) const;

	template<typename T>
	std::pair<Type, Handle> operator()(const T &) const
	{
		OC_ASSERT(false, "Not Implemented Yet");
		return std::pair<Type, Handle>();
	}

private:
	AtomSpacePtr _as;
	const string_seq &_labels;
	mutable id::procedure_type *_parent;
	type_node _out_type;
};

class ComboToAtomese
{
public:
	ComboToAtomese();
	ComboToAtomese(AtomSpacePtr as);
	ComboToAtomese(type_node output_type);

	/**
	 * Convert a combo_tree to atomese program.
	 *
	 * @param combo_tree   the combo_tree containing the combo program
	 * @return                 the Handle containing the atomese program
	 */
	Handle operator()(const combo_tree &tr, const string_seq &labels={});

private:
	AtomSpacePtr _as;
	type_node _output_type;

protected:
	/**
	 * Convert a combo_tree to atomese from a head of a combo_tree program.
	 *
	 * @param combo_tree::iterator   the iterater to the head of a combo_tree
	 * @return                 the Handle containing the atomese program
	 */
	template<typename Iter>
	opencog::Handle atomese_combo_it(Iter it,
	                                 id::procedure_type &parent_procedure_type,
	                                 const string_seq &labels)
	{

		id::procedure_type procedure_type = parent_procedure_type;
		combo_tree::iterator head = it;
		std::pair<Type, Handle> atomese = boost::apply_visitor(vertex_2_atom(&procedure_type, _as, labels, _output_type), *head);
		Type link_type = atomese.first;
		Handle handle = atomese.second;

		if (link_type != (unsigned short) -1) {
			HandleSeq handle_seq;
			for (auto sib = head.begin(); sib != head.end(); ++sib) {
				handle_seq.push_back(atomese_combo_it(sib, procedure_type, labels));
			}
			// Combos that are converted to the atomese GreaterThanLink are actually
			// greater_than_zero combo_vertices. So we need to add a NumberNode zero
			// to the atomese GreaterThanLink to keep the semantix.
			if (link_type == GREATER_THAN_LINK) handle_seq.push_back(createNode(NUMBER_NODE, "0.0"));
			handle = createLink(handle_seq, link_type);
		}
		return handle;
	}

};

class AtomeseToCombo
{
public:
	/**
	 * Convert a atomese to combo_tree program.
	 *
	 * @param Handle           the Handle containing the atomese program
	 * @return pair<combo_tree, vector>      the combo_tree converted from the
	 *                                       atomese program and labels
	 */
	std::pair<combo_tree, string_seq> operator()(const Handle &h);

protected:

	void atom2combo(const Handle &h, string_seq &labels, combo_tree &tr,
	                combo_tree::iterator &iter);

	void link2combo(const Handle &h, string_seq &labels, combo_tree &tr,
	                combo_tree::iterator &iter);

	void node2combo(const Handle &h, string_seq &labels, combo_tree &tr,
	                combo_tree::iterator &iter);

};

}  // ~namespace combo

// For pretty printing OpenCog objects while debugging, see
// https://wiki.opencog.org/w/Development_standards#Pretty_Print_OpenCog_Objects
std::string oc_to_string(const std::pair<combo::combo_tree, combo::string_seq>& ctr_labels,
                         const std::string& indent=empty_string);
}  // ~namespace opencog

#endif //MOSES_COMBO_ATOMESE_H
