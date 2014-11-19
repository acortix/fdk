#ifndef ATOM_H
#define ATOM_H
#include <FDK/fdkstructures.h>

namespace FDK {

class Atom
{
public:
    Atom();
    Time time();
    Time localTime();
    void step();
    void localStep();
private:
    Time _time = 0;
    Time _localTime = 0;
};
}
#endif // ATOM_H
