#ifndef FDKSTRUCTURES_H
#define FDKSTRUCTURES_H
#include <vector>
using namespace std;
namespace FDK {

typedef long long int           Time;
typedef unsigned long int       UInt;
typedef float                   Float;

struct UIntPoint{
    UIntPoint(){
        this->x = 0;
        this->y = 0;
    }

    UIntPoint(UInt x, UInt y){
        this->x = x;
        this->y = y;
    }

    UInt x = 0;
    UInt y = 0;
};
struct UIntSize{
    UInt width;
    UInt height;
};









inline Float randf() { return static_cast <Float> (rand()) / static_cast <Float> (RAND_MAX);}
inline UInt randuint(UInt a, UInt b) {return static_cast<UInt>( rand() % (b-a) + a); }
}
#endif // FDKSTRUCTURES_H
