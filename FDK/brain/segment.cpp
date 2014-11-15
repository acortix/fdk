#include "segment.h"
#include <FDK/brain/cell.h>

namespace FDK {
Segment::Segment(Cell *sourceCell)
{
    _synapses = new QList<Synapse*>();
    _sourceCell = sourceCell;
}

fdkuint Segment::activation(fdkutime currentTime){
    if(_calculationTime == currentTime){
        return _activeSynapses;
    } else {
        _activeSynapses = 0;
        for(Synapse * synapse : *_synapses){
            if(synapse->strength() > 0.5 && synapse->source()->wasExcited(currentTime)){
                _activeSynapses++;
            }
        }
        _calculationTime = currentTime;
        return _activeSynapses;
    }
}

void Segment::predict(fdkutime currentTime){
    if(_segmentTime.predictionTime != currentTime){
        _activeSynapses = 1;
        _segmentTime.predictionTime = currentTime;
    } else {
        _activeSynapses++;
        if(_activeSynapses > 15){
            _sourceCell->predict(currentTime);
        }
    }
}
}
