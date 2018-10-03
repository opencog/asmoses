#include <set>
#include <istream>

#include <moses/combo/combo/vertex.h>
#include <moses/combo/ant_combo_vocabulary/ant_combo_vocabulary.h>
#include <moses/combo/combo/builtin_action.h>
#include <moses/combo/combo/perception.h>
#include <moses/combo/combo/action_symbol.h>
#include <moses/combo/combo/indefinite_object.h>

namespace pleasure {
    typedef std::set<moses::combo::vertex> node_list;

    //std::istream& stream_to_node_list(std::istream& is, node_list& list);
    void stream_to_node_list(std::istream& is, node_list& list);
}

