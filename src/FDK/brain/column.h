#ifndef COLUMN_H
#define COLUMN_H
#include <FDK/atom.h>
#include <FDK/brain/cell.h>
#include <vector>
using namespace std;
namespace FDK {
class Region;
// TODO: Replace this structure with time vector
struct ColumnTime {
    UInt activeTime = 0;
    UInt predictionTime = 0;

    void reset(){
        activeTime = 0;
        predictionTime = 0;
    }
};
// Single column class
class Column : public Atom
{
public:
                    Column(UInt x, UInt y, UInt depth, Region * region);

    // Position of the column inside the 2D region array
    UInt            x()                                     { return _x; }
    UInt            y()                                     { return _y; }

    // TODO: See if these can be removed. Not used atm.
    void            activate(Time currentTime);
    void            deactivate(Time currentTime);

    // Mark column predicted and increment/reset prediction potential
    void            predict(Time currentTime);
    // Return current prediction potential
    UInt            predictionPotential()                   { return _predictionPotential; }

    // Checks if the column is active
    bool            isActive(Time currentTime)              { return currentTime - _columnTime.activeTime == 0 ? true : false; }
    // Checks if the column has been predicted
    bool            isPredicted(Time currentTime);

    // Returns how many cells the column has
    UInt            depth()                                 { return _depth; }
    // Returns a pointer to the vector of cells
    vector<Cell*>*  cells()                                 { return _cells; }

    // Pointer to a region this column belongs to (for reactive)
    Region*         region()                                {return _region; }

    // This function is used in column sorting to find top N columns with
    // highest predictive potential: How many time the cells in the column
    // have been predicted during single time step.
    static bool     comparePredictivePotential( Column * a, Column * b){
        return a->predictionPotential() < b->predictionPotential();
    }

private:
    // X position in the region
    UInt            _x;
    // Y position in the region
    UInt            _y;
    // Number of cells per column
    UInt            _depth;
    // How many time the cells in the column
    // have been predicted during single time step.
    UInt            _predictionPotential;
    // List of all cells in the column
    vector<Cell*> * _cells;
    // Column time object. TODO: Update the column to use time vecotr
    ColumnTime      _columnTime;
    // Pointer to a region this column is a part of
    Region *        _region;

};
}
#endif // COLUMN_H
