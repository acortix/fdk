#ifndef FDKSTRUCTURES_H
#define FDKSTRUCTURES_H
#include <QList>
namespace FDK {

typedef unsigned long long int  fdkutime;
typedef unsigned long int       fdkuint;
typedef float                   fdkfloat;

struct SensorSettings{
    fdkuint width;
    fdkuint height;
};

struct RegionSettings{
    SensorSettings sensorSettings;
    fdkuint depth;

    fdkfloat learningRadius;
    fdkfloat initialPerm;


    fdkfloat learningRate;
    fdkfloat sparsity;
    fdkuint desiredSparsity;

};

template<typename T>
struct EncoderSettings{
    RegionSettings regionSettings;
    QList<T> possibleSignals;
};

struct EncoderCharSettings : EncoderSettings<char>{

};

inline fdkfloat randf() { return static_cast <fdkfloat> (rand()) / static_cast <fdkfloat> (RAND_MAX);}
inline fdkuint randuint(fdkuint a, fdkuint b) {return static_cast<fdkuint>( rand() % (b-a) + a); }
}
#endif // FDKSTRUCTURES_H
