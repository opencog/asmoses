#ifndef _OPENCOG_CONSTANTS_H_
#define _OPENCOG_CONSTANTS_H_

#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/base/Link.h>

namespace opencog {
namespace atomese {

class Constants {
public:
	const static Handle value_key;
	const static Handle compressed_value_key;

	const static ValuePtr true_value;
	const static ValuePtr false_value;
};

} // ~namespace atomese
} // ~namespace opencog

#endif
