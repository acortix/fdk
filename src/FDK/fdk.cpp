#include "fdk.h"
#include <cstdlib>
#include <ctime>

FrankDevelopmentKit::FrankDevelopmentKit()
{
}

// BRAIN STRUCTURES
FDK::Brain * FrankDevelopmentKit::createBrain(){
    srand (static_cast <unsigned> (time(0)));

    return new FDK::Brain();
}

FDK::Region * FrankDevelopmentKit::createRegion(FDK::RegionSettings settings){
    return new FDK::Region(settings);
}
// CONNECTION STRUCTURES
FDK::Sensor * FrankDevelopmentKit::createSensor(FDK::SensorSettings settings){
    return new FDK::Sensor(settings);
}
FDK::Connection * FrankDevelopmentKit::createConnection(FDK::Sensor * from, FDK::Sensor * to){
    return new FDK::Connection(from,to);
}
// ENCODER STRUCTURES
FDK::SparseCharEncoder * FrankDevelopmentKit::createSparseCharEncoder(FDK::EncoderSettings<char> settings){
    return new FDK::SparseCharEncoder(settings);
}
