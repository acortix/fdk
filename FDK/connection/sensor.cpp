#include "sensor.h"
#include <QDebug>

namespace FDK {
Sensor::Sensor(fdkuint width, fdkuint height){
    _settings.width = width;
    _settings.height = height;
    _internalSize = width*height;
    _values = new bool[_internalSize];
    fullyDischarge();
}

Sensor::Sensor(SensorSettings settings) : Atom()
{
    _settings = settings;
    _internalSize = settings.width*settings.height;
    _values = new bool[_internalSize];
}


void Sensor::charge(fdkuint x, fdkuint y){
    write(x,y,true);
}
void Sensor::discharge(fdkuint x, fdkuint y){
    write(x,y,false);
}

void Sensor::fullyCharge(){
    for(fdkuint i = 0; i < _internalSize; i++){
        _values[i] = true;
    }
}
void Sensor::fullyDischarge(){
    for(fdkuint i = 0; i < _internalSize; i++){
        _values[i] = false;
    }
}

bool Sensor::read(fdkuint x, fdkuint y){
    if(x < _settings.width && y < _settings.height){
        return _values[ x + _settings.width*y  ];
    } else {
        qDebug() << "Sensor Error: Tried to access invalid index";
        return false;
    }
}

void Sensor::write(fdkuint x, fdkuint y, bool value){
    if(x < _settings.width && y < _settings.height){
        _values[ x + _settings.width*y  ] = value;
    } else {
        qDebug() << "Sensor Error: Tried to access invalid index";
    }
}

void Sensor::copyTo(Sensor * to){
    for(fdkuint i = 0; i < _internalSize; i++){
        to->_values[i] = _values[i];
    }
}

Sensor::~Sensor(){
    delete [] _values;
}
}
