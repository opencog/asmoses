/*
 * moses/comboreduct/reduct/mixed_reduction.cc
 *
 * Copyright (C) 2002-2008 Novamente LLC
 * All Rights Reserved
 *
 * Written by Nil Geisweiller
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
#include "reduct.h"
#include "opencog/asmoses/reduct/rules/meta_rules.h"
#include "opencog/asmoses/reduct/rules/general_rules.h"
#include "opencog/asmoses/reduct/rules/branch_rules.h"
#include "opencog/asmoses/reduct/rules/logical_rules.h"
#include "opencog/asmoses/reduct/rules/contin_rules.h"
#include "opencog/asmoses/reduct/rules/mixed_rules.h"
#include <mutex>

namespace opencog { namespace reduct {

const rule& mixed_reduction()
{
    // A note about the locking below, and the *pr pointer.  It can (and
    // does!) happen that two different threads may enter this routine
    // simltaneously.  Because c++ will defer running static initializers
    // until they are needed, then, if we did not lock below, then both
    // threads will start running the static initializers (constructors). 
    // The faster thread would have returned a rule, while the slower 
    // thread clobbered it, causing destructors to run on that rule.
    // As a result, the faster thread was found to be accessing freed
    // memory!  Ouch.  So a lock is needed.  To avoid locking *every*
    // time, the 'static rule *pr' is used to avoid locking if the
    // initializers have run at least once.
    static rule *pr = NULL;
    if (pr != NULL) return *pr;

    static std::mutex m;
    std::lock_guard<std::mutex> static_ctor_lock(m);

    static sequential non_recursive = 
        sequential(// General
                   downwards(level()),
                   upwards(eval_constants()),
                   
                   // Simple mixed
#ifndef ABS_LOG
                   downwards(reduce_gt_zero_log()),
#endif
                   downwards(reduce_gt_zero_exp()),
                   downwards(reduce_gt_zero_minus_exp()),
                   //downwards(reduce_gt_zero_times_const()),//deprecate
                   //downwards(reduce_gt_zero_const_div()),//deprecate
                   //downwards(reduce_gt_zero_prod_exp()),//deprecate
                   //downwards(reduce_gt_zero_const_sum_sin()), //deprecate
                   downwards(reduce_gt_zero_impulse()),
                   downwards(reduce_impulse_sum()),
                   downwards(reduce_contin_if_to_impulse()),

                   // Complex mixed
                   upwards(reorder_commutative()),
                   downwards(reduce_gt_zero_pair_power()),
                   downwards(reduce_impulse_power()),
                   downwards(reduce_impulse_prod()),
                   downwards(reduce_contin_if()),
                   downwards(reduce_op_contin_if()),
                   downwards(reduce_cond_else()),
                   downwards(reduce_contin_if_inner_op()),
                   downwards(reduce_contin_if_substitute_cond()),
                   downwards(reduce_junction_gt_zero_sum_constant()));


    // This set of rules is defined to avoid infinite recursion of the
    // rule reduce_gt_zero_prod
    static iterative r_without_reduce_gt_zero_prod =
        iterative(sequential(non_recursive,

                             // Very complex recursive mixed
                             downwards(reduce_from_assumptions
                                       (r_without_reduce_gt_zero_prod)),
                             downwards(reduce_inequality_from_assumptions()),
                             downwards(reduce_contin_if_not
                                       (r_without_reduce_gt_zero_prod)),
                             downwards(reduce_gt_zero_sum
                                       (r_without_reduce_gt_zero_prod)),
                             // Commented out to avoid infinite recursion
                             // downwards(reduce_gt_zero_prod(r)),
                             downwards(reduce_gt_zero_div
                                       (r_without_reduce_gt_zero_prod)),
                             downwards(reduce_gt_zero_sum_sin
                                       (r_without_reduce_gt_zero_prod)),
                             downwards(reduce_gt_zero_sin
                                       (r_without_reduce_gt_zero_prod)),
                             downwards(reduce_and_assumptions
                                       (r_without_reduce_gt_zero_prod))//,
                             // The following is commented because
                             // there are no boolean rules here.
                             // The double negation cannot be reduced
                             // and it leads to infinite recursion.
                             // downwards(reduce_or_assumptions(r))
                             ));
    
    static iterative r = iterative(sequential(non_recursive,

                             // Very complex mixed
                             downwards(reduce_from_assumptions(r)),
                             downwards(reduce_inequality_from_assumptions()),
                             downwards(reduce_contin_if_not(r)),
                             downwards(reduce_gt_zero_sum(r)),
                             downwards(reduce_gt_zero_prod
                                       (r, r_without_reduce_gt_zero_prod)),
                             downwards(reduce_gt_zero_div(r)),
                             downwards(reduce_gt_zero_sum_sin(r)),
                             downwards(reduce_gt_zero_sin(r)),
                             downwards(reduce_gt_division_of_constants(r)),
                             downwards(reduce_and_assumptions(r))//,
                             // The following is commented because
                             // there are no boolean rules here.
                             // The double negation cannot be reduced
                             // and it leads to infinite recursion.
                             // downwards(reduce_or_assumptions(r))
                             ));
    if (pr == NULL) pr = &r;
    return *pr;
}

} // ~namespace reduct
} // ~namespace opencog

