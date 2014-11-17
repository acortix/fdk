#include "segment.h"
#include <FDK/brain/cell.h>
#include <FDK/brain/column.h>
#include <FDK/brain/region.h>

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
    if(currentTime - _segmentTime.predictionTimeAtLowerThreshold == 0){
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
        if(_activeSynapses > _sourceCell->column()->region()->settings()->computedActivationThreshold ){
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
        if(_activeSynapsesAtLowerThreshold > _sourceCell->column()->region()->settings()->computedMinActivationThreshold ){
            _sourceCell->predictAtLowerThreshold(currentTime, _activeSynapsesAtLowerThreshold);
        }
    }
}
}
