#include "cell.h"
#include <FDK/brain/column.h>
#include <qdebug.h>
#include "region.h"
namespace FDK {

// Default constructor
Cell::Cell(UInt i, Column *column)
{
    // Index of a cell inside a column
    _index = i;

    // Reference to a column
    _column = column;

    _distalDendriteSegments = new vector<Segment *>();
    _distalExternalDendriteSegments = new vector<Segment *>();
    _axonalSynapses = new vector<Synapse *>();
}

// Learning algorithm for segments
void Cell::adaptSegments(Float rate, Time currentTime){
    // Iterate through each segment of all distal dendrites
    for(Segment * segment : *_distalDendriteSegments){
        // Iterate through each synapse of a current segment
        for(Synapse * synapse : *segment->synapses()){
            // If the cell on the other end of the synapse is excited at current time
            // Increase synapse strenght
            if(synapse->source()->isExcited( currentTime )){
                synapse->increase(rate);

            // If the cell on the other end of the synapse is NOT excited at current time
                // Increase synapse strenght
            } else {
                synapse->decrease(rate);
            }
        }
    }
}

// Add axonal synapse
void Cell::addAxonalSynapse(Synapse * synapse){
    _axonalSynapses->push_back(synapse);
}

// Add new distal dendrite segment
void Cell::addDistalDendriteSegment(Segment * segment){
    _distalDendriteSegments->push_back(segment);
}



// Predict cell
void Cell::predict(Time currentTime){
    if(!_cellTime.predictTime.get(currentTime)){
         _cellTime.predictTime.set( currentTime );
        _column->region()->addPredictedCell(this);
    }
    _column->predict(currentTime);
}

// Predict cell at lower threshold
void Cell::predictAtLowerThreshold(Time currentTime, UInt synapseActivation){
    if(!_cellTime.predictTimeAtLowerThreshold.get(currentTime)){
        _cellTime.predictTimeAtLowerThreshold.set( currentTime );
        _highestActivation = synapseActivation;
    }
    if(synapseActivation > _highestActivation){
        _highestActivation = synapseActivation;
    }

}
UInt Cell::activation(Time currentTime){
    if(_cellTime.predictTimeAtLowerThreshold.get(currentTime)){
        return _highestActivation;
    } else {
        return 0;
    }
}

}
