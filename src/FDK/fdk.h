#ifndef FDK_H
#define FDK_H
#include <FDK/brain/brain.h>
#include <FDK/brain/region.h>
#include <FDK/connection/connection.h>
#include <FDK/connection/sensor.h>
#include <FDK/encoder/sparsecharencoder.h>


class FrankDevelopmentKit
{
public:
    FrankDevelopmentKit();

    // BRAIN STRUCTURES
    static FDK::Brain * createBrain();
    static FDK::Region * createRegion(FDK::RegionSettings settings);

    // CONNECTION STRUCTURES
    static FDK::Sensor * createSensor(FDK::SensorSettings settings);
    static FDK::Connection * createConnection(FDK::Sensor * from, FDK::Sensor * to);
    // ENCODER
    static FDK::SparseCharEncoder * createSparseCharEncoder(FDK::EncoderCharSettings settings);

};



#endif // FDK_H
