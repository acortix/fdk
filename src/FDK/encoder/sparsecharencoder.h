#ifndef SPARSEENCODER_H
#define SPARSEENCODER_H
#include "encoder.h"
#include <string>
using namespace std;

namespace FDK {
class SparseCharEncoder : public Encoder<char>
{
public:
    SparseCharEncoder(EncoderSettings<char> settings);
    char     nextCharacter();
    void     appendToBuffer(string v);
    void     step();
};
}
#endif // SPARSEENCODER_H
