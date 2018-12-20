#include <list>

#include <opencog/combo/combo/vertex.h>
#include <opencog/combo/type_checker/type_tree.h>
#include <opencog/reduct/reduct/reduct.h>

using namespace moses;


namespace pleasure {
    typedef std::list<combo::combo_tree> population;
    
    //void reduce(population& pop, combo::arity_t arg_num, const reduct::rule& reduction_rule);
    void reduce(population& pop, const reduct::rule& reduction_rule);
    
    void reduced_insertion(population& pop, combo::combo_tree new_tree, const reduct::rule& reduction_rule);
}
