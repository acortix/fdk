#include "atom.h"

namespace FDK {
Atom::Atom()
{

}


Time Atom::time(){
    return _time;
}
Time Atom::localTime(){
    return _localTime;
}

void Atom::step(){
    _time++;
}

void Atom::localStep(){
    _localTime++;
}
}
