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

#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/value/SeqValue.h>
#include <opencog/utils/valueUtils.h>
#include <boost/iterator/zip_iterator.hpp>

namespace opencog{ namespace atomese{

/**
 * Performs logical_and of two ValuePtrs[Links].
 * this struct is expected to be used in boost::zip_iterator to perform
 * logical_and on SeqValues containing TRUE_LINK and FALSE_LINK.
 */
struct zip_and :
		public std::unary_function<const boost::tuple
				<const ValuePtr&, const ValuePtr&>&, void>
{
	std::vector<ValuePtr> _result;

	void operator()(const boost::tuple<const ValuePtr&, const ValuePtr&>& t)
	{
		if (bool_value_to_bool(t.get<0>())
		   && bool_value_to_bool(t.get<1>())) {
			_result.push_back(ValuePtr(createLink(TRUE_LINK)));
		}
		else {
			_result.push_back(ValuePtr(createLink(FALSE_LINK)));
		}
	}
};

/**
 * Performs logical_or of two ValuePtrs[Links].
 * this struct is expected to be used in boost::zip_iterator to perform
 * logical_or on SeqValues containing TRUE_LINK and FALSE_LINK.
 */
struct zip_or :
		public std::unary_function<const boost::tuple
				<const ValuePtr&, const ValuePtr&>&, void>
{
	std::vector<ValuePtr> _result;

	void operator()(const boost::tuple<const ValuePtr&, const ValuePtr&>& t)
	{
		if (bool_value_to_bool(t.get<0>())
		   || bool_value_to_bool(t.get<1>())) {
			_result.push_back(ValuePtr(createLink(TRUE_LINK)));
		}
		else {
			_result.push_back(ValuePtr(createLink(FALSE_LINK)));
		}
	}
};

/**
 * Compute logical_and.
 * it expects the values of the SeqValues to be TRUE_LINK and FALSE_LINK
 * @param SeqValuePtr&     p1
 * @param SeqValuePtr&     p2
 *
 * @return                 SeqValue pointer containing the logical_and.
 */
SeqValuePtr logical_and(const SeqValuePtr& p1, const SeqValuePtr& p2) {
	std::vector<ValuePtr> p1_value = p1->value();
	std::vector<ValuePtr> p2_value = p2->value();

	zip_and _and = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.begin(), p2_value.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.end(), p2_value.end())
			),
			zip_and()
	);

	std::vector<ValuePtr> _result = _and._result;
	return SeqValuePtr(new SeqValue(_result));
}

/**
 * Compute logical_or.
 * it expects the values of the SeqValues to be TRUE_LINK and FALSE_LINK
 * @param SeqValuePtr&     p1
 * @param SeqValuePtr&     p2
 *
 * @return                 SeqValue pointer containing the logical_or.
 */
SeqValuePtr logical_or(const SeqValuePtr& p1, const SeqValuePtr& p2) {
	std::vector<ValuePtr> p1_value = p1->value();
	std::vector<ValuePtr> p2_value = p2->value();

	zip_or _or = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.begin(), p2_value.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.end(), p2_value.end())
			),
			zip_or()
	);

	std::vector<ValuePtr> _result = _or._result;
	return SeqValuePtr(new SeqValue(_result));
}

/**
 * Compare Contents of two link_values.
 * @param SeqValuePtr&     p1
 * @param SeqValuePtr&     p2
 *
 * @return                 boolean of the comparision.
 */
bool logical_compare(const SeqValuePtr& p1, const SeqValuePtr& p2) {
	std::vector<ValuePtr> p1_value = p1->value();
	std::vector<ValuePtr> p2_value = p2->value();

	std::function<bool (const ValuePtr&, const ValuePtr&)> comparator = []
			(const ValuePtr& left, const ValuePtr& right)
	{
		return HandleCast(left)->get_type()==HandleCast(right)->get_type();
	};

	return std::equal(p1_value.begin(), p1_value.end(), p2_value.begin(), comparator);
}

/**
 * compute logical_not.
 * it expects the values of the LinkValues to be TRUE_LINK and FALSE_LINK
 * @param LinkValuePtr&     p1
 *
 * @return                 LinkValue pointer containing the logical_not.
 */
SeqValuePtr logical_not(const SeqValuePtr& p) {
	std::vector<ValuePtr> _result;
	std::vector<ValuePtr> p_value = p->value();
	std::vector<ValuePtr>::iterator it;
	for(it = p_value.begin(); it != p_value.end(); ++it)
		_result.push_back(bool_value_to_bool(HandleCast(*it)) ?
		ValuePtr(createLink(FALSE_LINK)): ValuePtr(createLink(TRUE_LINK)));
	return SeqValuePtr(new SeqValue(_result));
}
}}
#endif //MOSES_LOGICAL_INTERPRETER_H
