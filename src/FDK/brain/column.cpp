#include "column.h"
#include "region.h"

namespace FDK {
Column::Column(UInt x, UInt y, UInt depth, Region *region)
{
    _x = x;
    _y = y;
    _depth = depth;
    _cells = new vector<Cell*>();
    for(UInt i = 0; i < depth; i++){
        _cells->push_back( new Cell(i, this) );
    }
    _region = region;
}

void Column::activate(Time currentTime){
    _columnTime.activeTime = currentTime;
    _predictionPotential = 0;
}

void Column::deactivate(Time currentTime){
    _columnTime.reset();
    _predictionPotential = 0;
}

void Column::predict(Time currentTime){
    if(_columnTime.predictionTime != currentTime){
        _predictionPotential = 0;
        _region->addPredictedColumn(this);
    }
    _columnTime.predictionTime = currentTime;
    _predictionPotential += 1;
}
bool Column::isPredicted(Time currentTime){
    if( _columnTime.predictionTime == currentTime && _predictionPotential > 15){
        return true;
    } else {
        return false;
    }
}
}

