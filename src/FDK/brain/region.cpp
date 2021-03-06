#include "region.h"
#include <qdebug.h>
#include <iostream>
#include <ctime>

namespace FDK {

Region::Region(RegionSettings settings) : Atom()
{
    _settings = settings;
    _settings.compute();

    _input = new Sensor(settings.sensorSettings);
    _output = new Sensor(settings.sensorSettings);
    _debug = new Sensor(settings.sensorSettings);
    _columns = new vector<vector<Column*>>();

    // Init columns
    for(UInt x = 0; x < settings.sensorSettings.width; x++){
        vector<Column*> column;
        for(UInt y = 0; y < settings.sensorSettings.height; y++){
            column.push_back(new Column(x,y, settings.depth, this));
        }
        _columns->push_back(column);
    }
}

// Region processing function
void Region::step(){
    std::clock_t startTime = std::clock();

    // Update global clock. It is different from local clock,
    // because in case of an active temporal pooler clock of the
    // temporal memory is not updated every timestep
    Atom::step();
    Atom::localStep();

    _output->fullyDischarge();
    _debug->fullyDischarge();
    _regionData.reset();

    // Copy excited cells to previusly excited cells
    _previouslyExcitedCells.clear();
    _previouslyExcitedCells = _excitedCells;

    // Clear time cache
    _excitedCells.clear();
    _burstingCells.clear();
    _learningCells.clear();

    // SP/TP/Pass
    this->pooler();

    // Temporal memory
    this->growSegments();
    this->adjustDistalSynapses();

    // Make prediction for the next step
    this->makePrediction();

    _regionData.executionTime = (float( std::clock () - startTime ) /  CLOCKS_PER_SEC)*1000.0f;

}



void Region::pooler(){

    for(UIntPoint p : *_input->buffer()){
        activateColumn( _columns->at(p.x).at(p.y) );
    }
}

// Activate column
void Region::activateColumn(Column* column){
    // Activate column
    column->activate( this->localTime() );

    // Update debug info
    _regionData.activatedColumns++;

    // Check if there are any predicted cells in the column
    bool hasPredictedCells = false;

    // For each cell in the column
    for(Cell * cell : *column->cells()){

        // If the cell was predicted...
        if(cell->wasPredicted( this->localTime() )){

            // Excite this cell
            cell->excite( this->localTime() );

            // Add this cell to a buffer, so that we could keep track of it
            _excitedCells.push_back(cell);

            // Ensure that we know that there are predicted cells outside of the loop
            hasPredictedCells = true;

            // Update debug info
            _regionData.excitedCellsDueToPrediction++;
        }
    }

    // If there are no predicted cells, we should use other means of activating them.
    // Currently there are two ways of doing that:
    if(!hasPredictedCells){

        // Find best cell that had most activations at T-1
        // Grab first cell as the best one
        Cell * bestCell = column->cells()->at(0);

        // Ensure that the best score is 0
        UInt   bestScore = 0;

        // Check each cell for best activation
        for(Cell * cell : *column->cells()){
            if( cell->activation( this->localTime()-1) > bestScore){
                // Activation is precomputed reactively during prediction at previous time step
                bestScore = cell->activation( this->localTime()-1);
                bestCell = cell;
            }
        }

        // Best cell found if it had at least 1 activation in one of the segments
        if(bestScore > 1){
            bestCell->excite( this->localTime() );
            _excitedCells.push_back(bestCell);
            _learningCells.push_back(bestCell);
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
            // Make sure that we don't double excite the cell
            if(!bestCell->isExcited( this->localTime())){
                bestCell->excite( this->localTime() );
                _excitedCells.push_back(bestCell);
            }
            _learningCells.push_back(bestCell);
            _regionData.excitedCellsDueToLackOfSegments++;
        }

        // Burst cells for prediction
        for(Cell * cell : *column->cells()){
            // Check if cell isn't excited already
            if(!cell->isExcited( this->localTime())){
                cell->excite(this->localTime() );
                _burstingCells.push_back(cell);
            }
        }
    }
}

// Add segments to learning cells.
void Region::growSegments(){
    // Create synapses with previously excited cells if they don't exist.
    for(Cell * learningCell : _learningCells){
        if(learningCell->distalDendriteSegments()->size() < _settings.maxSegments){
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
}

// Adjust synapse weights for all predicted cells
void Region::adjustDistalSynapses(){

    for(Cell * predictedCell : _predictedCells){
        if(predictedCell->isExcited( this->localTime()) ){
            // Reinforce positively
            predictedCell->adaptSegments( _settings.learningRate, this->localTime() );
            _regionData.positiveAdjustedSegments++;
        } else if(!predictedCell->isExcited( this->localTime()) ){
            // Reinforce negatively
            predictedCell->adaptSegments( -_settings.learningRate, this->localTime() );
            _regionData.negativeAdjustedSegments++;
        }
    }
}

// Make prediction for T+1
void Region::makePrediction(){
    _predictedColumns.clear();
    _predictedCells.clear();


    // For each excited cell run prediction for T+1
    for(Cell * excitedCell : _excitedCells){
        // Go through all axonal synapses of the excited cell
        for(Synapse * synapse : *excitedCell->axonalSynapses() ){
            // Execute reactive prediction for T+1
            synapse->predict( this->localTime() );

            // Execute reactive prediction at lowere threshold
            // This calculation is used for choosing best cell,
            // when there are no predicted cells in the column
            // at T+1
            synapse->predictAtLowerThreshold( this->localTime() );

            // Update debug infor
            _regionData.excitedSynapsesForPrediction++;
        }
    }
    // For each bursting cell run prediction for T+1
    for(Cell * excitedCell : _burstingCells){
        // Go through all axonal synapses of the excited cell
        for(Synapse * synapse : *excitedCell->axonalSynapses() ){
            // Execute reactive prediction for T+1
            synapse->predict( this->localTime() );

            // Execute reactive prediction at lowere threshold
            // This calculation is used for choosing best cell,
            // when there are no predicted cells in the column
            // at T+1
            synapse->predictAtLowerThreshold( this->localTime() );

            // Update debug infor
            _regionData.excitedSynapsesForPrediction++;
        }
    }

    // Reset sensor output of the region
    _output->fullyDischarge();
    _regionData.predictedColumnsTotal = _predictedColumns.size();

    // Sort columns by their predictive potential
    std::sort(_predictedColumns.begin(),_predictedColumns.end(), Column::comparePredictivePotential);

    // Remove all columns above sparsity levels
    while( _predictedColumns.size() > _settings.computedSparsity){
        _predictedColumns.pop_back();
    }

    // Set sensor output
    for(Column * column : _predictedColumns){
        _output->charge(column->x(), column->y());
        _regionData.predictedColumnsAfterInhibition++;

    }

    // We're done. Ready to go to next time step!
}

// Sensor input accessor
Sensor * Region::input(){
    return _input;
}
Region * Region::inputRegion(){
    return _inputRegion;
}

// Sensor output accessor
Sensor * Region::output(){
    return _output;
}
Region * Region::outputRegion(){
    return _outputRegion;
}


// Pointer to settings. We need this to access parameters in
// Synapse, Segment, Cell and Column
RegionSettings * Region::settings(){
    return &_settings;
}
}
