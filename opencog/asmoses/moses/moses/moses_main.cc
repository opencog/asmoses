/** moses_main.cc ---
 *
 * Copyright (C) 2012 Pouling Holdings
 *
 * Author: Linas Vepstas <linasvepstas@gmailcom>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the
 * exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <opencog/util/Logger.h>

#include "opencog/asmoses/moses/metapopulation/metapopulation.h"
#include "distributed_moses.h"
#include "local_moses.h"
#include "mpi_moses.h"

#include "moses_main.h"

namespace opencog { namespace moses {

#define strform(x) #x
#define stringify(x) strform(x)

#ifdef MOSES_GIT_DESCRIBE
const char * version_string =
	stringify(ASMOSES_VERSION_MAJOR) "."
	stringify(ASMOSES_VERSION_MINOR) "."
	stringify(ASMOSES_VERSION_PATCH) " (git-describe "
	stringify(MOSES_GIT_DESCRIBE) ")";

#else
const char * version_string =
	stringify(ASMOSES_VERSION_MAJOR) "."
	stringify(ASMOSES_VERSION_MINOR) "."
	stringify(ASMOSES_VERSION_PATCH);

#endif

void run_moses(metapopulation& metapop,
               deme_expander& dex,
               const moses_parameters& moses_params,
               moses_statistics& stats)
{
	// Run moses, either on localhost, or distributed.
	if (moses_params.local)
		local_moses(metapop, dex, moses_params, stats);
	else if (moses_params.mpi)
		mpi_moses(metapop, dex, moses_params, stats);
	else
		distributed_moses(metapop, dex, moses_params, stats);
}

metapop_printer::metapop_printer(long _result_count,
                                 bool _output_score,
                                 bool _output_cscore,
                                 bool _output_bscore,
                                 bool _output_only_best,
                                 bool _output_ensemble,
                                 bool _output_eval_number,
                                 bool _output_with_labels,
                                 bool _output_demeID,
                                 const string_seq& _ilabels,
                                 const std::string& _output_file,
                                 combo::output_format _fmt,
                                 bool _is_mpi) :
	result_count(_result_count),
	output_score(_output_score),
	output_cscore(_output_cscore),
	output_bscore(_output_bscore),
	output_only_best(_output_only_best),
	output_ensemble(_output_ensemble),
	output_eval_number(_output_eval_number),
	output_with_labels(_output_with_labels),
	output_demeID(_output_demeID),
	ilabels(_ilabels),
	output_file(_output_file),
	fmt(_fmt),
	is_mpi(_is_mpi) {}

void metapop_printer::operator()(metapopulation &metapop,
                                 deme_expander& dex,
                                 moses_statistics& stats) const
{
	// We expect the mpi worker processes to have an empty
	// metapop at this point.  So don't print alarming output
	// messages.  In fact, the mpi workers should not even have
	// a printer at all, or use a null_printer.  Unfortunately,
	// the current code structure makes this hard to implement.
	// XXX TODO this should be fixed, someday...
	if (is_mpi && metapop.size() == 0)
		return;

	stringstream ss;

	// A number of external tools read the printed results, and
	// parse them. The floating-point scores must be printed with
	// reasonable accuracy, else these tools fail.
	ss << std::setprecision(opencog::io_score_precision);
	if (output_ensemble) {
		const scored_combo_tree_set& tree_set =
			metapop.get_ensemble().get_ensemble();
		if (output_format::python == fmt or output_format::python3 == fmt) {
			// Python boilerplate
			ss << "#!/usr/bin/env python3\n";
			ss << "#score: " << metapop.best_score() << std::endl
			   << "def moses_eval(i):\n"
			   << "    sum = 0.0 \\\n";
			for (const scored_combo_tree& sct : tree_set)
				ostream_combo_tree(ss << "      + " << sct.get_weight()
				                   << " * ", sct.get_tree(),
				                   output_with_labels? ilabels : string_seq(),
				                   fmt) << "\\\n";
			ss << "\n    return (0.0 < val)\n";
		} else {

			// For ensembles, output as usual: score followed by tree
			const ensemble& ensm(metapop.get_ensemble());
			ss << ensm.flat_score() << " "
			   << ensm.get_weighted_tree();

			// if (output_bscore)
			//    ss << " " <<
			//    metapop._cscorer.get_bscore(metapop.get_ensemble().get_weighted_tree());
			ss << std::endl;
		}

	} else {
		// scored_combo_tree_ptr_set keeps the trees in penalized-
		// score-sorted order. We want this, so that we can print
		// them out in this order.  Note, however, the printed scores
		// are the raw scores, not the penalized scores, so the
		// results can seem out-of-order.
		scored_combo_tree_ptr_set tree_set;
		if (output_only_best) {
			for (const scored_combo_tree& sct : metapop.best_candidates())
				tree_set.insert(new scored_combo_tree(sct));
		} else {
			tree_set = metapop.get_trees();
		}

		long cnt = 0;
		for (const scored_combo_tree& sct : tree_set) {
			if (result_count == cnt++) break;
			if (output_format::python == fmt or output_format::python3 == fmt) {
				// Python boilerplate
				ss << "#!/usr/bin/env python3\n";
				ss << "#score: " << sct.get_score() << std::endl
				   << "import operator as op\n"
				   << "from functools import reduce\n"
				   << "from math import log, exp, sin\n"
				   << "def l0(i): return 0 < i\n"
				   << "def adds(*args): return sum(args)\n"
				   << "def muls(*args): return reduce(op.mul, args)\n"
				   << "def pdiv(a, b): return a / (b + 0.000001)\n"
				   << "def impulse(a): return 1.0 if a else 0.0\n"
				   << "def moses_eval(i):\n"
				   << "    return ";
				ostream_combo_tree(ss, sct.get_tree(),
				                   output_with_labels? ilabels : string_seq(),
				                   fmt);
				ss << std::endl;
			} else {
				ostream_scored_combo_tree(ss, sct, output_score,
				                          output_cscore, output_demeID,
				                          output_bscore,
				                          output_with_labels? ilabels : string_seq(),
				                          fmt);
			}
		}
	}
	if (output_eval_number)
		ss << number_of_evals_str << ": " << stats.n_evals << std::endl;;

	if (output_file.empty())
		std::cout << ss.str();
	else {
		ofstream of(output_file.c_str());
		of << ss.str();
		of.close();
	}

	// Also log the thing, if logging is enabled.
	if (logger().is_info_enabled()) {
		if (output_ensemble) {
			stringstream ssb;
			for (const auto& cand : metapop.get_ensemble().get_ensemble()) {
				ssb << cand.get_weight() << " " << cand.get_tree();
			}

			if (ssb.str().empty())
				logger().warn("Ensemble was empty!");
			else
				logger().info("Final ensemble, consisting of %d members:\n%s",
				              metapop.get_ensemble().get_ensemble().size(),
				              ssb.str().c_str());
		} else {
			// Log the single best candidate
			stringstream ssb;
			metapop.ostream_metapop(ssb, 1);
			if (ssb.str().empty())
				logger().warn("No candidate is good enough to be returned. "
				              "Yeah that's bad!");
			else
				logger().info("Best candidate:\n%s", ssb.str().c_str());
		}
	}

#ifdef GATHER_STATS
	dex._optimize.hiscore /= dex._optimize.hicount;
	dex._optimize.num_improved /= dex._optimize.count_improved;
	logger().info() << "Avg number of improved scores = "
	                << dex._optimize.num_improved;
	logger().info() << "Avg improved as percentage= "
	                << 100.0 * dex._optimize.num_improved /
		dex._optimize.scores.size();

	for (unsigned i=0; i< dex._optimize.scores.size(); i++) {
		dex._optimize.scores[i] /= dex._optimize.counts[i];
		logger().info() << "Avg Scores: "
		                << i << "\t"
		                << dex._optimize.hiscore << "\t"
		                << dex._optimize.counts[i] << "\t"
		                << dex._optimize.scores[i];
	}
#endif
}

/**
 * This twiddles the trmination criteria for deciding when to finish
 * the search. Not sure why this code is here, instead of being earlier,
 * and getting set up during the option sepcification stage ... 
 */
void adjust_termination_criteria(const behave_cscore& c_scorer,
                                 optim_parameters& opt_params,
                                 moses_parameters& moses_params)
{
	// Update terminate_if_gte and max_score criteria. An explicit
	// user-specified max score always over-rides the inferred score.
	score_t target_score = c_scorer.best_possible_score();
	if (very_best_score != moses_params.max_score) {
		target_score = moses_params.max_score;
		logger().info("Target score = %g", target_score);
	} else {
		logger().info("Inferred target score = %g", target_score);
	}

	// negative min_improv is interpreted as percentage of
	// improvement, if so then don't substract anything, since in that
	// scenario the absolute min improvent can be arbitrarily small
	score_t actual_min_improv = std::max(c_scorer.min_improv(), (score_t)0);
	target_score -= actual_min_improv;
	logger().info("Subtract %g (minimum significant improvement) "
	              "from the target score to deal with float imprecision = %g",
	              actual_min_improv, target_score);

	opt_params.terminate_if_gte = target_score;
	moses_params.max_score = target_score;

	// update minimum score improvement
	opt_params.set_min_score_improv(c_scorer.min_improv());
}

void autoscale_diversity(const behave_cscore& sc,
                         metapop_parameters& meta_params)
{
	if (meta_params.diversity.enabled() and meta_params.diversity.autoscale) {
		score_t magnitude = sc.best_possible_score() - sc.worst_possible_score();
		meta_params.diversity.pressure *= magnitude;
		meta_params.diversity.set_dst2dp(meta_params.diversity.dst2dp_type);
		logger().info("Diversity pressure has been rescaled to %g",
		              meta_params.diversity.pressure);
	}
}

} // ~namespace moses
} // ~namespace opencog

