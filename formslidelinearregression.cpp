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

    if(0)
    {
        fileToOpen = "./XML/lines/slideV1_002/408nm.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = "./XML/lines/slideV1_002/438nm.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = "./XML/lines/slideV1_002/492nm.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = "./XML/lines/slideV1_002/550nm.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = "./XML/lines/slideV1_002/586nm.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = "./XML/lines/slideV1_002/620nm.xml";
        funcAddRowToTable(&fileToOpen);
        fileToOpen = "./XML/lines/slideV1_002/712nm.xml";
        funcAddRowToTable(&fileToOpen);
    }
    if(1)
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
    if( funcLetUserSelectFile(&fileOrigin) != _OK )
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
    }
    linearRegresion wave2DistLR, dist2WaveLR;
    dist2WaveLR = funcLinearRegression(lstX,lstY,numLines-1);
    wave2DistLR = funcLinearRegression(lstY,lstX,numLines-1);

    //--------------------------------------
    //Obtain Lower(Wavelength) Vertical Line
    //--------------------------------------
    structLine lowerVerLine;
    QString lowerBoundLinePath;
    //get filepath
    //if( funcLetUserSelectFile(&lowerBoundLinePath) != _OK )
    if(
            funcLetUserSelectFile(
                                    &lowerBoundLinePath,
                                    QString("Select the Lower Bound Line"),
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    QString("./XML/lines/")
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Reading File-path");
        return (void)false;
    }
    //get lower bound line
    if( funcReadLineFromXML(&lowerBoundLinePath,&lowerVerLine) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Line from XML");
        return (void)false;
    }
    //Obtain the Smallest Wavelength
    float dist2FirstLine, smallestWaveLen;
    dist2FirstLine  = (float)(lstLines.at(0).x1 - lowerVerLine.x1);
    smallestWaveLen = funcApplyLR( dist2FirstLine, dist2WaveLR, true );
    lowerVerLine.wavelength = lstLines.at(0).wavelength - smallestWaveLen;
    //std::cout << " smallestWaveLen: "           << smallestWaveLen       << std::endl;
    //std::cout << "dist2FirstLine: "             << dist2FirstLine           << std::endl;
    //std::cout << "lowerVerLine.wavelength: "    << lowerVerLine.wavelength  << std::endl;

    //std::cout << "Orig P1: " << lowerVerLine.x1 << ", " << lowerVerLine.y1 << std::endl;
    //std::cout << "Orig P2: " << lowerVerLine.x2 << ", " << lowerVerLine.y2 << std::endl;

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
    lowerVerLine.x1 = lowerVerLine.x2 - tmpXDifference;

    //std::cout << "Mod P1: " << lowerVerLine.x1 << ", " << lowerVerLine.y1 << std::endl;
    //std::cout << "Mod P2: " << lowerVerLine.x2 << ", " << lowerVerLine.y2 << std::endl;

    //--------------------------------------
    //Calculate Vertical LR
    //--------------------------------------
    double X[2];
    double Y[2];
    X[0]    = (double)lowerVerLine.y1;
    Y[0]    = (double)lowerVerLine.x1;
    X[1]    = (double)lowerVerLine.y2;
    Y[1]    = (double)lowerVerLine.x2;
    //std::cout << "Mod P1: " << X[0] << ", " << Y[0] << std::endl;
    //std::cout << "Mod P2: " << X[1] << ", " << Y[1] << std::endl;
    linearRegresion vertLR = funcLinearRegression(X,Y,2);


    //--------------------------------------
    //Save Vertical Calibration File
    //--------------------------------------
    if( funcSaveVerticalCalibrationFile(
                                            &lowerVerLine,
                                            &dist2WaveLR,
                                            &wave2DistLR,
                                            &vertLR
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
    ::funcSaveVerticalCalibrationFile(
                                        structLine* lowerVerLine,
                                        linearRegresion* dist2WaveLR,
                                        linearRegresion* wave2DistLR,
                                        linearRegresion* vertLR
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
                                    "./XML/lines/"
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
                << "lowerBoundWave"
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
    lstValues   << QString::number(dist2WaveLR->a);
    lstValues   << QString::number(dist2WaveLR->b);
    lstValues   << QString::number(wave2DistLR->a);
    lstValues   << QString::number(wave2DistLR->b);
    lstValues   << QString::number(vertLR->a);
    lstValues   << QString::number(vertLR->b);

    //------------------------------------
    //Save .XML file
    //------------------------------------
    if( funcSaveXML(&filenamePath,&lstFixtures,&lstValues) != _OK )
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
    X[0]    = (double)lstLines.at(0).x1;
    Y[0]    = (double)lstLines.at(0).y1;
    X[1]    = (double)lstLines.at(0).x2;
    Y[1]    = (double)lstLines.at(0).y2;
    linearRegresion horizLR = funcLinearRegression(X,Y,2);

    //--------------------------------------
    //Save Horizontal Calibration
    //--------------------------------------
    //Get Filename
    QString filenamePath;
    if(
            funcLetUserDefineFile(
                                    &filenamePath,
                                    "Select Half-Calibration File",
                                    ".xml",
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    "./XML/lines/"
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Defining Filename from User");
        return (void)false;
    }
    //main Weight
    int bigH = (lstLines.at(1).y2-lstLines.at(0).y2);
    int imgW = lstLines.at(0).originalW;
    int imgH = lstLines.at(0).originalH;
    //Save Line
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures << "imgW" << "imgH" << "H" << "a" << "b";
    lstValues   << QString::number(imgW)
                << QString::number(imgH)
                << QString::number(bigH)
                << QString::number(horizLR.a)
                << QString::number(horizLR.b);
    if( funcSaveXML(&filenamePath,&lstFixtures,&lstValues) != _OK )
    {
        funcShowMsgERROR_Timeout("Saving Calibration");
        return (void)false;
    }



    /*
    //--------------------------------------
    //Calculate Average Slope
    //--------------------------------------
    int difference;
    difference = round(
                        (float)(
                                    (lstLines.at(0).y2 - lstLines.at(0).y1) +
                                    (lstLines.at(1).y2 - lstLines.at(1).y1)
                                ) / 2.0
                      );
    float m;
    m = (float)(lstLines.at(0).y2-lstLines.at(0).y1) /
        (float)(lstLines.at(0).x2-lstLines.at(0).x1);
    structLine tmpLine;
    //Backup Line
    tmpLine.canvasW     = lstLines.at(0).canvasW;
    tmpLine.canvasH     = lstLines.at(0).canvasH;
    tmpLine.x1          = lstLines.at(0).x1;
    tmpLine.y1          = lstLines.at(0).y1;
    tmpLine.x2          = lstLines.at(0).x2;
    tmpLine.y2          = lstLines.at(0).y1 + difference;
    tmpLine.m           = m;
    tmpLine.wavelength  = 0;
    tmpLine.oritation   = _HORIZONTAL;
    tmpLine.colorR      = 255;
    tmpLine.colorG      = 255;
    tmpLine.colorB      = 255;

    //--------------------------------------
    //Save Horizontal Calibration
    //--------------------------------------
    //Get Filename
    QString filenamePath;
    if(
            funcLetUserDefineFile(
                                    &filenamePath,
                                    "Select Half-Calibration File",
                                    ".xml",
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    "./XML/lines/"
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Defining Filename from User");
        return (void)false;
    }
    //Save Line
    funcExportLineToXML(&tmpLine,filenamePath);
    */


}
