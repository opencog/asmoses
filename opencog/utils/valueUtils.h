#ifndef _OPENCOG_VALUE_UTILS_H_
#define _OPENCOG_VALUE_UTILS_H_

#include <opencog/atoms/value/Value.h>
#include <opencog/atoms/base/Link.h>
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
	static const ValuePtr t_value(createLink(TRUE_LINK));
	static const ValuePtr f_value(createLink(FALSE_LINK));
	return vertex_to_bool(v) ? t_value : f_value;
}
} // ~namespace opencog

#endif
