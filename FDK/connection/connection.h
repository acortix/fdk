#ifndef CONNECTION_H
#define CONNECTION_H
#include <FDK/atom.h>
#include "sensor.h"

namespace FDK {
class Connection : public Atom
{
public:
    Connection(Sensor * from, Sensor * to);
    void step();
private:
    Sensor * _from;
    Sensor * _to;
};
}
#endif // CONNECTION_H
