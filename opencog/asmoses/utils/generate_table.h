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
#include <dai/alldai.h>
#include <dai/jtree.h>
#include <dai/factorgraph.h>

using namespace opencog;
using namespace opencog::combo;
using namespace dai;
using namespace std;

namespace opencog
{

typedef std::unordered_map<vertex, Factor> vertex_map;
typedef vector<Factor> Factors;

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
void build_bayes_net(const vertex_set& features, AtomSpace* as, Factors& factors, vertex_map& vmap, const string_seq& labels)
{
    int i = 0, j = 0;

    //Set the mean of the stvs of each feature as its probability before adding node dependencies
    HandleSeq nodes;
    for(const vertex& f : features) {
        Handle h = as->get_handle(CONCEPT_NODE, (labels.at(boost::get<argument>(f).abs_idx_from_zero())));
        OC_ASSERT(h != Handle::UNDEFINED, "Feature " + h->get_name() + " doesn't exist in the "
                     "Atomspace. All features must exist in the AtomSpace!!")
        TruthValuePtr tv = h->getTruthValue();
        strength_t stv = tv->get_mean();
        nodes.push_back(h);
        Factor fact(Var(i, 2));
        fact.set(0, stv);
        fact.set(1, 1 - stv);
        factors.push_back(fact);
        vmap[f] = fact;
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
                    Factor fact1 = factors[i];
                    Factor fact2 = factors[j];
                    TruthValuePtr tv = res->getTruthValue();
                    strength_t stv = tv->get_mean();

                    Var f1_var = *(fact1.vars().begin());
                    Var f2_var = *(fact2.vars().begin());
                    
                    cout << "f1_var: "  << f1_var << endl;
                    cout << "f2_var: " << f2_var << endl;

                    Factor f3(VarSet(f1_var, f2_var));
                    f3.set(2, 1 - stv); // P(f1 = 0 | f2 = 1)
                    f3.set(3, stv); // P(f1 = 1 | f2 = 1)
                    vmap[f1] = f3;
                    factors.push_back(f3);
                }
            }
            j++;
        }
        j = 0;
        i++;
    }
}

bool run_jt(FactorGraph& bn, JTree& jt, BP& bp)
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
    vertex_map vmap;
    Factors factors;
    build_bayes_net(feats, as, factors, vmap, labels);

    FactorGraph baysNet(factors);
    JTree jt;
    //We are going to use the Junction tree algorithm to get exact prior probabilities of
    // the nodes in the bayesian network. Read about the algorithm here https://en.wikipedia.org/wiki/Junction_tree_algorithm
    BP bp;
    run_jt(baysNet, jt, bp);
    
	cout << "JIT Belief" << endl;

    for( size_t i = 0; i < baysNet.nrVars(); i++  ) { // iterate over all variables in fg
        cout << "JT: " << jt.belief(baysNet.var(i)) << endl; // display the "belief" of jt for that variable
        cout << "BP: " << bp.belief(baysNet.var(i)) << endl;
    }
    
    cout << "Factor Belief" << endl;

	for(auto& pr : vmap) { // iterate over all variables in fg
        Factor f = pr.second;
        cout << "JT: " << jt.belief(f.vars()) << endl; // display the belief of bp for that variable
        cout << "BP: "<< bp.belief(f.vars()) << endl; // display the belief of bp for that variable
    }

	int i;
    for(unsigned n = 0; n < num_samples; ++n) {
        vertex_seq row;
        i = 0;
        for(const vertex& f : feats) {
            float prob = jt.belief(baysNet.var(i))[1];
            row.push_back(bool_to_vertex(biased_randbool(prob)));
            i++;
        }

        mixed_interpreter mit(row);
        table.otable[n] = (mit(tr));
        table.itable[n] = row;
    }

}
}

#endif //ASMOSES_GENERATE_TABLE_H
