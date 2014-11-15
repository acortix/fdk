#include "column.h"
namespace FDK {
Column::Column(fdkuint x, fdkuint y, fdkuint depth, Region *region)
{
    _x = x;
    _y = y;
    _depth = depth;
    _cells = new vector<Cell*>();
    for(fdkuint i = 0; i < depth; i++){
        _cells->push_back( new Cell(i, this) );
    }
    _region = region;
}

void Column::activate(fdkutime currentTime){
    _columnTime.activeTime = currentTime;
    _predictionPotential = 0;
}

void Column::deactivate(fdkutime currentTime){
    _columnTime.reset();
    _predictionPotential = 0;
}

void Column::predict(fdkutime currentTime){
    if(_columnTime.predictionTime != currentTime){
        _predictionPotential = 0;
    }
    _columnTime.predictionTime = currentTime;
    _predictionPotential += 1;
}
bool Column::isPredicted(fdkutime currentTime){
    if( _columnTime.predictionTime == currentTime && _predictionPotential > 15){
        return true;
    } else {
        return false;
    }
}
}

