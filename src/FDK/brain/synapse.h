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

    Float strength() { return _strength; }
    void increase(Float value) { _strength += value; _strength = std::min(_strength,1.0f);}
    void decrease(Float value) { _strength -= value; _strength = std::max(_strength,0.0f);}

    void predictAtLowerThreshold(Time currentTime);
    void predict(Time currentTime);

    Cell * source() { return _source; }
    Segment * distanation() { return _distanation; }

private:
    Cell *      _source;
    Segment *   _distanation;
    Float       _strength;
};
}
#endif // SYNAPSE_H
