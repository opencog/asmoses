/** load_table.h ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Author: Yidnekachew Wondimu <searchyidne@gmail.com>
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

#ifndef _OPENCOG_LOAD_TABLE_H
#define _OPENCOG_LOAD_TABLE_H

namespace opencog {
namespace atomese {

/*
 * The atomese loaders here load a CSV file in the given path
 * and return an Atomese representation of it.
 *
 * Various types of atomese representations are implemented
 * for an experimentation purpose.
 *
 * If the target feature isn't specified, the first one is taken as the target.
 *
 * The features specified in ignore_features will be omitted
 * from the representation.
 *
 * TODO: timestamp feature isn't supported yet!
 */

/**
 * Represent the input and the output tables separately.
 *
 * For instance, a dataset like this:
 * o, i1, i2
 * 1, 0, 0
 * 0, 1, 0
 *
 * will be represented as follows.
 *
 * On boolean domain,
 *
 * (List
 *  (Similarity (stv 1 1)
 *   (Schema "o")
 *   (Set
 *    (List (Node "r0") (True))
 *    (List (Node "r1") (False))))
 *
 *  (Similarity (stv 1 1)
 *   (List (Schema "i1") (Schema "i2"))
 *   (Set
 *    (List (Node "r0") (List (False) (False)))
 *    (List (Node "r1") (List (True) (False)))
 *   ))
 * )
 *
 * On real domain,
 *
 * (List
 * (Similarity (stv 1 1)
 *  (Schema "o")
 *  (Set
 *   (List (Node "r0") (Number 1))
 *   (List (Node "r1") (Number 0))))
 *
 * (Similarity (stv 1 1)
 *  (List (Schema "i1") (Schema "i2"))
 *  (Set
 *   (List (Node "r0") (List (Number 0) (Number 0)))
 *   (List (Node "r1") (List (Number 1) (Number 0)))))
 * )
 *
 * @param file_name
 * @param target_feature
 * @param timestamp_feature
 * @param ignore_features
 * @return
 */
Handle load_atomese_io(
	const std::string& file_name,
	const std::string& target_feature=std::string(),
	const std::string& timestamp_feature=std::string(),
	const std::vector<std::string>& ignore_features=std::vector<std::string>());

/**
 * Represent a dataset as a list of rows while
 * the rows are represented as list of True/False Links (for boolean data)
 * or Number Nodes (for real data)
 *
 * For instance, a dataset like this:
 * o, i1, i2
 * 1, 0, 0
 * 0, 1, 0
 *
 * will be represented as follows.
 *
 * On boolean domain,
 *
 * (Evaluation
 *  (Predicate "AS-MOSES:table")
 *  (List
 *   (List (Schema "o") (Schema "i1") (Schema "i2"))
 *   (List (True) (False) (False))
 *   (List (False) (True) (False))
 *  ))
 *
 * On real domain,
 *
 * (Evaluation
 *  (Predicate "AS-MOSES:table")
 *  (List
 *   (List (Schema "o") (Schema "i1") (Schema "i2"))
 *   (List (Number 1) (Number 0) (Number 0))
 *   (List (Number 0) (Number 1) (Number 0))
 *  ))
 *
 * @param file_name
 * @param target_feature
 * @param timestamp_feature
 * @param ignore_features
 * @return
 */
Handle load_atomese_compact(
	const std::string& file_name,
	const std::string& target_feature=std::string(),
	const std::string& timestamp_feature=std::string(),
	const std::vector<std::string>& ignore_features=std::vector<std::string>());

/**
 * Represent a dataset using similarity link.
 *
 * For instance, a dataset like this:
 * o, i1, i2
 * 1, 0, 0
 * 0, 1, 0
 *
 * will be represented as follows.
 *
 * On boolean domain,
 *
 * (Similarity (stv 1 1)
 *  (List (Schema "o") (Schema "i1") (Schema "i2"))
 *  (Set
 *   (List (Node "r0") (List (True) (False) (False)))
 *   (List (Node "r1") (List (False) (True) (False)))
 *  ))
 *
 * On real domain,
 *
 * (Similarity (stv 1 1)
 *  (List (Schema "o") (Schema "i1") (Schema "i2"))
 *  (Set
 *   (List (Node "r0") (List (Number 1) (Number 0) (Number 0)))
 *   (List (Node "r1") (List (Number 0) (Number 1) (Number 0)))
 *  ))
 *
 * @param file_name
 * @param target_feature
 * @param timestamp_feature
 * @param ignore_features
 * @return
 */
Handle load_atomese_similarity(
	const std::string& file_name,
	const std::string& target_feature=std::string(),
	const std::string& timestamp_feature=std::string(),
	const std::vector<std::string>& ignore_features=std::vector<std::string>());

/**
 * Represent a table without using any compression.
 * For instance, a dataset like this:
 * o, i1, i2
 * 1, 0, 0
 * 0, 1, 0
 *
 * will be represented as follows.
 *
 * On boolean domain,
 *
 * (Set
 *  (List
 *   (Execution (Schema "o") (Node "r0") (True))
 *   (Execution (Schema "i1") (Node "r0") (False))
 *   (Execution (Schema "i2") (Node "r0") (False)))
 *
 *  (List
 *   (Execution (Schema "o") (Node "r1") (False))
 *   (Execution (Schema "i1") (Node "r1") (True))
 *   (Execution (Schema "i2") (Node "r1") (False)))
 * )
 *
 * On real domain,
 *
 * (Set
 *  (List
 *   (Execution (Schema "o") (Node "r0") (Number 1))
 *   (Execution (Schema "i1") (Node "r0") (Number 0))
 *   (Execution (Schema "i2") (Node "r0") (Number 0)))
 *
 *  (List
 *   (Execution (Schema "o") (Node "r1") (Number 0))
 *   (Execution (Schema "i1") (Node "r1") (Number 1))
 *   (Execution (Schema "i2") (Node "r1") (Number 0)))
 * )
 *
 *
 * @param file_name
 * @param target_feature
 * @param timestamp_feature
 * @param ignore_features
 * @param use_eval use EvaluationLink for boolean table representation.
 * @return
 */
Handle load_atomese_unfolded(
	const std::string& file_name,
	const std::string& target_feature=std::string(),
	const std::string& timestamp_feature=std::string(),
	const std::vector<std::string>& ignore_features=std::vector<std::string>(),
	const bool use_eval=false);

}
}

#endif //_OPENCOG_LOAD_TABLE_H
