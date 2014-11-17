#ifndef SEGMENT_H
#define SEGMENT_H
#include <FDK/atom.h>
#include <FDK/brain/synapse.h>
#include <vector>
using namespace std;
namespace FDK {
class Cell;
// Segment time structure. TODO: replace with new time vector
struct SegmentTime{
    UInt predictionTime = 0;
    UInt predictionTimeAtLowerThreshold = 0;

    void reset(){
        predictionTime = 0;
        predictionTimeAtLowerThreshold = 0;
    }
};
// Class that describes dendrical segments
class Segment : public Atom
{
public:
    Segment(Cell * sourceCell);

    UInt                    activation( Time currentTime );
    vector<Synapse*> *      synapses() { return _synapses; }
    void                    addSynapse( Synapse* synapse ) { _synapses->push_back(synapse); }
    void                    predict(Time currentTime);
    void                    predictAtLowerThreshold(Time currentTime);
private:
    vector<Synapse*> *      _synapses;

    UInt                    _activeSynapses = 0;
    UInt                    _activeSynapsesAtLowerThreshold = 0;
    Cell *                  _sourceCell;
    SegmentTime             _segmentTime;

};
}
#endif // SEGMENT_H
