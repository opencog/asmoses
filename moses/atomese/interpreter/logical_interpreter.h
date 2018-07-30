/** logical_interpreter.h ---
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


#ifndef MOSES_LOGICAL_INTERPRETER_H
#define MOSES_LOGICAL_INTERPRETER_H

namespace opencog{ namespace atomese{

/**
 * Performs logical_and of two ProtoAtomPtrs[Links].
 * this struct is expected to be used in boost::zip_iterator to perform
 * logical_and on LinkValues containing TRUE_LINK and FALSE_LINK.
 */
struct zip_and :
		public std::unary_function<const boost::tuple
				<const ProtoAtomPtr&, const ProtoAtomPtr&>&, void>
{
	std::vector<ProtoAtomPtr> _result;

	void operator()(const boost::tuple<const ProtoAtomPtr&, const ProtoAtomPtr&>& t)
	{
		if(HandleCast(t.get<0>())->get_type() == TRUE_LINK
		   && HandleCast(t.get<1>())->get_type() == TRUE_LINK){
			_result.push_back(ProtoAtomPtr(createLink(TRUE_LINK)));
		}
		else{
			_result.push_back(ProtoAtomPtr(createLink(FALSE_LINK)));
		}
	}
};

/**
 * Performs logical_or of two ProtoAtomPtrs[Links].
 * this struct is expected to be used in boost::zip_iterator to perform
 * logical_or on LinkValues containing TRUE_LINK and FALSE_LINK.
 */
struct zip_or :
		public std::unary_function<const boost::tuple
				<const ProtoAtomPtr&, const ProtoAtomPtr&>&, void>
{
	std::vector<ProtoAtomPtr> _result;

	void operator()(const boost::tuple<const ProtoAtomPtr&, const ProtoAtomPtr&>& t)
	{
		if(HandleCast(t.get<0>())->get_type() == TRUE_LINK
		   || HandleCast(t.get<1>())->get_type() == TRUE_LINK){
			_result.push_back(ProtoAtomPtr(createLink(TRUE_LINK)));
		}
		else{
			_result.push_back(ProtoAtomPtr(createLink(FALSE_LINK)));
		}
	}
};

}}
#endif //MOSES_LOGICAL_INTERPRETER_H
