/*
 * moses/moses/representation/KnobLink.cc
 *
 * Copyright (C) 2020 OpenCog Foundation
 *
 * Author: Kasim Ebrahim
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

#include <opencog/atoms/core/NumberNode.h>
#include <opencog/atoms/core/CondLink.h>
#include <opencog/atoms/core/TypeNode.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>
#include <opencog/util/oc_assert.h>

#include "KnobLink.h"

using namespace opencog;

KnobLink::KnobLink(const HandleSeq &&seq, Type t)
		: FunctionLink(std::move(seq), t)
{
	if (not nameserver().isA(t, KNOB_LINK))
	{
		const std::string& tname = nameserver().getTypeName(t);
		throw InvalidParamException(TRACE_INFO,
		                            "Expecting an KnobLink, got %s", tname.c_str());
	}
	init();
}

void KnobLink::init()
{
	if (5 !=_outgoing.size())
		throw SyntaxException(TRACE_INFO,
		                      "KnobLink expected 4 arguments found %d!",
		                      _outgoing.size());

	_is_component = _outgoing[3]->get_type() == TRUE_LINK ? true: false;

	auto multi = NumberNodeCast(_outgoing[2])->value();
	HandleSeq seq;
	size_t counter = 0;
	for (int i : multi)
	{
		Handle expr;
		switch(i)
		{
			case 0: // Absent
				expr = _outgoing[4]; // This is Identity. TrueLink if the incoming
				break;               // link is And, FalseLink if inc. link is Or.
			case 1: // Present
				expr = _outgoing[1];
				break;
			case 2: // Negated
				expr = createLink(HandleSeq{_outgoing[1]}, NOT_LINK);
				break;
			default:
				throw SyntaxException(TRACE_INFO, "Error Unknown knob setting!");
		}
		// push cond. we use separate counter because some knob_settings
		// [which are missing in multi] could be disallowed.
		seq.push_back(createLink(HandleSeq{
						_outgoing[0],
						Handle(createNumberNode(counter))}, EQUAL_LINK));
		// push expr
		seq.push_back(expr);
		counter++;
	}
	_equiv = createLink(seq, COND_LINK);
}

ValuePtr KnobLink::execute(opencog::AtomSpace *scratch, bool silent)
{
	return CondLinkCast(_equiv)->execute(scratch, silent);
}

DEFINE_LINK_FACTORY(KnobLink, KNOB_LINK)
