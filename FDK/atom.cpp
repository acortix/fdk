#include "atom.h"

namespace FDK {
Atom::Atom()
{

}


fdkutime Atom::time(){
    return _time;
}

void Atom::step(){
    _time++;
}
}
