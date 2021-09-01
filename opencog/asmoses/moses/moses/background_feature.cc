/*
 * asmoses/moses/moses/background_feature.cc
 *
 * Copyright (C) 2021 SingularityNET Foundation
 *
 * Author: Abdulrahman Semrie<hsamireh@gmail.com>
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

#include <opencog/asmoses/combo/converter/combo_atomese.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>
#include <opencog/atoms/truthvalue/TruthValue.h>
#include <math.h>
#include "background_feature.h"

namespace opencog {
namespace moses {

using namespace opencog;
using namespace opencog::combo;

const score_t MAX_PENALTY = 10000.0;
const score_t reflexive_rel_pen = 0.1;
const score_t degenerate_val = 0.1;

score_t BackgroundFeature::operator()(const Handle& prog)
{
	HandleSet feats;;
	get_features(prog, feats);
	return get_relationshipness(feats);
}

score_t BackgroundFeature::operator()(const combo_tree& prog, const std::vector<std::string>& labels)
{
    //TODO Use leaf iterator instead of converting to a handle
	Handle h = _comboAtomese(prog, labels);
	return (*this)(h);
}

void BackgroundFeature::get_features(const Handle& prog, HandleSet& features)
{
    if(prog->get_type() == PREDICATE_NODE){
        features.insert(prog);
    } else {
        for(const Handle& h : prog->getOutgoingSet())
        {
            get_features(h, features);
        }
    }

}

score_t BackgroundFeature::get_relationshipness(HandleSet& features)
{
	score_t cons = 0;
	int total = 0;
	for(const Handle& h1 : features)
	{
		for(const Handle& h2 : features)
		{
            auto res = get_pairwise_relationshipness(h1, h2);
            cons += res.first;
            total += res.second;
		}
	}

	if (cons == 0) { //There is no relationship between the features
		return MAX_PENALTY;
	}
	cons = cons / total;

	return -std::min(std::log2(cons), MAX_PENALTY);
}


std::pair<score_t, int> BackgroundFeature::get_pairwise_relationshipness(const Handle& h1, const Handle& h2)
{
    score_t prob;
    int i;

    if(!content_eq(h1, h2)) {
        Handle f1 = _as->get_handle(featureType, arg2str(h1->get_name()));
        Handle f2 = _as->get_handle(featureType, arg2str(h2->get_name()));
        OC_ASSERT(f1 != Handle::UNDEFINED, "There is no such feature in the Atomspace: " + f1->to_short_string()
                                                                + ". All features must be present in the Atomspace!");
        OC_ASSERT(f2 != Handle::UNDEFINED, "There is no such feature in the Atomspace: " + f2->to_short_string()
                                                                + ". All features must be present in the Atomspace!");

        for(Type t : relationsType){
            Handle res = _as->get_link(t, f2, f1);
            if(res != Handle::UNDEFINED) {
                prob += get_cons_prob(res);
                i++;
            }
            else { //Maybe the relationship is (Link (And f1 f3) f2) or (Link (Not f2) f1)
                score_t res = check_logical(f1, f2, t);
                if(res > 0) {
                    prob += res;
                    i++;
                }
            }
        }
    } else {
        prob += reflexive_rel_pen;
        i++;
    }

    return {prob, i};
}

score_t BackgroundFeature::check_logical(const Handle &h1, const Handle &h2, Type t)
{
    score_t cons = 0;
    const Handle ln = _as->get_link(t, createLink(NOT_LINK, h2), h1);
    if(ln != Handle::UNDEFINED){
        cons = 1 - get_cons_prob(ln);
        return cons;
    }

    HandleSeq lns = h1->getIncomingSetByType(t, _as);
    for(const Handle& handle : lns) {
        HandleSeq out = handle->getOutgoingSet();
        if(content_eq(h1, out.back()) && out.front()->get_type() == AND_LINK) {
            HandleSeq and_out = out.front()->getOutgoingSet();
            if(std::find(and_out.begin(), and_out.end(), h2) != out.end()) {
                cons = get_cons_prob(handle) * degenerate_val;
                return cons;
            }

        }
    }

    return cons;

}

score_t BackgroundFeature::get_cons_prob(const Handle& ln)
{
    TruthValuePtr tv = ln->getTruthValue();
    strength_t st = tv->get_mean();
    confidence_t conf = tv->get_confidence();
    return st * conf;
}


}
}