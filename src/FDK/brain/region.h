#ifndef REGION_H
#define REGION_H
#include <vector>
#include <FDK/atom.h>
#include <FDK/connection/sensor.h>
#include "column.h"

using namespace std;

namespace FDK {
struct SpatialPoolerSettings{
    Bool  enabled = false;
    Float learningRadius;
    Float initialPerm;
};
struct TemporalPoolerSettings{
    Bool enabled = false;
    UInt sparsityThreshold = 0;
};

struct RegionSettings{
    // General settings
    // Size of the sensor NxM
    SensorSettings          sensorSettings;
    // Number of cells in the column
    UInt                    depth;

    // General desired sparsity
    Float                   sparsity                      = 0.02;
    // Learning rate
    Float                   learningRate                  = 0.01;

    // Topology
    // Max distal dendrite segments
    UInt                    maxSegments                    = 10;

    // Signaling
    // Percent of active synapses in a segment for it to become active
    Float                   activationThreshold           = 0.37;
    // Percent of minimum active synapses in a segment for it to cause
    // activity in the cell (for choosing non predicted cell)
    Float                   minActivationThreshold        = 0.05;
    Float                   defaultSynapseStrength        = 0.3;
    Float                   synapseThreshold              = 0.2;
    Float                   minSynapseThreshold           = 0.1;

    // Pooler settings
    SpatialPoolerSettings   spatialPoolerSettings;
    TemporalPoolerSettings  temporalPoolerSettings;


    // Camputed internally
    UInt                    computedSparsity;
    UInt                    computedActivationThreshold;
    UInt                    computedMinActivationThreshold;

    // Fill in some derivative values
    void compute(){
        computedSparsity                            = (UInt)( (Float)(sensorSettings.width*sensorSettings.height)*sparsity);
        computedActivationThreshold                 = computedSparsity*activationThreshold;
        computedMinActivationThreshold              = computedSparsity*minActivationThreshold;
        temporalPoolerSettings.sparsityThreshold    = computedSparsity*3;
    }

};
// Debug data. This structure is updated each time step.
struct RegionData {
    UInt activatedColumns = 0;
    UInt predictedColumnsTotal = 0;
    UInt predictedColumnsAfterInhibition = 0;

    UInt newSegments = 0;
    UInt newSynapses = 0;
    UInt positiveAdjustedSegments = 0;
    UInt negativeAdjustedSegments = 0;

    UInt excitedCellsDueToPrediction = 0;
    UInt excitedCellsDueActivation = 0;
    UInt excitedCellsDueToLackOfSegments = 0;

    UInt excitedSynapsesForPrediction = 0;

    UInt executionTime = 0;

    void reset(){
        activatedColumns = 0;
        predictedColumnsTotal = 0;
        predictedColumnsAfterInhibition = 0;

        newSegments = 0;
        newSynapses = 0;
        positiveAdjustedSegments = 0;
        negativeAdjustedSegments = 0;

        excitedCellsDueToPrediction = 0;
        excitedCellsDueActivation = 0;
        excitedCellsDueToLackOfSegments = 0;

        excitedSynapsesForPrediction = 0;

        executionTime = 0;
    }
};

class Region : public Atom
{


public:
    Region(RegionSettings settings);
    void step();
    // Accessor for input sensor.
    Sensor *                    input();
    // Input (lower)region for temporal pooler of this region
    Region *                    inputRegion();
    // Accessor for output sensor
    Sensor *                    output();
    // Output region for the tempolar pooler of the higher region.
    Region *                    outputRegion();

    // Accessor for settings
    RegionSettings *            settings();

    // Accessor for debug information about region statistic
    RegionData                  regionData()    { return _regionData; }


    // Debug sensor. Can be used in code to visualize any data
    Sensor *                    _debug;

    // Adds a column to predicted column list
    void                        addPredictedColumn(Column* c) {_predictedColumns.push_back(c); }
    // Adds a cell to predicted cell list
    void                        addPredictedCell(Cell* c) { _predictedCells.push_back(c);}

private:
    // Region settings
    RegionSettings              _settings;
    // Debug and statistical information about the region
    RegionData                  _regionData;

    // Input sensor for column activation
    Sensor *                    _input;
    // Input (lower)region for temporal pooler of this region
    Region *                    _inputRegion        = NULL;
    // Output sensor for decoding, or pushing to higher region
    Sensor *                    _output;
    // Output region for the tempolar pooler of the higher region.
    Region *                    _outputRegion       = NULL;

    // List of all columns in the region as a 2D vector
    vector<vector<Column*>> *   _columns;

    // List of cells excited at T
    vector<Cell*>               _excitedCells;
    // List of cells that were excited at T-1
    vector<Cell*>               _previouslyExcitedCells;
    // List of cells that should be used in learning
    vector<Cell*>               _learningCells;
    // List of cells that have been predicted at T-1
    vector<Cell*>               _predictedCells;
    // List of bursting cells at T
    vector<Cell*>               _burstingCells;
    // List of predicted clumns at T+1
    vector<Column*>             _predictedColumns;

    // Pipeline for the temporal memory
    void                        pooler();
    void                        activateColumn(Column* column);
    void                        growSegments();
    void                        adjustDistalSynapses();
    void                        makePrediction();



};
}
#endif // REGION_H
