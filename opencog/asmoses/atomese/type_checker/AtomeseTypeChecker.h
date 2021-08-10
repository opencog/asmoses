/** AtomeseTypeChecker.h ---
 *
 * Copyright (C) 2019 OpenCog Foundation
 *
 * Authors: Kiros Hailay <kiroshailay12@gmail.com>
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


#ifndef ASMOSES_ATOMESETYPECHECKER_H
#define ASMOSES_ATOMESETYPECHECKER_H

#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>
#include <opencog/asmoses/combo/type_checker/type_tree_def.h>
#include <opencog/asmoses/combo/type_checker/type_tree.h>


using namespace opencog::combo;


namespace opencog
{
namespace atomese
{

class AtomeseTypeChecker
{

public:
	/**
	* converts the type tree to corresponding atomese type
	* @param tt              type tree of the combo program
	* @return handle       atomese type converted from typetree
	*/
	Handle operator()(const type_tree &tt);

	/**
	* convert each type node to corresponding atomese type
	* @param tt        takes type node as argument then change
	*                  to the corresponding to atomese type
	* @return handle    corresponding to each  type_node
	*/
	Handle convert_type_node(const type_node &tt);

protected:

	/**
	* Convert a type_tree to atomese type from a head of the type_tree.
	*
	* @param type_tree::iterator   the iterater to the head of a type_tree
	* @return               the Handle containing the atomese type
	*/
	template<typename Iter>
	Handle type_tree_to_atomese_type(Iter it)
	{
		type_tree::iterator head = it;
		Handle atomeseType = convert_type_node(*head);

		if (atomeseType->get_type() == ARROW_LINK) {
			HandleSeq handleSeq;

			for (auto sib = head.begin(); sib != head.end(); ++sib)
				handleSeq.push_back(type_tree_to_atomese_type(sib));

			Handle lst = createLink(HandleSeq(handleSeq.begin(), handleSeq.end() - 1), LIST_LINK);
			atomeseType = createLink(HandleSeq{lst, handleSeq.back()}, ARROW_LINK);
		}

		return atomeseType;
	}

};

}
}

#endif //ASMOSES_ATOMESETYPECHECKER_H
