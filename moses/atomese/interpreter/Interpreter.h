/** program_interpreter.h ---
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


#ifndef MOSES_INTERPRETER_H
#define MOSES_INTERPRETER_H

#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/proto/LinkValue.h>
#include <boost/iterator/zip_iterator.hpp>
#include <moses/atomese/interpreter/logical_interpreter.h>

/**
 * class Interpreter -- create an unfolded program from compressed program
 *
 * Given a program(i:e (Plus (Schema i1) (Schema i2))) that operates on features
 * of problem data[schema or predicate], this class will create unfolded program
 * that operate on observations of the features (i1 and i2).
 * */
namespace opencog
{
namespace atomese
{

class Interpreter
{

	opencog::Handle _problem_data;
	std::vector<double >::size_type _problem_data_size;

public:
	Interpreter(opencog::Handle &input_table);

/**
 * executes/evaluates a program over a problem data.
 *
 * @param Handle&       handle ref, stores the atomese program
 * @return               return ProtoAtomPtr containing the output
 *                       of the program executed/evaluated on the problem data.
 */
	opencog::ProtoAtomPtr interpret(opencog::Handle& program);

private:
	ProtoAtomPtr unwrap_node(Handle& handle);

	ProtoAtomPtr execute(Type t, ProtomSeq &params);
};

LinkValuePtr logical_and(const LinkValuePtr& p1, const LinkValuePtr& p2){
	std::vector<ProtoAtomPtr> p1_value = p1->value();
	std::vector<ProtoAtomPtr> p2_value = p2->value();

	zip_and _and = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.begin(), p2_value.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.end(), p2_value.end())
			),
			zip_and()
	);

	std::vector<ProtoAtomPtr> _result = _and._result;
	return LinkValuePtr(new LinkValue(_result));
}

LinkValuePtr logical_or(const LinkValuePtr& p1, const LinkValuePtr& p2){
	std::vector<ProtoAtomPtr> p1_value = p1->value();
	std::vector<ProtoAtomPtr> p2_value = p2->value();

	zip_or _or = std::for_each(
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.begin(), p2_value.begin())
			),
			boost::make_zip_iterator(
					boost::make_tuple(p1_value.end(), p2_value.end())
			),
			zip_or()
	);

	std::vector<ProtoAtomPtr> _result = _or._result;
	return LinkValuePtr(new LinkValue(_result));
}

}
}


#endif //MOSES_INTERPRETER_H
