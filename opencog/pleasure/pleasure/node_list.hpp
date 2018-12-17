#include <set>
#include <istream>

#include <opencog/combo/combo/vertex.h>
#include <opencog/combo/ant_combo_vocabulary/ant_combo_vocabulary.h>
#include <opencog/combo/combo/builtin_action.h>
#include <opencog/combo/combo/perception.h>
#include <opencog/combo/combo/action_symbol.h>
#include <opencog/combo/combo/indefinite_object.h>

namespace pleasure {
    typedef std::set<moses::combo::vertex> node_list;

    //std::istream& stream_to_node_list(std::istream& is, node_list& list);
    void stream_to_node_list(std::istream& is, node_list& list);
}

