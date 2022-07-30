/*
 * asmoses/opencog/atomese/atom_types/ImpulseLink.cc
 *
 * Copyright (C) 2019 Yidnekachew W.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the
 * exceptions at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <opencog/atoms/base/ClassServer.h>
#include <opencog/atoms/execution/EvaluationLink.h>
#include <opencog/atoms/core/NumberNode.h>

#include "ImpulseLink.h"

using namespace opencog;

MpulseLink::MpulseLink(const HandleSeq& oset, Type t)
		: FunctionLink(oset, t)
{
	// Type must be as expected
	if (not nameserver().isA(t, MPULSE_LINK)) {
		const std::string& tname = nameserver().getTypeName(t);
		throw SyntaxException(TRACE_INFO,
		                      "Expecting a MpulseLink, got %s", tname.c_str());
	}

	if (1 != oset.size())
		throw SyntaxException(TRACE_INFO,
		                      "MpulseLink expects only one argument.");
}

ValuePtr MpulseLink::execute(AtomSpace* scratch, bool silent)
{
	TruthValuePtr tvp(EvaluationLink::do_evaluate(scratch, _outgoing.at(0)));

	if (tvp->get_mean() > 0.5)
		return ValuePtr(createNumberNode(1));
	else
		return ValuePtr(createNumberNode(0));
}

DEFINE_LINK_FACTORY(MpulseLink, MPULSE_LINK)
