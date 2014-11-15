#include "sparsecharencoder.h"
#include <qdebug.h>
namespace FDK {
SparseCharEncoder::SparseCharEncoder(EncoderSettings<char> settings) : Encoder<char>(settings)
{

    _output = new Sensor(settings.regionSettings.sensorSettings);
    _neededCharges = settings.regionSettings.sensorSettings.width * settings.regionSettings.sensorSettings.height * settings.regionSettings.sparsity;

    // Generate SDR representations for all possible inputs
    Sensor * progressSensor = new Sensor(settings.regionSettings.sensorSettings);
    progressSensor->fullyDischarge();

    for(char c : settings.possibleSignals){

        fdkuint activeCharges = 0;

        Sensor * randomSensor = new Sensor(settings.regionSettings.sensorSettings);
        randomSensor->fullyDischarge();

        for(fdkuint x = 0; x < settings.regionSettings.sensorSettings.width; x++){
            for(fdkuint y = 0; y < settings.regionSettings.sensorSettings.height; y++){
                if(!progressSensor->read(x,y)){
                    activeCharges++;
                    randomSensor->charge(x,y);
                }
            }
        }

        while(activeCharges > _neededCharges){
            for(fdkuint x = 0; x < settings.regionSettings.sensorSettings.width; x++){
                for(fdkuint y = 0; y < settings.regionSettings.sensorSettings.height; y++){
                    if(activeCharges > _neededCharges && randomSensor->read(x,y)){
                        fdkfloat r = randf();
                        if(r > 0.8){
                            randomSensor->discharge(x,y);
                            activeCharges--;
                        }
                    }
                }
            }
        }

        for(fdkuint x = 0; x < settings.regionSettings.sensorSettings.width; x++){
            for(fdkuint y = 0; y < settings.regionSettings.sensorSettings.height; y++){
                if(randomSensor->read(x,y)){
                    progressSensor->charge(x,y);
                }
            }
        }

        _map[c] = randomSensor;
        qDebug() << "Generated SDR for " << c;
    }

    delete progressSensor;

}
char SparseCharEncoder::nextCharacter(){
    if(!_buffer.isEmpty()) {
        return _buffer.first();
    } else {
        return 0;
    }
}
void SparseCharEncoder::appendToBuffer(QString v){
    for(QChar c : v){
        Encoder::appendToBuffer(c.toLatin1());
    }

}
void SparseCharEncoder::step(){
    Encoder::step();
}
}
