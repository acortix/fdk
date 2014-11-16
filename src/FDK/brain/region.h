#ifndef REGION_H
#define REGION_H
#include <vector>
#include <FDK/atom.h>
#include <FDK/connection/sensor.h>
#include "column.h"
using namespace std;

namespace FDK {
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
    Sensor *                    input();
    Sensor *                    output();
    RegionData                  regionData()    { return _regionData; }

    Sensor *                    _debug;
    void                        addPredictedColumn(Column* c) {_predictedColumns.push_back(c); }
    void                        addPredictedCell(Cell* c) { _predictedCells.push_back(c);}

private:
    RegionSettings              _settings;
    RegionData                  _regionData;
    Sensor *                    _input;
    Sensor *                    _output;


    vector<vector<Column*>> *   _columns;

    vector<Cell*>               _excitedCells;
    vector<Cell*>               _learningCells;
    vector<Cell*>               _predictedCells;
    vector<Cell*>               _previouslyExcitedCells;
    vector<Cell*>               _burstingCells;

    vector<Column*>             _predictedColumns;

    void                        spatialPooler();
    void                        activateColumns();
    void                        growSegments();
    void                        adjustDistalSynapses();
    void                        makePrediction();



};
}
#endif // REGION_H
