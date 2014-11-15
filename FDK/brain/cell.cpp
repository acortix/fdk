#include "cell.h"
#include <FDK/brain/column.h>
#include <qdebug.h>

namespace FDK {

// Default constructor
Cell::Cell(fdkuint i, Column *column)
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
void Cell::adaptSegments(fdkfloat rate, fdkutime currentTime){
    // Iterate through each segment of all distal dendrites
    for(Segment * segment : *_distalDendriteSegments){
        // Iterate through each synapse of a current segment
        for(Synapse * synapse : *segment->synapses()){
            // If the cell on the other end of the synapse is excited at current time
            // Increase synapse strenght
            if(synapse->source()->isExcited(currentTime)){
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
void Cell::predict(fdkutime currentTime){
    _cellTime.predictTime = currentTime;
    _column->predict(currentTime);
}

// Reset cell
void Cell::reset(){
    _cellTime.reset();
}
}
