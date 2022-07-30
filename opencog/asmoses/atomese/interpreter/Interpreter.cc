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
#include <opencog/asmoses/atomese/interpreter/logical_interpreter.h>
#include <opencog/asmoses/atomese/interpreter/condlink_interpreter.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>

#include "Interpreter.h"

using namespace opencog;
using namespace atomese;

Interpreter::Interpreter(const opencog::Handle &key, const int size)
		: _key(key), _problem_data_size(size)
{}

opencog::ValuePtr Interpreter::operator()(const opencog::Handle& program)
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
		// if the program is a constant then the output size should be one
		// regardless of the expected _problem_data_size.
		if (!_problem_data_size) _problem_data_size = 1;

		return unwrap_constant(program);
	}

	ValueSeq params;
	for (const Handle &h : program->getOutgoingSet()) {
		params.push_back(this->operator()(h));
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
		ValuePtr constant(createFloatValue(constant_value));
		return constant;
	}
	if (FALSE_LINK == t || TRUE_LINK == t) {
		ValueSeq constant_value(_problem_data_size, ValuePtr(handle));
		ValuePtr constant(createLinkValue(constant_value));
		return constant;
	}
	OC_ASSERT(false, "Unsupported Constant Type");
	return ValuePtr();
}

ValuePtr Interpreter::execute(const Type t, const ValueSeq &params)
{
	if (t == PLUS_LINK) {
		std::vector<double> _result(_problem_data_size, 0.0);
		ValuePtr result(createFloatValue(_result));

		for (const ValuePtr & p : params) {
			result = plus(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if (t == TIMES_LINK) {
		std::vector<double> _result(FloatValueCast(params[0])->value().size(), 1.0);
		ValuePtr result(createFloatValue(_result));

		for (const ValuePtr & p : params) {
			result = times(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if (t == DIVIDE_LINK) {
		std::vector<double> _result(FloatValueCast(params[0])->value().size(), 1.0);
		ValuePtr result(createFloatValue(_result));

		for (const ValuePtr & p : params) {
			result = divide(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if (t == AND_LINK) {
		ValueSeq _result(_problem_data_size, Constants::true_value);
		LinkValuePtr result(createLinkValue(_result));

		for (const ValuePtr &p : params) {
			result = logical_and(result, LinkValueCast(p));
		}
		return ValuePtr(result);
	}
	if (t == OR_LINK) {
		ValueSeq _result(_problem_data_size, Constants::false_value);
		LinkValuePtr result(createLinkValue(_result));

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
	if (t == EXP_LINK) {
		std::vector<double> _result = {};
		for (float value :FloatValueCast(params[0])->value() )_result.push_back(exp(value));
		return  ValuePtr(createFloatValue(_result));
	}
	if(t == SIN_LINK){
		std::vector<double> _result = {};
		for (float value :FloatValueCast(params[0])->value() )_result.push_back(sin(value));
		return  ValuePtr(createFloatValue(_result));
	}
	if(t == LOG_LINK){
		std::vector<double> _result = {};
		for (float value :FloatValueCast(params[0])->value() )_result.push_back(log(value));
		return  ValuePtr(createFloatValue(_result));
	}

	if (t == COND_LINK) {
		ValueSeq l_result, conds, exps, default_exp, l_result2;
		std::vector<double> f_result, f_result2;
		if (params.size() == 0) {
			throw SyntaxException(TRACE_INFO,
			                      "CondLink is expected to be arity greater-than 0!");
		}
		if (params.size() == 1) {
			default_exp.push_back(params[0]);
		}

		// If the conditions and expressions are flattened in even and odd
		// positions respectively.
		for (unsigned i = 0; i < params.size(); ++i) {

			if (i % 2 == 0) {
				if (i == params.size() - 1) {
					default_exp.push_back(params[i]);
					break;
				}
				conds.push_back(params[i]);
			} else {
				exps.push_back(params[i]);
			}
		}

		for (unsigned i = 0; i < conds.size(); i++) {
			auto f_value = FloatValueCast(exps[i]);
			if (f_value) {
				f_result2 = condlink_exec_floatvalue(LinkValueCast(conds[i]),
				                                     FloatValueCast(exps[i]),
				                                     FloatValueCast
						                                     (default_exp[i]));
				f_result.insert(f_result.end(),
				                f_result2.begin(), f_result2.end());
			} else {
				l_result2 = condlink_exec_linkvalue(LinkValueCast(conds[i]),
				                                    LinkValueCast(exps[i]),
				                                    LinkValueCast
						                                    (default_exp[i]));
				l_result.insert(l_result.end(),
				                l_result2.begin(), l_result2.end());
			}
		}
		if (l_result.empty()) {
			return ValuePtr(createFloatValue(f_result));
		} else {
			return ValuePtr(createLinkValue(l_result));
		}
	}
	if (t == MPULSE_LINK) {
		OC_ASSERT(params.size() == 1);
		ValueSeq p_value = LinkValueCast(params[0])->value();
		ValueSeq::iterator it;
		std::vector<double> _result = {};
		for (it = p_value.begin(); it != p_value.end(); ++it)
			_result.push_back(bool_value_to_bool(HandleCast(*it)) ? 1 : 0);
		return ValuePtr(createFloatValue(_result));
	}
	if (t == GREATER_THAN_LINK) {
		OC_ASSERT(params.size() == 2)

		ValuePtr result = greater_than(FloatValueCast(params[0]),
		                               FloatValueCast(params[1]));
		return result;
	}

	// Silent warning
	return nullptr;
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
	// We need to know if an atom is a constant rather than containing
	// a vector of values for each observation[row]. Another way to
	// handle this is introduce a new atom type [CONSTANT_ATOM], But
	// we don't want to introduce a new type just for this purpose.
	// If this use case is more ubiquitous we might want to consider it.
	return t == NUMBER_NODE || t == TRUE_LINK || t == FALSE_LINK;
}
