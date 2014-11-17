#ifndef REGION_H
#define REGION_H
#include <vector>
#include <FDK/atom.h>
#include <FDK/connection/sensor.h>
#include "column.h"
using namespace std;

namespace FDK {
struct RegionSettings{
    SensorSettings sensorSettings;
    UInt  depth;

    Float learningRadius;
    Float initialPerm;

    Float learningRate;
    Float sparsity;
    UInt  desiredSparsity;

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
    }
};

class Region : public Atom
{


public:
    Region(RegionSettings settings);
    void step();
    // Accessor for input sensor.
    Sensor *                    input();
    // Accessor for output sensor
    Sensor *                    output();

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
    // Output sensor for decoding, or pushing to higher region
    Sensor *                    _output;

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
    void                        spatialPooler();
    void                        activateColumns();
    void                        growSegments();
    void                        adjustDistalSynapses();
    void                        makePrediction();



};
}
#endif // REGION_H
