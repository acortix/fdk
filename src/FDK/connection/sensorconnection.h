#ifndef SENSORCONNECTION_H
#define SENSORCONNECTION_H
#include <FDK/atom.h>
#include "sensor.h"

namespace FDK {
class SensorConnection : public Atom
{
public:
    SensorConnection(Sensor * from, Sensor * to);
    void step();
private:
    Sensor * _from;
    Sensor * _to;
};
}
#endif // SENSORCONNECTION_H
