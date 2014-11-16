#include "region.h"
#include <qdebug.h>
namespace FDK {
Region::Region(RegionSettings settings) : Atom()
{
    _settings = settings;


    _input = new Sensor(settings.sensorSettings);
    _output = new Sensor(settings.sensorSettings);
    _debug = new Sensor(settings.sensorSettings);
    _columns = new QList<QList<Column*>>();

    // Init columns
    for(UInt x = 0; x < settings.sensorSettings.width; x++){
        QList<Column*> column;
        for(UInt y = 0; y < settings.sensorSettings.height; y++){
            column.append(new Column(x,y, settings.depth, this));
        }
        _columns->append(column);
    }
}

// Region processing function
void Region::step(){
    Atom::step();

    _output->fullyDischarge();
    _debug->fullyDischarge();

    _regionData.reset();

    // Copy excited cells to previusly excited cells
    _previouslyExcitedCells.clear();
    _previouslyExcitedCells = QList<Cell*>(_excitedCells);

    // TODO: Perform SP

    // Clear time cache
    _excitedCells.clear();
    _burstingCells.clear();

    _learningCells.clear();




    this->activateColumns();
    this->growSegments();
    this->adjustDistalSynapses();
    this->makePrediction();




}
void Region::spatialPooler(){

}
/* Problems:
 * No cells are chosen due to activation
 * Negative adjustment works poorly
 *
 *
 *
 *
 */
void Region::activateColumns(){
    // Activate columns
    for(UIntPoint p : *_input->buffer()){

        // Set column variable to make code more readable
        Column * column = _columns->at(p.x).at(p.y);

        column->activate( this->time() );
        _regionData.activatedColumns++;

        bool hasPredictedCells = false;
        for(Cell * cell : *column->cells()){
            if(cell->wasPredicted( this->time() )){
                cell->excite( this->time() );
                _excitedCells.append(cell);
                hasPredictedCells = true;
                _regionData.excitedCellsDueToPrediction++;
            }
        }

        if(!hasPredictedCells){

            // Find best cell
            Cell * bestCell = column->cells()->at(0);
            UInt   bestScore = 0;

            for(Cell * cell : *column->cells()){
                for(Segment * segment : *cell->distalDendriteSegments()){
                    UInt activation = segment->activation( this->time() );
                    if(activation > bestScore){
                        bestScore = activation;
                        bestCell = cell;
                    }
                }
            }

            // Best cell found
            if(bestScore > 1){
                bestCell->excite( this->time() );
                _excitedCells.append(bestCell);
                _learningCells.append(bestCell);
                _regionData.excitedCellsDueActivation++;
                // Best cell not found - return one with least amount of segments
            } else {
                UInt numberOfSegments = bestCell->distalDendriteSegments()->size();
                for(Cell * cell: *column->cells()){
                    if(cell->distalDendriteSegments()->size() < numberOfSegments){
                        numberOfSegments = cell->distalDendriteSegments()->size();
                        bestCell = cell;
                    }
                }

                if(!bestCell->isExcited( this->time())){
                bestCell->excite( this->time() );
                _excitedCells.append(bestCell);
                }
                _learningCells.append(bestCell);
                _regionData.excitedCellsDueToLackOfSegments++;
            }

            // Burst cells for prediction
            for(Cell * cell : *column->cells()){
                // Check if cell wasn't excited in T-1
                if(!cell->wasExcited( this->time())){
                    cell->excite(this->time() );
                    _burstingCells.append(cell);
                }
            }
        }


    }
}

void Region::growSegments(){
    // Create synapses with previously excited cells if they don't exist.
    for(Cell * learningCell : _learningCells){
        Segment * newSegment = new Segment(learningCell);
        _regionData.newSegments++;
        for(Cell * previouslyExcitedCell : _previouslyExcitedCells){
            _debug->charge( previouslyExcitedCell->column()->x(), previouslyExcitedCell->column()->y() );
            new Synapse(previouslyExcitedCell,newSegment);
            _regionData.newSynapses++;
        }
        learningCell->addDistalDendriteSegment(newSegment);
    }
}

void Region::adjustDistalSynapses(){

    for(Cell * predictedCell : _predictedCells){
        if(predictedCell->isExcited( this->time()) ){
            // Reinforce positively
            predictedCell->adaptSegments( _settings.learningRate, this->time() );
            _regionData.positiveAdjustedSegments++;
        } else if(!predictedCell->isExcited( this->time()) ){
            // Reinforce negatively
            predictedCell->adaptSegments( -_settings.learningRate, this->time() );
            _regionData.negativeAdjustedSegments++;
        }
    }
}

void Region::makePrediction(){
    _predictedColumns.clear();
    _predictedCells.clear();



    for(Cell * excitedCell : _excitedCells){
        for(Synapse * synapse : *excitedCell->axonalSynapses() ){
            synapse->predict( this->time() );
            _regionData.excitedSynapsesForPrediction++;
        }
    }
    for(Cell * excitedCell : _burstingCells){
        for(Synapse * synapse : *excitedCell->axonalSynapses() ){
            synapse->predict( this->time() );
            _regionData.excitedSynapsesForPrediction++;
        }
    }
    _output->fullyDischarge();

    for(UInt x = 0; x < _settings.sensorSettings.width;x++){
        for(UInt y = 0; y < _settings.sensorSettings.height; y++){
            Column * column = _columns->at(x).at(y);
            if(column->isPredicted( this->time())){
                _predictedColumns.append(column);
                _regionData.predictedColumnsTotal++;
            }
        }
    }


    qSort(_predictedColumns.begin(),_predictedColumns.end(), Column::comparePredictivePotential);

    while( _predictedColumns.length() > _settings.desiredSparsity){
        _predictedColumns.pop_back();
    }

    // Set output
    for(Column * column : _predictedColumns){
        _output->charge(column->x(), column->y());
        _regionData.predictedColumnsAfterInhibition++;

    }


    // Set predicted state for cells
    for(UInt x = 0; x < _settings.sensorSettings.width;x++){
        for(UInt y = 0; y < _settings.sensorSettings.height; y++){
            Column * column = _columns->at(x).at(y);
            if(_input->read(x,y)){
                for(Cell * cell : *column->cells()){
                    if(cell->isPredicted( this->time() )){
                        _predictedCells.append(cell);
                    }
                }
            } else {
                for(Cell * cell : *column->cells()){
                    if(cell->isPredicted( this->time() )){
                        _predictedCells.append(cell);
                    }
                }
            }
        }
    }


}

Sensor * Region::input(){
    return _input;
}

Sensor * Region::output(){
    return _output;
}
}
