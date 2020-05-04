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

	std::multiset<field_set::spec> specs;
	for (const auto ds : disc)
		specs.insert(ds.first);
	for (const auto cs : contin)
		specs.insert(cs.first);
	set_fields(field_set(specs.begin(), specs.end()));

	disc_map::const_iterator it_d = disc.cbegin();
	int count_d=_fields.begin_disc_raw_idx();
	for (it_d; it_d != disc.cend(); ++it_d)
	{
		disc_lookup[it_d->second] = count_d;
		count_d++;
	}

	contin_map::const_iterator it_c = contin.cbegin();
	int count_c=_fields.begin_contin_raw_idx();
	for (it_c; it_c != contin.cend(); ++it_c)
	{
		contin_lookup[it_c->second] = count_c;
		count_c++;
	}
}

void Atomese_Representation::set_rep(Handle rep)
{
	_rep = _as->add_atom(rep);
}

void Atomese_Representation::set_fields(field_set fields)
{
	_fields = fields;
}

void Atomese_Representation::set_variables(HandleSeq vars)
{
	_variables = vars;
}

Handle Atomese_Representation::get_candidate(const Handle &h)
{
	Handle ex = _as->add_atom(createLink(HandleSeq{_DSN, h}, PUT_LINK));

	Instantiator inst(_as);
	return HandleCast(inst.execute(ex));
}

Handle Atomese_Representation::get_candidate(const instance inst, bool reduce)
{
	HandleSeq seq;
	for (Handle var : _variables) {
		auto dt = disc_lookup.find(var);
		if (dt != disc_lookup.end()) {
			std::cout << var->to_string() << " -> "<<dt->second<<"\n";
			seq.push_back(Handle(createNumberNode(_fields.get_raw(inst, dt->second))));
			continue;
		}
		auto ct = contin_lookup.find(var);
		if (ct != contin_lookup.end()) {
			seq.push_back(Handle(createNumberNode(_fields.get_contin(inst, ct->second))));
		}
	}

	Handle candidate = get_candidate(createLink(seq, LIST_LINK));

	clean_atomese_prog(candidate, reduce);
	return candidate;
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
