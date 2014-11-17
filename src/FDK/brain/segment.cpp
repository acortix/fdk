#include "segment.h"
#include <FDK/brain/cell.h>
#include <qDebug>
namespace FDK {
Segment::Segment(Cell *sourceCell)
{
    _synapses       = new vector<Synapse*>();
    _sourceCell     = sourceCell;
    _activeSynapses = 0;
}

// Activation is calculated by calling predict from the cell.
// _activeSynapses should already contain necessary information.
UInt Segment::activation(Time currentTime){

    // The cell has had predictions at T-1 because predict()
    // is called at the end of the step. So for this iteration
    // we should check the T-1 value.
    if(currentTime - _segmentTime.predictionTimeAtLowerThreshold == 1){
        return _activeSynapsesAtLowerThreshold;

    // There were no prediction at T-1 - return 0
    } else {
        return 0;
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

void Segment::predictAtLowerThreshold(Time currentTime){
    if(_segmentTime.predictionTimeAtLowerThreshold != currentTime){
        _activeSynapsesAtLowerThreshold = 1;
        _segmentTime.predictionTimeAtLowerThreshold = currentTime;
    } else {
        _activeSynapsesAtLowerThreshold++;
        // No need to call predict on cell, as the lower threshold is not used
        /*
        if(_activeSynapsesAtLowerThreshold > 5){
            _sourceCell->predictAtLowerThreshold(currentTime);
        }
        */
    }
}
}
