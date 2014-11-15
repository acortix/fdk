#ifndef COLUMN_H
#define COLUMN_H
#include <FDK/atom.h>
#include <FDK/brain/cell.h>
#include <vector>
using namespace std;
namespace FDK {
class Region;
struct ColumnTime {
    fdkuint activeTime = 0;
    fdkuint predictionTime = 0;

    void reset(){
        activeTime = 0;
        predictionTime = 0;
    }
};
class Column : public Atom
{
public:
                    Column(fdkuint x, fdkuint y, fdkuint depth, Region * region);

    fdkuint         x()                                     { return _x; }
    fdkuint         y()                                     { return _y; }

    void            activate(fdkutime currentTime);
    void            deactivate(fdkutime currentTime);
    void            predict(fdkutime currentTime);
    fdkuint         predictionPotential()                   { return _predictionPotential; }

    bool            isActive(fdkutime currentTime)          { return currentTime - _columnTime.activeTime == 0 ? true : false; }

    bool            isPredicted(fdkutime currentTime);

    fdkuint         depth()                                 { return _depth; }

    vector<Cell*>*  cells()                                 { return _cells; }

    Region*         region()                                {return _region; }

    static bool comparePredictivePotential( Column * a, Column * b){
        return a->predictionPotential() < b->predictionPotential();
    }

private:
    fdkuint         _x;
    fdkuint         _y;
    fdkuint         _depth;

    fdkuint         _predictionPotential;

    vector<Cell*> * _cells;
    ColumnTime      _columnTime;
    Region *        _region;

};
}
#endif // COLUMN_H
