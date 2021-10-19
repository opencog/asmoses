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
#include <opencog/atoms/pattern/BindLink.h>
#include <opencog/persist/file/fast_load.h>
#include <dai/alldai.h>
#include <dai/jtree.h>
#include <dai/factorgraph.h>
#include <dlib/bayes_utils.h>
#include <dlib/graph_utils.h>
#include <dlib/graph.h>
#include <dlib/directed_graph.h>
#include <iostream>


using namespace dlib;
using namespace opencog;
using namespace opencog::combo;
using namespace dai;
using namespace std;
using namespace bayes_node_utils;


namespace opencog
{

typedef std::unordered_map<Handle, int> handle_idx_map;
typedef std::unordered_map<string, Var> label_var_map;
typedef std::vector<Factor> Factors;
typedef directed_graph<bayes_node>::kernel_1a_c BaysNet;
typedef dlib::set<unsigned long>::compare_1b_c set_type;
typedef graph<set_type, set_type>::kernel_1a_c join_tree_t;

double get_prob(const Handle& ln)
{
    TruthValuePtr tv = ln->getTruthValue();
    if(tv == TruthValue::DEFAULT_TV()) return -1;
    strength_t st = tv->get_mean();
    return st;
}

void get_features(const combo_tree& tr, vertex_set& features)
{
    for(combo_tree::leaf_iterator lit = tr.begin_leaf(); lit != tr.end_leaf(); ++lit) {
        if(is_argument(*lit)) features.insert(*lit);
    }
}

void check_and_link(const Handle& ln, VarSet& vs, const label_var_map& lmp)
{
    const Handle and_lnk = ln->getOutgoingSet().front();
    if(and_lnk->get_type() == AND_LINK) {
        for(const Handle& pr : and_lnk->getOutgoingSet()) {
            vs.insert(lmp.at(pr->get_name()));
        }
    } //TODO Consider a NotLink
}

void handle_rel(const Handle& h, const Handle& ln, BaysNet& bn, handle_idx_map& hmp, assignment& ps) {
    int k = hmp[h];
    if(ln->get_type() == PREDICATE_NODE) {
        int j = hmp[ln];
        if(not bn.has_edge(j, k)) {
            bn.add_edge(j, k);
            ps.add(j, 1);
            double prob = get_prob(ln);
            set_node_probability(bn, k, 1, ps, prob);
            set_node_probability(bn, k, 0, ps, 1 - prob);
        }

    }
    else if(ln->get_type() == NOT_LINK) {
        Handle h2 = ln->getOutgoingAtom(0);
        int j = hmp[h2];
        ps.add(j, 0);
        if(not bn.has_edge(j, k)) {
            bn.add_edge(j, k);
            double prob = get_prob(ln);
            set_node_probability(bn, k, 1, ps, prob);
            set_node_probability(bn, k, 0, ps, 1 - prob);
        }

    }
    else if(ln->get_type() == AND_LINK) {
        HandleSeq andOut = ln->getOutgoingSet();
        for(const Handle& cond : andOut) {
            handle_rel(h, cond, bn, hmp, ps);
        }
    }
}

void handle_node(const Handle& h, AtomSpace* as, BaysNet& bn, handle_idx_map& hmp)
{
    string name = h->get_name();
    std::string patt = "(Bind (Implication (Variable \"$var\") (Predicate \"" + name + "\" )" +
                              " (Implication (Variable \"$var\") (Predicate \"" + name + "\" )";

    Handle pattern = parseExpression(patt, *as);
    ValuePtr pattResult = pattern->execute(as);
    Handle res = (Handle) std::dynamic_pointer_cast<Atom>(pattResult);
    HandleSeq resOut = res->getOutgoingSet();
    for(const Handle& ln : resOut) {
        for(const Handle& atom : ln->getOutgoingSet()) {
            assignment ps;
            handle_rel(h, atom, bn, hmp, ps);
        }
    }

}

/*
 * Given a set of features, an AtomSpace, the function creates a random boolean corresponding to each feature
 * using its strength if it is not related to any other atom, and if it has a relation with an other atom
 * ,the strength of the stv of its relation will be used
 */
/*void build_bayes_net(const vertex_set& features, AtomSpace* as, Factors& factors, label_var_map& lbl_mp, const string_seq& labels)
{
    int i = 0;

    //Set the mean of the stvs of each feature as its probability before adding node dependencies
    HandleSeq nodes;
    as->get_handles_by_type(nodes, CONCEPT_NODE);
    for(const Handle& h : nodes) {
        string h_name = h->get_name();
//        string name = labels.at(boost::get<argument>(f).abs_idx_from_zero());
        OC_ASSERT(h != Handle::UNDEFINED, "Feature " + h_name + " doesn't exist in the "
                     "Atomspace. All features must exist in the AtomSpace!!")
        TruthValuePtr tv = h->getTruthValue();
        strength_t stv = tv->get_mean();
        Var vr = Var(i, 2);
        Factor fact(vr);
        fact.set(0, 1 - stv);
        fact.set(1, stv);
        factors.push_back(fact);
        lbl_mp[h_name] = vr;
        i++;
    }

    for(const Handle& h : nodes) {
        handle_impl_link(h, as, factors, lbl_mp);
    }
}*/



void build_bayes_net(AtomSpace* as, BaysNet& bn, handle_idx_map& hmp, const string_seq& labels)
{
    int i = 0;

    //Set the mean of the stvs of each feature as its probability before adding node dependencies
    HandleSeq nodes;
    as->get_handles_by_type(nodes, PREDICATE_NODE);
    for(const Handle& h : nodes) {
        set_node_num_values(bn, i, 2);
        double prob = get_prob(h);
        if(prob > 0) {
            assignment ps;
            set_node_probability(bn, i, 1, ps, prob);
            set_node_probability(bn, i, 0, ps, 1 - prob);
        }
        hmp[h] = i;
        i++;
    }

    bn.set_number_of_nodes(nodes.size());

    for(const Handle& h : nodes) {
        handle_node(h, as, bn , hmp);
    }

}

/*bool run_jt(FactorGraph& bn, JTree& jt, BP& bp)
{
    size_t maxstates = 1000000;
    // Store the constants in a PropertySet object
    size_t maxiter = 10000;
    Real tol = 1e-9;
    size_t verb = 1;

    PropertySet opts;
    opts.set("maxiter", maxiter);  // Maximum number of iterations
    opts.set("tol", tol);          // Tolerance for convergence
    opts.set("verbose", verb);     // Verbosity (amount of output generated)

    // Bound treewidth for junctiontree
    bool do_jt = true;
    try {
        boundTreewidth(bn, &eliminationCost_MinFill, maxstates );
    } catch( Exception &e ) {
        if( e.getCode() == Exception::OUT_OF_MEMORY ) {
            do_jt = false;
            cout << "Skipping junction tree (need more than " << maxstates << " states)." << endl;
            return do_jt;
        }
        else
            throw;
    }
    jt = JTree(bn, opts("updates", string("HUGIN")));
    jt.init();
    jt.run();

    bp = BP(bn, opts("updates",string("SEQRND"))("logdomain",false) );
	bp.init();
	bp.run();

    return do_jt;
}*/


/*
 * Given a program tr generate an num_samples of samples where the inputs features
 * of the table are generated randomly according to the knowledge
 * in the AtomSpace and the target column will be generated using the program.
 *
 */

/*void gen_table(const combo_tree& tr, AtomSpace* as, Table& table, const string_seq& labels, unsigned num_samples=100)
{

    table.itable.resize(num_samples);
    table.otable.resize(num_samples);
    std::vector<multi_type_seq> samples(num_samples);
    vertex_set feats;
    get_features(tr, feats);

    //Define the bayesian network
    label_var_map lmp;
    Factors factors;
    build_bayes_net(feats, as, factors, lmp, labels);

    FactorGraph baysNet(factors);

    JTree jt;
    //We are going to use the Junction tree algorithm to get exact prior probabilities of
    // the nodes in the bayesian network. Read about the algorithm here https://en.wikipedia.org/wiki/Junction_tree_algorithm
    BP bp;
    run_jt(baysNet, jt, bp);


    for (int n = 0; n < num_samples; n++) {
        vertex_seq row;
        for(size_t i = 0; i < baysNet.nrVars(); i++) {
            float prob = jt.belief(baysNet.var(i))[1];
            row.push_back(bool_to_vertex(biased_randbool(prob)));
        }

        mixed_interpreter mit(row);
        table.otable[n] = (mit(tr));
        table.itable[n] = row;
    }

}*/


void gen_table_dl(const combo_tree& tr, AtomSpace* as, Table& table, const string_seq& labels, unsigned num_samples=100)
{
    table.itable.resize(num_samples);
    table.otable.resize(num_samples);
    std::vector<multi_type_seq> samples(num_samples);
    vertex_set feats;
    get_features(tr, feats);

    //Define the bayesian network
    BaysNet bn;
    handle_idx_map hmp;
    build_bayes_net(as, bn, hmp, labels);

    join_tree_t join_tree;
    create_moral_graph(bn, join_tree);
    create_join_tree(join_tree, join_tree);

    bayesian_network_join_tree solution(bn, join_tree);

    int i;
    /*for(unsigned n = 0; n < num_samples; ++n) {
        vertex_seq row;


        mixed_interpreter mit(row);
        table.otable[n] = (mit(tr));
        table.itable[n] = row;
    }*/
    for(const vertex& f : feats) {
        cout << "feat: " << f <<", i: " << i << endl;
        float prob = solution.probability(i)(1);
        cout << "prob: " << f <<", " << prob << endl;
        i++;
    }
}


}

#endif //ASMOSES_GENERATE_TABLE_H
