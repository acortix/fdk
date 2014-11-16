#ifndef COLUMN_H
#define COLUMN_H
#include <FDK/atom.h>
#include <FDK/brain/cell.h>
#include <vector>
using namespace std;
namespace FDK {
class Region;
struct ColumnTime {
    UInt activeTime = 0;
    UInt predictionTime = 0;

    void reset(){
        activeTime = 0;
        predictionTime = 0;
    }
};
class Column : public Atom
{
public:
                    Column(UInt x, UInt y, UInt depth, Region * region);

    UInt            x()                                     { return _x; }
    UInt            y()                                     { return _y; }

    void            activate(Time currentTime);
    void            deactivate(Time currentTime);
    void            predict(Time currentTime);
    UInt            predictionPotential()                   { return _predictionPotential; }

    bool            isActive(Time currentTime)              { return currentTime - _columnTime.activeTime == 0 ? true : false; }

    bool            isPredicted(Time currentTime);

    UInt            depth()                                 { return _depth; }

    vector<Cell*>*  cells()                                 { return _cells; }

    Region*         region()                                {return _region; }

    static bool     comparePredictivePotential( Column * a, Column * b){
        return a->predictionPotential() < b->predictionPotential();
    }

private:
    UInt            _x;
    UInt            _y;
    UInt            _depth;

    UInt            _predictionPotential;

    vector<Cell*> * _cells;
    ColumnTime      _columnTime;
    Region *        _region;

};
}
#endif // COLUMN_H
