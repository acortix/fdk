#ifndef CELL_H
#define CELL_H
#include <FDK/atom.h>
#include <FDK/brain/segment.h>
#include <FDK/timevector.h>
#include <vector>

using namespace std;
namespace FDK {
class Column;

// Structure that represents current cell time.
// It remembers when it was last excited, bursted and predicted
struct CellTime {
    TimeVector excitedTime;
    TimeVector prevExcitedTime;
    TimeVector burstTime;
    TimeVector predictTime;
};

// Class that represents functionality of cells
class Cell : public Atom
{
public:
    Cell(UInt i, Column * column);

    // Getters for internal data
    // Cell's column
    Column *            column()                                { return _column; }
    // Cell's index inside a column
    UInt                index()                                 { return _index;}
    // Cell's time structure
    CellTime            cellTime()                              { return _cellTime; }
    // List of distal dendrites
    vector<Segment*> *  distalDendriteSegments()                { return _distalDendriteSegments; }
    // List of axonal synapses
    vector<Synapse*> *  axonalSynapses()                        { return _axonalSynapses; }

    // Reset cell
    void                reset();

    // Excite cell at particular time currentTime
    void                excite(Time currentTime)                { _cellTime.excitedTime.set( currentTime ); }

    // Predict cell at particular time currentTime
    void                predict(Time currentTime);

    // Burst cell at particular time currentTime
    void                burst(Time currentTime)                 { _cellTime.burstTime.set( currentTime ); }

    // Check if cell IS @t or WAS @(t-1) at a particular state
    bool                isExcited(Time currentTime)          { return _cellTime.excitedTime.get( currentTime ); }
    bool                wasExcited(Time currentTime)         { return _cellTime.excitedTime.get( currentTime -1 );  }
    bool                isPredicted(Time currentTime)        { return _cellTime.predictTime.get( currentTime ); }
    bool                wasPredicted(Time currentTime)       { return _cellTime.predictTime.get( currentTime -1 ); }
    bool                isBursting(Time currentTime)         { return _cellTime.burstTime.get( currentTime ); }
    bool                wasBursting(Time currentTime)        { return _cellTime.burstTime.get( currentTime -1 ); }

    // Learning algorithm for all segments
    void                adaptSegments(Float rate, Time currentTime);

    // Adds new distal dendrite segment
    void                addDistalDendriteSegment(Segment * segment);

    // Adds new axonal synapse
    void                addAxonalSynapse(Synapse * synapse);

private:
    UInt                _index;
    Column *            _column;
    CellTime            _cellTime;

    vector<Segment *> * _distalDendriteSegments;
    vector<Segment *> * _distalExternalDendriteSegments;
    vector<Synapse *> * _axonalSynapses;
};
}
#endif // CELL_H
