#include "LogUpload.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>

LogUpload* LogUpload::single = nullptr;
pthread_once_t LogUpload::ponce_ = PTHREAD_ONCE_INIT;

int FileSize(const char* fname);

LogUpload* LogUpload::instance()
{
    pthread_once(&LogUpload::ponce_, &LogUpload::creatSingle);
    assert(single != NULL); //假设成立，程序继续运行，否则报错终止
    return single;
}

void LogUpload::setHttpPath(std::string path)
{
    m_httpPath = path;
}

LogUpload::LogUpload()
{

}

LogUpload::~LogUpload()
{

}

void LogUpload::creatSingle()
{
    single = new LogUpload();
    ::atexit(destorySingle);
}

void LogUpload::destorySingle()
{
    delete single;
    single = nullptr;
}

void* LogUpload::run(void *arg)
{
    off_t fileSize = FileSize(LOG_PATH);
    if (fileSize < 0) {
        return nullptr;
    }

    char time[64]={0};
    long long int timestamp;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    timestamp = (long long int)tv.tv_sec * 1000 + tv.tv_usec;
    snprintf(time, 64, "%lld", timestamp);

    std::stringstream content;
    std::string kBoundary(time);
    content << "--" + kBoundary << "\r\n";
    content << "Content-Disposition: form-data; name=\"upload\"; filename=\"log\"\r\n";
    content << "Content-Type: text/plain\r\n\r\n";
    std::string contentEnd = "\r\n--" + kBoundary + "--\r\n";

    HttpRequest upload(m_httpPath);
    upload.setRequestMethod("POST");
    upload.setRequestProperty("Content-Type", "multipart/form-data; boundary=" + kBoundary);
    upload.setRequestProperty("Cache-Control", "no-cache");
    upload.setRequestProperty("Content-Length", std::to_string(fileSize + content.str().size() + contentEnd.size()));
    upload.setRequestProperty("Connection", "close\r\n");
    upload.setRequestBody(content.str());
    upload.connect(80);
    upload.send();
    upload.uploadFile(LOG_PATH,contentEnd);
}

int FileSize(const char* fname)
{
    struct stat statbuf;
    if(stat(fname,&statbuf)==0)
        return statbuf.st_size;
    return -1;
}

