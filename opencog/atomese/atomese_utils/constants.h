#ifndef _OPENCOG_CONSTANTS_H_
#define _OPENCOG_CONSTANTS_H_

#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/base/Link.h>

namespace opencog
{
namespace atomese
{

const static Handle value_key = createNode(NODE, "*-AS-MOSES:SchemaValuesKey-*");
const static Handle compressed_value_key = createNode(NODE, "*-AS-MOSES:SchemaCompressedValuesKey-*");

const static ValuePtr true_value(createLink(TRUE_LINK));
const static ValuePtr false_value(createLink(FALSE_LINK));

} // ~namespace atomese
} // ~namespace opencog

#endif
