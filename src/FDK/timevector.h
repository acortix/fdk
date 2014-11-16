#ifndef TIMEVECTOR_H
#define TIMEVECTOR_H

#include <FDK/fdkstructures.h>
#include <list>
using namespace std;

namespace FDK{
class TimeVector
{
public:
    TimeVector(UInt length = 10);
    void         set(Time t);
    bool         get(Time t);

private:
    Time         _length;
    Time         _currentTime;
    list<bool>   _historyData;
};
}
#endif // TIMEVECTOR_H
