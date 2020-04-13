/*
 * moses/moses/representation/Build_Atomese_Knobs.cc
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
#include <future>

#include <boost/range/irange.hpp>

#include <opencog/util/iostreamContainer.h>
#include <opencog/util/lazy_random_selector.h>
#include <opencog/util/exceptions.h>
#include <opencog/util/dorepeat.h>
#include <opencog/util/oc_omp.h>
#include <opencog/util/random.h>

#include <opencog/combo/combo/convert_ann_combo.h>
#include <opencog/reduct/rules/meta_rules.h>
#include <opencog/reduct/rules/general_rules.h>
#include <opencog/moses/moses/complexity.h>

#include <opencog/atoms/core/Variables.h>
#include <opencog/atoms/core/TypeNode.h>
#include <opencog/atoms/core/NumberNode.h>

#include "Build_Atomese_Knobs.h"

namespace opencog
{
namespace moses
{
Build_Atomese_Knobs::Build_Atomese_Knobs(Handle &exemplar,
                                         const Handle &t,
                                         Atomese_Representation &rep,
                                         const Handle &DSN,
                                         const HandleSet &ignore_ops,
                                         contin_t step_size,
                                         contin_t expansion,
                                         field_set::width_t depth,
                                         float perm_ratio)
		: _exemplar(exemplar), _rep(rep), _skip_disc_probe(true),
		  _arity(t->getOutgoingAtom(0)->get_arity()), _signature(t),
		  _step_size(step_size), _expansion(expansion), _depth(depth),
		  _perm_ratio(perm_ratio), _ignore_ops(ignore_ops)
{
	Handle tn = _signature->getOutgoingAtom(1);
	Type output_type = TypeNodeCast(tn)->get_kind();

	HandleSeq knob_vars;
	if (output_type == BOOLEAN_NODE) {
		logical_canonize(_exemplar);
		HandleSeq path={};
		knob_vars = build_logical(path, _exemplar);
		// TODO: logical_cleanup
	}
	else {
		OC_ASSERT(true, "NonBoolean output type is not supported")
		return;
	}
}

void Build_Atomese_Knobs::logical_canonize(Handle &prog)
{
	OC_ASSERT(true, "Error: Not Implemented!")
}

HandleSeq Build_Atomese_Knobs::build_logical(HandleSeq& path, Handle &prog)
{
	OC_ASSERT(true, "Error: Not Implemented!")
}

}
}
