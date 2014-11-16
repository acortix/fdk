#ifndef ENCODER_H
#define ENCODER_H
#include "encoderinterface.h"
#include <FDK/brain/region.h>
#include <list>
#include <map>
using namespace std;
namespace FDK {

template<typename T>
struct EncoderSettings{
    RegionSettings regionSettings;
    vector<T> possibleSignals;
};

template<class T>
class Encoder : public EncoderInterface
{
public:
    Encoder(EncoderSettings<T> settings) : EncoderInterface(){
        _settings = settings;
    }

    void step(){
        Atom::step();
        if(!_buffer.empty()){
            _map[_buffer.front()]->copyTo( _output );
            _buffer.pop_front();
        }
        this->output()->step();
    }


    Sensor * nextOutput(){
        if(!_buffer.empty()) {
            return _map[ _buffer.front() ];
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
        _buffer.push_back(v);
    }

    T       decode(Sensor * sensor){
        UInt bestScore = 0;
        UInt currentScore = 0;


        T bestChar = _map.begin()->first;

        for( pair<T,Sensor*> value : _map ){
            currentScore = 0;
            for(UInt x = 0; x < _settings.regionSettings.sensorSettings.width; x++){
                for(UInt y = 0; y < _settings.regionSettings.sensorSettings.height; y++){
                    if(_map[ value.first ]->read(x,y) &&  (_map[ value.first ]->read(x,y) == sensor->read(x,y)) ){
                        currentScore++;
                    }
                }
            }
            if(currentScore > bestScore){
                bestScore = currentScore;
                bestChar = value.first ;
            }
        }
        return bestChar;
    }

protected:
    EncoderSettings<T>      _settings;
    UInt                    _neededCharges;
    Sensor *                _output;
    map<T,Sensor*>          _map;
    list<T>                 _buffer;
};
}
#endif // ENCODER_H
