#include "Ota.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

Ota* Ota::single = nullptr;
pthread_once_t Ota::ponce_ = PTHREAD_ONCE_INIT;

Ota* Ota::instance()
{
    pthread_once(&Ota::ponce_, &Ota::creatSingle);
    assert(single != NULL); //假设成立，程序继续运行，否则报错终止
    return single;
}

void Ota::setHttpPath(std::string path)
{
    m_httpPath = path;
}

Ota::Ota()
{

}

Ota::~Ota()
{

}

void Ota::creatSingle()
{
    single = new Ota();
    ::atexit(destorySingle);
}

void Ota::destorySingle()
{
    delete single;
    single = nullptr;
}

void* Ota::run(void *arg)
{
    HttpRequest downFile(m_httpPath);
    downFile.setRequestMethod("GET");
    //不直接使用缓存，要求向服务器发起（新鲜度校验）请求
    downFile.setRequestProperty("Cache-Control", "no-cache");
    /* ****************************************
     * text/html ： HTML格式
     * text/plain ：纯文本格式
     * text/xml ：  XML格式
     * image/gif ：gif图片格式
     * image/jpeg ：jpg图片格式
     * image/png：png图片格式
     * application/xhtml+xml ：XHTML格式
     * application/xml     ： XML数据格式
     * application/atom+xml  ：Atom XML聚合格式
     * application/json    ： JSON数据格式
     * application/pdf       ：pdf格式
     * application/msword  ： Word文档格式
     * application/octet-stream ： 二进制流数据（如常见的文件下载）
     * application/x-www-form-urlencoded ： <form encType=””>中默认的encType，form表单数据被编码为key/value格式发送到服务器（表单默认的提交数据的格式）
     * 另外一种常见的媒体格式是上传文件之时使用的：multipart/form-data ： 需要在表单中进行文件上传时，就需要使用该格式
     *  ************************************* */
    downFile.setRequestProperty("Content-Type", "application/octet-stream");
    // 表示是否需要持久连接。（HTTP 1.1默认进行持久连接）
    downFile.setRequestProperty("Connection", "close\r\n");
    // tcp链接到服务器
    downFile.connect(80);
    // 将填写好的报文写入socket套接字
    downFile.send();
    // 服务器的响应存在buff里
    downFile.handleRead();
    downFile.downloadFile("/home/debian/test/App");
    system("sudo chmod 777 /home/debian/test/App");

    return nullptr;
}


