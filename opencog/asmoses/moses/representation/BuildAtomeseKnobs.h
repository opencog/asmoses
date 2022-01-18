/*
 * moses/moses/representation/BuildAtomeseKnobs.h
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
#ifndef _BUILD_ATOMESE_KNOBS_H
#define _BUILD_ATOMESE_KNOBS_H

#include "field_set.h"
#include "AtomeseRepresentation.h"

namespace opencog
{
namespace moses
{

struct BuildAtomeseKnobs : boost::noncopyable
{
	BuildAtomeseKnobs(Handle& exemplar,
	                  const Handle& tt,
	                  AtomeseRepresentation& rep,
	                  const Handle &DSN,
	                  bool linear_contin,
	                  const HandleSet& ignore_ops={},
	                  contin_t step_size=1.0,
	                  contin_t expansion=1.0,
	                  field_set::width_t depth=4,
	                  float perm_ratio=0.0);

protected:
	Handle& _exemplar;
	AtomeseRepresentation& _rep;
	const int _arity;
	const Handle _signature;
	contin_t _step_size, _expansion;
	field_set::width_t _depth;
	float _perm_ratio;
	bool _skip_disc_probe;
	bool _linear_contin;

	const HandleSet &_ignore_ops;

	HandleSeq _variables;

	void logical_canonize(Handle &prog);

	void build_logical(HandleSeq &path, Handle &prog);

	void build_contin(Handle &prog);

	void add_logical_knobs(HandleSeq &path, Handle &prog,
	                            bool add_if_in_exemplar=true);

	void sample_logical_perms(HandleSeq &seq, Type tp);

	void logical_probe_rec(HandleSeq &path, Handle &prog,
	                            const HandleSeq &seq,
	                            bool add_if_in_exemplar=true);

	bool logical_subtree_knob(Handle &, const Handle &,
	                          bool add_if_in_exemplar=true);

	Handle disc_probe(HandleSeq &path, Handle &prog,
	                  const Handle &child, int, bool is_comp);

	Handle create_const_knob(Handle const_node, Type type=NOTYPE);

	Handle create_contin_knob(int i);

	Handle linear_combination(Handle prog, bool in_SLE=false);

	HandleSeq linear_combination(bool in_SLE);

	Handle multi_const(Handle l, Handle r=Handle());

	Handle div_with_lc(Handle n, Handle d=Handle());

	Handle add_to(Handle l, Handle r=Handle());

	Handle make_knob_rec(Handle prog);

	bool is_predicate(const Handle &prog) const;

	void build_predicate(Handle &prog, Handle child);
};

}
}
#endif
