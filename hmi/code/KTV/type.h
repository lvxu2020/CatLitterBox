#ifndef KTV_TYPE_H_
#define KTV_TYPE_H_

#include "variant.h"
#include <istream>
#include <iostream>
#include <map>

namespace KTV {
typedef std::map<std::string, KTV::Variant> Map_t;
} /* namespace KTV */
#endif /* KTV_TYPE_H_ */
