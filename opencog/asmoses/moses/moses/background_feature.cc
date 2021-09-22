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
#include <opencog/atoms/atom_types/NameServer.h>
#include <math.h>
#include "background_feature.h"

namespace opencog {
namespace moses {

using namespace opencog;
using namespace opencog::combo;

const score_t degenerate_val = 0.1;

score_t BackgroundFeature::operator()(const Handle& prog)
{
	HandleSet feats;;
	get_features(prog, feats);
	return get_relationshipness(feats);
}

score_t BackgroundFeature::operator()(const combo_tree& prog, const std::vector<std::string>& labels)
{
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

void BackgroundFeature::calculate_total_weight()
{
    int n = 0;
    for(Type t : relationsType) {
        HandleSeq rels;
        _as->get_handles_by_type(rels, t);
        n += rels.size();
        for(const Handle& h : rels) {
            _weight_sum += get_cons_prob(h);
        }
    }
    _total_rels = n;
    _floor = std::pow(_logBase, -_weight_sum) * _weight_sum;
    logger().info() << "Found " << n << " relationships, with total weight sum = " << _weight_sum
                        << " Floor value = " << _floor;
}

score_t BackgroundFeature::get_relationshipness(HandleSet& features)
{
	score_t cons = 0;
	int total = 0;
	for(const Handle& h1 : features)
	{
		for(const Handle& h2 : features)
		{
            std::pair<score_t, int> res;
            get_pairwise_relationshipness(h1, h2, res);
            cons += res.first;
            total += res.second;
		}
	}

	score_t p;
	if(total > 0) {
        p  = _weight_sum / std::max(cons, _floor);
        return std::log2(p)/std::log2(_logBase);
	}
	else if (total == 0 && _total_rels == 0) {
	    return 0;
	} else {
	    return _weight_sum;
	}
}


void BackgroundFeature::get_pairwise_relationshipness(const Handle& h1, const Handle& h2, std::pair<score_t, int>& score)
{
    score_t prob = 0.0;
    int i = 0;

    if(!content_eq(h1, h2)) {
        Handle f1 = _as->get_handle(featureType, arg2str(h1->get_name()));
        Handle f2 = _as->get_handle(featureType, arg2str(h2->get_name()));
        OC_ASSERT(f1 != Handle::UNDEFINED, "There is no such feature in the Atomspace named %s with type %s. All features must be present in the Atomspace!",
                                arg2str(h1->get_name()).c_str(), nameserver().getTypeName(featureType).c_str());
        OC_ASSERT(f2 != Handle::UNDEFINED, "There is no such feature in the Atomspace named %s with type %s. All features must be present in the Atomspace!",
                  arg2str(h2->get_name()).c_str(), nameserver().getTypeName(featureType).c_str());

        for(Type t : relationsType){
            score_t cons_prob = get_pairwise_relationshipness(f1, f2, t);
            prob += cons_prob;
            if(cons_prob > 0) {
                i++;
            }
        }
    }

    score.first = prob;
    score.second = i;
}

score_t BackgroundFeature::get_pairwise_relationshipness(const Handle &h1, const Handle &h2, Type t)
{
    score_t cons = 0;

    Handle res = _as->get_link(t, h2, h1);
    if(res != Handle::UNDEFINED) {
        cons += get_cons_prob(res);
        return cons;
    }

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
            if(content_contains(HandleSeq(out.begin(), out.end()), h2)) {
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
