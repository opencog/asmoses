/** generate_table.h ---
 *
 * Copyright (c) 2021 SingularityNET Foundation
 *
 * Authors: Abdulrahman Semrie <hsamireh@gmail.com>
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

#ifndef ASMOSES_GENERATE_TABLE_H
#define ASMOSES_GENERATE_TABLE_H

#include <opencog/asmoses/combo/combo/combo.h>
#include <opencog/asmoses/combo/interpreter/interpreter.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/asmoses/data/table/table.h>
#include <opencog/asmoses/data/table/table_io.h>
#include <opencog/atoms/atom_types/atom_types.h>
#include <opencog/util/random.h>
#include <dlib/bayes_utils.h>
#include <dlib/graph_utils.h>
#include <dlib/graph.h>
#include <dlib/directed_graph.h>

using namespace opencog;
using namespace opencog::combo;
using namespace dlib;
using namespace dlib::bayes_node_utils;

namespace opencog
{

typedef std::unordered_map<vertex, Handle> vertex_map;
typedef directed_graph<bayes_node>::kernel_1a_c BaysNet;
typedef dlib::set<unsigned long>::compare_1a_c set_t;
typedef graph<set_t, set_t>::kernel_1a_c join_tree_t;



void get_features(const combo_tree& tr, vertex_set& features)
{
    for(combo_tree::leaf_iterator lit = tr.begin_leaf(); lit != tr.end_leaf(); ++lit) {
        if(is_argument(*lit)) features.insert(*lit);
    }
}

/*
 * Given a set of features, an AtomSpace, the function creates a random boolean corresponding to each feature
 * using its strength if it is not related to any other atom, and if it has a relation with an other atom
 * ,the strength of the stv of its relation will be used
 */
void build_bayes_net(const vertex_set& features, AtomSpace* as, BaysNet& bn, const string_seq& labels)
{
    unsigned long i = 0, j = 0;
    assignment parent_state;

    //Set the mean of the stvs of each feature as its probability before adding node dependencies
    HandleSeq nodes;
    for(const vertex& f : features) {
        Handle h = as->get_handle(CONCEPT_NODE, (labels.at(boost::get<argument>(f).abs_idx_from_zero())));
        OC_ASSERT(h != Handle::UNDEFINED, "All features are assumed to exist in "
                                          "the Atomspace as ConceptNodes! " + h->get_name() + " not found!");
        TruthValuePtr tv = h->getTruthValue();
        strength_t stv = tv->get_mean();
        set_node_num_values(bn, i, 2);
        set_node_probability(bn, i, 1, parent_state, stv); // P(f = 1);
        set_node_probability(bn, i, 0, parent_state, 1 - stv); // P(f = 0);
        nodes.push_back(h);
        i++;
    }

    i = 0;

    for(const vertex& f1 : features) {
        Handle h1 = nodes[i];
        for(const vertex& f2: features) {
            if(f1 != f2){
                Handle h2 = nodes[j];
                Handle res = as->get_link(IMPLICATION_LINK, {h2, h1});
                if(res != Handle::UNDEFINED) {
                    TruthValuePtr tv = res->getTruthValue();
                    double stv = tv->get_mean();
                    if(not bn.has_edge(j, i))
                        bn.add_edge(j, i);
                    parent_state.add(j, 1);

                    set_node_probability(bn, i, 1, parent_state, stv); // P(f1 = 1 | f2 = 1)
                    set_node_probability(bn, i, 0, parent_state, 1 - stv); // P(f1 = 0 | f2 = 1);
                    parent_state.clear();
                }
                //Check for Implication(not(A) B) case;
                res = as->get_link(IMPLICATION_LINK, {createLink(NOT_LINK, h2), h1});
                if(res != Handle::UNDEFINED) {
                    if(not bn.has_edge(j, i))
                        bn.add_edge(j, i);
                    TruthValuePtr tv = res->getTruthValue();
                    double stv = tv->get_mean();
                    parent_state.add(j, 0);

                    set_node_probability(bn, i, 1, parent_state, stv); //P(f1 = 1 | f2 = 0)
                    set_node_probability(bn, i, 0, parent_state, 1 - stv); //P(f1 = 0 | f2 = 0);
                    parent_state.clear();
                }
            }
            j++;

        }
        j = 0;
        i++;
    }
}

/*
 * Given a program tr generate an num_samples of samples where the inputs features
 * of the table are generated randomly according to the knowledge
 * in the AtomSpace and the target column will be generated using the program.
 *
 */

void gen_table(const combo_tree& tr, AtomSpace* as, Table& table, const string_seq& labels, unsigned num_samples=100)
{

    table.itable.resize(num_samples);
    table.otable.resize(num_samples);
    std::vector<multi_type_seq> samples(num_samples);
    vertex_set feats;
    get_features(tr, feats);

    //Define the bayesian network
    BaysNet bays_net;
    bays_net.set_number_of_nodes(feats.size());

    build_bayes_net(feats, as, bays_net, labels);

    //We are going to use the Junction tree algorithm to get exact prior probabilities of
    // the nodes in the bayesian network. Read about the algorithm here https://en.wikipedia.org/wiki/Junction_tree_algorithm
    join_tree_t join_tree;
    create_moral_graph(bays_net, join_tree);
    create_join_tree(join_tree, join_tree);

    bayesian_network_join_tree solution(bays_net, join_tree);

    int i;
    for(unsigned n = 0; n < num_samples; ++n) {
        vertex_seq row;
        i = 0;
        for(const vertex& f : feats) {
            double prob = solution.probability(i)(1);
            row.push_back(bool_to_vertex(biased_randbool(prob)));
            i++;
        }

        mixed_interpreter mit(row);
        table.otable[i] = (mit(tr));
        table.itable[i] = row;
    }

}
}

#endif //ASMOSES_GENERATE_TABLE_H
