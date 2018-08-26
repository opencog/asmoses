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
#include <opencog/atoms/proto/ProtoAtom.h>
#include <opencog/atoms/base/Atom.h>
#include <opencog/atoms/core/NumberNode.h>
#include <opencog/atoms/base/Link.h>
#include <moses/atomese/interpreter/logical_interpreter.h>

#include "Interpreter.h"

using namespace opencog;
using namespace atomese;

Interpreter::Interpreter(const opencog::Handle &key)
		: _key(key), _problem_data_size(0)
{}

opencog::ProtoAtomPtr Interpreter::operator()(const opencog::Handle &program)
{
	// if this program or any sub-program of this program is previously
	// interpreted it will contain the result values with '_key',
	// return the values with out re-interpreting.
	if (ProtoAtomPtr v = program->getValue(_key))
		return v;

	// this assumes one Interpreter object per data-set
	// if the size of the data-set changes for every interpretation
	// this line must be replaced with
	// _problem_data_size = extract_output_size(program, _key);
	if (not _problem_data_size)
		_problem_data_size = extract_output_size(program, _key);

	if (nameserver().isA(program->get_type(), NODE)) {
		return unwrap_node(program);
	}

	ProtomSeq params;
	for (const Handle& h : program->getOutgoingSet()) {
		params.push_back((*this)(h));
	}

	ProtoAtomPtr result = execute(program->get_type(), params);

	// we store the result with '_key' to prevent re-interpretation for the future
	program->setValue(_key, result);

	return result;
}

ProtoAtomPtr Interpreter::unwrap_node(const Handle& handle)
{
	// if the handle is a constant, we need to build a protoatom from the constant
	// with size of '_problem_data_size'.
	if (NUMBER_NODE == handle->get_type()) {
		std::vector<double> constant_value(_problem_data_size,
			                                   NumberNodeCast(handle)->get_value());
		ProtoAtomPtr constant(new FloatValue(constant_value));
		return constant;
	}
	return (*this)(handle);
}

ProtoAtomPtr Interpreter::execute(const Type t, const ProtomSeq& params)
{
	if (t == PLUS_LINK) {
		std::vector<double> _result(_problem_data_size, 0.0);
		ProtoAtomPtr result(new FloatValue(_result));

		for (const ProtoAtomPtr & p : params) {
			result = plus(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if (t == TIMES_LINK) {
		std::vector<double> _result(FloatValueCast(params[0])->value().size(), 1.0);
		ProtoAtomPtr result(new FloatValue(_result));

		for (const ProtoAtomPtr & p : params) {
			result = times(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if (t == AND_LINK) {
		std::vector<ProtoAtomPtr> _result(_problem_data_size,
		                                  ProtoAtomPtr(createLink(TRUE_LINK)));
		LinkValuePtr result(new LinkValue(_result));

		for (const ProtoAtomPtr &p : params) {
			result = logical_and(result, LinkValueCast(p));
		}
		return ProtoAtomPtr(result);
	}
	if (t == OR_LINK) {
		std::vector<ProtoAtomPtr> _result(_problem_data_size,
		                                  ProtoAtomPtr(createLink(FALSE_LINK)));
		LinkValuePtr result(new LinkValue(_result));

		for (const ProtoAtomPtr &p : params) {
			result = logical_or(result, LinkValueCast(p));
		}
		return ProtoAtomPtr(result);
	}
	return ProtoAtomPtr();
}

value_size Interpreter::extract_output_size(const Handle &program, const Handle &key)
{
	// Look for a value stored with "key" and return it's size.
	// if this program or any sub-program inside this program
	// has been interpreted before, it will contain a value of
	// the same size as the output of the interpretation of this
	// program.
	if (ProtoAtomPtr v = program->getValue(key)) {
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
