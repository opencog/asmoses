/** behave_bg_cscore.cc ---
 *
 * Copyright (c) 2021 SingularityNET Foundation
 *
 * Authors: Abdulrahman Semrie <hsamireh@gmail.com>
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

#include <algorithm>
#include <vector>
#include <opencog/asmoses/moses/moses/types.h>
#include <opencog/asmoses/atomese/atomese_utils/constants.h>
#include "behave_bg_cscore.h"

namespace opencog {
namespace moses {

behave_bg_cscore::behave_bg_cscore(bscore_base &b, AtomSpace* as, Type feature_type,
                                   Types& relations, std::vector<std::string>& labels,
                                   score_t incons_coef, score_t logBase, score_t reflexive_pen, size_t initial_cache_size)
		:  behave_cscore(b, initial_cache_size),
		  _bf(as, feature_type, relations, logBase),
		  _ilabels(labels),
		  _inconsistency_coef(incons_coef) { }

composite_score behave_bg_cscore::get_cscore(const combo_tree &tr)
{
	composite_score cscore = get_cscore_nocache(tr);
	score_t incons_penalty = _bf(tr, _ilabels);
	cscore.set_inconsistency_penalty(incons_penalty * _inconsistency_coef);
	return cscore;
}

composite_score behave_bg_cscore::get_cscore(const scored_combo_tree_set &ensemble)
{
	score_t incons_penalty = 0;
	for(const scored_combo_tree &sct : ensemble)
	{
		incons_penalty += _bf(sct.get_tree(), _ilabels);
	}

	composite_score cscore = behave_cscore::get_cscore(ensemble);
	cscore.set_inconsistency_penalty(incons_penalty * _inconsistency_coef);
	return cscore;
}

composite_score behave_bg_cscore::get_cscore(const Handle &handle)
{
	composite_score cscore = get_cscore_nocache(handle);
	score_t incons_penalty = _bf(handle);
	cscore.set_inconsistency_penalty(incons_penalty * _inconsistency_coef);
	return cscore;
}

}
}
