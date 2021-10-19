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

const score_t degenerate_val = 0.01;

BackgroundFeature::BackgroundFeature(AtomSpace* atmSpace, Type feature, TypeSet& relations, score_t alpha, score_t logBase):
    _comboAtomese(type_node::boolean_type), relationsType(relations), _logBase(logBase)
{
        _as = atmSpace;
        featureType = feature;
        set_alpha(alpha);
        calculate_max_weights();
}

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

void BackgroundFeature::set_alpha(const score_t alpha)
{
    OC_ASSERT(alpha >= 0.0f, "BackgroundFeature: alpha value cannot be negative!");
    if(alpha != 0.0f){
        _alpha = alpha;
        return;
    }
    HandleSeq feats;
    _as->get_handles_by_type(feats, featureType);

    float total_weight = 0.0f;
    int n = 0;
    for(const Handle& h1 : feats) {
        for(const Handle& h2 : feats) {
            score_t p = get_pairwise_relationshipness(h1, h2);
            if(p > 0.0f) {
                total_weight += p;
                n += 1;
            }
        }
    }

    if(n > 0) //there is at least one relationship in the atomspace
    {
        float avg = total_weight / n;
        _alpha = avg;
    }

    logger().info() << "Setting alpha value to: " << _alpha << std::endl;

}

void BackgroundFeature::calculate_max_weights()
{
    float max_score = 0.0;

    HandleSeq feats;
    _as->get_handles_by_type(feats, featureType);

    int curr_m_i = 0;
    int curr_m_j = 0;
    //Find size two maximum relationship
    for(int i = 0; i < feats.size(); i++)
    {
        for(int j = 0; j < feats.size(); j++)
        {
            float cons1 = get_pairwise_relationshipness(feats[i], feats[j]);
            float cons2 = get_pairwise_relationshipness(feats[j], feats[i]);
            if(std::max(cons1, cons2) > max_score) {
                curr_m_i = i;
                curr_m_j = j;
                max_score = std::max(cons1, cons2);
            }
        }
    }

    std::set<int> emptyS;
    MaxRel relData1(_alpha, emptyS);

    std::set<int> indxs = {curr_m_i, curr_m_j};
    max_score = max_score + 2*_alpha;
    MaxRel relData2{max_score, indxs};

    maxRelSeq.push_back(relData1);
    maxRelSeq.push_back(relData2);

    for (int k = 2; k < feats.size(); k++) {
        ScoreIdx sIdx;
        select_max_feat(feats, maxRelSeq[k-1], sIdx);
        float mscore = maxRelSeq[k-1].maxScore + sIdx.first + _alpha;
        std::set<int> idxs = maxRelSeq[k-1].featIdxs;
        indxs.insert(sIdx.second);
        maxRelSeq.push_back(MaxRel(mscore, indxs));
    }

}

void BackgroundFeature::select_max_feat(const HandleSeq& features, const MaxRel& data, ScoreIdx& scoreIdx) {

    float max_score = 0.0;

    int curr_m_j = 0;


    for(int j = 0; j < features.size(); j++) {
        float weight_sum = 0.0;
        if(data.featIdxs.find(j) == data.featIdxs.end()) //not found in the previous indices
        {
            for (int i : data.featIdxs) {
                float cons1 = get_pairwise_relationshipness(features.at(i), features.at(j));
                float cons2 = get_pairwise_relationshipness(features.at(j), features.at(i));

                weight_sum += std::max(cons1, cons2);
            }
        }

        if (weight_sum > max_score) {
            curr_m_j = j;
            max_score = weight_sum;
        }
    }


    scoreIdx.first = max_score;
    scoreIdx.second = curr_m_j;
}

score_t BackgroundFeature::get_relationshipness(const HandleSet& features)
{
    if (features.empty() || features.size() == 1) return log2(maxRelSeq[0].maxScore);
    score_t cons = 0;

    for(const Handle& h1 : features)
    {
        for(const Handle& h2 : features)
        {
            cons += get_pairwise_relationshipness(h1, h2);
        }
    }

    score_t maxScore = maxRelSeq[features.size()-1].maxScore;
    if(cons > maxScore) {
        maxRelSeq[features.size()-1].maxScore = cons;
        return 0;
    }

    score_t p = cons / maxScore;
    return log2(p)/ log2(_logBase);;
}


score_t BackgroundFeature::get_pairwise_relationshipness(const Handle& h1, const Handle& h2)
{

    if(!content_eq(h1, h2)) {
        score_t weight = 0.0;
        Handle f1 = _as->get_handle(featureType, arg2str(h1->get_name()));
        Handle f2 = _as->get_handle(featureType, arg2str(h2->get_name()));
        OC_ASSERT(f1 != Handle::UNDEFINED, "There is no such feature in the Atomspace named %s with type %s. All features must be present in the Atomspace!",
                                arg2str(h1->get_name()).c_str(), nameserver().getTypeName(featureType).c_str());
        OC_ASSERT(f2 != Handle::UNDEFINED, "There is no such feature in the Atomspace named %s with type %s. All features must be present in the Atomspace!",
                  arg2str(h2->get_name()).c_str(), nameserver().getTypeName(featureType).c_str());

        for(Type t : relationsType){
            weight += get_pairwise_relationshipness(f1, f2, t);
        }
        return weight;
    }

    return _alpha;
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
