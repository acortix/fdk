#ifndef CELL_H
#define CELL_H
#include <FDK/atom.h>
#include <FDK/brain/segment.h>
#include <vector>

using namespace std;
namespace FDK {
class Column;

// Structure that represents current cell time.
// It remembers when it was last excited, bursted and predicted
struct CellTime {
    fdkutime excitedTime = 0;
    fdkutime burstTime = 0;
    fdkutime predictTime = 0;

    // Resets time in the cell
    void reset(){
        excitedTime = 0;
        burstTime = 0;
        predictTime = 0;
    }
};

// Class that represents functionality of cells
class Cell : public Atom
{
public:
    Cell(fdkuint i, Column * column);

    // Getters for internal data
    // Cell's column
    Column *            column()                                { return _column; }
    // Cell's index inside a column
    fdkuint             index()                                 { return _index;}
    // Cell's time structure
    CellTime            cellTime()                              { return _cellTime; }
    // List of distal dendrites
    vector<Segment*> *  distalDendriteSegments()                { return _distalDendriteSegments; }
    // List of axonal synapses
    vector<Synapse*> *  axonalSynapses()                        { return _axonalSynapses; }

    // Reset cell
    void                reset();

    // Excite cell at particular time currentTime
    void                excite(fdkutime currentTime)             { _cellTime.excitedTime = currentTime; }

    // Predict cell at particular time currentTime
    void                predict(fdkutime currentTime);

    // Burst cell at particular time currentTime
    void                burst(fdkutime currentTime)              { _cellTime.burstTime = currentTime; }

    // Check if cell IS @t or WAS @(t-1) at a particular state
    bool                isExcited(fdkutime currentTime)          { return currentTime - _cellTime.excitedTime == 0 ? true : false; }
    bool                wasExcited(fdkutime currentTime)         { return currentTime - _cellTime.excitedTime == 1 ? true : false; }
    bool                isPredicted(fdkutime currentTime)        { return currentTime - _cellTime.predictTime == 0 ? true : false; }
    bool                wasPredicted(fdkutime currentTime)       { return currentTime - _cellTime.predictTime == 1 ? true : false; }
    bool                isBursting(fdkutime currentTime)         { return currentTime - _cellTime.burstTime   == 0 ? true : false; }
    bool                wasBursting(fdkutime currentTime)        { return currentTime - _cellTime.burstTime   == 1 ? true : false; }

    // Learning algorithm for all segments
    void                adaptSegments(fdkfloat rate, fdkutime currentTime);

    // Adds new distal dendrite segment
    void                addDistalDendriteSegment(Segment * segment);

    // Adds new axonal synapse
    void                addAxonalSynapse(Synapse * synapse);

private:
    fdkuint             _index;
    Column *            _column;
    CellTime            _cellTime;

    vector<Segment *> * _distalDendriteSegments;
    vector<Segment *> * _distalExternalDendriteSegments;
    vector<Synapse *> * _axonalSynapses;
};
}
#endif // CELL_H
