#ifndef SEGMENT_H
#define SEGMENT_H
#include <FDK/atom.h>
#include <FDK/brain/synapse.h>
#include <QList>

namespace FDK {
class Cell;
struct SegmentTime{
    fdkuint predictionTime = 0;

    void reset(){
        predictionTime = 0;
    }
};

class Segment : public Atom
{
public:
    Segment(Cell * sourceCell);
    fdkuint activation( fdkutime currentTime );
    QList<Synapse*> * synapses() { return _synapses; }
    void addSynapse( Synapse* synapse ) { _synapses->append(synapse); }
    void predict(fdkutime currentTime);
private:
    QList<Synapse*> * _synapses;
    fdkuint _calculationTime = -1;
    fdkuint _activeSynapses = 0;
    Cell * _sourceCell;
    SegmentTime _segmentTime;

};
}
#endif // SEGMENT_H
