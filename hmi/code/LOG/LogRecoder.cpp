#include "LogRecoder.h"

#include <ctime>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include "Debug.h"

using namespace std;

pthread_once_t LogRecoder::ponce_ = PTHREAD_ONCE_INIT;
LogRecoder*    LogRecoder::value_ = nullptr;

/*
 * 初始化log文件夹
 */
bool LogRecoder::initLogDir()
{
    if (isExist(LOG_FILE_DIR))
        return true;

    if (createLogDir(LOG_FILE_DIR))
        return true;

    return false;
}

/*
 * 存储log数据 并在数据前添加时间戳以及类型
 */
void LogRecoder::insertLog(const string &logType, const std::string &logData)
{
    if(!mEnableLogRecod){
        return;
    }

    std::string logStr;
    logStr.append(getCurrentTime());
    logStr.append(logType);
    logStr.append(logData);

    {
        std::lock_guard<std::mutex> lck(mtx);
        mLogDataVector.insert(mLogDataVector.end(), logStr);

#if 1
        // 当mVectorB2K数量大于预留空间大小 将数据写入文件
        if (mLogDataVector.size() > (VECTOR_RESERVE_SIZE * 2)) {
            mLogDataVector.clear();
        }
#endif
    }
}

/*
 * getCurrentTime 获取当前日期时间的时间戳
 * 时间戳格式: [year-month-day hour:min:sec:msec] // [2019-08-05 21:59:25:074]
 */
const std::string LogRecoder::getCurrentTime()
{
    char timeStr[100] = {0};

    auto timeNow = std::chrono::system_clock::now();
    auto microtime = std::chrono::duration_cast<std::chrono::microseconds>(timeNow.time_since_epoch()).count();
    int msec = (microtime / 1000) % 1000;

    auto tt = std::chrono::system_clock::to_time_t(timeNow);
    struct tm* ptm = localtime(&tt);

    sprintf(timeStr, "[%d-%02d-%02d %02d:%02d:%02d:%03d %s]",
            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, msec, std::to_string(microtime).c_str());

    return std::string(timeStr);
}

/*
 * 获取文件的大小（总字节数）
 */
std::size_t LogRecoder::getFileSize(const string &file)
{
    struct stat info;
    stat(file.c_str(), &info);
    std::size_t size =static_cast<std::size_t>(info.st_size);

    return size;
}

/*
 * 获取log文件总数量
 */
std::size_t LogRecoder::getLogFileCount()
{
    return (mLogLastIndex - mLogFirstIndex);
}

/*
 * 创建新的log文件，用于保存数据
 */
const std::string LogRecoder::createLogFile()
{
    // 如果存储的log总文件数量超过100个 删除最老的那个
    if (getLogFileCount() >= LOG_FILE_COUNT_MAX) {
        std::string oldFile = LOG_FILE_DIR LOG_FILE_NAME_HEAD + std::to_string(mLogFirstIndex);
        if (!removeFile(oldFile)) {
            return "";
        }

        updateLogConfigFile(++mLogFirstIndex, mLogLastIndex);
    }

    // 创建新的log文件 并刷新log配置文件的索引值（firstIndex lastIndex）
    std::string currentLogFile = LOG_FILE_DIR LOG_FILE_NAME_HEAD + std::to_string(++mLogLastIndex);
    if (!createFile(currentLogFile)) {
        return "";
    }

    updateLogConfigFile(mLogFirstIndex, mLogLastIndex);
    return currentLogFile;
}

void LogRecoder::enableLogRecod()
{
    mEnableLogRecod = true;
}

void LogRecoder::disableLogRecod()
{
    mEnableLogRecod = false;
}

/*
 * 创建文件
 */
bool LogRecoder::createFile(const string &file)
{
    fstream out_file;
    out_file.open(file.c_str(), std::ios::out);
    if (!out_file) {
        return false;
    }
    out_file.close();

    return true;
}

/*
 * 删除文件
 */
bool LogRecoder::removeFile(const string &file)
{
    // 删除文件，成功返回0，否则返回-1
    if (-1 == remove(file.c_str())) {
        return false;
    }

    return true;
}

/*
 * 查看某个文件或文件夹是否存在
 */
bool LogRecoder::isExist(const string &pathname)
{
    if (access(pathname.c_str(), F_OK) == 0) {
        return true;
    }

    return false;
}

/*
 * 创建文件夹
 */
bool LogRecoder::createLogDir(const std::string &dir)
{

#if 0
    // 不能创建多级目录
    int a = mkdir(dir.c_str(), 0777);
#endif

    std::string cmd = "mkdir -p ";
    cmd.append(dir);

    const int dir_err = system(cmd.c_str());
    if (-1 == dir_err) {
        return false;
    }

    return true;
}

/*
 * 更新log配置文件的内容，记录当前log文件的索引值(例：firstIndex=0;lasteIndex=100)
 */
void LogRecoder::updateLogConfigFile(std::size_t firstIndex, std::size_t lastIndex)
{
    std::string logConfigFile = LOG_FILE_DIR LOG_CONFIG_FILE;
    std::fstream in_conf_file(logConfigFile, std::fstream::out);

    char str[100];
    sprintf(str, "firstIndex=%zu;lasteIndex=%zu", firstIndex, lastIndex);

    in_conf_file << str << '\n';

    in_conf_file.close();
    in_conf_file.clear();
}

/*
 * 获取当前log文件，用于保存log
 */
const string LogRecoder::getCurrentLogFile()
{
    // 读取log配置文件的数据
    std::string configData = readLogConfigFile();

    // 解析log配置文件的数据，获取log文件的索引值 用来创建log文件的名称以及计算log文件的数量
    std::sscanf(configData.c_str(), "firstIndex=%zu;lasteIndex=%zu", &mLogFirstIndex, &mLogLastIndex);

    // 获取当前文件名称
    std::string currentLogFile = LOG_FILE_DIR LOG_FILE_NAME_HEAD + std::to_string(mLogLastIndex);

    // 如果文件不存在 则创建该文件
    if (!isExist(currentLogFile)) {
        if (createFile(currentLogFile)) {
            return currentLogFile;
        }

        return "";
    }

    // 如果文件大小大于10M 则创建新文件
    if (getFileSize(currentLogFile) > LOG_FILE_SIZE_MAXIMUM) {
        currentLogFile = createLogFile();
    }

    return currentLogFile;
}

/*
 * 获取log配置文件的数据
 */
const std::string LogRecoder::readLogConfigFile()
{
    std::string logConfigFile = LOG_FILE_DIR LOG_CONFIG_FILE;
    std::string dataStr;

    // 文件不存在 创建该文件并写入数据
    if (!isExist(logConfigFile)) {
        std::fstream in_conf_file(logConfigFile);
        in_conf_file.open(logConfigFile, std::fstream::out);

        dataStr = "firstIndex=0;lasteIndex=0";
        in_conf_file << dataStr.c_str() << '\n';

        in_conf_file.close();
        in_conf_file.clear();

        return dataStr;
    }

    // 文件存在 读取数据
    std::fstream in_conf_file(logConfigFile);
    getline(in_conf_file, dataStr);
    in_conf_file.close();
    in_conf_file.clear();

    return dataStr;
}

/*
 * 将缓存的log数据写入文件
 */
void LogRecoder::writeToFile(const std::string& file)
{
    if(!mEnableLogRecod){
        return;
    }

    // 添加线程锁
    std::lock_guard<std::mutex> lck (mtx);

    if (mLogDataVector.size() == 0)
        return;

    std::ofstream fout(file, std::ios::app);

    //is_open()返回真（1），代表打开成功
    if(!fout.is_open()) {
//        mLogDataVector.clear();
        return;
    }

    for(auto const& x : mLogDataVector) {
        if (x.c_str() == nullptr)
            continue;

        fout << x << '\n';
    }

    fout.close();
    mLogDataVector.clear();
}

LogRecoder &LogRecoder::instance()
{
    pthread_once(&ponce_, &LogRecoder::init);
    assert(value_ != NULL);
    return *value_;
}

void LogRecoder::init()
{
    value_ = new LogRecoder();
    ::atexit(destroy);
}

void LogRecoder::destroy()
{
    delete value_;
    value_ = nullptr;
}

LogRecoder::LogRecoder()
{
    // vector预留空间 防止内存重新分配而引起的性能消耗问题
    mLogDataVector.reserve(VECTOR_RESERVE_SIZE);
}

LogRecoder::~LogRecoder() {}
