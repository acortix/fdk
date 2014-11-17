#include "synapse.h"
#include <FDK/brain/cell.h>

namespace FDK {
Synapse::Synapse(Cell *source, Segment *distanation)
{
    _source = source;
    _distanation = distanation;
    _strength = 0.3 + randf()/10.0;

    _source->addAxonalSynapse(this);
    _distanation->addSynapse(this);
}

void Synapse::predict(Time currentTime){
    if( _strength > 0.2)
        _distanation->predict(currentTime);
}

void Synapse::predictAtLowerThreshold(Time currentTime){
    if( _strength > 0.1)
        _distanation->predictAtLowerThreshold(currentTime);
}
}
