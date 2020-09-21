#include <string>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/uio.h>  // readv
#include <stdint.h>
#include <endian.h>
#include <unistd.h>
#include <map>
#include <fstream>

#include "HttpRequest.h"

const std::map<std::string, int>::value_type init_value[] =
{
    std::map<std::string, int>::value_type( "GET", HttpRequest::GET),

    std::map<std::string, int>::value_type( "POST", HttpRequest::POST)
};

const static std::map<std::string, int> kRequestMethodMap(init_value, init_value + (sizeof init_value / sizeof init_value[0]));

static inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htobe16(host16);
}


int sockets::createSocket(sa_family_t family){
  // Call "socket()" to create a (family) socket of the specified type.
  // But also set it to have the 'close on exec' property (if we can)

    int sock;

    //CLOEXEC，即当调用exec（）函数成功后，文件描述符会自动关闭。
    //在以往的内核版本（2.6.23以前）中，需要调用 fcntl(fd, F_SETFD, FD_CLOEXEC) 来设置这个属性。
    //而新版本（2.6.23开始）中，可以在调用open函数的时候，通过 flags 参数设置 CLOEXEC 功能，
#ifdef SOCK_CLOEXEC
  sock = socket(family, SOCK_STREAM|SOCK_CLOEXEC, 0);
  if (sock != -1 || errno != EINVAL) return sock;
  // An "errno" of EINVAL likely means that the system wasn't happy with the SOCK_CLOEXEC; fall through and try again without it:
#endif

  sock = socket(family, SOCK_STREAM, 0);

#ifdef FD_CLOEXEC
  if (sock != -1) fcntl(sock, F_SETFD, FD_CLOEXEC);
#endif
  return sock;
}

int sockets::connect(int sockfd, const struct sockaddr* addr)
{
  return ::connect(sockfd, addr, sizeof(struct sockaddr));
}

void sockets::fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);//主机转网络
  //将点分十进制的ip地址转化为用于网络传输的数值格式
  //返回值：若成功则为1，若输入不是有效的表达式则为0，若出错则为-1
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    std::cout << "sockets::fromIpPort";
  }
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
  return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
  return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
  return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd)
{
  if (::close(sockfd) < 0)
  {
    std::cout << "sockets::close";
  }
}

void sockets::delaySecond(int sec)
{
  struct  timeval tv;
  tv.tv_sec = sec;
  tv.tv_usec = 0;
  /* ****************************
   * 参数一：最大的文件描述符加1。
   * 参数二：用于检查可读性。
   * 参数三：用于检查可写性。
   * 参数四：用于检查带外数据。
   * 参数五：一个指向timeval结构的指针，
   * 用于决定select等待I/o的最长时间。
   * 如果为空将一直等待
   * 返回值：>0：就绪描述字的正数目，
   * -1：出错，0 ：超时
   * ************************* */
  select(0, NULL, NULL, NULL, &tv);
}



InetAddress::InetAddress(std::string ip, uint16_t port)
{
  ::bzero(&m_addr, sizeof m_addr);
  sockets::fromIpPort(ip.c_str(), port, &m_addr);
}

uint32_t InetAddress::ipNetEndian() const
{
  assert(family() == AF_INET);
  return m_addr.sin_addr.s_addr;
}


HttpRequest::HttpRequest(std::string httpUrl)
  :m_httpUrl(httpUrl)
{

}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::connect(int port = 80)
{
  char ip[32] = {0};
  while(true)
  {
    struct hostent* phost = NULL;
    //解析域名
    phost = gethostbyname(m_httpUrl.domain().c_str());
    if (NULL == phost)
    {
      std::cout << "HttpUrlToIp(): gethostbyname error : " << errno << " : "<< strerror(errno) << " continue." << std::endl;
      sockets::delaySecond(1);
      continue;
    }
    //将数值格式转化为点分十进制的ip地址格式
    inet_ntop(phost->h_addrtype,  phost->h_addr, ip, sizeof ip);
    std::cout << "HttpRequest::Connector() gethostbyname Successful";

    InetAddress serverAddr = InetAddress(ip, port);
    //创建套接字接口
    m_sockfd = sockets::createSocket(serverAddr.family());
    if(m_sockfd < 0) std::cout << "HttpRequest::connect() : createSocket error";
    //客户端建立tcp链接
    int ret = sockets::connect(m_sockfd, serverAddr.getSockAddr());
    std::cout << "sockfd : " << m_sockfd << "sockets::connect ret : " << ret << std::endl;
    int savedErrno = (ret == 0) ? 0 : errno;

    switch (savedErrno)
    {
      case 0:
      case EINPROGRESS:
      case EINTR:
      case EISCONN:
        std::cout << "HttpRequest::connect() sockfd : " << m_sockfd << " Successful" << std::endl;
        break;
      default :
        std::cout << "Connect Error ";
        sockets::delaySecond(1);
        continue;
    }

    break;
  }

  std::cout << "HttpRequest::Connector() end" << std::endl;
}

void HttpRequest::setRequestMethod(const std::string &method)
{
    switch(kRequestMethodMap.at(method))
    {
        case HttpRequest::GET :
            m_stream << "GET " << "/" << m_httpUrl.getHttpUrlSubSeg(HttpUrl::URI) << " HTTP/1.1\r\n";
            std::cout << m_stream.str().c_str();
            break;
        case HttpRequest::POST :
            m_stream << "POST "  << "/" << m_httpUrl.getHttpUrlSubSeg(HttpUrl::URI) << " HTTP/1.1\r\n";
            std::cout << m_stream.str().c_str();
            break;
        default :
            std::cout << "No such Method : " << method.c_str();
            break;
    }

    m_stream << "Host: " << m_httpUrl.getHttpUrlSubSeg(HttpUrl::HOST) << "\r\n";
}


void HttpRequest::setRequestProperty(const std::string &key, const std::string &value)
{
    m_stream << key << ": " << value << "\r\n";
}

void HttpRequest::setRequestBody(const std::string &content)
{
    m_stream << content;
}

/********************************
 * 报文存在buffer的容器m_buffer里。4096
 * 的空间，存完包头之后存数据，haswriten
 * 记录着数据写入的索引
 ***************************** */
void HttpRequest::handleRead()
{
    assert(!m_haveHandleHead);
    ssize_t nread = 0;

    nread = sockets::read(m_sockfd, m_buffer.beginWrite(), kBufferSize);
    if(nread < 0) std::cout << "sockets::read";
    m_buffer.hasWritten(nread);
    std::cout << "sockets::read(): nread: " << nread << " remain: " << m_buffer.writableBytes();
    size_t remain = kBufferSize - nread;
    while(remain > 0)
    {
        size_t n = sockets::read(m_sockfd, m_buffer.beginWrite(), remain);
        if(n < 0) std::cout << "sockets::read";
        m_buffer.hasWritten(n);
        if(0 == n)
        {
            std::cout << "sockets::read finish";
            break;
        }
        remain = remain - n;
    }
    //std::cout << m_buffer.peek();

    //for(int i = 0; i < nread; i++) printf("%02x%c",(unsigned char)buffer[i],i==nread - 1 ?'\n':' ');
    //std::cout << "handleRead Recv Response : \n" << m_buffer.peek();
    int headsize = 0;
    std::string line;
    std::stringstream ss(m_buffer.peek());
    std::vector<std::string> v;
    // getline(<字符数组chs>，<读取字符的个数n>，<终止符>)
    // 成员函数getline()是从输入流中读取一行字符，读到终止符时会将'0'存入结果缓冲区中，
    //作为输入的终止。终止符可以是默认的终止符，也可以是定义的终止符。
    getline(ss, line);
    //std::cout << line;
    headsize += line.size() + 1;
    SplitString(line, v, " ");
    //for(int i = 0; i < v.size(); i++) std::cout << v[i] << std::endl;
    m_code = std::stoi(v[1]);
    // HTTP/1.0 200 OK
    // Date: Thu, 03 Sep 2020 07:10:11 GMT
    // Server: Boa/0.94.13
    // Connection: close
    // Content-Length: 32035
    // Last-Modified: Tue, 01 Sep 2020 11:26:52 GMT
    // Content-Type: image/jpeg

    if(v[1] != "200")
    {
      std::cout << "Error Http Server Response : " << v[1].c_str();
    }

    do{
        getline(ss, line);
        headsize += line.size() + 1;  // + 1('\n')
        if(!line.empty()) line.erase(line.end()-1); // remove '/r'
        //std::cout << line;
        v.clear();
        SplitString(line, v, ":");
        if(v.size() == 2){
            m_ackProperty[v[0]] = v[1].erase(0,v[1].find_first_not_of(" "));
        }
    }while(!line.empty());

    std::cout << "Http Head Size is " << headsize;
    std::string res(m_buffer.peek(), headsize);
    std::cout << "Http Response :\n" << res;
    m_buffer.retrieve(headsize);

    m_haveHandleHead = true;

}

void HttpRequest::uploadFile(const std::string& file, const std::string& contentEnd)
{

    FILE* fp = fopen(file.c_str(), "rb");
    if(fp == NULL)
    {
        std::cout << "fopen() File :" << file.c_str() << " Errno";
    }

    bool isEnd = false;
    ssize_t writtenBytes = 0;

    assert(m_buffer.writableBytes() == Buffer::kInitialSize);

    while(!isEnd)
    {
        ssize_t nread = fread(m_buffer.beginWrite(), 1, kBufferSize, fp);
        m_buffer.hasWritten(nread);
        while(m_buffer.writableBytes() > 0)
        {
//            std::cout << "file read(): nread: " << nread << " remain: " << m_buffer.writableBytes();
            size_t n = fread(m_buffer.beginWrite(), 1, m_buffer.writableBytes(), fp);
            m_buffer.hasWritten(n);
            if(0 == n)
            {   int err = ferror(fp);
                if(err)
                {
                    fprintf(stderr, "fread failed : %s\n", strerror(err));
                }
                std::cout << "sockets::read finish";
                isEnd = true;
                break;
            }
        }

        ssize_t nwrite = sockets::write(m_sockfd, m_buffer.peek(), m_buffer.readableBytes());
        if(nwrite < 0) std::cout << "sockets::write";
        writtenBytes += nwrite;
        std::cout << "sockets::write nread " << m_buffer.readableBytes() << " nwrite " << nwrite << " writtenBytes " << writtenBytes;
        m_buffer.retrieve(nwrite);
    }

    fclose(fp);

    m_buffer.retrieveAll();

    ssize_t n = sockets::write(m_sockfd, contentEnd.c_str(), contentEnd.size());
    if(n < 0) std::cout << "sockets::write";
}

void HttpRequest::downloadFile(const std::string& file)
{
    assert(m_haveHandleHead);
    std::cout << "downloadFile in" << std::endl;
    bool isEnd = false;
    ssize_t nread = 0;
    ssize_t writtenBytes = 0;
    // ios::binary  换行符被翻译成 \n ，window的是\r\n
    std::ofstream output(file, std::ios::binary);
    if (!output.is_open()) { // 检查文件是否成功打开
        std::cout << "open file error" << file << std::endl;
    }

    output.write(m_buffer.peek(), m_buffer.readableBytes());
    writtenBytes += m_buffer.readableBytes();
    m_buffer.retrieve(m_buffer.readableBytes());

    std::cout << "Content-Length : " << getResponseProperty("Content-Length") << std::endl;

    while(!isEnd)
    {
        nread = sockets::read(m_sockfd, m_buffer.beginWrite(), kBufferSize);
        if(nread < 0) std::cout << "sockets::read";
        m_buffer.hasWritten(nread);
//        std::cout << "sockets::read(): nread: " << nread << " remain: " << m_buffer.writableBytes() << " writtenBytes: " << writtenBytes;
        size_t remain = kBufferSize - nread; 
        /*********************************************************
         * 小循环意义： 上面的read由于tcp的包一次传不完。有可能继续续包传输
         * 上面的read，是从fd中读取kBufferSize（4096字节）数据，但有时在
         * 未读到结尾时，只读到了2400（举例）字节数据。这种事经过测试是随机
         * 产生的，为了数据连续，小循环的作用就是：将其剩下的读完。并提供数
         * 据读取完毕的终止条件。不使用这个循环直接判别从fd读取的返回值n=0；
         * 也能完成功能，但是保持4096字节进行一次io操作。减少调用io的次数
         * 能提升一点点效率，对于小的下载任务可以忽略的其实。这里我保留了原来
         * 作者的设计初中。继续使用这个小循环。
         * ***************************************************** */
        while(remain > 0)
        {
            size_t n = sockets::read(m_sockfd, m_buffer.beginWrite(), remain);
            if(n < 0) std::cout << "sockets::read";
            m_buffer.hasWritten(n);
            if(0 == n)
            {
                std::cout << "sockets::read finish";
                isEnd = true;
                break;
            }
            remain = remain - n;
        }
        output.write(m_buffer.peek(), m_buffer.readableBytes());
        writtenBytes += m_buffer.readableBytes();
        m_buffer.retrieve(m_buffer.readableBytes());
    }
    std::cout << " writtenBytes " << writtenBytes << std::endl;

    output.close();
    sockets::close(m_sockfd);
    m_buffer.retrieveAll();
}

void HttpRequest::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}
