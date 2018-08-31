/*
 * moses/moses/representation/instance_scorer.h
 *
 * Copyright (C) 2002-2008 Novamente LLC
 * Copyright (C) 2012,2013 Poulin Holdings LLC
 * All Rights Reserved
 *
 * Written by Moshe Looks, Nil Geisweiller, Linas Vepstas
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

#include "instance_scorer.h"

namespace opencog
{
namespace moses
{

composite_score distance_based_scorer::operator()(const instance &inst) const
{
	score_t sc = -fs.hamming_distance(target_inst, inst);
	// Logger
	if (logger().is_fine_enabled()) {
		logger().fine() << "distance_based_scorer - Evaluate instance: "
		                << fs.to_string(inst) << "\n"
		                << "Score = " << sc << std::endl;
	}
	// ~Logger
	return composite_score(sc, 0, 0, 0);
}

composite_score combo_based_scorer::operator()(const instance &inst) const
{
	if (logger().is_fine_enabled()) {
		logger().fine() << "combo_based_scorer - Evaluate instance: "
		                << _rep.fields().to_string(inst);
	}

	try {
		combo_tree tr = _rep.get_candidate(inst, _reduce);
		return _cscorer.get_cscore(tr);
	} catch (...) {
// XXX FIXME, calling score_tree above does not throw the exception; this should be done
// differntly, maybe call bscorer directly, then ascorer...
// ??? Huh? why couldn't we evaluate a tree anyway?  why would we want an exception here?
		combo_tree raw_tr = _rep.get_candidate(inst, false);
		combo_tree red_tr = _rep.get_candidate(inst, true);
		logger().warn() << "The following instance could not be evaluated: "
		                << _rep.fields().to_string(inst)
		                << "\nUnreduced tree: " << raw_tr
		                << "\nreduced tree: " << red_tr;
	}
	return worst_composite_score;
}

composite_score atomese_based_scorer::operator()(const instance &inst) const
{
	if (logger().is_fine_enabled()) {
		logger().fine() << "atomese_based_scorer - Evaluate instance: "
		                << _rep.fields().to_string(inst);
	}

	combo_tree tr = _rep.get_candidate(inst, _reduce);
	Handle handle = atomese_combo(tr);
	return _cscorer.get_cscore(handle);
}

}
}
