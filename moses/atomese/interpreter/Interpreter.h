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

typedef std::vector<double >::size_type value_size;

class Interpreter
{

	opencog::Handle _key;

public:
	Interpreter(const opencog::Handle &input_table);

	/**
	 * executes/evaluates a program over a problem data.
	 *
	 * @param Handle&       handle ref, stores the atomese program
	 * @return               return ProtoAtomPtr containing the output
	 *                       of the program executed/evaluated on the problem data.
	 */
	opencog::ProtoAtomPtr interpret(const opencog::Handle& program);

private:
	value_size _problem_data_size;
	/**
	 * extract a protoAtomPtr containing the the values of the handle
	 *
	 * @param Handle&       handle ref, containing values with _key.
	 * @param value_size    size of the values stored in the handle.
	 * @return              protoAtomPtr containing values.
	 */
	ProtoAtomPtr unwrap_node(const Handle& handle);

	/**
	 * returns the execution/evaluation of type t with the given parameters
	 *
	 * @param Type          type of the atomese scheme to execute or evaluate.
	 * @param ProtomSeq     sequence containing parameters of the execution/evaluation.
	 * @param value_size    size of the output.
	 * @return              protoAtomPtr containing the result.
	 */
	ProtoAtomPtr execute(const Type t, const ProtomSeq &params);

	/**
	 * retrive the size of the output after the program is interpreted. The size is
	 * equal to the input size of the features[#of observations].
	 *
	 * @param Handle&       handle ref, the program
	 * @param Handle&       handle ref, the key for which the values of the features
	 *                      or the sub-programs is stored.
	 * @return value_size   return the size of the output
	 */
	static value_size extract_output_size(const Handle &program, const Handle &key);
};

}
}


#endif //MOSES_INTERPRETER_H
