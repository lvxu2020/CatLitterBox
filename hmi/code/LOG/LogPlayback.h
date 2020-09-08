#ifndef LOGPLAYBACK_H
#define LOGPLAYBACK_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
//#include "platform.h"
#define ULL unsigned long long

class LogPlayback
{
public:
    LogPlayback();
    ~LogPlayback();

    bool parseLogFile(const std::string& file);
    bool hasLodData();
    void playback();

private:
    void appedData(const unsigned long long slpTime, const std::string &data);

private:
    std::vector<std::pair<ULL, std::string>> mDataVector;
};

#endif // LOGPLAYBACK_H
