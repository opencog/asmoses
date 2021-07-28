/** moses-framework.h ---
 *
 * Copyright (C) 2011 OpenCog Foundation
 *
 * Author: Nil Geisweiller <nilg@desktop>
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

#include <unistd.h>
#include <fstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <opencog/util/oc_assert.h>

#include <opencog/asmoses/combo/combo/vertex.h>
#include <opencog/asmoses/moses/moses/types.h>
#include <opencog/asmoses/moses/main/moses_exec.h>

using namespace std;
using boost::trim_copy;
using namespace boost::posix_time;
using namespace opencog::moses;
using namespace opencog::combo;

string mkstemp_moses_output() {
	char tempfile[] = "/tmp/mosesUTestXXXXXX";
	int fd = mkstemp(tempfile);
	OC_ASSERT (fd != -1);
	return tempfile;
}

// Return a pair holding the command to run and the output file
std::pair<vector<string>, string> build_cmd(const vector<string>& arguments);

// Like above but the arguments are already joined
std::pair<string, string> build_cmd(const string& arguments);

pair<score_t, combo_tree> parse_result(const string& tempfile);

// like parse_result but doesn't actually attempt to parse the combo,
// instead it just fill the string where it is expected to be found.
pair<score_t, string> cheap_parse_result(const string& tempfile);

// Parse all results, including the composite and behavioral
// scores. It is assumed the file contains the output of the scores,
// the composite and behavioral scores.
vector<scored_combo_tree> parse_scored_combo_trees(const string& tempfile);

// Test passes only if the score is exactly equal to expected_sc
void moses_test_score(vector<string> arguments, score_t expected_sc = 0);

// Same as above, except that we accept any score that is better
// than the expected score.
void moses_test_good_enough_score(vector<string> arguments, score_t expected_sc);

// test that the first candidate is one of the expected combo tree
void moses_test_combo(vector<string> arguments,
                      vector<string> expected_tr_strs);

// test the atomese program candidates
void moses_test_atomese(const vector<string>& arguments,
						const vector<string>& expected_atomese_strs);

// like above but uses cheap_parse_result instead of parse_result
void cheap_moses_test_combo(vector<string> arguments, vector<string> expected_tr_strs);

// Test AS-Moses using cheap parse
void cheap_moses_test_atomese(const vector<string>& arguments,
							  const vector<string>& expected_tr_strs);

// Test multiple solutions, trees and scores
void moses_test_scored_combo_trees(const vector<string>& arguments,
								   const vector<scored_combo_tree>& expected_scts);
