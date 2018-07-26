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

#include "Interpreter.h"

using namespace opencog;
using namespace atomese;

Interpreter::Interpreter(opencog::Handle &input_table)
		: _problem_data(input_table)
{
	// TODO: we need a better way of getting the size of the data set
	HandleSet keys = input_table->getKeys();
	Handle f_key = *keys.begin();
	_problem_data_size = LinkValueCast(input_table->getValue(f_key))->value().size();
}

opencog::ProtoAtomPtr Interpreter::interpret(opencog::Handle &program)
{
	ProtomSeq params;
	HandleSeq _child_atoms = program->getOutgoingSet();

	for(Handle h : _child_atoms){
		if(nameserver().isA(h->get_type(), NODE)){
			params.push_back(unwrap_node(h));
		}
		else {
			params.push_back(interpret(h));
		}
	}

	ProtoAtomPtr result = execute(program->get_type(), params);
	return result;
}

ProtoAtomPtr Interpreter::unwrap_node(Handle& handle)
{
	if(SCHEMA_NODE == handle->get_type() || PREDICATE_NODE == handle->get_type()){
		return _problem_data->getValue(handle);
	}
	if(NUMBER_NODE == handle->get_type()){
		std::vector<double> constant_value(_problem_data_size,
		                                   NumberNodeCast(handle)->get_value());
		ProtoAtomPtr constant(new FloatValue(constant_value));
		return constant;
	}
}

ProtoAtomPtr Interpreter::execute(Type t, ProtomSeq& params){
	if(t == PLUS_LINK){
		std::vector<double> _result(_problem_data_size, 0.0);
		ProtoAtomPtr result(new FloatValue(_result));

		for(ProtoAtomPtr & p : params){
			result = plus(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if(t == TIMES_LINK){
		std::vector<double> _result(FloatValueCast(params[0])->value().size(), 1.0);
		ProtoAtomPtr result(new FloatValue(_result));

		for(ProtoAtomPtr & p : params){
			result = times(FloatValueCast(result), FloatValueCast(p));
		}
		return result;
	}
	if(t == AND_LINK){
		std::vector<ProtoAtomPtr> _result(_problem_data_size, ProtoAtomPtr(createLink(TRUE_LINK)));
		LinkValuePtr result(new LinkValue(_result));

		for(ProtoAtomPtr &p : params){
			result = logical_and(result, LinkValueCast(p));
		}
		return ProtoAtomPtr(result);
	}
}