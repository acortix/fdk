#ifndef ATOM_H
#define ATOM_H
#include <FDK/fdkstructures.h>

namespace FDK {

class Atom
{
public:
    Atom();
    Time time();
    void step();
private:
    Time _time = 0;
};
}
#endif // ATOM_H
