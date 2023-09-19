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
#include <opencog/atoms/value/LinkValue.h>
#include <opencog/asmoses/utils/valueUtils.h>
#include <boost/iterator/zip_iterator.hpp>

namespace opencog { namespace atomese {

/**
 * Performs logical_and of two ValuePtrs[Links].
 * this struct is expected to be used in boost::zip_iterator to perform
 * logical_and on LinkValues containing TRUE_LINK and FALSE_LINK.
 */
struct zip_and
{
	ValueSeq _result;
	void operator()(const boost::tuple<const ValuePtr&, const ValuePtr&>& t);
};

/**
 * Performs logical_or of two ValuePtrs[Links].
 * this struct is expected to be used in boost::zip_iterator to perform
 * logical_or on LinkValues containing TRUE_LINK and FALSE_LINK.
 */
struct zip_or
{
	ValueSeq _result;
	void operator()(const boost::tuple<const ValuePtr&, const ValuePtr&>& t);
};

/**
 * Performs greater_than comparison of two doubles.
 */
struct zip_greater_than
{
	ValueSeq _result;
	void operator()(const boost::tuple<const double&, const double&>& t);
};

/**
 * Compute logical_and.
 * it expects the values of the LinkValues to be TRUE_LINK and FALSE_LINK
 * @param LinkValuePtr&     p1
 * @param LinkValuePtr&     p2
 *
 * @return                 LinkValue pointer containing the logical_and.
 */
LinkValuePtr logical_and(const LinkValuePtr& p1, const LinkValuePtr& p2);

/**
 * Compute logical_or.
 * it expects the values of the LinkValues to be TRUE_LINK and FALSE_LINK
 * @param LinkValuePtr&     p1
 * @param LinkValuePtr&     p2
 *
 * @return                 LinkValue pointer containing the logical_or.
 */
LinkValuePtr logical_or(const LinkValuePtr& p1, const LinkValuePtr& p2);

/**
 * Compare Contents of two link_values.
 * @param LinkValuePtr&     p1
 * @param LinkValuePtr&     p2
 *
 * @return                 boolean of the comparision.
 */
bool logical_compare(const LinkValuePtr& p1, const LinkValuePtr& p2);

/**
 * compute logical_not.
 * it expects the values of the LinkValues to be TRUE_LINK and FALSE_LINK
 * @param LinkValuePtr&     p1
 *
 * @return                 LinkValue pointer containing the logical_not.
 */
LinkValuePtr logical_not(const LinkValuePtr& p);

/**
 * Evaluate Greater_than.
 *
 * @param FloatValuePtr&     p1
 * @param FloatValuePtr&     p2
 *
 * @return                 LinkValue pointer containing the comparisions.
 */
LinkValuePtr greater_than(const FloatValuePtr& p1, const FloatValuePtr& p2);

}}

#endif //MOSES_LOGICAL_INTERPRETER_H
