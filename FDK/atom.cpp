#include "atom.h"

namespace FDK {
Atom::Atom()
{

}


Time Atom::time(){
    return _time;
}

void Atom::step(){
    _time++;
}
}
