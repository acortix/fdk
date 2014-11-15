#ifndef ATOM_H
#define ATOM_H
#include <FDK/fdkstructures.h>

namespace FDK {

class Atom
{
public:
    Atom();
    fdkutime time();
    void step();
private:
    fdkutime _time = 0;
};
}
#endif // ATOM_H
