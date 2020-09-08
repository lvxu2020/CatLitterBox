#include "Writer.h"

namespace KTV 
{
Writer::Writer() {
}

Writer::~Writer() {
}

bool Writer::Write(Map_t &data, std::string& outstr) {

    std::string& writeStr = outstr;
    Map_t::const_iterator it = data.begin();
    for (; it != data.end(); ++it) {
        writeStr += it->first + ':' + it->second.encoding() + ':' + it->second.value() + '\n';
    }
    return 0;
}

bool Writer::Write(std::string& key, Variant& value, std::string& outstr) {
    std::string& writeStr = outstr;
    writeStr += key + ':' + value.encoding() + ':' + value.value() + '\n';

    return 0;
}

} 
