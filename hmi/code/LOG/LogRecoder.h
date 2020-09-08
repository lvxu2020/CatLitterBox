#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

#define LOG_FILE_SIZE_MAXIMUM (1024 * 1024 * 10) // 10M
#define LOG_FILE_COUNT_MAX 2
#define VECTOR_RESERVE_SIZE 20000


#define LOG_FILE_DIR "/tmp/CatLog/"

#define LOG_CONFIG_FILE "LogConfig"
#define LOG_FILE_NAME_HEAD "Log_"

class LogRecoder {
public:
    static LogRecoder& instance();

    bool initLogDir();
    void insertLog(const std::string &logType, const std::string &logData);
    void writeToFile(const std::string &file);

    const std::string getCurrentLogFile();
    std::size_t getFileSize(const std::string &file);
    const std::string createLogFile();

    void enableLogRecod();
    void disableLogRecod();

    bool isExist(const std::string &pathname);

private:
    const std::string getCurrentTime();
    bool createFile(const std::string &file);
    bool removeFile(const std::string &file);
    bool createLogDir(const std::string &dir);

    std::size_t getLogFileCount();
    const std::string readLogConfigFile();
    void updateLogConfigFile(std::size_t firstIndex, std::size_t lastIndex);

private:
    static void init();
    static void destroy();

private:
    std::mutex mtx;
    std::vector<std::string> mLogDataVector;

    bool mEnableLogRecod = true;

    std::size_t mLogFirstIndex = 0;
    std::size_t mLogLastIndex = 0;

private:
    LogRecoder();
    ~LogRecoder();
    LogRecoder(const LogRecoder&) = delete; // 禁止拷贝
    LogRecoder& operator=(const LogRecoder&) = delete; //禁止赋值

    static pthread_once_t ponce_;
    static LogRecoder*    value_;
};
