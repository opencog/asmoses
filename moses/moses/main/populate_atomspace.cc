#include <moses/data/table/table.h>
#include <moses/data/table/table_io.h>
#include <opencog/atomspace/AtomSpace.h>
#include <moses/utils/value_key.h>
#include <opencog/atoms/base/Link.h>
#include "populate_atomspace.h"

namespace opencog {
    namespace moses {

        void populate(AtomSpace &as, ITable &itable) {

            std::vector<multi_type_seq>::const_iterator it, end;
            int col_size = itable.get_types().size();
            for (int i = 0; i < col_size; i++) {
                id::type_node col_type = itable.get_types().at(i);

                int row_size = itable.size();

                switch (col_type) {
                    case id::boolean_type: {
                        std::vector<ProtoAtomPtr> col_values = {};
                        Handle feature = createNode(SCHEMA_NODE,"f"+ std::to_string(i+1));
                        for (int j = 0; j < row_size; j++) {
                            // for each element the ith column and the jth row
                            // change the vertex to bool
                            bool col_data = vertex_to_bool(itable.get_column_data(itable.get_labels().at(i)).at(j));
                            col_values.push_back(ProtoAtomPtr(createLink(col_data ? TRUE_LINK : FALSE_LINK)));
                        }
                        ProtoAtomPtr ptr_atom(new LinkValue(col_values));
                        feature->setValue(value_key, ptr_atom);
                        as.add_atom(feature);
                        break;

                    }
                    case id::contin_type: {
                        std::vector<double> col_values_contin = {};
                        Handle feature = createNode(SCHEMA_NODE,"f"+ std::to_string(i+1));
                        for (int j = 0; j < row_size; j++)
                            col_values_contin.push_back(
                                    get_contin(itable.get_column_data(itable.get_labels().at(i)).at(j)));

                        ProtoAtomPtr ptr_atom(new FloatValue(col_values_contin));
                        feature->setValue(value_key, ptr_atom);
                        as.add_atom(feature);
                        break;
                    }
                    case id::enum_type: {
                        //TODO enum_type data to be added to Atomspace
                        break;
                    }

                    default: {
                        std::stringstream ss;
                        ss << col_type;
                        throw ComboException(TRACE_INFO,
                                             "populate atomspace can not handle type_node %s",
                                             ss.str().c_str());
                        break;
                    }


                }

            }


        }
    }
}