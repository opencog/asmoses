#ifndef _OPENCOG_VALUE_KEY_H_
#define _OPENCOG_VALUE_KEY_H_

#include <opencog/atoms/base/Node.h>
namespace opencog
{
namespace moses
{

const static Handle value_key = createNode(NODE, "*-AS-MOSES:SchemaValuesKey-*");
const static Handle compressed_value_key = createNode(NODE, "*-AS-MOSES:SchemaCompressedValuesKey-*");

} // ~namespace moses
} // ~namespace opencog

#endif
