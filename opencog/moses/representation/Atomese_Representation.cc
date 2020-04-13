/*
 * moses/moses/representation/Atomese_Representation.cc
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

#include <opencog/reduct/reduct/reduct.h>
#include <opencog/reduct/rules/meta_rules.h>
#include <opencog/reduct/rules/logical_rules.h>
#include <opencog/reduct/rules/general_rules.h>
#include <opencog/atoms/execution/Instantiator.h>

#include "Atomese_Representation.h"
#include "Build_Atomese_Knobs.h"

namespace opencog
{
namespace moses
{

static contin_t stepsize = 1.0;
static contin_t expansion = 2.0;
static int depth = 5;

Atomese_Representation::Atomese_Representation(const reduct::rule &simplify_candidate,
                                               const reduct::rule &simplify_knob_building,
                                               const Handle &exemplar,
                                               const Handle &t,
                                               AtomSpace* as,
                                               const HandleSet &ignore_ops,
                                               float perm_ratio)
                                               : _exemplar(exemplar),
                                               _simplify_candidate(&simplify_candidate),
                                               _simplify_knob_building(&simplify_knob_building),
                                               _as(as)
{
	Build_Atomese_Knobs(_exemplar, t, *this, _DSN, ignore_ops,
	                    stepsize, expansion, depth, perm_ratio);
	// TODO: Create field_set
}

void Atomese_Representation::set_rep(Handle rep)
{
	_rep = _as->add_atom(rep);
}

void Atomese_Representation::set_fields(field_set fields)
{
	_fields = fields;
}

Handle Atomese_Representation::get_candidate(const Handle &h)
{
	Handle ex = _as->add_atom(createLink(HandleSeq{_DSN, h}, PUT_LINK));

	Instantiator inst(_as);
	return HandleCast(inst.execute(ex));
}

Handle Atomese_Representation::get_candidate(const instance inst)
{
	auto from = _fields.begin_disc(inst);
	auto to = _fields.end_disc(inst);
	HandleSeq seq;
	while (from!=to)
	{
		seq.push_back(Handle(createNumberNode(from.operator*())));
		from++;
	}
	return get_candidate(createLink(seq, LIST_LINK));
}

void Atomese_Representation::clean_atomese_prog(Handle &prog,
                                                bool reduce,
                                                bool knob_building)
{
	reduct::clean_reduction()(prog);
	if (reduce) {
		if (knob_building)
			(*_simplify_knob_building)(prog);
		else
			(*_simplify_candidate)(prog);
	}
}

}
}
