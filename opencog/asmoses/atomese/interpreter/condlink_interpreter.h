/** condlink_interpreter.h ---
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

#ifndef ASMOSES_CONDLINK_INTERPRETER_H
#define ASMOSES_CONDLINK_INTERPRETER_H

#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/value/LinkValue.h>
#include <opencog/asmoses/utils/valueUtils.h>
#include <boost/iterator/zip_iterator.hpp>

namespace opencog { namespace atomese {

/**
 * this struct is expected to be used in boost::zip_iterator to perform
 * condlink on LinkValuePtr
 * and it has method operator overloading that takes three ValuePtr
 * which are the conditions, experssions, and defaults
 *
*/
struct zip_cond
{
    ValueSeq _result;
    void operator()(const boost::tuple<const ValuePtr &,
                    const ValuePtr &, const ValuePtr &> &t);
};

/**
 * this struct is expected to be used in boost::zip_iterator to perform
 * condlink on FloatValuePtr
 * and it has method operator overloading that takes 3 parametrs
 * which are the conditions, experssions, and defaults
 *
*/
struct zip_cond2
{
    std::vector<double> _result;
    void operator()(const boost::tuple<const ValuePtr &,
                    const double &, const double &> &t);

};

/**
 * this function works when input columns are Link values
 * @param   conds LinkValuePtr& conds
 * @param   exps  LinkValuePtr& exps
 * @param   default_exp   LinkValuePtr& default_exp
 * @return  vector of valuePtr
 */
ValueSeq condlink_exec_linkvalue(const LinkValuePtr &conds,
                                 const LinkValuePtr &exps,
                                 const LinkValuePtr &default_exp);

/**
 * this function works when input columns are floatValues
 * @param   conds LinkValuePtr& conds
 * @param   exps  FloatValuePtr& exps
 * @param   default_exp FloatValuePtr& default_exp
 * @return  vector of double
 */
std::vector<double> condlink_exec_floatvalue(const LinkValuePtr &conds,
                                             const FloatValuePtr &exps,
                                             const FloatValuePtr &default_exp);
}}
#endif //ASMOSES_CONDLINK_INTERPRETER_H
