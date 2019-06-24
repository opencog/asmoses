/** condlink_interpreter.cc ---
 *
 * Copyright (C) 2019 OpenCog Foundation
 *
 * Authors: Kiros Hailay <kiros.hailay12@gmail.com>
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

#include "condlink_interpreter.h"

namespace opencog
{
namespace atomese
{

ValueSeq condlink_exec_linkvalue(const LinkValuePtr &conds,
                                 const LinkValuePtr &exps,
                                 const LinkValuePtr &default_exp)
{
	ValueSeq cond = conds->value();
	ValueSeq exp = LinkValueCast(exps)->value();
	ValueSeq default_ex = LinkValueCast(default_exp)->value();

	zip_cond _cond = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(cond.begin(),
					                  exp.begin(), default_ex.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(cond.end(),
					                  exp.end(), default_ex.end())
			),
			zip_cond()

	);
	ValueSeq _result = _cond._result;
	return _result;
}

void zip_cond::operator()(const boost::tuple<const ValuePtr &,
		const ValuePtr &, const ValuePtr &> &t)
{
	if (bool_value_to_bool(t.get<0>())) {
		_result.push_back(t.get<1>());
	} else {
		_result.push_back(t.get<2>());
	}

}

void zip_cond2::operator()(const boost::tuple<const ValuePtr &,
		const double &, const double &> &t)
{
	if (bool_value_to_bool(t.get<0>())) {
		_result.push_back(t.get<1>());
	} else {
		_result.push_back(t.get<2>());
	}
}

std::vector<double> condlink_exec_floatvalue(const LinkValuePtr &conds,
                                             const FloatValuePtr &exps,
                                             const FloatValuePtr &default_exp)
{
	ValueSeq cond = conds->value();
	std::vector<double> exp = exps->value();
	std::vector<double> default_ex = default_exp->value();
	zip_cond2 _cond = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(cond.begin(),
					                  exp.begin(), default_ex.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(cond.end(), exp.end(), default_ex.end())
			),
			zip_cond2()

	);
	std::vector<double> _result = _cond._result;
	return _result;
}

}
}
