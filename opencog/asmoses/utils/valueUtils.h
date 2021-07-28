#ifndef _OPENCOG_VALUE_UTILS_H_
#define _OPENCOG_VALUE_UTILS_H_

#include <opencog/atoms/value/Value.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/asmoses/combo/combo/vertex.h>

#include <opencog/asmoses/atomese/atomese_utils/constants.h>

namespace opencog
{
using namespace combo;

inline bool bool_value_to_bool (const ValuePtr &valuePtr)
{
	return valuePtr->get_type() == TRUE_LINK;
}

inline ValuePtr bool_vertex_to_value (const vertex &v)
{
	return vertex_to_bool(v) ? atomese::Constants::true_value
		: atomese::Constants::false_value;
}
inline enum_t value_to_enum(const ValuePtr &valuePtr) {
	return HandleCast(valuePtr)->get_name();
}
} // ~namespace opencog

#endif
