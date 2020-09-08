#ifndef KTV_WRITER_H_
#define KTV_WRITER_H_
#include "type.h"

namespace KTV {

class Writer {
public:
    virtual ~Writer();
    static bool Write(Map_t &data, std::string& outstr);
    static bool Write(std::string &key, Variant &value, std::string& outstr);
private:
    Writer();
};
} /* namespace KTV */
#endif /* KTV_WRITER_H_ */
