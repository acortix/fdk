#ifndef SYNAPSE_H
#define SYNAPSE_H
#include <FDK/atom.h>
#include <algorithm>

namespace FDK {
class Cell;
class Segment;
class Synapse : public Atom
{
public:
    Synapse(Cell * source, Segment * distanation);

    fdkfloat strength() { return _strength; }
    void increase(fdkfloat value) { _strength += value; _strength = std::min(_strength,1.0f);}
    void decrease(fdkfloat value) { _strength -= value; _strength = std::max(_strength,0.0f);}

    void predict(fdkutime currentTime);

    Cell * source() { return _source; }
    Segment * distanation() { return _distanation; }

private:
    Cell *      _source;
    Segment *   _distanation;
    fdkfloat       _strength;
};
}
#endif // SYNAPSE_H
