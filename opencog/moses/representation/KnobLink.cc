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
#include <opencog/atomese/atom_types/atom_types.h>
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
	if (4 !=_outgoing.size())
		throw SyntaxException(TRACE_INFO,
		                      "KnobLink expected 4 arguments found %d!",
		                      _outgoing.size());

	_is_component = _outgoing[3]->get_type() == TRUE_LINK ? true: false;

	auto multi = NumberNodeCast(_outgoing[2])->value();
	HandleSeq seq;
	for (int i : multi)
	{
		Handle expr;
		switch(i)
		{
			case 0: // TODO use identity
				expr = _outgoing[1];
				break;
			case 1:
				expr = _outgoing[1];
				break;
			case 2:
				expr = createLink(HandleSeq{_outgoing[1]}, NOT_LINK);
				break;
			default:
				throw SyntaxException(TRACE_INFO, "Error Unknown knob setting!");
		}
		// push cond
		seq.push_back(createLink(HandleSeq{_outgoing[0], Handle(createNumberNode(i))}, EQUAL_LINK));
		// push expr
		seq.push_back(expr);
	}
	_equiv = createLink(seq, COND_LINK);
}

ValuePtr KnobLink::execute(opencog::AtomSpace *scratch, bool silent)
{
	OC_ASSERT(true, "Not Implemented");
}

DEFINE_LINK_FACTORY(KnobLink, KNOB_LINK)
