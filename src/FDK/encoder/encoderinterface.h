#ifndef ENCODERINTERFACE_H
#define ENCODERINTERFACE_H
#include <FDK/atom.h>
#include <FDK/connection/sensor.h>
#include <QHash>

namespace FDK {
class EncoderInterface : public Atom
{
public:
    EncoderInterface() : Atom(){

    }
    virtual Sensor *            output() = 0;
    virtual Sensor *            nextOutput() = 0;
    virtual void                step() = 0;
};
}
#endif // ENCODERINTERFACE_H
