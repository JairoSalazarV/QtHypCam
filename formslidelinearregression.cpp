#include "formslidelinearregression.h"
#include "ui_formslidelinearregression.h"
#include <QFileDialog>
#include <__common.h>
#include <customline.h>

formSlideLinearRegression::formSlideLinearRegression(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formSlideLinearRegression)
{
    ui->setupUi(this);

    ui->tableLstPoints->setColumnWidth(0,150);
    ui->tableLstPoints->setColumnWidth(1,105);
    ui->tableLstPoints->setColumnWidth(2,105);
    ui->tableLstPoints->setColumnWidth(3,115);
    ui->tableLstPoints->setColumnWidth(4,115);

    QString fileToOpen;

    if(1)//VERTICAL
    {
        QString path = "/home/jairo/Documentos/DOTECH/Calibraciones/slideBeta_001/attempt3/XML/lineas/";
        fileToOpen = path + "408.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = path + "438.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = path + "490.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = path + "548.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = path + "585.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = path + "615.xml";
        funcAddRowToTable(&fileToOpen);
        //fileToOpen = path + "710.xml";
        //funcAddRowToTable(&fileToOpen);
    }
    if(0)
    {
        //fileToOpen = "./XML/lines/slideV1_002/verticalLowerBound.xml";
        //funcAddRowToTable(&fileToOpen);        
        fileToOpen = "./XML/lines/slideV1_002/horizontalUpper.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = "./XML/lines/slideV1_002/horizontalLower.xml";
        funcAddRowToTable(&fileToOpen);
    }

}

formSlideLinearRegression::~formSlideLinearRegression()
{
    delete ui;
}

void formSlideLinearRegression::on_pbSelectFile_clicked()
{
    //---------------------------------------
    //Let the user select the file
    //---------------------------------------
    QString fileOrigin;
    if( funcLetUserSelectFile(&fileOrigin,"Select a line...",this) != _OK )
    {
        return (void)false;
    }

    //---------------------------------------
    //Add new line to the table of lines
    //---------------------------------------
    if( funcAddRowToTable(&fileOrigin) == true )
    {
        return (void)false;
    }
}

bool formSlideLinearRegression::funcAddRowToTable(QString* filePath)
{
    //Get Line
    structLine tmpNewLine;
    if( funcReadLineFromXML(filePath,&tmpNewLine) == _OK )
    {
        //Add Row to Table
        ui->tableLstPoints->insertRow( ui->tableLstPoints->rowCount() );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        0,
                                        new QTableWidgetItem(QString::number(tmpNewLine.wavelength))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        1,
                                        new QTableWidgetItem(QString::number(tmpNewLine.x1)+", "+QString::number(tmpNewLine.y1))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        2,
                                        new QTableWidgetItem(QString::number(tmpNewLine.x2)+", "+QString::number(tmpNewLine.y2))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        3,
                                        new QTableWidgetItem(QString::number(tmpNewLine.originalW)+", "+QString::number(tmpNewLine.originalH))
                                  );
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,3)->setTextAlignment(Qt::AlignCenter);
        return _OK;
    }
    return _ERROR;
}

void formSlideLinearRegression::on_pbGenRegression_clicked()
{
    //--------------------------------------
    //This function Assumes that the lower
    //wavelength is located at the left
    //and is vertical
    //--------------------------------------
    int i, numLines;
    //--------------------------------------
    //Save Lines Into Memory
    //--------------------------------------
    numLines = ui->tableLstPoints->rowCount();
    QList<structLine> lstLines;
    funcTableToList(&lstLines);    

    //--------------------------------------
    //Obtain Wavelength LR
    //--------------------------------------
    double lstX[numLines-1];//Distance in pixels between lines
    double lstY[numLines-1];//Wavelength
    float wavePixLen = 0.0;
    for( i=0; i<numLines-1; i++ )
    {
        wavePixLen  = (
                        (lstLines.at(i+1).x2 - lstLines.at(i).x2) +
                        (lstLines.at(i+1).x1 - lstLines.at(i).x1)
                      )/2.0;
        lstX[i]     = wavePixLen;
        lstY[i]     = (float)(lstLines.at(i+1).wavelength -
                              lstLines.at(i).wavelength);
        //std::cout << "i: " << i << " x: " << lstX[i]
        //                        << " y: " << lstY[i] << std::endl;
    }
    linearRegresion wave2DistLR, dist2WaveLR;
    dist2WaveLR = funcLinearRegression(lstX,lstY,numLines-1);
    wave2DistLR = funcLinearRegression(lstY,lstX,numLines-1);
    //std::cout << "a: " << wave2DistLR.a << " b: " << wave2DistLR.b << std::endl;
    //std::cout << "dista: " << dist2WaveLR.a << " distb: " << dist2WaveLR.b << std::endl;

    //Read Min Wavelength
    float smallestWaveLen   = readAllFile(_PATH_SLIDE_MIN_WAVELENGTH).trimmed().toFloat(0);
    smallestWaveLen         = ( smallestWaveLen < _RASP_CAM_MIN_WAVELENGTH )?_RASP_CAM_MIN_WAVELENGTH:smallestWaveLen;
    smallestWaveLen         = lstLines.at(0).wavelength - smallestWaveLen;

    //Obtain the Smallest Wavelength
    float tmpDist;
    int firstLineX1, firstLineX2;
    tmpDist     = funcApplyLR( smallestWaveLen, wave2DistLR, false );    
    firstLineX1 = round((float)lstLines.at(0).x1 - tmpDist);
    //std::cout << " firstLineX1: "       << firstLineX1       << std::endl;
    //std::cout << "smallestWaveLen: "    << smallestWaveLen   << std::endl;

    //--------------------------------------
    //Obtain Higher(Wavelength)
    //--------------------------------------
    structVerticalCalibrationSettings vertCalibSettings;
    if( fileExists(_PATH_SLIDE_MAX_WAVELENGTH) )
    {
        vertCalibSettings.maxWavelength = readAllFile( _PATH_SLIDE_MAX_WAVELENGTH ).trimmed().toFloat(0);
    }
    else
    {
        vertCalibSettings.maxWavelength = _RASP_CAM_MAX_WAVELENGTH;
    }

    //--------------------------------------
    //Get Average Slope and,
    //Generate global Slope
    //--------------------------------------
    //Get Average Slope
    int tmpXDifference = 0;
    for( i=0; i<numLines; i++ )
    {
        tmpXDifference += lstLines.at(i).x2 - lstLines.at(i).x1;
    }
    tmpXDifference  = round( (float)tmpXDifference / (float)numLines );
    firstLineX2     = firstLineX1 + tmpXDifference;

    //std::cout << "Mod P1: " << lowerVerLine.x1 << ", " << lowerVerLine.y1 << std::endl;
    //std::cout << "Mod P2: " << lowerVerLine.x2 << ", " << lowerVerLine.y2 << std::endl;

    //--------------------------------------
    //Calculate Vertical LR
    //--------------------------------------
    double X[2];
    double Y[2];
    X[0]    = (double)lstLines.at(0).y1;
    Y[0]    = (double)firstLineX1;
    X[1]    = (double)lstLines.at(0).y2;
    Y[1]    = (double)firstLineX2;
    //std::cout << "Mod P1: " << X[0] << ", " << Y[0] << std::endl;
    //std::cout << "Mod P2: " << X[1] << ", " << Y[1] << std::endl;
    linearRegresion vertLR = funcLinearRegression(X,Y,2);


    //--------------------------------------
    //Save Vertical Calibration File
    //--------------------------------------
    //Obtain Lower(Wavelength) Vertical Line
    structLine lowerVerLine;
    lowerVerLine.originalW  = lstLines.at(0).originalW;
    lowerVerLine.originalH  = lstLines.at(0).originalH;
    lowerVerLine.x1         = firstLineX1;
    lowerVerLine.y1         = lstLines.at(0).y1;
    lowerVerLine.x2         = firstLineX2;
    lowerVerLine.y2         = lstLines.at(0).y2;
    lowerVerLine.wavelength = lstLines.at(0).wavelength - smallestWaveLen;

    //Save file
    if( funcSaveVerticalCalibrationFile(
                                            &lowerVerLine,
                                            &dist2WaveLR,
                                            &wave2DistLR,
                                            &vertLR,
                                            &vertCalibSettings
                                        ) != _OK
    ){
        funcShowMsgERROR_Timeout("Saving Half Calibration File");
        return (void)false;
    }
}

void formSlideLinearRegression::funcTableToList(QList<structLine>* lstLines)
{
    //--------------------------------------
    //Save Lines Into Memory
    //--------------------------------------
    int i, numLines;
    numLines = ui->tableLstPoints->rowCount();
    QString tmpItem;
    QList<QString> coordinates;
    for( i=0; i<numLines; i++ )
    {
        //Prepare new line
        structLine tmpNewLine;
        //Get P1
        tmpItem                 = ui->tableLstPoints->item(i,1)->text().trimmed();
        coordinates             = tmpItem.split(",");
        tmpNewLine.x1           = coordinates.at(0).trimmed().toInt(0);
        tmpNewLine.y1           = coordinates.at(1).trimmed().toInt(0);
        //Get P2
        tmpItem                 = ui->tableLstPoints->item(i,2)->text().trimmed();
        coordinates             = tmpItem.split(",");
        tmpNewLine.x2           = coordinates.at(0).trimmed().toInt(0);
        tmpNewLine.y2           = coordinates.at(1).trimmed().toInt(0);
        //Get Image Size
        tmpItem                 = ui->tableLstPoints->item(i,3)->text().trimmed();
        coordinates             = tmpItem.split(",");
        tmpNewLine.originalW    = coordinates.at(0).trimmed().toInt(0);
        tmpNewLine.originalH    = coordinates.at(1).trimmed().toInt(0);
        //Get wavelength
        tmpNewLine.wavelength  = ui->tableLstPoints->item(i,0)->text().trimmed().toFloat(0);
        //Add Line
        lstLines->append(tmpNewLine);
    }
}

int formSlideLinearRegression
    ::funcUpdateVerticalCalibrationFile(
        structVerticalCalibration* vertCalibSettings
){

    //------------------------------------
    //Get Output Filename from User
    //------------------------------------
    QString filenamePath;
    if(
            funcLetUserDefineFile(
                                    &filenamePath,
                                    "Select Half-Calibration File",
                                    ".xml",
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    new QString("./XML/lines/"),
                                    this
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Defining Filename from User");
        return _ERROR;
    }

    //------------------------------------
    //Prepare .XML contain
    //------------------------------------
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures << "imgW"           << "imgH"
                << "lowerBoundX1"   << "lowerBoundY1"
                << "lowerBoundX2"   << "lowerBoundY2"
                << "lowerBoundWave" << "higherBoundWave"
                << "dist2WaveA"     << "dist2WaveB"
                << "wave2DistA"     << "wave2DistB"
                << "vertA"          << "vertB";
    lstValues   << QString::number(vertCalibSettings->imgW);
    lstValues   << QString::number(vertCalibSettings->imgH);
    lstValues   << QString::number(vertCalibSettings->x1);
    lstValues   << QString::number(vertCalibSettings->y1);
    lstValues   << QString::number(vertCalibSettings->x2);
    lstValues   << QString::number(vertCalibSettings->y2);
    lstValues   << QString::number(vertCalibSettings->minWave);
    lstValues   << QString::number(vertCalibSettings->maxWave);
    lstValues   << QString::number(vertCalibSettings->dist2WaveLR.a);
    lstValues   << QString::number(vertCalibSettings->dist2WaveLR.b);
    lstValues   << QString::number(vertCalibSettings->wave2DistLR.a);
    lstValues   << QString::number(vertCalibSettings->wave2DistLR.b);
    lstValues   << QString::number(vertCalibSettings->vertLR.a);
    lstValues   << QString::number(vertCalibSettings->vertLR.b);

    //------------------------------------
    //Save .XML file
    //------------------------------------
    if( funcSaveXML(filenamePath,&lstFixtures,&lstValues) != _OK )
    {
        return _ERROR;
    }
    return _OK;
}

int formSlideLinearRegression
    ::funcSaveVerticalCalibrationFile(
                                        structLine* lowerVerLine,
                                        linearRegresion* dist2WaveLR,
                                        linearRegresion* wave2DistLR,
                                        linearRegresion* vertLR,
                                        structVerticalCalibrationSettings* vertCalibSettings
){
    //------------------------------------
    //Get Output Filename from User
    //------------------------------------
    QString filenamePath;
    if(
            funcLetUserDefineFile(
                                    &filenamePath,
                                    "Select Half-Calibration File",
                                    ".xml",
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    new QString("./XML/lines/"),
                                    this
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Defining Filename from User");
        return _ERROR;
    }

    //------------------------------------
    //Prepare .XML contain
    //------------------------------------
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures << "imgW"           << "imgH"                
                << "lowerBoundX1"   << "lowerBoundY1"
                << "lowerBoundX2"   << "lowerBoundY2"
                << "lowerBoundWave" << "higherBoundWave"
                << "dist2WaveA"     << "dist2WaveB"
                << "wave2DistA"     << "wave2DistB"
                << "vertA"          << "vertB";    
    lstValues   << QString::number(lowerVerLine->originalW);
    lstValues   << QString::number(lowerVerLine->originalH);    
    lstValues   << QString::number(lowerVerLine->x1);
    lstValues   << QString::number(lowerVerLine->y1);
    lstValues   << QString::number(lowerVerLine->x2);
    lstValues   << QString::number(lowerVerLine->y2);
    lstValues   << QString::number(lowerVerLine->wavelength);
    lstValues   << QString::number(vertCalibSettings->maxWavelength);
    lstValues   << QString::number(dist2WaveLR->a);
    lstValues   << QString::number(dist2WaveLR->b);
    lstValues   << QString::number(wave2DistLR->a);
    lstValues   << QString::number(wave2DistLR->b);
    lstValues   << QString::number(vertLR->a);
    lstValues   << QString::number(vertLR->b);

    //------------------------------------
    //Save .XML file
    //------------------------------------
    if( funcSaveXML(filenamePath,&lstFixtures,&lstValues) != _OK )
    {
        return _ERROR;
    }
    return _OK;
}

void formSlideLinearRegression::on_pbRemoveItem_clicked()
{
    ui->tableLstPoints->removeRow( ui->tableLstPoints->currentRow() );
}

void formSlideLinearRegression::on_pbGenHorRegression_clicked()
{
    int numLines;
    //--------------------------------------
    //Save Lines Into Memory
    //--------------------------------------
    numLines = ui->tableLstPoints->rowCount();
    QList<structLine> lstLines;
    funcTableToList(&lstLines);
    if( numLines < 2 )
    {
        funcShowMsgERROR_Timeout("It requires 2 Lines");
        return (void)false;
    }

    //--------------------------------------
    //Calculate Average Slope
    //--------------------------------------
    double X[2];
    double Y[2];
    X[0]    = (double)lstLines.at(1).x1;
    Y[0]    = (double)lstLines.at(1).y1;
    X[1]    = (double)lstLines.at(1).x2;
    Y[1]    = (double)lstLines.at(1).y2;
    linearRegresion horizLR = funcLinearRegression(X,Y,2);

    //--------------------------------------
    //Save Horizontal Calibration
    //--------------------------------------
    //Get Filename
    QString filenamePath;
    if(
            funcLetUserDefineFile(
                                    &filenamePath,
                                    "Select Half-Calibration File...",
                                    ".xml",
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    new QString("./XML/lines/"),
                                    this
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Defining Filename from User");
        return (void)false;
    }
    //main Weight
    int bigH = (lstLines.at(0).y2-lstLines.at(1).y2);
    int imgW = lstLines.at(1).originalW;
    int imgH = lstLines.at(1).originalH;
    //Save Line
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures << "imgW" << "imgH" << "H" << "a" << "b";
    lstValues   << QString::number(imgW)
                << QString::number(imgH)
                << QString::number(bigH)
                << QString::number(horizLR.a)
                << QString::number(horizLR.b);
    if( funcSaveXML(filenamePath,&lstFixtures,&lstValues) != _OK )
    {
        funcShowMsgERROR_Timeout("Saving Calibration");
        return (void)false;
    }
}

void formSlideLinearRegression::on_pbGenAffinTrans_clicked()
{
    int numLines;
    //--------------------------------------
    //Save Lines Into Memory
    //--------------------------------------
    numLines = ui->tableLstPoints->rowCount();
    QList<structLine> lstLines;
    funcTableToList(&lstLines);
    if( numLines < 2 )
    {
        funcShowMsgERROR_Timeout("It requires 2 Lines");
        return (void)false;
    }

    //--------------------------------------
    //Get Hor Lower and later Upper Bound
    //--------------------------------------
    QTransform tmpTrans;
    if( funcLines2Translation(
                                &tmpTrans,
                                lstLines.at(0),
                                lstLines.at(1)
                             ) != _OK
    ){
        funcShowMsgERROR_Timeout("Reading Slide Calibration");
        return (void)false;
    }

    //--------------------------------------
    //Save Horizontal Calibration
    //--------------------------------------
    //Get Filename
    QString filenamePath;
    if(
            funcLetUserDefineFile(
                                    &filenamePath,
                                    "Define Affine Transformation Output File...",
                                    ".xml",
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    new QString("./XML/lines/"),
                                    this
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Defining Filename from User");
        return (void)false;
    }
    //Save Line
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures << "Tm11" << "Tm12" << "Tm13"
                << "Tm21" << "Tm22" << "Tm23"
                << "Tm31" << "Tm32" << "Tm33";
    lstValues   << QString::number(tmpTrans.m11())
                << QString::number(tmpTrans.m12())
                << QString::number(tmpTrans.m13())
                << QString::number(tmpTrans.m21())
                << QString::number(tmpTrans.m22())
                << QString::number(tmpTrans.m23())
                << QString::number(tmpTrans.m31())
                << QString::number(tmpTrans.m32())
                << QString::number(tmpTrans.m33());
    if( funcSaveXML(filenamePath,&lstFixtures,&lstValues) != _OK )
    {
        funcShowMsgERROR_Timeout("Saving Affine Transformation");
        return (void)false;
    }
}

/*
void formSlideLinearRegression::on_pbUpdateVerticalLR_clicked()
{

    //---------------------------------------
    //Get Vertical Calibration
    //---------------------------------------
    //Get filename
    QString fileName;
    if( funcLetUserSelectFile(&fileName,"Select Vertical Calibration",this) != _OK )
    {
        funcShowMsgERROR_Timeout("Getting Vertical Calibration");
        return (void)false;
    }
    //Get Calibration Parameters from .XML
    structVerticalCalibration tmpVertCal;
    funcReadVerticalCalibration(&fileName,&tmpVertCal);

    //--------------------------------------
    //This function Assumes that the lower
    //wavelength is located at the left
    //and is vertical
    //--------------------------------------
    int i, numLines;
    float newMinWave, newMaxWave;

    //--------------------------------------
    //Save Lines Into Memory
    //--------------------------------------
    numLines = ui->tableLstPoints->rowCount();
    QList<structLine> lstLines;
    funcTableToList(&lstLines);

    //--------------------------------------
    //Obtain NEW Wavelength LR
    //--------------------------------------
    double lstX[numLines-1];//Distance in pixels between lines
    double lstY[numLines-1];//Wavelength
    float wavePixLen = 0.0;
    for( i=0; i<numLines-1; i++ )
    {
        wavePixLen  = (
                        (lstLines.at(i+1).x2 - lstLines.at(i).x2) +
                        (lstLines.at(i+1).x1 - lstLines.at(i).x1)
                      )/2.0;
        lstX[i]     = wavePixLen;
        lstY[i]     = (float)(lstLines.at(i+1).wavelength -
                              lstLines.at(i).wavelength);
    }
    linearRegresion newWave2DistLR, newDist2WaveLR;
    newDist2WaveLR = funcLinearRegression(lstX,lstY,numLines-1);
    newWave2DistLR = funcLinearRegression(lstY,lstX,numLines-1);

    //--------------------------------------
    //Calc New Bounded Wavelengths
    //--------------------------------------
    int tmpPixDist;
    float tmpWaveDist;

    //Calc Distance to Lower Wave
    tmpPixDist  = round(
                            ((float)lstLines.at(0).x1 / (float)lstLines.at(0).canvasW) *
                            lstLines.at(0).originalW
                       );
    tmpWaveDist = funcApplyLR( tmpPixDist, newDist2WaveLR, true );
    newMinWave  = lstLines.at(0).wavelength - tmpWaveDist;

    //Calc Distance to Higher Wave
    tmpPixDist  = lstLines.at(0).originalW;
    tmpWaveDist = funcApplyLR( tmpPixDist, newDist2WaveLR, true );
    newMaxWave  = newMinWave + tmpWaveDist;

    //--------------------------------------
    //Update Vertical Calibration
    //--------------------------------------
    tmpVertCal.minWave          = newMinWave;
    tmpVertCal.maxWave          = newMaxWave;
    tmpVertCal.dist2WaveLR.a    = newDist2WaveLR.a;
    tmpVertCal.dist2WaveLR.b    = newDist2WaveLR.b;
    tmpVertCal.wave2DistLR.a    = newWave2DistLR.a;
    tmpVertCal.wave2DistLR.b    = newWave2DistLR.b;

    //--------------------------------------
    //Save Vertical Calibration File
    //--------------------------------------
    //Save file
    if( funcUpdateVerticalCalibrationFile( &tmpVertCal ) != _OK )
    {
        funcShowMsgERROR_Timeout("Updating Vertical Calibration");
        return (void)false;
    }
    else
    {
        funcShowMsgSUCCESS_Timeout("Vertical Calibration Updated");
    }
}*/
