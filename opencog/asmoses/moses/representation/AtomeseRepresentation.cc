/*
 * moses/moses/representation/AtomeseRepresentation.cc
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

#include <opencog/asmoses/reduct/reduct/reduct.h>
#include <opencog/asmoses/reduct/rules/meta_rules.h>
#include <opencog/asmoses/reduct/rules/logical_rules.h>
#include <opencog/asmoses/reduct/rules/general_rules.h>
#include <opencog/atoms/execution/Instantiator.h>

#include "AtomeseRepresentation.h"
#include "BuildAtomeseKnobs.h"

namespace opencog
{
namespace moses
{

static contin_t stepsize = 1.0;
static contin_t expansion = 2.0;
static int depth = 5;

AtomeseRepresentation::AtomeseRepresentation(const Handle &exemplar,
                                             const Handle &tt,
                                             AtomSpacePtr as,
                                             const HandleSet &ignore_ops,
                                             const representation_parameters& rp)
: _exemplar(exemplar),
  _as(as),
  _rep_params(rp)
{
	BuildAtomeseKnobs(_exemplar, tt, *this, _DSN, _rep_params.linear_contin,
	                  ignore_ops, stepsize, expansion, depth, _rep_params.perm_ratio);

	std::multiset<field_set::spec> specs;
	for (const auto& ds : disc)
		specs.insert(ds.first);
	for (const auto& cs : contin)
		specs.insert(cs.first);
	set_fields(field_set(specs.begin(), specs.end()));

	disc_map::const_iterator it_d = disc.cbegin();
	int count_d=_fields.begin_disc_raw_idx();
	for (; it_d != disc.cend(); ++it_d)
	{
		disc_lookup[it_d->second] = count_d;
		count_d++;
	}

	contin_map::const_iterator it_c = contin.cbegin();
	int count_c=_fields.begin_contin_raw_idx();
	for (; it_c != contin.cend(); ++it_c)
	{
		contin_lookup[it_c->second] = count_c;
		count_c++;
	}
}

void AtomeseRepresentation::set_rep(Handle rep)
{
	_rep = _as->add_atom(rep);
}

void AtomeseRepresentation::set_fields(field_set fields)
{
	_fields = fields;
}

void AtomeseRepresentation::set_variables(HandleSeq vars)
{
	_variables = vars;
}

Handle AtomeseRepresentation::get_candidate(const Handle &h)
{
	Handle ex = _as->add_atom(createLink(HandleSeq{_DSN, h}, PUT_LINK));

	Instantiator inst(_as.get());
	return HandleCast(inst.execute(ex));
}

Handle AtomeseRepresentation::get_candidate(const instance inst, bool reduce)
{
	HandleSeq seq;
	for (Handle var : _variables) {
		auto dt = disc_lookup.find(var);
		if (dt != disc_lookup.end()) {
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

void AtomeseRepresentation::clean_atomese_prog(Handle &prog,
                                               bool reduce,
                                               bool knob_building)
{
	reduct::clean_reduction()(prog);
	if (reduce) {
		if (knob_building)
			(*_rep_params.rep_reduct)(prog);
		else
			(*_rep_params.opt_reduct)(prog);
	}
}

bool AtomeseRepresentation::operator==(const AtomeseRepresentation& other) const
{
	return this->_rep == other.rep();
}
std::ostream &AtomeseRepresentation::ostream_rep(std::ostream &out) const
{
	using std::endl;

	out << "#disc knobs= " << disc.size() << ";\n"
	    << "#contin knobs= " << contin.size() << ";\n";

	ostream_exemplar(out);

	return out;
}

std::ostream &AtomeseRepresentation::ostream_exemplar(std::ostream &out) const
{
	out << "Rep = ";
	ostream_handle(_exemplar, out);
	return out;
}

inline void ostream_knob(Handle h, const std::vector<double> &st, std::ostream &out)
{
	auto msg = nameserver().isA(h->get_type(), LINK) ?
	           oc_to_string(h->get_type(), empty_string) :
	           h->get_name();
	out << "[";
	for (double d : st)
	{
		if (d == 0.0)
			out << "nil" << " ";
		else if (d == 1.0)
			out << msg << " ";
		else if (d == 2.0)
			out << "!" << msg;
	}
	out << "] ";
}

std::ostream&
AtomeseRepresentation::ostream_link(const Handle &h, std::ostream &out) const
{
	out << "(" << oc_to_string(h->get_type(), empty_string) << " ";
	for (Handle ch : h->getOutgoingSet())
	{
		ostream_handle(ch, out);
	}
	out << ")";
	return out;
}

std::ostream&
AtomeseRepresentation::ostream_handle(const Handle &h, std::ostream &out) const
{
	Type t = h->get_type();

	if (nameserver().isA(t, NODE)) {
		auto ct = contin_lookup.find(h);
		if (ct != contin_lookup.end())
			out << "["
			    << _fields.contin().at(ct->second).mean
			    << "] ";
		else
			out << h->get_name() << " ";
		return out;
	}

	if (t == KNOB_LINK) {
		Handle body = h->getOutgoingAtom(1);
		auto settings = NumberNodeCast(h->getOutgoingAtom(2))->value();
		if (nameserver().isA(body->get_type(), LINK)) {
			out << "(";
			ostream_knob(body, settings, out);
			for (Handle ch : body->getOutgoingSet())
			{
				ostream_handle(ch, out);
			}
			out << ") ";
		} else {
			ostream_knob(body, settings, out);
		}
		return out;
	}
	return ostream_link(h, out);
}
}

std::string
oc_to_string(const moses::AtomeseRepresentation &rep, const std::string &indent)
{
	std::stringstream ss;
	rep.ostream_rep(ss);
	return ss.str();
}

}
