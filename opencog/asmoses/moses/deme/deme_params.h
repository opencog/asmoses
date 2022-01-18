/** deme_params.h --- 
 *
 * Copyright (C) 2013 OpenCog Foundation
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


#ifndef _OPENCOG_DEME_PARAMETERS_H
#define _OPENCOG_DEME_PARAMETERS_H

#include "feature_selector.h"
#include "opencog/asmoses/moses/representation/representation.h"

namespace opencog { namespace moses {

/**
 * parameters for deme management
 */
struct deme_parameters
{
    deme_parameters(int mcpd=-1,
                    bool _reduce_all=true,
                    const feature_selector* _fstor=nullptr,
                    bool as_store=true,
                    bool as_port=false,
                    AtomSpacePtr as=nullptr) :
        max_candidates_per_deme(mcpd),
        reduce_all(_reduce_all),
        fstor(_fstor),
        atomspace_store(as_store),
        atomspace_port(as_port),
        atomspace(as)
        {}

    // The max number of candidates considered to be added to the
    // metapopulation, if negative then all candidates are considered.
    int max_candidates_per_deme;

    // If true then all candidates are reduced before evaluation.
    bool reduce_all;

    const feature_selector* fstor;

    // Flag used to store atomese programs to atomspace
    bool atomspace_store;

    // Flag used to run port as-moses codes.
    bool atomspace_port;

    // Atomspace used for storing candidate programs and input features
    AtomSpacePtr atomspace;
};

} // ~namespace moses
} // ~namespace opencog

#endif // _OPENCOG_DEME_PARAMETERS_H
