#include "brain.h"
#include <QDebug>
namespace FDK{

// Default constructor
Brain::Brain() : Atom()
{
    // Initialize all necessary arrays
    _connections = new vector<Connection*>();
    _encoders = new vector<EncoderInterface*>();
    _regions = new vector<Region*>();
}

// Add new region to the brain
void Brain::addRegion(Region * region){
    _regions->push_back(region);
}

// Add new encoder to the brain
void Brain::addEncoder(EncoderInterface *encoder){
    _encoders->push_back(encoder);
}

// Add new connection to the brain
void Brain::addConnection(Connection * connection){
    _connections->push_back(connection);
}

// Run simmulation for one time step
void Brain::step(){

    // Progress current time to t+1
    Atom::step();


    // Progress all encoders. Since encoders are the only structures that have no inputs,
    // they should be executed first
    for( EncoderInterface * encoder : *_encoders){
        // Progress encoder to t+1
        encoder->step();
    }

    // Variable that checks if all regions had progressed and updated to t+1
    bool allRegionsUpdated = true;
    do {
        // Reset to default value
        allRegionsUpdated = true;

        // Progress all connections. This will run through most connections.
        // At first pass it will solve all Encoder->Region connections ensuring
        // that input() of each region is exactly at t+1. After most connections
        // have been progressed to t+1, _some_ regions will be able to calulate
        // and set their output() to t+1. At this point we will progress more
        // connections to ensure that regions that have other regions as inputs
        // will progress to t+1. We'll do so untill we run out of regions that
        // are at time t.
        for(Connection * connection : *_connections){
            connection->step();
        }

        // Progress all regions
        for(Region * region : *_regions){
            if( (region->input()->time() == this->time() ) && (region->time() < this->time()) ){
                region->step();
            } else if(region->time() != this->time()){
                allRegionsUpdated = false;
            }
        }
    } while(!allRegionsUpdated);
}
}
