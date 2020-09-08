#ifndef KTV_VARIANT_P
#define KTV_VARIANT_P

#include "variant.h"

namespace KTV {

class Variant::Private {
public:
    Private(Variant *parent);
    virtual ~Private();
    Variant *p;
    std::string m_value;
    std::string m_encoding;
    bool m_isValid;
};

}

#endif // KTV_VARIANT_P
