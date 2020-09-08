#include "LogPlayback.h"

#include <string.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <unistd.h>



LogPlayback::LogPlayback()
{
    mDataVector.reserve(20000);
}

LogPlayback::~LogPlayback()
{
    mDataVector.clear();
}

bool LogPlayback::parseLogFile(const std::string &file)
{
    mDataVector.clear();

    std::fstream fileStream;

    // 判断文件是否存在
    if (access(file.c_str(), F_OK) != 0) {
        std::cout << file << " not exist" << std::endl;
        return false;
    }

    // 打开文件
    fileStream.open(file.c_str(), std::fstream::in);
    if (!fileStream.is_open()) {
        std::cout << file << " open failed" << std::endl;
        return false;
    }


    unsigned long long microseconds;
    char dataType[20];
    char dataValue[5000];

    std::string data;
    bool isFirst = true;
    ULL prevMicroSeconds = 0;

    while (!(fileStream.peek() == EOF)) {
        std::getline(fileStream, data); // fileStream.readLine();
        sscanf(data.c_str(), "[%*[^ ] %*[^ ] %llu][%[^]]]%s", &microseconds, dataType, dataValue);

        if (strcmp(dataType, "B2K") != 0) {
            continue;
        }

        if (isFirst) {
            isFirst = false;
            appedData(0, dataValue);
            prevMicroSeconds = microseconds;
            continue;
        }

        appedData(microseconds - prevMicroSeconds, dataValue);
        prevMicroSeconds = microseconds;
    }

    fileStream.close();

    return true;
}

bool LogPlayback::hasLodData()
{
    return (mDataVector.size() > 0);
}

void LogPlayback::playback()
{
    for (auto data : mDataVector) {
        std::this_thread::sleep_for(std::chrono::microseconds(data.first));
//        Send_a_signal_UI(data.second);
    }
}

void LogPlayback::appedData(const ULL slpTime, const std::string &data)
{
    mDataVector.emplace_back(std::make_pair(slpTime, data));
}
