#ifndef SPARSEENCODER_H
#define SPARSEENCODER_H
#include "encoder.h"


namespace FDK {
class SparseCharEncoder : public Encoder<char>
{
public:
    SparseCharEncoder(EncoderSettings<char> settings);
    char     nextCharacter();
    void     appendToBuffer(QString v);
    void     step();
};
}
#endif // SPARSEENCODER_H
