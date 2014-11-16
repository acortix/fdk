#include "sensor.h"
#include <QDebug>
#include <algorithm>
namespace FDK {
Sensor::Sensor(UInt width, UInt height){
    _settings.width = width;
    _settings.height = height;
    _internalSize = width*height;
    _values = new bool[_internalSize];
    _buffer = new vector<UIntPoint>();
    fullyDischarge();
}

Sensor::Sensor(SensorSettings settings) : Atom()
{
    _settings = settings;
    _internalSize = settings.width*settings.height;
    _values = new bool[_internalSize];
    _buffer = new vector<UIntPoint>();
}


void Sensor::charge(UInt x, UInt y){
    if(!read(x,y)) _buffer->push_back( UIntPoint(x,y) );
    write(x,y,true);
}
void Sensor::discharge(UInt x, UInt y){
    if(read(x,y)){
        _buffer->erase( std::remove_if(_buffer->begin(),_buffer->end(), [=](const UIntPoint& p)
        { return ( (p.x==x) && (p.y==y) ) ; }) );
    }
    write(x,y,false);
}

void Sensor::fullyCharge(){
    for(UInt i = 0; i < _internalSize; i++){
        _values[i] = true;
    }
}
void Sensor::fullyDischarge(){

    for(UInt i = 0; i < _internalSize; i++){
        _values[i] = false;
    }
    _buffer->clear();
}

bool Sensor::read(UInt x, UInt y){
    if(x < _settings.width && y < _settings.height){
        return _values[ x + _settings.width*y  ];
    } else {
        qDebug() << "Sensor Error: Tried to access invalid index";
        return false;
    }
}

void Sensor::write(UInt x, UInt y, bool value){
    if(x < _settings.width && y < _settings.height){
        _values[ x + _settings.width*y  ] = value;
    } else {
        qDebug() << "Sensor Error: Tried to access invalid index";
    }
}

void Sensor::copyTo(Sensor * to){
    for(UInt i = 0; i < _internalSize; i++){
        to->_values[i] = _values[i];
    }
    *to->buffer() = *_buffer;
}

Sensor::~Sensor(){
    delete [] _values;
}
}
