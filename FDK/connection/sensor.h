#ifndef SENSOR_H
#define SENSOR_H
#include <FDK/atom.h>

namespace FDK {
class Sensor : public Atom
{
public:
    Sensor(SensorSettings settings);
    Sensor(fdkuint width, fdkuint height);
    ~Sensor();

    bool read(fdkuint x, fdkuint y);
    void write(fdkuint x, fdkuint y, bool value);
    void copyTo(Sensor * to);
    void charge(fdkuint x, fdkuint y);
    void discharge(fdkuint x, fdkuint y);
    void fullyCharge();
    void fullyDischarge();
    SensorSettings settings() { return _settings; }

private:
    SensorSettings _settings;
    bool * _values;
    fdkuint _internalSize;

};
}
#endif // SENSOR_H
