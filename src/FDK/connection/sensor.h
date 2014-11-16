#ifndef SENSOR_H
#define SENSOR_H
#include <FDK/atom.h>
#include <vector>
using namespace std;
namespace FDK {

class Sensor : public Atom
{
public:
    Sensor(SensorSettings settings);
    Sensor(UInt width, UInt height);
    ~Sensor();

    bool                            read(UInt x, UInt y);
    void                            write(UInt x, UInt y, bool value);
    void                            copyTo(Sensor * to);
    void                            charge(UInt x, UInt y);
    void                            discharge(UInt x, UInt y);
    void                            fullyCharge();
    void                            fullyDischarge();
    vector<UIntPoint> *             buffer()                                { return _buffer; }
    SensorSettings                  settings()                              { return _settings; }

private:
    SensorSettings                  _settings;
    bool *                          _values;
    vector<UIntPoint> *             _buffer;
    UInt                            _internalSize;

};
}
#endif // SENSOR_H
