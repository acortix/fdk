#include "sensorconnection.h"
namespace FDK {
SensorConnection::SensorConnection(Sensor *from, Sensor *to) : Atom()
{
    _to = to;
    _from = from;
}

void SensorConnection::step(){
    if(_from->time() > _to->time()){
        _to->step();
        _from->copyTo(_to);
        Atom::step();
    }
}
}
