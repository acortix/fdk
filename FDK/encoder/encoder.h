#ifndef ENCODER_H
#define ENCODER_H
#include "encoderinterface.h"
#include <qdebug.h>
namespace FDK {
template<class T>
class Encoder : public EncoderInterface
{
public:
    Encoder(EncoderSettings<T> settings) : EncoderInterface(){
        _settings = settings;
    }

    void step(){
        Atom::step();
        if(!_buffer.isEmpty()){
            _map[_buffer.first()]->copyTo( _output );
            _buffer.pop_front();
        }
        this->output()->step();
    }


    Sensor * nextOutput(){
        if(!_buffer.isEmpty()) {
            return _map[ _buffer.first() ];
        } else {
            return NULL;
        }
    }

    Sensor * output(){
        return _output;
    }

    T       nextValue(){
        if(!_buffer.isEmpty()) {
            return _buffer.first();
        } else {
            return 0;
        }
    }
    void    appendToBuffer(T v){
        _buffer.append(v);
    }

    T       decode(Sensor * sensor){
        fdkuint bestScore = 0;
        fdkuint currentScore = 0;
        char bestChar = _map.keys().first();
        for( T c : _map.keys() ){
            currentScore = 0;
            for(fdkuint x = 0; x < _settings.regionSettings.sensorSettings.width; x++){
                for(fdkuint y = 0; y < _settings.regionSettings.sensorSettings.height; y++){
                    if(_map[c]->read(x,y) &&  (_map[c]->read(x,y) == sensor->read(x,y)) ){
                        currentScore++;
                    }
                }
            }
            if(currentScore > bestScore){
                bestScore = currentScore;
                bestChar = c;
            }
        }
        return bestChar;
    }

protected:
    EncoderSettings<T>      _settings;
    fdkuint            _neededCharges;
    Sensor *                _output;
    QHash<T,Sensor*>        _map;
    QList<T>                _buffer;
};
}
#endif // ENCODER_H
