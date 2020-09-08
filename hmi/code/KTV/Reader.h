#ifndef KTV_READER_H_
#define KTV_READER_H_

#include <istream>
#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>
#include "type.h"

namespace KTV {

class Reader {
public:
    static bool Read(KTV::Map_t &assignments, std::string &instr);
    virtual ~Reader();
private:
    static bool ParseLines(const std::vector<std::string>& lines, KTV::Map_t &assignments);
private:
    Reader();
};

} /* namespace KTV */
#endif /* KTV_READER_H_ */
