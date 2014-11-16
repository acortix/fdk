#include "synapse.h"
#include <FDK/brain/cell.h>

namespace FDK {
Synapse::Synapse(Cell *source, Segment *distanation)
{
    _source = source;
    _distanation = distanation;
    _strength = 1.0;

    _source->addAxonalSynapse(this);
    _distanation->addSynapse(this);
}
void Synapse::predict(Time currentTime){
    _distanation->predict(currentTime);
}
}
