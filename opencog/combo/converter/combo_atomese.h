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

#include <opencog/atoms/base/Handle.h>
#include <opencog/combo/combo/vertex.h>
#include <opencog/atoms/base/Link.h>


namespace opencog { namespace combo {

namespace id {
//    Atomese procedure types
enum __attribute__((packed)) procedure_type{
	unknown=0,
	predicate,
	schema
};

}

struct vertex_2_atom : boost::static_visitor<std::pair<Type, Handle>>
{
public:
	vertex_2_atom (id::procedure_type* parent, AtomSpace* as=nullptr);
	std::pair<Type, Handle> operator()(const argument& a) const;
	std::pair<Type, Handle> operator()(const builtin& b) const;
	std::pair<Type, Handle> operator()(const enum_t& e) const;
	std::pair<Type, Handle> operator()(const contin_t& c) const;
	template <typename T>
	std::pair<Type, Handle> operator()(const T&) const
	{
		OC_ASSERT(false, "Not Implemented Yet");
		return std::pair<Type, Handle>();
	}

private:
	AtomSpace* _as;
	mutable id::procedure_type* _parent;
};

class ComboToAtomeseConverter
{
public:
	ComboToAtomeseConverter(AtomSpace* as=nullptr);

	/**
	 * Convert a combo_tree to atomese program.
	 *
	 * @param combo_tree   the combo_tree containing the combo program
	 * @return                 the Handle containing the atomese program
	 */
	Handle operator()(const combo_tree &tr);

private:
	AtomSpace* _as;

protected:
	/**
	 * Convert a combo_tree to atomese from a head of a combo_tree program.
	 *
	 * @param combo_tree::iterator   the iterater to the head of a combo_tree
	 * @return                 the Handle containing the atomese program
	 */
	template<typename Iter>
	opencog::Handle atomese_combo_it(Iter it,
	                                 id::procedure_type &parent_procedure_type)
	{

		id::procedure_type procedure_type = parent_procedure_type;
		combo_tree::iterator head = it;
		std::pair <Type, Handle> atomese = boost::apply_visitor(vertex_2_atom(&procedure_type, _as), *head);
		Type link_type = atomese.first;
		Handle handle = atomese.second;

		if (link_type != (unsigned short) -1) {
			HandleSeq handle_seq;
			for (auto sib = head.begin(); sib != head.end(); ++sib) {
				handle_seq.push_back(atomese_combo_it(sib, procedure_type));
			}
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
	std::pair<combo_tree, std::vector<std::string>> operator()(const Handle &h);

protected:

	void atom2combo(const Handle &h, std::vector<std::string> &labels, combo_tree &tr,
	                combo_tree::iterator &iter);
};
}}  // ~namespaces combo opencog

#endif //MOSES_COMBO_ATOMESE_H
