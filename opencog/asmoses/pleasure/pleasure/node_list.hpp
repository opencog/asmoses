#include <set>
#include <istream>

#include <opencog/asmoses/combo/combo/vertex.h>
#include <opencog/asmoses/combo/ant_combo_vocabulary/ant_combo_vocabulary.h>
#include <opencog/asmoses/combo/combo/builtin_action.h>
#include <opencog/asmoses/combo/combo/perception.h>
#include <opencog/asmoses/combo/combo/action_symbol.h>
#include <opencog/asmoses/combo/combo/indefinite_object.h>

namespace pleasure {
    typedef std::set<moses::combo::vertex> node_list;

    //std::istream& stream_to_node_list(std::istream& is, node_list& list);
    void stream_to_node_list(std::istream& is, node_list& list);
}

