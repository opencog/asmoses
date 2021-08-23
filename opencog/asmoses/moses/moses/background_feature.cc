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

const score_t MAX_PENALTY = -10000.0;

score_t BackgroundFeature::operator()(const Handle& prog)
{
	HandleSet feats;;
	get_features(prog, feats);
	return get_relationship(feats);
}

score_t BackgroundFeature::operator()(const combo_tree& prog, const std::vector<std::string>& labels)
{
    //TODO Use leaf iterator instead of converting to a handle
	Handle h = _comboAtomese(prog, labels);
	return (*this)(h);
}

void BackgroundFeature::get_features(const Handle& prog, HandleSet& features)
{
	for(const Handle& h : prog->getOutgoingSet())
	{
		if(h->get_type() == PREDICATE_NODE) features.insert(h);
		else get_features(h, features);
	}
}

score_t BackgroundFeature::get_relationship(HandleSet& features)
{
	score_t cons = 0;
	int i = 0;
	for(const Handle& h1 : features)
	{
		Handle f1 = createNode(featureType, arg2str(h1->get_name()));
		for(const Handle& h2 : features)
		{
			if(!content_eq(h1, h2)) {
				HandleSeq seq = {f1, createNode(featureType, arg2str(h2->get_name()))};
				for(Type t : relationsType){
					Handle rel = createLink(seq, t);
					Handle res = _as->get_atom(rel);
					if(res != Handle::UNDEFINED) {
						TruthValuePtr tv = res->getTruthValue();
						strength_t st = tv->get_mean();
						confidence_t conf = tv->get_confidence();
						cons += (st * conf);
						i++;
					}
				}
			}
		}
	}

	if (cons == 0) { //There is no relationship between the features
		return MAX_PENALTY;
	}
	cons = cons / i;

	return std::max(std::log2(cons), MAX_PENALTY) ;
}


}
}