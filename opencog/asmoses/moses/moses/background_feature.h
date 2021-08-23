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

class BackgroundFeature
{
public:
	BackgroundFeature(AtomSpace* atmSpace, Type feature, Types& relations): _comboAtomese
	(type_node::boolean_type), relationsType(relations)
	{
		_as = atmSpace;
		featureType = feature;
	}

	score_t operator()(const Handle& prog);
	score_t operator()(const combo_tree& prog, const std::vector<std::string>& labels);


private:

	void get_features(const Handle& prog, HandleSet& features);
	score_t get_relationship(HandleSet& features);

	inline std::string arg2str(const std::string& arg)
	{
		if(arg[0] == '$') return arg.substr(1);
		return arg;
	}

	AtomSpace* _as;
	ComboToAtomese _comboAtomese;
	Type featureType;
	Types relationsType;
	};
} // ~namespace moses
} // ~namespace opencog



#endif //ASMOSES_BACKGROUND_FEATURE_H
