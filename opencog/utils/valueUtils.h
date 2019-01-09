#ifndef _OPENCOG_VALUE_UTILS_H_
#define _OPENCOG_VALUE_UTILS_H_

#include <opencog/atoms/value/Value.h>
#include <opencog/combo/combo/vertex.h>

namespace opencog
{
using namespace combo;

inline bool bool_value_to_bool (const ValuePtr &valuePtr)
{
	return valuePtr->get_type() == TRUE_LINK;
}

inline ValuePtr bool_vertex_to_value (const vertex &v)
{
	return ValuePtr(vertex_to_bool(v) ? createLink(TRUE_LINK) : createLink(FALSE_LINK));
}
} // ~namespace opencog

#endif
