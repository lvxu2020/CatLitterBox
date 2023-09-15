#include "Ota.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fstream>
#include <thread>
#include <fstream>

#define HMI_RUNNING_CONF_PATH "/home/debian/Back/running"
#define VERSION_PATH_1 "/home/debian/Cat/ver1/ver.conf"
#define VERSION_PATH_2 "/home/debian/Cat/ver2/ver.conf"

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
    std::ifstream conf(HMI_RUNNING_CONF_PATH);
    char buf[10] = {0};
    conf.getline(buf,10);
    int run = atoi(buf), oldV = 0, newV = 0;
    DEBUG("running app is %d", run);
    conf.close();
    if (run == 2) {
        system("rm /home/debian/Cat/ver1/update.tar");
        std::ifstream ver(VERSION_PATH_1);
        char buf[10] = {0};
        ver.getline(buf,10);
        oldV = atoi(buf);
        ver.close();
        downFile.downloadFile("/home/debian/Cat/ver1/update.tar");
        DEBUG("down load ok");
        popen("sync", "r");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        popen("sync", "r");

        FILE *fp;
        // 解压会失败
        for (int i = 10; i > 0; i--) {
            if (newV > oldV) break;
            popen("tar -xf /home/debian/Cat/ver1/update.tar", "r");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            fp = popen("sync", "r");
            std::ifstream verNew(VERSION_PATH_1);
            memset(buf, '\0', 10);
            verNew.seekg(0);
            verNew.getline(buf,10,'\0');
            newV = atoi(buf);
            verNew.close();
        }

        system("chmod 777 /home/debian/Cat/ver1/App");
        DEBUG("New v:%d;Old v:%d", newV, oldV);
        upDateResault(newV > oldV);
    } else if (run == 1) {
        system("rm /home/debian/Cat/ver2/update.tar");
        std::ifstream ver(VERSION_PATH_2);
        char buf[10] = {0};
        ver.getline(buf,10);
        oldV = atoi(buf);
        ver.close();
        downFile.downloadFile("/home/debian/Cat/ver2/update.tar");
        popen("sync", "r");
        DEBUG("down load ok");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        popen("sync", "r");

        FILE *fp;
        // 解压会失败
        for (int i = 10; i > 0; i--) {
            if (newV > oldV) break;
            popen("tar -xf /home/debian/Cat/ver2/update.tar", "r");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            fp = popen("sync", "r");
            std::ifstream verNew(VERSION_PATH_2);
            memset(buf, '\0', 10);
            verNew.seekg(0);
            verNew.getline(buf,10, '\0');
            newV = atoi(buf);
            DEBUG("i is:%d,new is :%d\n", i, newV);
            verNew.close();
        }

        system("chmod 777 /home/debian/Cat/ver2/App");
        DEBUG("New v:%d;Old v:%d", newV, oldV);
        upDateResault(newV > oldV);
    } else {
        DEBUG("OTA CONF is bad");
        upDateResault(false);
        return nullptr;
    }

    return nullptr;
}

bool Ota::upDateResault(bool resault)
{
    Node root;
    std::string str;
    JsonAdapter::addValueToNode(root, "id", "0");
    if (resault) {
        JsonAdapter::addValueToNode(root, "data", "1");
    } else {
        JsonAdapter::addValueToNode(root, "data", "0");
    }
    JsonAdapter::getUnFormatStrFromNode(root,str);

    DbusAdapter send;
    if (send.sendASignal("/hmi/path", "code.back", "signal", str)) {
        DEBUG("%s", str.c_str());
    }
}


