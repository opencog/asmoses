/** logical_interpreter.cc ---
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


#include "logical_interpreter.h"

namespace opencog { namespace atomese {

void zip_and::operator()(const boost::tuple<const ValuePtr&, const ValuePtr&>& t)
{
	if (bool_value_to_bool(t.get<0>())
	    && bool_value_to_bool(t.get<1>())) {
		_result.push_back(Constants::true_value);
	}
	else {
		_result.push_back(Constants::false_value);
	}
}

void zip_or::operator()(const boost::tuple<const ValuePtr&, const ValuePtr&>& t)
{
	if (bool_value_to_bool(t.get<0>())
	    || bool_value_to_bool(t.get<1>())) {
		_result.push_back(Constants::true_value);
	}
	else {
		_result.push_back(Constants::false_value);
	}
}

void zip_greater_than::operator()(const boost::tuple<const double&, const double&>& t)
{
	_result.push_back(t.get<0>() > t.get<1>() ?
	                  Constants::true_value: Constants::false_value);
}

LinkValuePtr logical_and(const LinkValuePtr& p1, const LinkValuePtr& p2)
{
	ValueSeq p1_value = p1->value();
	ValueSeq p2_value = p2->value();

	zip_and _and = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.begin(), p2_value.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.end(), p2_value.end())
			),
			zip_and()
	);

	ValueSeq _result = _and._result;
	return LinkValuePtr(new LinkValue(_result));
}

LinkValuePtr logical_or(const LinkValuePtr& p1, const LinkValuePtr& p2) {
	ValueSeq p1_value = p1->value();
	ValueSeq p2_value = p2->value();

	zip_or _or = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.begin(), p2_value.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.end(), p2_value.end())
			),
			zip_or()
	);

	ValueSeq _result = _or._result;
	return LinkValuePtr(new LinkValue(_result));
}

bool logical_compare(const LinkValuePtr& p1, const LinkValuePtr& p2) {
	ValueSeq p1_value = p1->value();
	ValueSeq p2_value = p2->value();

	std::function<bool (const ValuePtr&, const ValuePtr&)> comparator = []
			(const ValuePtr& left, const ValuePtr& right)
	{
		return HandleCast(left)->get_type()==HandleCast(right)->get_type();
	};

	return std::equal(p1_value.begin(), p1_value.end(), p2_value.begin(), comparator);
}

LinkValuePtr logical_not(const LinkValuePtr& p) {
	ValueSeq _result;
	ValueSeq p_value = p->value();
	ValueSeq::iterator it;
	for(it = p_value.begin(); it != p_value.end(); ++it)
		_result.push_back(bool_value_to_bool(HandleCast(*it)) ?
		Constants::false_value : Constants::true_value);
	return LinkValuePtr(new LinkValue(_result));
}

LinkValuePtr greater_than(const FloatValuePtr& p1, const FloatValuePtr& p2)
{
	std::vector<double> p1_value = p1->value();
	std::vector<double> p2_value = p2->value();

	zip_greater_than g_than = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.begin(), p2_value.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.end(), p2_value.end())
			),
			zip_greater_than()
	);

	ValueSeq _result = g_than._result;
	return LinkValuePtr(new LinkValue(_result));
}

}}
