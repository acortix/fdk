#include "timevector.h"
#include <qdebug.h>
namespace FDK{
TimeVector::TimeVector(UInt length)
{
    _length = length;
    _currentTime = -1;
    for(UInt i = 0; i < _length; i++){
        _historyData.push_back(false);
    }
}

void TimeVector::set(Time t){

    if(_currentTime != t){
        for(UInt i = 0; i < t-_currentTime-1;i++){
            _historyData.pop_front();
            _historyData.push_back(false);
        }
        _historyData.pop_front();
        _historyData.push_back(true);
        _currentTime = t;
    }
}

bool TimeVector::get(Time t){
    if( t > _currentTime){
        //qDebug() << "Out of range in future";
        return false;
    } else if( (t - _currentTime) >= _length){
        //qDebug() << "Out of range: " << (t - _currentTime) << ">=" << _length;
        return false;
    } else {
        auto back = _historyData.end();
        //qDebug() << "Advance: " << (-( _currentTime-t));
        std::advance( back, -1- (_currentTime-t) );
        return *back;
    }
}
}
