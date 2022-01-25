/** representation_params.h ---
 *
 * Copyright (C) 2022 SingularityNET Foundation
 *
 * Author: Nil Geisweiller
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

#ifndef _OPENCOG_REPRESENTATION_PARAMETERS_H
#define _OPENCOG_REPRESENTATION_PARAMETERS_H

#include <opencog/asmoses/combo/combo/vertex.h>

namespace opencog { namespace moses {

static const combo::operator_set empty_ignore_ops = combo::operator_set();

enum class knob_probing_enum {
	kp_auto,
	kp_on,
	kp_off
};

static inline knob_probing_enum parse_knob_probing(std::string& kp_str)
{
	knob_probing_enum kp = knob_probing_enum::kp_auto;
	if (kp_str == "auto")
		kp = knob_probing_enum::kp_auto;
	else if (kp_str == "1" or kp_str == "on")
		kp = knob_probing_enum::kp_on;
	else if (kp_str == "0" or kp_str == "off")
		kp = knob_probing_enum::kp_off;
	else
		OC_ASSERT(false, "Knob probing option %s not supported",
		          kp_str.c_str());
	return kp;
}

/**
 * Parameters for representation building
 */
struct representation_parameters
{
	representation_parameters(reduct::rule* opt_red=NULL,
	                          reduct::rule* rep_red=NULL,
	                          const combo::operator_set& igops=empty_ignore_ops,
	                          knob_probing_enum kp=knob_probing_enum::kp_auto,
	                          bool linc=false,
	                          float permr=0.0,
	                          const combo::combo_tree_ns_set* prcts=nullptr,
	                          const combo::combo_tree_ns_set* acts=nullptr)
		: opt_reduct(opt_red),
		  rep_reduct(rep_red),
		  ignore_ops(igops),
		  knob_probing(kp),
		  linear_contin(linc),
		  perm_ratio(permr),
		  perceptions(prcts),
		  actions(acts)
		{}

	// Reduction during optimization
	const reduct::rule* opt_reduct;

	// Reduction for representation
	const reduct::rule* rep_reduct;

	// Set of operators to ignore
	combo::operator_set ignore_ops;

	// Whether knob probing is auto, on or off
	knob_probing_enum knob_probing;

	// Build only linear expressions involving contin features.
	// This can greatly decrease the number of knobs created during
	// representation building, resulting in much smaller field sets,
	// and instances that can be searched more quickly. However, in
	// order to fit the data, linear expressions may not be as good,
	// and thus may require more time overall to find...
	bool linear_contin;

	// Defines how many pairs of literals constituting subtrees op(l1
	// l2) are considered while creating the prototype of an
	// exemplar. It ranges from 0 to 1, 0 means arity positive
	// literals and arity pairs of literals, 1 means arity positive
	// literals and arity*(arity-1) pairs of literals
	float perm_ratio;

	// Set or perceptions and actions.  Only used for procedural
	// learning, nullptr otherwise.
	const combo::combo_tree_ns_set* perceptions;
	const combo::combo_tree_ns_set* actions;

};

} // ~namespace moses
} // ~namespace opencog

#endif // _OPENCOG_REPRESENTATIOB_PARAMETERS_H
