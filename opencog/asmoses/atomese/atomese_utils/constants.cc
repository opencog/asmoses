#include "constants.h"

#include <iostream>

namespace opencog {
namespace atomese {

const Handle Constants::value_key = createNode(NODE, "*-AS-MOSES:SchemaValuesKey-*");
const Handle Constants::compressed_value_key = createNode(NODE, "*-AS-MOSES:SchemaCompressedValuesKey-*");

const ValuePtr Constants::true_value = createLink(TRUE_LINK);
const ValuePtr Constants::false_value = createLink(FALSE_LINK);

} // ~namespace atomese
} // ~namespace opencog
