#include "segment.h"
#include <FDK/brain/cell.h>
#include <qDebug>
namespace FDK {
Segment::Segment(Cell *sourceCell)
{
    _synapses = new vector<Synapse*>();
    _sourceCell = sourceCell;
    _activeSynapses = 0;
}

UInt Segment::activation(Time currentTime){
    if(_calculationTime == currentTime){
        return _activeSynapses;
    } else {
        _activeSynapses = 0;
        for(Synapse * synapse : *_synapses){
            if(synapse->strength() > 0.1 && synapse->source()->wasExcited(currentTime)){
                _activeSynapses++;
                qDebug() << "Found";
            }
        }
        _calculationTime = currentTime;
        return _activeSynapses;
    }
}

void Segment::predict(Time currentTime){
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
