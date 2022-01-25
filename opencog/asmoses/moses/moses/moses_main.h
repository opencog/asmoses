/** moses_main.h ---
 *
 * Copyright (C) 2010 OpenCog Foundation
 *
 * Author: Nil Geisweiller <ngeiswei@gmail.com>
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

#ifndef _OPENCOG_MOSES_MAIN_H
#define _OPENCOG_MOSES_MAIN_H

#include <iostream>
#include <fstream>

#include <opencog/util/Logger.h>

#include <opencog/asmoses/combo/combo/combo.h>

#include "opencog/asmoses/moses/deme/deme_expander.h"
#include "opencog/asmoses/moses/metapopulation/metapopulation.h"

#include "opencog/asmoses/moses/optimization/hill-climbing.h"
#include "opencog/asmoses/moses/optimization/star-anneal.h"
#include "opencog/asmoses/moses/optimization/univariate.h"
#include "opencog/asmoses/moses/optimization/particle-swarm.h"
#include "opencog/asmoses/moses/scoring/behave_cscore.h"
#include "opencog/asmoses/moses/scoring/ss_bscore.h"
#include "distributed_moses.h"
#include "moses_params.h"

namespace opencog { namespace moses {

using namespace std;

extern const char * version_string;

/**
 * Run moses
 */
void run_moses(metapopulation&,
               deme_expander&,
               const moses_parameters&,
               moses_statistics&);

/// Print metapopulation results to stdout, logfile, etc.
struct metapop_printer
{
	metapop_printer() {}
	metapop_printer(long result_count,
	                bool output_score,
	                bool output_cscore,
	                bool output_bscore,
	                bool output_only_best,
	                bool output_ensemble,
	                bool output_eval_number,
	                bool output_with_labels,
	                bool output_demeID,
	                const string_seq& ilabels,
	                const std::string& output_file,
	                combo::output_format fmt,
	                bool is_mpi);

	/**
	 * Print metapopulation summary.
	 */
	void operator()(metapopulation &metapop,
	                deme_expander& dex,
	                moses_statistics& stats) const;

private:
	long result_count;
	bool output_score;
	bool output_cscore;
	bool output_bscore;
	bool output_only_best;
	bool output_ensemble;
	bool output_eval_number;
	bool output_with_labels;
	bool output_demeID;
public:
	string_seq ilabels;
private:
	string output_file;
	output_format fmt;
	bool is_mpi;
};

/**
 * Create metapopulation, run moses, print results.
 */
template<typename Printer>
void metapop_moses_results_b(const combo_tree_seq& bases,
                             const opencog::combo::type_tree& tt,
                             behave_cscore& sc,
                             const optim_parameters& opt_params,
                             const hc_parameters& hc_params,
                             const ps_parameters& ps_params,
                             const representation_parameters& rep_params,
                             const deme_parameters& deme_params,
                             const subsample_deme_filter_parameters& filter_params,
                             const metapop_parameters& meta_params,
                             const moses_parameters& moses_params,
                             Printer& printer,
                             type_node t_output=id::boolean_type,
                             const string_seq& labels={})
{
	moses_statistics stats;
	optimizer_base* optimizer = nullptr;

	if (opt_params.opt_algo == hc) { // stochastic local search
		optimizer = new hill_climbing(opt_params, hc_params);
	}
	else if (opt_params.opt_algo == sa) { // simulated annealing
		optimizer = new simulated_annealing(opt_params);
	}
	else if (opt_params.opt_algo == un) { // univariate
		optimizer = new univariate_optimization(opt_params);
	}
	else if (opt_params.opt_algo == ps) { // particle swarm
		optimizer = new particle_swarm(opt_params, ps_params);
	}
	else {
		std::cerr << "Unknown optimization algo " << opt_params.opt_algo
		          << ". Supported algorithms are un (for univariate),"
		          << " sa (for star-shaped search), hc (for local search)"
		          << " and ps (for particle swarm)"
		          << std::endl;
		exit(1);
	}

	logger().info("Reduce initial exemplars in case they were provided "
	              "(or synthesized) in unreduced form");
	combo_tree_seq simple_bases;
	for (const combo_tree& xmplr: bases) {
		combo_tree siba(xmplr);
		(*rep_params.opt_reduct)(siba);
		simple_bases.push_back(siba);
	}

	deme_expander dex(tt, sc, *optimizer, deme_params, rep_params,
	                  filter_params, t_output, labels);
	metapopulation metapop(simple_bases, sc, meta_params, filter_params);

	run_moses(metapop, dex, moses_params, stats);
	printer(metapop, dex, stats);
	delete optimizer;
}

/**
 * Adjust the termination criteria.
 */
void adjust_termination_criteria(const behave_cscore& sc,
                                 optim_parameters& opt_params,
                                 moses_parameters& moses_params);

/**
 * Autoscale diversity pressure to match the magnitude of the scorer
 */
void autoscale_diversity(const behave_cscore& sc,
                         metapop_parameters& meta_params);

/**
 * like above, but assumes that the score is bscore based
 */
template<typename Printer>
void metapop_moses_results(const combo_tree_seq& bases,
                           const opencog::combo::type_tree& type_sig,
                           behave_cscore& c_scorer,
                           const optim_parameters opt_params,
                           const hc_parameters hc_params,
                           const ps_parameters ps_params,
                           const representation_parameters& rep_params,
                           const deme_parameters& deme_params,
                           const subsample_deme_filter_parameters& filter_params,
                           const metapop_parameters& meta_params,
                           const moses_parameters moses_params,
                           Printer& printer,
                           type_node t_output = id::boolean_type,
                           const string_seq& labels={})
{
	// Parameters that might get tweaked are copied
	optim_parameters twk_opt_params(opt_params);
	metapop_parameters twk_meta_params(meta_params);
	moses_parameters twk_moses_params(moses_params);
	adjust_termination_criteria(c_scorer, twk_opt_params, twk_moses_params);
	autoscale_diversity(c_scorer, twk_meta_params);

	if (filter_params.n_subsample_fitnesses > 1
	    and filter_params.low_dev_pressure > 0.0)
	{
		// Enable SS-fitness
		const bscore_base& bscorer = c_scorer.get_bscorer();
		ss_bscore ss_bscorer(bscorer,
		                     filter_params.n_subsample_fitnesses,
		                     filter_params.low_dev_pressure,
		                     filter_params.by_time);
		behave_cscore ss_cscorer(ss_bscorer);
		metapop_moses_results_b(bases, type_sig, ss_cscorer,
		                        twk_opt_params, hc_params, ps_params,
		                        rep_params, deme_params, filter_params,
		                        twk_meta_params, twk_moses_params, printer);
	} else {
		metapop_moses_results_b(bases, type_sig, c_scorer,
		                        twk_opt_params, hc_params, ps_params,
		                        rep_params, deme_params, filter_params,
		                        twk_meta_params, twk_moses_params, printer,
		                        t_output, labels);
	}
}

} // ~namespace moses
} // ~namespace opencog

#endif // _OPENCOG_MOSES_MAIN_H
