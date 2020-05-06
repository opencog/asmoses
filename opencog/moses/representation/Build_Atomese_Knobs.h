/*
 * moses/moses/representation/Build_Atomese_Knobs.h
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
#include "Atomese_Representation.h"

namespace opencog
{
namespace moses
{

struct Build_Atomese_Knobs : boost::noncopyable
{
	Build_Atomese_Knobs(Handle& exemplar,
	            const Handle& tt,
	            Atomese_Representation& rep,
	            const Handle &DSN,
	            const HandleSet& ignore_ops={},
	            contin_t step_size = 1.0,
	            contin_t expansion = 1.0,
	            field_set::width_t depth = 4,
	            float perm_ratio = 0.0);

protected:
	Handle& _exemplar;
	Atomese_Representation& _rep;
	const int _arity;
	const Handle _signature;
	contin_t _step_size, _expansion;
	field_set::width_t _depth;
	float _perm_ratio;
	bool _skip_disc_probe;

	const HandleSet &_ignore_ops;

	HandleSeq _variables;

	void logical_canonize(Handle &prog);

	void build_logical(HandleSeq &path, Handle &prog);

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
};

}
}
#endif
