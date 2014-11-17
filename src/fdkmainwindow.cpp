#include "fdkmainwindow.h"
#include "ui_fdkmainwindow.h"
#include <qdebug.h>
FDKMainWindow::FDKMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FDKMainWindow)
{
    ui->setupUi(this);
    // Setup interface
    connect(ui->stepButton, SIGNAL(clicked()),this,SLOT(step()));

    // Setup brain
    FDK::RegionSettings regionSettings;
    regionSettings.sensorSettings.width = 45;
    regionSettings.sensorSettings.height = 45;
    regionSettings.depth = 32;
    regionSettings.learningRate = 0.01;
    regionSettings.sparsity = 0.02;
;

    string stringOfCharacters = "qwertyuiopasdfghjklzxcvbnm. ";
    vector<char> charactersToLearn;
    for(char ch : stringOfCharacters){
        charactersToLearn.push_back(ch);
    }


    FDK::EncoderSettings<char> encoderSettings;
    encoderSettings.regionSettings = regionSettings;
    encoderSettings.possibleSignals = charactersToLearn;

    brain = FrankDevelopmentKit::createBrain();
    region = FrankDevelopmentKit::createRegion(regionSettings);
    encoder = FrankDevelopmentKit::createSparseCharEncoder(encoderSettings);
    connection = FrankDevelopmentKit::createSensorConnection(encoder->output(),region->input());

    brain->addSensorConnection(connection);
    brain->addEncoder(encoder);
    brain->addRegion(region);

    // Feed encoder some information
    for(int x = 0; x < 10000; x++){
        //encoder->appendToBuffer( string("the quick brown fox jumps over the lazy dog. ") );
        encoder->appendToBuffer( string("the quick and brown fox jumped. the fox jumped so high that the lazy dog was quick. the quick dog has jumped too.") );
    }


    ui->regionOutputWidget->setSensor( encoder->output() );
    ui->debugSensor->setSensor( region->_debug);

}

FDKMainWindow::~FDKMainWindow()
{
    delete ui;
}

void FDKMainWindow::step(){

    for(FDK::UInt i = 0; i < 200; i++){
        brain->step();
        //ui->synapseDynamicsGraph->addValue( region->regionData().synapseChanges );
        QString sc = " - ";
        if(encoder->nextCharacter() != encoder->decode( region->output() )){
            sc = "***";
        }
        qDebug()
        << "Next->" << encoder->nextCharacter() << sc
        << encoder->decode( region->output() ) << " <- Pred "
        << " AC: "      <<   QString("%1").arg(region->regionData().activatedColumns, 5, 10, QChar(' '))
        << " PCT: "     <<   QString("%1").arg(region->regionData().predictedColumnsTotal, 5, 10, QChar(' '))
        << " PCTAI: "   <<   QString("%1").arg(region->regionData().predictedColumnsAfterInhibition, 5, 10, QChar(' '))
        << " EC-P: "    <<   QString("%1").arg(region->regionData().excitedCellsDueToPrediction, 5, 10, QChar(' '))
        << " EC-A: "    <<   QString("%1").arg(region->regionData().excitedCellsDueActivation, 5, 10, QChar(' '))
        << " EC-S: "    <<   QString("%1").arg(region->regionData().excitedCellsDueToLackOfSegments, 5, 10, QChar(' '))
        << " ES-P: "    <<   QString("%1").arg(region->regionData().excitedSynapsesForPrediction, 5, 10, QChar(' '))
        << " SgAdj+: "  <<   QString("%1").arg(region->regionData().positiveAdjustedSegments, 5, 10, QChar(' '))
        << " SgAdj-: "  <<   QString("%1").arg(region->regionData().negativeAdjustedSegments, 5, 10, QChar(' '))
        << " NwSg: "    <<   QString("%1").arg(region->regionData().newSegments, 5, 10, QChar(' '))
        << " NwSn: "    <<   QString("%1").arg(region->regionData().newSynapses, 5, 10, QChar(' '))
        << " RT: "      <<   QString("%1").arg(region->regionData().executionTime, 5, 10, QChar(' '));
    }
    ui->regionOutputWidget->update();
    ui->debugSensor->update();
    ui->synapseDynamicsGraph->update();

}
