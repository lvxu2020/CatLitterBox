#include "Reader.h"

namespace KTV {

std::vector<std::string> split(std::string str, std::string separator) {
    std::vector<std::string> result;
    unsigned int cutAt;
    while ((cutAt = str.find_first_of(separator)) != str.npos) {
        result.push_back(str.substr(0, cutAt));
        str = str.substr(cutAt + 1);
    }
    if (str.length() > 0) {
        result.push_back(str);
    }
    return result;
}

Reader::Reader() {
}

bool Reader::Read(Map_t &assignments, std::string &instr) {
    const std::vector<std::string> lines = split(instr, "\n");
    return ParseLines(lines, assignments);
}

bool Reader::ParseLines(const std::vector<std::string>& lines, Map_t &assignments) {
    std::vector<std::string>::const_iterator iter = lines.begin();

    std::string curClient = "";
    bool retOk = true;

    for (; iter != lines.end(); iter++) {
        std::string line = *iter;
        if (line.empty()) {
            continue;
        }

        int firstColon = line.find_first_of(':');
        std::string subline = line.substr(firstColon + 1);
        int secondColon = subline.find_first_of(':') + firstColon + 1;

        if (firstColon == -1 || secondColon == -1) { // bad format
            printf("Invalid format (no '::' encountered) : [%s]\n", line.c_str());
            retOk = false;
            break;
        }

        const std::string name = line.substr(0, firstColon);
        const std::string encoding = line.substr(firstColon + 1, secondColon - firstColon - 1);
        const std::string value = line.substr(secondColon + 1);

        printf("name = [%s]", name.c_str());
        printf("encoding = [%s]", encoding.c_str());
        printf("value = [%s]", value.c_str());

        Variant valueV(value.data(), encoding.data());
        assignments.insert(std::make_pair(name, valueV));
    }

    return retOk;
}

Reader::~Reader() {
}

} /* namespace KTV */
