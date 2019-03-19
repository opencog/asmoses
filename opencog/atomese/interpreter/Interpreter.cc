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


#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/value/Value.h>
#include <opencog/atoms/base/Atom.h>
#include <opencog/atoms/core/NumberNode.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/atomese/interpreter/logical_interpreter.h>

#include "Interpreter.h"

using namespace opencog;
using namespace atomese;

Interpreter::Interpreter(const opencog::Handle &key)
		: _key(key), _problem_data_size(0)
{}

opencog::ValuePtr Interpreter::operator()(const opencog::Handle &program)
{
	// if this program or any sub-program of this program is previously
	// interpreted it will contain the result values with '_key',
	// return the values with out re-interpreting.
	if (ValuePtr v = program->getValue(_key))
		return v;

	// this assumes one Interpreter object per data-set
	// if the size of the data-set changes for every interpretation
	// this line must be replaced with
	// _problem_data_size = extract_output_size(program, _key);
	if (not _problem_data_size)
		_problem_data_size = extract_output_size(program, _key);

	if (is_constant(program->get_type())) {
		return unwrap_constant(program);
	}

	ValueSeq params;
	for (const Handle& h : program->getOutgoingSet()) {
		params.push_back((*this)(h));
	}

	ValuePtr result = execute(program->get_type(), params);

	// we store the result with '_key' to prevent re-interpretation for the future
	program->setValue(_key, result);

	return result;
}

ValuePtr Interpreter::unwrap_constant(const Handle &handle)
{
	// if the handle is a constant, we need to build a protoatom from the constant
	// with size of '_problem_data_size'.
	Type t = handle->get_type();
	if (NUMBER_NODE == t) {
		std::vector<double> constant_value(_problem_data_size,
		                                   NumberNodeCast(handle)->get_value());
		ValuePtr constant(new FloatValue(constant_value));
		return constant;
	}
	if (FALSE_LINK == t || TRUE_LINK == t) {
		ValueSeq constant_value(_problem_data_size, ValuePtr(handle));
		ValuePtr constant(new LinkValue(constant_value));
		return constant;
	}
	OC_ASSERT(false, "Unsupported Constant Type");
	return ValuePtr();
}

ValuePtr Interpreter::execute(const Type t, const ValueSeq& params)
{
	if (t == PLUS_LINK) {
		std::vector<double> _result(_problem_data_size, 0.0);
		ValuePtr result(new FloatValue(_result));

		for (const ValuePtr & p : params) {
			result = plus(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if (t == TIMES_LINK) {
		std::vector<double> _result(FloatValueCast(params[0])->value().size(), 1.0);
		ValuePtr result(new FloatValue(_result));

		for (const ValuePtr & p : params) {
			result = times(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if (t == AND_LINK) {
		ValueSeq _result(_problem_data_size,
		                              ValuePtr(createLink(TRUE_LINK)));
		LinkValuePtr result(new LinkValue(_result));

		for (const ValuePtr &p : params) {
			result = logical_and(result, LinkValueCast(p));
		}
		return ValuePtr(result);
	}
	if (t == OR_LINK) {
		ValueSeq _result(_problem_data_size,
		                                  ValuePtr(createLink(FALSE_LINK)));
		LinkValuePtr result(new LinkValue(_result));

		for (const ValuePtr &p : params) {
			result = logical_or(result, LinkValueCast(p));
		}
		return ValuePtr(result);
	}
	if (t == NOT_LINK) {
		OC_ASSERT(params.size() == 1);
		LinkValuePtr result;
		result = logical_not( LinkValueCast(params[0]));
		return ValuePtr(result);
	}
	return ValuePtr();
}

value_size Interpreter::extract_output_size(const Handle &program, const Handle &key)
{
	// Look for a value stored with "key" and return it's size.
	// if this program or any sub-program inside this program
	// has been interpreted before, it will contain a value of
	// the same size as the output of the interpretation of this
	// program.
	if (ValuePtr v = program->getValue(key)) {
		auto f_value = FloatValueCast(v);
		if (f_value) {
			return f_value->value().size();
		}
		return LinkValueCast(v)->value().size();
	}
	if (program->is_link()) {
		for (const Handle& child : program->getOutgoingSet()) {
			auto s = extract_output_size(child, key);
			if (s) return s;
		}
	}
	return 0;
}

bool Interpreter::is_constant(const Type t)
{
	// We need to know if an atom is a constant rather than containing a vector of
	// values for each observation[row]. Another way to handle this is introduce a new
	// atom type [CONSTANT_ATOM], But we don't want to introduce a new type just for
	// this purpose. If this use case is more ubiquitous we might want to consider it.
	return t == NUMBER_NODE || t == TRUE_LINK || t == FALSE_LINK;
}
