/** atomese_representation.h ---
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

#ifndef _OPENCOG_ATOMESE_REPRESENTATION_H
#define _OPENCOG_ATOMESE_REPRESENTATION_H

#include <opencog/atoms/base/Atom.h>
#include <moses/comboreduct/type_checker/type_tree_def.h>

namespace opencog { namespace atomese {

/**
 * Parse header of a real data to its Atomese counterpart.
 *
 * For instance, the header:
 * i1,i2,o
 * is expected to generate
 * (List (Schema "i1") (Schema "i2") (Schema "o"))
 *
 * @param labels - header labels of the data.
 * @return
 */
Handle parse_header(const std::vector<std::string>& labels);

/**
 * Generate an Atomese counterpart for a value row.
 *
 * 0,1,1 is expected to generate
 * (List (Node "rx") (List (Number 0) (Number 1) (Number 1)))
 * where x is the row_number
 *
 * @param line
 * @param row_number
 * @param row_type
 * @param labels
 * @return
 */
Handle parse_row(const std::string& line, const int& row_number,
                 combo::id::type_node row_type,
                 const std::vector<std::string>& labels);

/**
 * Load a CSV file in the given path and
 * return an Atomese representation of it.
 * @param file_name
 * @return
 */
Handle load_atomese(const std::string& file_name);

/**
 * Generate an Atomese representation for a dataset.
 *
 * The expected representation differs based on the
 * domain of the dataset.
 *
 * A real-domain dataset is represented using SimilarityLink,
 * SchemaNode and NumberNode.
 *
 * For instance,  a dataset like this:
 * i1,i2,o
 * 0.0,1.0,1.0
 * 0.0,0.0,0.0
 *
 * will be represented as:
 *
 * (Similarity (stv 1 1)
 * 	(List (Schema "i1") (Schema "i2") (Schema "o"))
 * 	(Set
 * 		(List (Node "r1") (List (Number 0) (Number 1) (Number 1)))
 * 		(List (Node "r2") (List (Number 0) (Number 0) (Number 0)))))
 *
 * Where as, a boolean-domain dataset is represented using
 * PredicateNode and EvaluationLink.
 *
 * For instance, a dataset like this:
 *
 * i1,i2,o
 * 0,1,1
 * 0,0,0
 *
 * will be represented as:
 *
 * (Set
 * 	(List
 * 		(Evaluation (stv 0 1) (Predicate "i1") (Node "r1"))
 * 		(Evaluation (stv 1 1) (Predicate "i2") (Node "r1"))
 * 		(Evaluation (stv 1 1) (Predicate "o") (Node "r1")))
 * 	(List
 * 		(Evaluation (stv 0 1) (Predicate "i1") (Node "r2"))
 * 		(Evaluation (stv 0 1) (Predicate "i2") (Node "r2"))
 * 		(Evaluation (stv 0 1) (Predicate "o") (Node "r2"))))
 *
 * @param in
 * @param repr
 * @return
 */
Handle& get_atomese_representation(std::istream& in, Handle& repr);

}}

#endif //_OPENCOG_ATOMESE_REPRESENTATION_H
