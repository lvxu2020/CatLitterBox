#ifndef BASE_H
#define BASE_H

#include <QObject>
#include <QDebug>
#include "../LOG/LogRecoder.h"
#include "../LOG/Debug.h"

namespace BaseSpace{
    using namespace std;
    void mySleep(int );
    std::vector<std::string> mySplit(const std::string& in, const std::string& delim);
    char* qStringToC(QString& );
    std::string trimStr(const std::string& str);
}
using namespace BaseSpace;
class Base : public QObject
{
    Q_OBJECT
public:
    Base();
};

#endif // BASE_H
