#include "connection.h"
namespace FDK {
Connection::Connection(Sensor *from, Sensor *to) : Atom()
{
    _to = to;
    _from = from;
}

void Connection::step(){
    if(_from->time() > _to->time()){
        _to->step();
        _from->copyTo(_to);
        Atom::step();
    }
}
}
