/** behave_bg_cscore.h ---
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


#ifndef ASMOSES_BEHAVE_BG_CSCORE_H
#define ASMOSES_BEHAVE_BG_CSCORE_H

#include <opencog/asmoses/moses/moses/background_feature.h>
#include "behave_cscore.h"

namespace opencog {
namespace moses {
class behave_bg_cscore : public behave_cscore
{
public:
	behave_bg_cscore(bscore_base &b, AtomSpace* as, Type feature_type,
					 Types& relations, std::vector<std::string>& labels,
                     score_t incons_coef = 1, score_t logBase = 2, score_t reflexive_pen = 0.1, size_t initial_cache_size = 0);

	composite_score get_cscore(const combo_tree &tr);

	composite_score get_cscore(const scored_combo_tree_set &ensemble);

	composite_score get_cscore(const Handle &handle);


private:
	BackgroundFeature _bf;
	std::vector<std::string> _ilabels;
	score_t _inconsistency_coef;
};
}
}

#endif //ASMOSES_BEHAVE_BG_CSCORE_H
