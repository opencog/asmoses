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

namespace opencog { namespace moses {

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
	else if (kp_str == "0")
		kp = knob_probing_enum::kp_on;
	else if (kp_str == "1")
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
	                          knob_probing_enum kp=knob_probing_enum::kp_auto,
	                          float pr=0.0)
		: opt_reduct(opt_red),
		  rep_reduct(rep_red),
		  knob_probing(kp),
		  perm_ratio(pr)
		{}

	// NEXT: better comment
	const reduct::rule* opt_reduct;  // Reduction during optimization
	const reduct::rule* rep_reduct;  // Reduction for representation
	knob_probing_enum knob_probing;  // Whether knob probing is auto, on or off
	float perm_ratio;                // Permutation ratio of logical knobs
};

} // ~namespace moses
} // ~namespace opencog

#endif // _OPENCOG_REPRESENTATIOB_PARAMETERS_H
