/** moses-framework.cc ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Author: Behailu Dereje <behailu04@gmail.com>
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

#include <cxxtest/TestSuite.h>
#include "moses-framework.h"

using namespace std;
using boost::trim_copy;
using namespace boost::posix_time;
using namespace opencog::moses;
using namespace opencog::combo;
using namespace opencog;

std::pair<vector<string>, string> build_cmd(const vector<string>& arguments)
{
	string tempfile = mkstemp_moses_output();
	vector<string> cmd = arguments;
	cmd.insert(cmd.begin(), string("moses-exec"));
	cmd.push_back(string("-o").append(tempfile));
	return {cmd, tempfile};
}

std::pair<string, string> build_cmd(const string& arguments)
{
	string tempfile = mkstemp_moses_output();
	string cmd = string("moses-exec ") + arguments + " -o" + tempfile;
	return {cmd, tempfile};
}

pair<score_t, combo_tree> parse_result(const string& tempfile)
{
	cout << "tempfile " << tempfile << endl;

	// open file
	ifstream in(tempfile);

	score_t hiscore = -1.0e37;
	combo_tree hitr;

	// Results are printed in random order these days...
	// WARNING: you must output only the score of the candidate!
	while (not in.eof()) {
		// parse result
		score_t score;
		in >> score;

		if (in.eof()) break;
		// If the score is a NaN, then the read will fail.
		if (in.fail()) {
			std::cout << "Error: fail to read score and tree" << std::endl;
			break;
		}
		// score_t weight;
		// in >> weight;
		combo_tree tr;
		in >> tr;
		if (hiscore < score) {
			hiscore = score;
			hitr = tr;
			std::cout << score << " " << tr << std::endl;
		}
	}

	return {hiscore, hitr};
}

// like parse_result but doesn't actually attempt to parse the combo,
// instead it just fill the string where it is expected to be found.
pair<score_t, string> cheap_parse_result(const string& tempfile)
{
	// open file
	ifstream in(tempfile);

	score_t hiscore = -1.0e37;
	string hitr_str;

	// Results are printed in random order these days...
	while (!in.eof()) {
		// parse result
		score_t score;
		in >> score;

		if (in.eof()) break;
		// If the score is a NaN, then the read will fail.
		if (in.fail()) {
			std::cout << "Error: fail to read score and tree" << std::endl;
			break;
		}
		// score_t weight;
		// in >> weight;
		char tr_chars[4096];
		in.getline(tr_chars, 4096);
		char* junk = strstr(tr_chars, "[score");
		if (junk) *junk = 0x0;
		string tr_str(tr_chars);
		if (hiscore < score) {
			hiscore = score;
			hitr_str = tr_str;
			std::cout << "score = " << score << " tr_str = " << tr_str << std::endl;
		}
	}

	return {hiscore, hitr_str};
}

// Parse all results, including the composite and behavioral
// scores. It is assumed the file contains the output of the scores,
// the composite and behavioral scores.
vector<scored_combo_tree> parse_scored_combo_trees(const string& tempfile)
{
	cout << "tempfile " << tempfile << endl;

	// open file
	ifstream in(tempfile);

	vector<scored_combo_tree> scts;
	istream_scored_combo_trees(in, scts);

	return scts;
}

// Test passes only if the score is exactly equal to expected_sc
void moses_test_score(vector<string> arguments, score_t expected_sc)
{
	arguments.push_back("--output-format=combo");
	auto t1 = microsec_clock::local_time();
	// build arguments
	pair<vector<string>, string> cmd_tmp = build_cmd(arguments);
	// run moses
	moses_exec(cmd_tmp.first);
	// parse the result
	auto result = parse_result(cmd_tmp.second);
	// check that the result is the expected one
	TS_ASSERT_LESS_THAN(fabs(result.first - expected_sc), 1.0e-3);
	auto t2 = microsec_clock::local_time();
	std::cout << "Wallclock time: " << (t2 - t1) << std::endl;

	// Unlink only if test passed.
	if (fabs(result.first - expected_sc) < 1.0e-3)
		unlink(cmd_tmp.second.c_str());
}

// Same as above, except that we accept any score that is better
// than the expected score.
void moses_test_good_enough_score(vector<string> arguments, score_t expected_sc)
{
	auto t1 = microsec_clock::local_time();
	// build arguments
	pair<vector<string>, string> cmd_tmp = build_cmd(arguments);
	// run moses
	moses_exec(cmd_tmp.first);
	// parse the result
	auto result = parse_result(cmd_tmp.second);
	// check that the result is the expected one
	cout << "Expected=" << expected_sc << " Got=" << result.first << endl;
	TS_ASSERT_LESS_THAN(expected_sc, result.first);
	auto t2 = microsec_clock::local_time();
	std::cout << "Wallclock time: " << (t2 - t1) << std::endl;

	// Unlink only if test passed.
	if (expected_sc < result.first)
		unlink(cmd_tmp.second.c_str());
}

// test that the first candidate is one of the expected combo tree
void moses_test_combo(vector<string> arguments,
                      vector<string> expected_tr_strs)
{
	auto t1 = microsec_clock::local_time();
	// build command
	pair<vector<string>, string> cmd_tmp = build_cmd(arguments);
	// run moses
	moses_exec(cmd_tmp.first);
	// parse the result
	auto result = parse_result(cmd_tmp.second);
	// check that the result is one of the expected ones
	auto f_it = boost::find_if(expected_tr_strs,
	                           [&](const string& tr_str) {
		                           combo_tree tr;
		                           stringstream(tr_str) >> tr;
		                           return tr == result.second;});
	TS_ASSERT(f_it != expected_tr_strs.end());
	auto t2 = microsec_clock::local_time();
	std::cout << "Wallclock time: " << (t2 - t1) << std::endl;

	// Unlink only if test passed.
	if (f_it != expected_tr_strs.end())
		unlink(cmd_tmp.second.c_str());
}

// test the atomese program candidates
void moses_test_atomese(const vector<string>& arguments,
						const vector<string>& expected_atomese_strs)
{
	auto t1 = microsec_clock::local_time();
	// build command
	pair<vector<string>, string> cmd_tmp = build_cmd(arguments);
	// run moses
	moses_exec(cmd_tmp.first);
	// parse the result
	auto result = parse_result(cmd_tmp.second);
	// check that the result is one of the expected ones
	auto f_it = boost::find_if(expected_atomese_strs,
	                           [&](const string& tr_str) {
		                           combo_tree tr;
		                           stringstream(tr_str) >> tr;
		                           return tr == result.second;});
	TS_ASSERT(f_it != expected_atomese_strs.end());
	auto t2 = microsec_clock::local_time();
	std::cout << "Wallclock time: " << (t2 - t1) << std::endl;

	// Unlink only if test passed.
	if (f_it != expected_atomese_strs.end())
		unlink(cmd_tmp.second.c_str());
}
// like above but uses cheap_parse_result instead of parse_result
void cheap_moses_test_combo(vector<string> arguments,
                            vector<string> expected_tr_strs)
{
	arguments.push_back("--output-format=combo");
	auto t1 = microsec_clock::local_time();
	// build command
	pair<vector<string>, string> cmd_tmp = build_cmd(arguments);
	// run moses
	moses_exec(cmd_tmp.first);
	// parse the result
	auto result = cheap_parse_result(cmd_tmp.second);
	// check that the result is one of the expected ones
	auto f_it = boost::find_if(expected_tr_strs, [&](const string& tr_str) {
		return trim_copy(result.second) == trim_copy(tr_str); });
	TS_ASSERT(f_it != expected_tr_strs.end());
	auto t2 = microsec_clock::local_time();
	std::cout << "Wallclock time: " << (t2 - t1) << std::endl;

	// Unlink only if test passed.
	if (f_it != expected_tr_strs.end())
		unlink(cmd_tmp.second.c_str());
}

// Test AS-Moses using cheap parse
void cheap_moses_test_atomese(const vector<string>& arguments,
							  const vector<string>& expected_tr_strs)
{
	auto t1 = microsec_clock::local_time();
	// build command
	pair<vector<string>, string> cmd_tmp = build_cmd(arguments);
	// run moses
	moses_exec(cmd_tmp.first);
	// parse the result
	auto result = cheap_parse_result(cmd_tmp.second);
	// check that the result is one of the expected ones
	auto f_it = boost::find_if(expected_tr_strs, [&](const string& tr_str) {
		return trim_copy(result.second) == trim_copy(tr_str); });
	TS_ASSERT(f_it != expected_tr_strs.end());
	auto t2 = microsec_clock::local_time();
	std::cout << "Wallclock time: " << (t2 - t1) << std::endl;

	// Unlink only if test passed.
	if (f_it != expected_tr_strs.end())
		unlink(cmd_tmp.second.c_str());
}

// Test multiple solutions, trees and scores
void moses_test_scored_combo_trees(const vector<string>& arguments,
                                   const vector<scored_combo_tree>& expected_scts)
{
	auto t1 = microsec_clock::local_time();
	// build command
	auto cmd_tmp = build_cmd(arguments);
	// run moses
	moses_exec(cmd_tmp.first);
	// parse the result
	vector<scored_combo_tree> scts = parse_scored_combo_trees(cmd_tmp.second);

	TSM_ASSERT_EQUALS("Length miscompare", scts.size(), expected_scts.size());
	cout << "moses_test_scored_combo_trees results:" << endl;
	int n = expected_scts.size();
	for (int i=0; i<n; i++)
	{
		cout << "output scored tree = " << scts[i] << endl;
		cout << "output expect tree = " << expected_scts[i] << endl;
		cout << "i=" << i << " (scts[i] == exptected_scts[i]) = "
		     << (scts[i] == expected_scts[i] ? "true" : "false") << endl;
	}

	// Check all results match the expected ones
	//
	// Due to some weird ambiguity between CxxTest::equals and
	// boost::algorithm::equal we avoid using TS_ASSERT_EQUALS
	TS_ASSERT(scts == expected_scts);

	auto t2 = microsec_clock::local_time();
	std::cout << "Wallclock time: " << (t2 - t1) << std::endl;

	// Unlink only if test passed.
	if (scts == expected_scts)
		unlink(cmd_tmp.second.c_str());
}
