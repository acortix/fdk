#include "synapse.h"
#include <FDK/brain/cell.h>
#include <FDK/brain/column.h>
#include <FDK/brain/region.h>

namespace FDK {
Synapse::Synapse(Cell *source, Segment *distanation)
{
    _source = source;
    _distanation = distanation;
    _strength = source->column()->region()->settings()->defaultSynapseStrength + randf()/10.0;

    _source->addAxonalSynapse(this);
    _distanation->addSynapse(this);
}

void Synapse::predict(Time currentTime){
    if( _strength > _source->column()->region()->settings()->synapseThreshold )
        _distanation->predict(currentTime);
}

void Synapse::predictAtLowerThreshold(Time currentTime){
    if( _strength > _source->column()->region()->settings()->minSynapseThreshold )
        _distanation->predictAtLowerThreshold(currentTime);
}
}
