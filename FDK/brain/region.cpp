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
    for(fdkuint x = 0; x < settings.sensorSettings.width; x++){
        QList<Column*> column;
        for(fdkuint y = 0; y < settings.sensorSettings.height; y++){
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
    _predictedColumns.clear();
    _learningCells.clear();
    _predictedCells.clear();



    this->activateColumns();
    this->growSegments();


    this->adjustDistalSynapses();

    // Clear predictions



    this->makePrediction();




}
void Region::spatialPooler(){

}

void Region::activateColumns(){
    // Activate columns
    for(fdkuint x = 0; x < _settings.sensorSettings.width;x++){
        for(fdkuint y = 0; y < _settings.sensorSettings.height; y++){
            Column * column = _columns->at(x).at(y);
            if(_input->read(x,y)){
                column->activate( this->time() );
                _regionData.activatedColumns++;

                bool hasPredictedCells = false;
                for(Cell * cell : *column->cells()){
                    if(cell->wasPredicted( this->time() )){
                        cell->excite( this->time() );
                        _excitedCells.append(cell);
                        _predictedCells.append(cell);
                        hasPredictedCells = true;
                        _regionData.excitedCellsDueToPrediction++;
                    }
                }
                if(!hasPredictedCells){

                    // Find best cell
                    Cell * bestCell = column->cells()->at(0);
                    fdkuint bestScore = 0;
                    fdkuint currentScore = 0;

                    for(Cell * cell : *column->cells()){
                        for(Segment * segment : *cell->distalDendriteSegments()){
                            fdkuint activation = segment->activation( this->time() );
                            if(activation > currentScore){
                                bestScore = activation;
                                bestCell = cell;
                            }
                        }
                    }

                    // Best cell found
                    if(bestScore > 10){
                        bestCell->excite( this->time() );
                        _excitedCells.append(bestCell);
                        _learningCells.append(bestCell);
                        _regionData.excitedCellsDueActivation++;
                    // Best cell not found - return one with least amount of segments
                    } else {
                        fdkuint numberOfSegments = bestCell->distalDendriteSegments()->size();
                        for(Cell * cell: *column->cells()){
                            if(cell->distalDendriteSegments()->size() < numberOfSegments){
                                numberOfSegments = cell->distalDendriteSegments()->size();
                                bestCell = cell;
                            }
                        }

                        bestCell->excite( this->time() );
                        _excitedCells.append(bestCell);
                        _learningCells.append(bestCell);
                        _regionData.excitedCellsDueToLackOfSegments++;
                    }

                    // Burst cells for prediction
                    for(Cell * cell : *column->cells()){
                        // Check if cell wasn't excited in T-1
                        if(this->time() - cell->cellTime().excitedTime != 1){
                            cell->excite(this->time() );
                            _burstingCells.append(cell);
                        }
                    }
                }
            } else {
                column->deactivate( this->time() );
                for(Cell * cell : *column->cells()){
                    if(cell->wasPredicted( this->time() )){
                        _predictedCells.append(cell);
                    }
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

    for(fdkuint x = 0; x < _settings.sensorSettings.width;x++){
        for(fdkuint y = 0; y < _settings.sensorSettings.height; y++){
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

}

Sensor * Region::input(){
    return _input;
}

Sensor * Region::output(){
    return _output;
}
}
