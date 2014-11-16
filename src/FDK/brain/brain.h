#ifndef BRAIN_H
#define BRAIN_H
#include <FDK/atom.h>
#include <FDK/connection/connection.h>
#include <FDK/encoder/sparsecharencoder.h>

#include "region.h"
#include <vector>

using namespace std;
namespace FDK {

/* Main container and processing class for all brain atoms
 *
 *
 */
class Brain : public Atom {
public:
    Brain();

    // Add new region to the brain
    void                        addRegion(Region * region);

    // Add new encoder to the brain
    void                        addEncoder(EncoderInterface * encoder);

    // Add new connection to the brain. Connections usually connect region to region or encoder to region
    void                        addConnection(Connection * connection);

    // Simmulate one time step for the whole system
    void                        step();

private:
    // List of all connections
    vector<Connection*> *       _connections;

    // List containing all encoders
    vector<EncoderInterface*> * _encoders;

    // List containting all regions
    vector<Region*>     *       _regions;
};
}
#endif // BRAIN_H
