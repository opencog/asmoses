/** AtomeseTypeChecker.cpp ---
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


#include "AtomeseTypeChecker.h"

#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>
#include <opencog/asmoses/combo/type_checker/type_tree_def.h>
#include <opencog/asmoses/combo/type_checker/type_tree.h>
#include <opencog/atoms/base/Node.h>

using namespace opencog::combo;

namespace opencog
{
namespace atomese
{

Handle AtomeseTypeChecker::operator()(const type_tree &tt)
{
	Handle handle;
	type_tree_pre_it it = tt.begin();
	handle = AtomeseTypeChecker::type_tree_to_atomese_type(it);
	return handle;
}

Handle AtomeseTypeChecker::convert_type_node(const type_node &tt)
{
	Handle handle;

	if (tt == id::boolean_type)
		return handle = createNode(TYPE_NODE, "BooleanNode");

	else if (tt == id::contin_type)
		return handle = createNode(TYPE_NODE, "NumberNode");

	else if (tt == id::lambda_type)
		return createLink(HandleSeq{}, ARROW_LINK);

	OC_ASSERT(false, "unsupported type");
	return Handle::UNDEFINED;
}

}
}
