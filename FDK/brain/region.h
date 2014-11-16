#ifndef REGION_H
#define REGION_H
#include <QList>
#include <FDK/atom.h>
#include <FDK/connection/sensor.h>
#include "column.h"


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

private:
    RegionSettings              _settings;
    RegionData                  _regionData;
    Sensor *                    _input;
    Sensor *                    _output;


    QList<QList<Column*>> *     _columns;

    QList<Cell*>                _excitedCells;
    QList<Cell*>                _learningCells;
    QList<Cell*>                _predictedCells;
    QList<Cell*>                _previouslyExcitedCells;
    QList<Cell*>                _burstingCells;

    QList<Column*>              _predictedColumns;

    void                        spatialPooler();
    void                        activateColumns();
    void                        growSegments();
    void                        adjustDistalSynapses();
    void                        makePrediction();



};
}
#endif // REGION_H
