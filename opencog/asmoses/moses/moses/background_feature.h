/*
 * asmoses/moses/moses/background_feature.h
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

#ifndef ASMOSES_BACKGROUND_FEATURE_H
#define ASMOSES_BACKGROUND_FEATURE_H

#include <opencog/atoms/base/Handle.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/core/TypeNode.h>
#include <opencog/asmoses/combo/combo/combo.h>
#include <opencog/asmoses/combo/converter/combo_atomese.h>
#include <opencog/asmoses/combo/type_checker/type_tree_def.h>
#include "types.h"

namespace opencog {
namespace moses {

using namespace opencog;
using namespace opencog::combo;
using namespace opencog::combo::id;

typedef std::vector<Type> Types;

/**
 * Implements the code to penalize programs based on the relationship between their features.
 * The following are the three assumptions about the penalty
 * AS1 = penalty ceiling that depends on the atomspace
 * AS2 = invariant to number of standalone features in the program
 * AS3 = differentiate weight of relationship of features in the program only
 *
 * weight of a relationship is defined as the product of strength and confidence of the relationship
 * weight = strength x confidence
 *
 * AS1 - we can take the sum weight of the relevant relationships as a penalty ceiling. Here, by relevant
 * we mean the types of relationships the user provided to look in the programs.
 *
 *
 *
 * -log(sum_weight_prog / weight_sum)
 *
 * AS2 - The penalty is log(P) where P = _weight_sum / std::max(sum_weight_prog_i, floor). Here floor value is used
 * so that the penalty for programs that have very weak relationships between the features is not not greater than that of programs
 * with standalone features.
 * In the above equation, if a program has a single feature or n standalone feature (no relationship b/n the features) the penalty
 * will be log(average_weight) - this is inline with AS2
 *
 * AS3 - Suppose we have two programs, prog1 and prog2 with the same feature size between the weights of the relationships of prog1
 * are greater than that of prog2. Using the above formula, P_of_prog1 > P_of_prog2, hence Penalty_of_prog1 < Penalty_of_prog2, thereby
 * satisfying AS3
 *
 * A corollary of AS1 is given two programs with the same features and feature size but different background knowledge bases (aka atomsapces),
 * they will be penalized differently. Suppose we have two atomspaces A1 and A2 where there are relationships between the features in A1 and there
 * are no relationships A2. If we have a simple program prog1:= and(f1 f2 f3) where there is no relationship between f1, f2 and f3 in both atomspaces,
 * Penalty of prog1 when using A1 as background knowledge will be greater than that of using A2, where it will be 0 in the case of using A2. This makes
 * sense because in A2 since there are no relationships, the penalty, which will be 0 for all programs, becomes irrelevant to program learning.
 */

class BackgroundFeature
{
public:
    BackgroundFeature(AtomSpace* atmSpace, Type feature, Types& relations, score_t logBase):
            _comboAtomese(type_node::boolean_type), relationsType(relations), _logBase(logBase)
    {
        _as = atmSpace;
        featureType = feature;
        calculate_total_weight();
    }

    score_t operator()(const Handle& prog);
    score_t operator()(const combo_tree& prog, const std::vector<std::string>& labels);


private:

    void get_features(const Handle& prog, HandleSet& features);
    score_t get_relationshipness(HandleSet& features);
    void get_pairwise_relationshipness(const Handle& f1, const Handle& f2, std::pair<score_t, int>& score);
    virtual score_t get_pairwise_relationshipness(const Handle& h1, const Handle& h2, Type t);

    static score_t get_cons_prob(const Handle& ln);

    void calculate_total_weight();

    inline std::string arg2str(const std::string& arg)
    {
        if(arg[0] == '$') return arg.substr(1);
        return arg;
    }

    AtomSpace* _as;
    ComboToAtomese _comboAtomese;
    Type featureType;
    Types relationsType;
    score_t _logBase;
    score_t _weight_sum = 0.0;
    int _total_rels = 0;
    score_t _floor;
    };
} // ~namespace moses
} // ~namespace opencog



#endif //ASMOSES_BACKGROUND_FEATURE_H
