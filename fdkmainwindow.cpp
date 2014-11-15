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
    regionSettings.learningRate = 0.1;
    regionSettings.sparsity = 0.02;
    regionSettings.desiredSparsity = (FDK::fdkuint)( (FDK::fdkfloat)(45*45)*0.02f);

    QString stringOfCharacters = "qwertyuiopasdfghjklzxcvbnm. ";
    QList<char> charactersToLearn;
    for(QChar ch : stringOfCharacters){
        charactersToLearn.append(ch.toLatin1());
    }


    FDK::EncoderCharSettings encoderSettings;
    encoderSettings.regionSettings = regionSettings;
    encoderSettings.possibleSignals = charactersToLearn;

    brain = FrankDevelopmentKit::createBrain();
    region = FrankDevelopmentKit::createRegion(regionSettings);
    encoder = FrankDevelopmentKit::createSparseCharEncoder(encoderSettings);
    connection = FrankDevelopmentKit::createConnection(encoder->output(),region->input());

    brain->addConnection(connection);
    brain->addEncoder(encoder);
    brain->addRegion(region);

    // Feed encoder some information
    for(int x = 0; x < 10000; x++){
        //encoder->appendToBuffer( QString("the quick brown fox jumps over the lazy dog. the lazy dog fox jumps over the quick brown. ") );
        encoder->appendToBuffer( QString("abcdefgfedcb") );
    }


    ui->regionOutputWidget->setSensor( encoder->output() );
    ui->debugSensor->setSensor( region->_debug);

}

FDKMainWindow::~FDKMainWindow()
{
    delete ui;
}

void FDKMainWindow::step(){
    //for(fdkuint i = 0; i < 200; i++){
        brain->step();
        //ui->synapseDynamicsGraph->addValue( region->regionData().synapseChanges );
        qDebug()
        << "Next->" << encoder->nextCharacter() << " - "
        << encoder->decode( region->output() ) << " <- Pred "
        << " AC: " << region->regionData().activatedColumns
        << " PCT: " << region->regionData().predictedColumnsTotal
        << " PCTAI: " << region->regionData().predictedColumnsAfterInhibition
        << " EC-P: " << region->regionData().excitedCellsDueToPrediction
        << " EC-A: " << region->regionData().excitedCellsDueActivation
        << " EC-S: " << region->regionData().excitedCellsDueToLackOfSegments
        << " ES-P: " << region->regionData().excitedSynapsesForPrediction
        << " SgAdj+: " << region->regionData().positiveAdjustedSegments
        << " SgAdj-: " << region->regionData().negativeAdjustedSegments
        << " NwSg: " << region->regionData().newSegments
        << " NwSn: " << region->regionData().newSynapses;
    //}
    ui->regionOutputWidget->update();
    ui->debugSensor->update();
    ui->synapseDynamicsGraph->update();
}
