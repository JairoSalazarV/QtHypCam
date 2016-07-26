#include "gencalibxml.h"
#include "ui_gencalibxml.h"
#include <__common.h>
#include <QFileDialog>

#include <lstStructs.h>
#include <QDesktopServices>
#include <customQMatrix4x3.h>
#include <customQMatrix3x3.h>

#include <QMatrix4x4>
#include <QVector4D>
#include <QMatrix4x2>

bool isExportable = false;





genCalibXML::genCalibXML(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::genCalibXML)
{
    ui->setupUi(this);    
    autoLoadCentroids();


    disableButtons();


}

genCalibXML::~genCalibXML()
{
    delete ui;
}



void genCalibXML::disableButtons(){
    ui->pbRedLeftUp->setVisible(false);
    ui->pbGreenLeftUp->setVisible(false);
    ui->pbBlueLeftUp->setVisible(false);

    ui->pbRedLeftDown->setVisible(false);
    ui->pbGreenLeftDown->setVisible(false);
    ui->pbBlueLeftDown->setVisible(false);

    ui->pbRedRightUp->setVisible(false);
    ui->pbGreenRightUp->setVisible(false);
    ui->pbBlueRightUp->setVisible(false);

    ui->pbRedRightDown->setVisible(false);
    ui->pbGreenRightDown->setVisible(false);
    ui->pbBlueRightDown->setVisible(false);
}

void genCalibXML::funcGetFilePath(QPushButton *tmpPb){
    QString auxQstring;
    auxQstring = QFileDialog::getOpenFileName(
                                                    this,
                                                    tr("Select setting..."),
                                                    "./settings/Calib/",
                                                    "(*.hypcam);;"
                                             );
    if( !auxQstring.trimmed().isEmpty() ){
        setButton(tmpPb,auxQstring);
    }
}





lstCalibFileNames genCalibXML::funcFillCalibStruct(){

    lstCalibFileNames calibFileNames;

    calibFileNames.source           = "./settings/Calib/source.hypcam";

    calibFileNames.blueRightDown    = "./settings/Calib/bRD.hypcam";
    calibFileNames.blueRight        = "./settings/Calib/bR.hypcam";
    calibFileNames.blueRightUp      = "./settings/Calib/bRU.hypcam";
    calibFileNames.blueUp           = "./settings/Calib/bU.hypcam";
    calibFileNames.blueLeftUp       = "./settings/Calib/bLU.hypcam";
    calibFileNames.blueLeft         = "./settings/Calib/bL.hypcam";
    calibFileNames.blueLeftDown     = "./settings/Calib/bLD.hypcam";
    calibFileNames.blueDown         = "./settings/Calib/bD.hypcam";

    calibFileNames.greenRightDown   = "./settings/Calib/gRD.hypcam";
    calibFileNames.greenRight       = "./settings/Calib/gR.hypcam";
    calibFileNames.greenRightUp     = "./settings/Calib/gRU.hypcam";
    calibFileNames.greenUp          = "./settings/Calib/gU.hypcam";
    calibFileNames.greenLeftUp      = "./settings/Calib/gLU.hypcam";
    calibFileNames.greenLeft        = "./settings/Calib/gL.hypcam";
    calibFileNames.greenLeftDown    = "./settings/Calib/gLD.hypcam";
    calibFileNames.greenDown        = "./settings/Calib/gD.hypcam";

    calibFileNames.redRightDown     = "./settings/Calib/rRD.hypcam";
    calibFileNames.redRight         = "./settings/Calib/rR.hypcam";
    calibFileNames.redRightUp       = "./settings/Calib/rRU.hypcam";
    calibFileNames.redUp            = "./settings/Calib/rU.hypcam";
    calibFileNames.redLeftUp        = "./settings/Calib/rLU.hypcam";
    calibFileNames.redLeft          = "./settings/Calib/rL.hypcam";
    calibFileNames.redLeftDown      = "./settings/Calib/rLD.hypcam";
    calibFileNames.redDown          = "./settings/Calib/rD.hypcam";

    return calibFileNames;

}


void genCalibXML::autoLoadCentroids(){

    lstCalibFileNames calibFileNames = funcFillCalibStruct();
    int i,acum;
    i=0;
    acum=0;
    //Source
    setButton(ui->pbSource,calibFileNames.source,false);
    //Blues
    setButton(ui->pbBlueRightDown,calibFileNames.blueRightDown,false);
    acum=setButton(ui->pbBlueRight,calibFileNames.blueRight,false)?++i:i;
    setButton(ui->pbBlueRightUp,calibFileNames.blueRightUp,false);
    acum=setButton(ui->pbBlueUp,calibFileNames.blueUp,false)?++i:i;
    setButton(ui->pbBlueLeftUp,calibFileNames.blueLeftUp,false);
    acum=setButton(ui->pbBlueLeft,calibFileNames.blueLeft,false)?++i:i;
    setButton(ui->pbBlueLeftDown,calibFileNames.blueLeftDown,false);
    acum=setButton(ui->pbBlueDown,calibFileNames.blueDown,false)?++i:i;
    //Greens
    setButton(ui->pbGreenRightDown,calibFileNames.greenRightDown,false);
    acum=setButton(ui->pbGreenRight,calibFileNames.greenRight,false)?++i:i;
    setButton(ui->pbGreenRightUp,calibFileNames.greenRightUp,false);
    acum=setButton(ui->pbGreenUp,calibFileNames.greenUp,false)?++i:i;
    setButton(ui->pbGreenLeftUp,calibFileNames.greenLeftUp,false);
    acum=setButton(ui->pbGreenLeft,calibFileNames.greenLeft,false)?++i:i;
    setButton(ui->pbGreenLeftDown,calibFileNames.greenLeftDown,false);
    acum=setButton(ui->pbGreenDown,calibFileNames.greenDown,false)?++i:i;
    //Reds
    setButton(ui->pbRedRightDown,calibFileNames.redRightDown,false);
    acum=setButton(ui->pbRedRight,calibFileNames.redRight,false)?++i:i;
    setButton(ui->pbRedRightUp,calibFileNames.redRightUp,false);
    acum=setButton(ui->pbRedUp,calibFileNames.redUp,false)?++i:i;
    setButton(ui->pbRedLeftUp,calibFileNames.redLeftUp,false);
    acum=setButton(ui->pbRedLeft,calibFileNames.redLeft,false)?++i:i;
    setButton(ui->pbRedLeftDown,calibFileNames.redLeftDown,false);
    acum=setButton(ui->pbRedDown,calibFileNames.redDown,false)?++i:i;

    if(acum==12)
    {
        isExportable = true;
    }
    else
    {
        isExportable = false;
    }

}

bool genCalibXML::setButton(QPushButton *tmpPb, QString tip, bool doAlert){
    QFile tmpfile(tip);
    if( !tmpfile.exists() ){
        if( doAlert ){
            funcShowMsg("Alert","File does not exist: "+tip);
        }
        return false;
    }
    tmpPb->setToolTip(tip);
    tmpPb->setStyleSheet("width:27px;background-color:#000;color:#FFF;");
    return true;
}


void genCalibXML::on_pbSource_clicked()
{
    funcGetFilePath(ui->pbSource);
}

void genCalibXML::on_pbBlueRight_clicked()
{
    funcGetFilePath(ui->pbBlueRight);
}

void genCalibXML::on_pbGreenRight_clicked()
{
    funcGetFilePath(ui->pbGreenRight);
}

void genCalibXML::on_pbRedRight_clicked()
{
    funcGetFilePath(ui->pbRedRight);
}

void genCalibXML::on_pbBlueRightUp_clicked()
{
    funcGetFilePath(ui->pbBlueRightUp);
}

void genCalibXML::on_pbGreenRightUp_clicked()
{
    funcGetFilePath(ui->pbGreenRightUp);
}

void genCalibXML::on_pbRedRightUp_clicked()
{
    funcGetFilePath(ui->pbRedRightUp);
}

void genCalibXML::on_pbBlueUp_clicked()
{
    funcGetFilePath(ui->pbBlueUp);
}

void genCalibXML::on_pbGreenUp_clicked()
{
    funcGetFilePath(ui->pbGreenUp);
}

void genCalibXML::on_pbRedUp_clicked()
{
    funcGetFilePath(ui->pbRedUp);
}

void genCalibXML::on_pbBlueLeftUp_clicked()
{
    funcGetFilePath(ui->pbBlueLeftUp);
}

void genCalibXML::on_pbGreenLeftUp_clicked()
{
    funcGetFilePath(ui->pbGreenLeftUp);
}

void genCalibXML::on_pbRedLeftUp_clicked()
{
    funcGetFilePath(ui->pbRedLeftUp);
}

void genCalibXML::on_pbBlueLeft_clicked()
{
    funcGetFilePath(ui->pbBlueLeft);
}

void genCalibXML::on_pbGreenLeft_clicked()
{
    funcGetFilePath(ui->pbGreenLeft);
}

void genCalibXML::on_pbRedLeft_clicked()
{
    funcGetFilePath(ui->pbRedLeft);
}

void genCalibXML::on_pbBlueLeftDown_clicked()
{
    funcGetFilePath(ui->pbBlueLeftDown);
}

void genCalibXML::on_pbGreenLeftDown_clicked()
{
    funcGetFilePath(ui->pbGreenLeftDown);
}

void genCalibXML::on_pbRedLeftDown_clicked()
{
    funcGetFilePath(ui->pbRedLeftDown);
}

void genCalibXML::on_pbBlueDown_clicked()
{
    funcGetFilePath(ui->pbBlueDown);
}

void genCalibXML::on_pbRedDown_clicked()
{
    funcGetFilePath(ui->pbRedDown);
}

void genCalibXML::on_pbGreenDown_clicked()
{
    funcGetFilePath(ui->pbGreenDown);
}

void genCalibXML::on_pbBlueRightDown_clicked()
{
    funcGetFilePath(ui->pbBlueRightDown);
}

void genCalibXML::on_pbGreenRightDown_clicked()
{
    funcGetFilePath(ui->pbGreenRightDown);
}

void genCalibXML::on_pbRedRightDown_clicked()
{
    funcGetFilePath(ui->pbRedRightDown);
}

strAllLinReg genCalibXML::calcAllLinReg(lstCalibFileNames *centroids)
{
    strAllLinReg linRegRes;
    double pointsX[7];
    double pointsY[7];

    //Horizontal
    //..
    pointsX[0] = (double)centroids->redLeft.split(",").at(0).toInt(0);
    pointsX[1] = (double)centroids->greenLeft.split(",").at(0).toInt(0);
    pointsX[2] = (double)centroids->blueLeft.split(",").at(0).toInt(0);
    pointsX[3] = (double)centroids->source.split(",").at(0).toInt(0);
    pointsX[4] = (double)centroids->redRight.split(",").at(0).toInt(0);
    pointsX[5] = (double)centroids->greenRight.split(",").at(0).toInt(0);
    pointsX[6] = (double)centroids->blueRight.split(",").at(0).toInt(0);
    pointsY[0] = (double)centroids->redLeft.split(",").at(1).toInt(0);
    pointsY[1] = (double)centroids->greenLeft.split(",").at(1).toInt(0);
    pointsY[2] = (double)centroids->blueLeft.split(",").at(1).toInt(0);
    pointsY[3] = (double)centroids->source.split(",").at(1).toInt(0);
    pointsY[4] = (double)centroids->redRight.split(",").at(1).toInt(0);
    pointsY[5] = (double)centroids->greenRight.split(",").at(1).toInt(0);
    pointsY[6] = (double)centroids->blueRight.split(",").at(1).toInt(0);
    linearRegresion horB = funcLinearRegression( pointsX, pointsY, 7 );
    linRegRes.horizA = horB.a;
    linRegRes.horizB = horB.b;

    //Vertical
    //..
    pointsX[0] = (double)centroids->redDown.split(",").at(1).toInt(0);
    pointsX[1] = (double)centroids->greenDown.split(",").at(1).toInt(0);
    pointsX[2] = (double)centroids->blueDown.split(",").at(1).toInt(0);
    pointsX[3] = (double)centroids->source.split(",").at(1).toInt(0);
    pointsX[4] = (double)centroids->redUp.split(",").at(1).toInt(0);
    pointsX[5] = (double)centroids->greenUp.split(",").at(1).toInt(0);
    pointsX[6] = (double)centroids->blueUp.split(",").at(1).toInt(0);    
    pointsY[0] = (double)centroids->redDown.split(",").at(0).toInt(0);
    pointsY[1] = (double)centroids->greenDown.split(",").at(0).toInt(0);
    pointsY[2] = (double)centroids->blueDown.split(",").at(0).toInt(0);
    pointsY[3] = (double)centroids->source.split(",").at(0).toInt(0);
    pointsY[4] = (double)centroids->redUp.split(",").at(0).toInt(0);
    pointsY[5] = (double)centroids->greenUp.split(",").at(0).toInt(0);
    pointsY[6] = (double)centroids->blueUp.split(",").at(0).toInt(0);
    linearRegresion verB = funcLinearRegression( pointsX, pointsY, 7 );
    linRegRes.vertA = verB.a;
    linRegRes.vertB = verB.b;

    //Wavelength horizontal
    //..
    double auxL, auxR, aux;
    aux  = centroids->source.split(",").at(0).toFloat(0);
    //Delta blue X
    auxL = (double)abs(aux - centroids->blueLeft.split(",").at(0).toFloat(0));
    auxR = (double)abs(aux - centroids->blueRight.split(",").at(0).toFloat(0));
    pointsX[0] = (double)_BLUE_WAVELENGHT;
    pointsY[0] = (auxL+auxR) / 2.0;
    //Delta green X
    auxL = (double)abs(aux - centroids->greenLeft.split(",").at(0).toFloat(0));
    auxR = (double)abs(aux - centroids->greenRight.split(",").at(0).toFloat(0));
    pointsX[1] = (double)_GREEN_WAVELENGHT;
    pointsY[1] = (auxL+auxR) / 2.0;
    //Delta red X
    auxL = (double)abs(aux - centroids->redLeft.split(",").at(0).toFloat(0));
    auxR = (double)abs(aux - centroids->redRight.split(",").at(0).toFloat(0));
    pointsX[2] = (double)_RED_WAVELENGHT;
    pointsY[2] = (auxL+auxR) / 2.0;
    //Delta source X
    pointsX[3] = 0.0;
    pointsY[3] = 0.0;
    linearRegresion waveXB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.waveHorizA = waveXB.a;
    linRegRes.waveHorizB = waveXB.b;

    //Wavelength vertical
    //..
    double auxU, auxD;
    aux         = centroids->source.split(",").at(1).toFloat(0);
    //Delta blue X
    auxU        = (double)abs(aux - centroids->blueUp.split(",").at(1).toFloat(0));
    auxD        = (double)abs(aux - centroids->blueDown.split(",").at(1).toFloat(0));
    pointsX[0]  = (double)_BLUE_WAVELENGHT;
    pointsY[0]  = (auxU+auxD) / 2.0;
    //Delta green X
    auxU        = (double)abs(aux - centroids->greenUp.split(",").at(1).toFloat(0));
    auxD        = (double)abs(aux - centroids->greenDown.split(",").at(1).toFloat(0));
    pointsX[1]  = (double)_GREEN_WAVELENGHT;
    pointsY[1]  = (auxU+auxD) / 2.0;
    //Delta red X
    auxU        = (double)abs(aux - centroids->redUp.split(",").at(1).toFloat(0));
    auxD        = (double)abs(aux - centroids->redDown.split(",").at(1).toFloat(0));
    pointsX[2]  = (double)_RED_WAVELENGHT;
    pointsY[2]  = (auxU+auxD) / 2.0;
    //Delta source X
    pointsX[3]  = 0.0;
    pointsY[3]  = 0.0;
    linearRegresion waveYB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.waveVertA = waveYB.a;
    linRegRes.waveVertB = waveYB.b;

    //Delta horizontal
    //..
    aux  = centroids->source.split(",").at(0).toFloat(0);
    //Delta blue X
    auxL = (double)abs(aux - centroids->blueLeft.split(",").at(0).toFloat(0));
    auxR = (double)abs(aux - centroids->blueRight.split(",").at(0).toFloat(0));
    pointsY[0] = (double)_BLUE_WAVELENGHT;
    pointsX[0] = (auxL+auxR) / 2.0;
    //Delta green X
    auxL = (double)abs(aux - centroids->greenLeft.split(",").at(0).toFloat(0));
    auxR = (double)abs(aux - centroids->greenRight.split(",").at(0).toFloat(0));
    pointsY[1] = (double)_GREEN_WAVELENGHT;
    pointsX[1] = (auxL+auxR) / 2.0;
    //Delta red X
    auxL = (double)abs(aux - centroids->redLeft.split(",").at(0).toFloat(0));
    auxR = (double)abs(aux - centroids->redRight.split(",").at(0).toFloat(0));
    pointsY[2] = (double)_RED_WAVELENGHT;
    pointsX[2] = (auxL+auxR) / 2.0;
    //Delta source X
    pointsX[3] = 0.0;
    pointsY[3] = 0.0;
    linearRegresion deltaXB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.deltaHorizA = deltaXB.a;
    linRegRes.deltaHorizB = deltaXB.b;

    //Delta vertical
    //..
    aux         = centroids->source.split(",").at(1).toFloat(0);
    //Delta blue X
    auxU        = (double)abs(aux - centroids->blueUp.split(",").at(1).toFloat(0));
    auxD        = (double)abs(aux - centroids->blueDown.split(",").at(1).toFloat(0));
    pointsY[0]  = (double)_BLUE_WAVELENGHT;
    pointsX[0]  = (auxU+auxD) / 2.0;
    //Delta green X
    auxU        = (double)abs(aux - centroids->greenUp.split(",").at(1).toFloat(0));
    auxD        = (double)abs(aux - centroids->greenDown.split(",").at(1).toFloat(0));
    pointsY[1]  = (double)_GREEN_WAVELENGHT;
    pointsX[1]  = (auxU+auxD) / 2.0;
    //Delta red X
    auxU        = (double)abs(aux - centroids->redUp.split(",").at(1).toFloat(0));
    auxD        = (double)abs(aux - centroids->redDown.split(",").at(1).toFloat(0));
    pointsY[2]  = (double)_RED_WAVELENGHT;
    pointsX[2]  = (auxU+auxD) / 2.0;
    //Delta source X
    pointsY[3]  = 0.0;
    pointsX[3]  = 0.0;
    linearRegresion deltaYB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.deltaVertA = deltaYB.a;
    linRegRes.deltaVertB = deltaYB.b;

    return linRegRes;
}

strAllLinReg genCalibXML::getAllLR(){
    lstCalibFileNames calibPoints = fillLstCalibPoints();
    return calcAllLinReg(&calibPoints);
}

strLimits genCalibXML::getLimitsFromHDD(){
    QString aux;
    strLimits limits;

    aux = readAllFile(_PATH_LIMIT_R);
    limits.rightInf = aux.split(",").at(2).toInt(0);
    limits.rightSup = aux.split(",").at(0).toInt(0);

    aux = readAllFile(_PATH_LIMIT_U);
    limits.upInf = aux.split(",").at(2).toInt(0);
    limits.upSup = aux.split(",").at(0).toInt(0);

    aux = readAllFile(_PATH_LIMIT_L);
    limits.leftInf = aux.split(",").at(2).toInt(0);
    limits.leftSup = aux.split(",").at(0).toInt(0);

    aux = readAllFile(_PATH_LIMIT_D);
    limits.downInf = aux.split(",").at(2).toInt(0);
    limits.downSup = aux.split(",").at(0).toInt(0);

    aux = readAllFile(_PATH_LIMIT_S);
    limits.sourceX = aux.split(",").at(0).toInt(0);
    limits.sourceY = aux.split(",").at(1).toInt(0);

    return limits;

}

QVector2D genCalibXML::getWavelengthFrontiers()
{
    QVector2D fontier;
    strLimits limits = getLimitsFromHDD();
    float waveRight, waveUp, waveLeft, waveDown;
    float waveLimInf, waveLimSup;
    strAllLinReg linRegRes = getAllLR();

    //It obtains direction of the final limit
    //..
    //Min
    waveRight   = linRegRes.deltaHorizA + ( linRegRes.deltaHorizB * (double)abs(limits.sourceX - limits.rightInf) );
    waveUp      = linRegRes.deltaVertA + ( linRegRes.deltaVertB * (double)abs(limits.sourceY - limits.upInf) );
    waveLeft    = linRegRes.deltaHorizA + ( linRegRes.deltaHorizB * (double)abs(limits.sourceX - limits.leftInf) );
    waveDown    = linRegRes.deltaVertA + ( linRegRes.deltaVertB * (double)abs(limits.sourceY - limits.downInf) );
    waveLimInf  = (waveRight > waveUp)?waveRight:waveUp;
    waveLimInf  = (waveLimInf > waveLeft)?waveLimInf:waveLeft;
    waveLimInf  = (waveLimInf > waveDown)?waveLimInf:waveDown;
    //Max
    waveRight   = linRegRes.deltaHorizA + ( linRegRes.deltaHorizB * (double)abs(limits.sourceX - limits.rightSup) );
    waveUp      = linRegRes.deltaVertA + ( linRegRes.deltaVertB * (double)abs(limits.sourceY - limits.upSup) );
    waveLeft    = linRegRes.deltaHorizA + ( linRegRes.deltaHorizB * (double)abs(limits.sourceX - limits.leftSup) );
    waveDown    = linRegRes.deltaVertA + ( linRegRes.deltaVertB * (double)abs(limits.sourceY - limits.downSup) );
    waveLimSup  = (waveRight < waveUp)?waveRight:waveUp;
    waveLimSup  = (waveLimSup < waveLeft)?waveLimSup:waveLeft;
    waveLimSup  = (waveLimSup < waveDown)?waveLimSup:waveDown;

    //Set and return
    //..
    fontier.setX(waveLimInf);
    fontier.setY(waveLimSup);
    return fontier;
}

void genCalibXML::on_pbGenCal_clicked()
{
    if( isExportable )
    {
        //lstCalibFileNames calibPoints = fillLstCalibPoints();
        QString newFileCon = "";

        QString sourcePath = readFileParam(_PATH_CALBKG);

        //Region of interes
        //..
        squareAperture *regOfInteres = (squareAperture*)malloc(sizeof(squareAperture));
        if( !funGetSquareXML( _PATH_REGION_OF_INTERES, regOfInteres ) )
        {
            funcShowMsg("ERROR","Loading _PATH_REGION_OF_INTERES");
            return (void)false;
        }
        double xB,yB,wB,hB;
        xB = (double)regOfInteres->rectX / (double)regOfInteres->canvasW;
        yB = (double)regOfInteres->rectY / (double)regOfInteres->canvasH;
        wB = (double)regOfInteres->rectW / (double)regOfInteres->canvasW;
        hB = (double)regOfInteres->rectH / (double)regOfInteres->canvasH;

        //Square aperture
        //..
        squareAperture *sqApert = (squareAperture*)malloc(sizeof(squareAperture));
        if( !funGetSquareXML( _PATH_SQUARE_APERTURE, sqApert ) )
        {
            funcShowMsg("ERROR","Loading _PATH_SQUARE_APERTURE");
            return (void)false;
        }

        //Calculates the position expected in the received image
        //..
        int auxSqX, auxSqY, auxSqW, auxSqH, auxBigX, auxBigY;
        auxSqX  = round((float)_BIG_WIDTH  * ((float)sqApert->rectX / (float)sqApert->canvasW));
        auxSqY  = round((float)_BIG_HEIGHT * ((float)sqApert->rectY / (float)sqApert->canvasH));
        auxSqW  = round((float)_BIG_WIDTH  * ((float)sqApert->rectW / (float)sqApert->canvasW));
        auxSqH  = round((float)_BIG_HEIGHT * ((float)sqApert->rectH / (float)sqApert->canvasH));
        auxBigX = round((float)_BIG_WIDTH  * ((float)regOfInteres->rectX / (float)regOfInteres->canvasW));
        auxBigY = round((float)_BIG_HEIGHT * ((float)regOfInteres->rectY / (float)regOfInteres->canvasH));
        //In reference to the final position
        auxSqX -= auxBigX;
        auxSqY -= auxBigY;

        //Calculates linear regressions
        //..
        strAllLinReg linRegRes = getAllLR();

        //Obtains limits from HDD
        //..
        QString minWavelength, maxWavelength;
        QVector2D waveLim;
        waveLim = getWavelengthFrontiers();
        minWavelength = QString::number(waveLim.x());
        maxWavelength = QString::number(waveLim.y());

        //Square aperture as percentage
        //..
        double xs,ys,ws,hs;
        xs = (double)sqApert->rectX / (double)sqApert->canvasW;
        ys = (double)sqApert->rectY / (double)sqApert->canvasH;
        ws = (double)sqApert->rectW / (double)sqApert->canvasW;
        hs = (double)sqApert->rectH / (double)sqApert->canvasH;

        newFileCon.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");

        newFileCon.append("<calib>\n");

            newFileCon.append("    <bkgPath>"+ sourcePath                                       + "</bkgPath>\n");

            newFileCon.append("    <origin>(0,0)=(left,up)</origin>\n");

            newFileCon.append("    <W>"+ QString::number(_BIG_WIDTH)                            + "</W>\n");
            newFileCon.append("    <H>"+ QString::number(_BIG_HEIGHT)                           + "</H>\n");

            newFileCon.append("    <bigX>"+ QString::number( xB )                               + "</bigX>\n");
            newFileCon.append("    <bigY>"+ QString::number( yB )                               + "</bigY>\n");
            newFileCon.append("    <bigW>"+ QString::number( wB )                               + "</bigW>\n");
            newFileCon.append("    <bigH>"+ QString::number( hB )                               + "</bigH>\n");

            newFileCon.append("    <squareX>"+ QString::number( xs )                            + "</squareX>\n");
            newFileCon.append("    <squareY>"+ QString::number( ys )                            + "</squareY>\n");
            newFileCon.append("    <squareW>"+ QString::number( ws )                            + "</squareW>\n");
            newFileCon.append("    <squareH>"+ QString::number( hs )                            + "</squareH>\n");

            newFileCon.append("    <squarePixX>"+ QString::number( auxSqX )                     + "</squarePixX>\n");
            newFileCon.append("    <squarePixY>"+ QString::number( auxSqY )                     + "</squarePixY>\n");
            newFileCon.append("    <squarePixW>"+ QString::number( auxSqW )                     + "</squarePixW>\n");
            newFileCon.append("    <squarePixH>"+ QString::number( auxSqH )                     + "</squarePixH>\n");

            newFileCon.append("    <horizontalA>"+ QString::number( linRegRes.horizA )          + "</horizontalA>\n");
            newFileCon.append("    <horizontalB>"+ QString::number( linRegRes.horizB )          + "</horizontalB>\n");
            newFileCon.append("    <verticalA>"+ QString::number( linRegRes.vertA )             + "</verticalA>\n");
            newFileCon.append("    <verticalB>"+ QString::number( linRegRes.vertB )             + "</verticalB>\n");
            newFileCon.append("    <waveHorizA>"+ QString::number( linRegRes.waveHorizA )       + "</waveHorizA>\n");
            newFileCon.append("    <waveHorizB>"+ QString::number( linRegRes.waveHorizB )       + "</waveHorizB>\n");
            newFileCon.append("    <waveVertA>"+ QString::number( linRegRes.waveVertA )         + "</waveVertA>\n");
            newFileCon.append("    <waveVertB>"+ QString::number( linRegRes.waveVertB )         + "</waveVertB>\n");
            newFileCon.append("    <deltaHorizA>"+ QString::number( linRegRes.deltaHorizA )     + "</deltaHorizA>\n");
            newFileCon.append("    <deltaHorizB>"+ QString::number( linRegRes.deltaHorizB )     + "</deltaHorizB>\n");
            newFileCon.append("    <deltaVertA>"+ QString::number( linRegRes.deltaVertA )       + "</deltaVertA>\n");
            newFileCon.append("    <deltaVertB>"+ QString::number( linRegRes.deltaVertB )       + "</deltaVertB>\n");

            newFileCon.append("    <minWavelength>"   + minWavelength                           + "</minWavelength>\n");
            newFileCon.append("    <maxWavelength>"   + maxWavelength                           + "</maxWavelength>\n");

        newFileCon.append("</calib>\n");


        //Save file
        if( saveFile(_PATH_CALIBRATION_FILE,newFileCon) )
        {
            funcShowMsg(" ","File saved");
        }
        else
        {
            funcShowMsg("ERROR","Saving file");
        }







        /*
        double xs,ys,ws,hs;
        xs = (double)sqApert->rectX / (double)sqApert->canvasW;
        ys = (double)sqApert->rectY / (double)sqApert->canvasH;
        ws = (double)sqApert->rectW / (double)sqApert->canvasW;
        hs = (double)sqApert->rectH / (double)sqApert->canvasH;

        newFileCon.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");

        newFileCon.append("<calib>\n");

            newFileCon.append("    <bkgPath>"+ sourcePath               +"</bkgPath>\n");

            newFileCon.append("    <origin>(0,0)=(left,up)</origin>\n");

            newFileCon.append("    <W>"+ QString::number(_BIG_WIDTH)                        +"</W>\n");
            newFileCon.append("    <H>"+ QString::number(_BIG_HEIGHT)                       +"</H>\n");

            newFileCon.append("    <source>"+ calibPoints.source                            +"</source>\n");

            newFileCon.append("    <bigX>"+ QString::number( xB )                           +"</bigX>\n");
            newFileCon.append("    <bigY>"+ QString::number( yB )                           +"</bigY>\n");
            newFileCon.append("    <bigW>"+ QString::number( wB )                           +"</bigW>\n");
            newFileCon.append("    <bigH>"+ QString::number( hB )                           +"</bigH>\n");

            newFileCon.append("    <squareX>"+ QString::number( xs )                        +"</squareX>\n");
            newFileCon.append("    <squareY>"+ QString::number( ys )                        +"</squareY>\n");
            newFileCon.append("    <squareW>"+ QString::number( ws )                        +"</squareW>\n");
            newFileCon.append("    <squareH>"+ QString::number( hs )                        +"</squareH>\n");

            newFileCon.append("    <bRD>"+ calibPoints.blueRightDown                        +"</bRD>\n");
            newFileCon.append("    <bR>" + calibPoints.blueRight                            +"</bR>\n");
            newFileCon.append("    <bRU>"+ calibPoints.blueRightUp                          +"</bRU>\n");
            newFileCon.append("    <bU>" + calibPoints.blueUp                               +"</bU>\n");
            newFileCon.append("    <bLU>"+ calibPoints.blueLeftUp                           +"</bLU>\n");
            newFileCon.append("    <bL>" + calibPoints.blueLeft                             +"</bL>\n");
            newFileCon.append("    <bLD>"+ calibPoints.blueLeftDown                         +"</bLD>\n");
            newFileCon.append("    <bD>" + calibPoints.blueDown                             +"</bD>\n");

            newFileCon.append("    <gRD>"+ calibPoints.greenRightDown                       +"</gRD>\n");
            newFileCon.append("    <gR>" + calibPoints.greenRight                           +"</gR>\n");
            newFileCon.append("    <gRU>"+ calibPoints.greenRightUp                         +"</gRU>\n");
            newFileCon.append("    <gU>" + calibPoints.greenUp                              +"</gU>\n");
            newFileCon.append("    <gLU>"+ calibPoints.greenLeftUp                          +"</gLU>\n");
            newFileCon.append("    <gL>" + calibPoints.greenLeft                            +"</gL>\n");
            newFileCon.append("    <gLD>"+ calibPoints.greenLeftDown                        +"</gLD>\n");
            newFileCon.append("    <gD>" + calibPoints.greenDown                            +"</gD>\n");

            newFileCon.append("    <rRD>"+ calibPoints.redRightDown                         +"</rRD>\n");
            newFileCon.append("    <rR>" + calibPoints.redRight                             +"</rR>\n");
            newFileCon.append("    <rRU>"+ calibPoints.redRightUp                           +"</rRU>\n");
            newFileCon.append("    <rU>" + calibPoints.redUp                                +"</rU>\n");
            newFileCon.append("    <rLU>"+ calibPoints.redLeftUp                            +"</rLU>\n");
            newFileCon.append("    <rL>" + calibPoints.redLeft                              +"</rL>\n");
            newFileCon.append("    <rLD>"+ calibPoints.redLeftDown                          +"</rLD>\n");
            newFileCon.append("    <rD>" + calibPoints.redDown                              +"</rD>\n");


        newFileCon.append("</calib>\n");


        //Save file
        if( saveFile(_PATH_CALIBRATION_FILE,newFileCon) )
        {
            funcShowMsg("Success","File saved");
        }
        else
        {
            funcShowMsg("ERROR","Saving file");
        }
        */

    }
    else
    {
        funcShowMsg("Lack","Calibrations points incomplete");
    }

}

customQMatrix4x3 genCalibXML::mulLinRegXYW(customQMatrix4x4 X)
{
    //X={1,x,y,wavelength}
    QVector4D tmpY;
    QVector3D tmpBeta;
    customQMatrix4x3 mulLinReg;
    customQMatrix4x3 tmpX;

    //Wavelength
    //..
    tmpX.setRow(0,X.getRowCells(1,1,2,3));
    tmpX.setRow(1,X.getRowCells(2,1,2,3));
    tmpX.setRow(2,X.getRowCells(3,1,2,3));
    tmpX.setRow(3,X.getRowCells(4,1,2,3));
    tmpY = X.getCol1Index(4);
    //tmpX.print("tmpX wave:");
    tmpBeta = multipleLinearRegression(tmpX, tmpY);
    mulLinReg.setRow(0,tmpBeta);
    //X-axis
    //..
    tmpX.setRow(0,X.getRowCells(1,1,3,4));
    tmpX.setRow(1,X.getRowCells(2,1,3,4));
    tmpX.setRow(2,X.getRowCells(3,1,3,4));
    tmpX.setRow(3,X.getRowCells(4,1,3,4));
    tmpY = X.getCol1Index(2);
    //tmpX.print("tmpX x:");
    tmpBeta = multipleLinearRegression(tmpX, tmpY);
    mulLinReg.setRow(1,tmpBeta);
    //Y-axis
    //..
    tmpX.setRow(0,X.getRowCells(1,1,2,4));
    tmpX.setRow(1,X.getRowCells(2,1,2,4));
    tmpX.setRow(2,X.getRowCells(3,1,2,4));
    tmpX.setRow(3,X.getRowCells(4,1,2,4));
    tmpY = X.getCol1Index(3);
    //tmpX.print("tmpX y:");
    tmpBeta = multipleLinearRegression(tmpX, tmpY);
    mulLinReg.setRow(2,tmpBeta);
    //XY Simple Linear Regression
    //..
    double slrX[4], slrY[4];
    slrX[0] = X.getCell(1,2);   slrY[0] = X.getCell(1,3);
    slrX[1] = X.getCell(2,2);   slrY[1] = X.getCell(2,3);
    slrX[2] = X.getCell(3,2);   slrY[2] = X.getCell(3,3);
    slrX[3] = X.getCell(4,2);   slrY[3] = X.getCell(4,3);
    linearRegresion tmpSlr = funcLinearRegression(slrX,slrY,4);
    tmpBeta.setX(tmpSlr.a);
    tmpBeta.setY(tmpSlr.b);
    tmpBeta.setZ(0);
    mulLinReg.setRow(3,tmpBeta);

    //Finishes
    return mulLinReg;
}

QVector3D genCalibXML::multipleLinearRegression( customQMatrix4x3 X, QVector4D y )
{
    QVector3D B;
    QMatrix3x4 Xt = X.transposed();//X'
    customQMatrix3x3 Xprod;
    Xprod = matMultiply(&Xt,&X);//(X'X)
    Xprod = Xprod.inverted();//(X'X)-1
    QMatrix3x4 H = matMultiply(&Xprod,&Xt);//H = (X'X)-1 X'
    B = matMultiply(&H,&y);
    return B;
}

lstCalibFileNames genCalibXML::fillLstCalibPoints(){
    lstCalibFileNames calibFileNames = funcFillCalibStruct();
    lstCalibFileNames calibPoints;

    calibPoints.source          = readFileParam(calibFileNames.source);

    calibPoints.blueRightDown   = (QFile::exists(calibFileNames.blueRightDown))?readFileParam(calibFileNames.blueRightDown):"";
    calibPoints.blueRight       = (QFile::exists(calibFileNames.blueRight))?readFileParam(calibFileNames.blueRight):"";
    calibPoints.blueRightUp     = (QFile::exists(calibFileNames.blueRightUp))?readFileParam(calibFileNames.blueRightUp):"";
    calibPoints.blueUp          = (QFile::exists(calibFileNames.blueUp))?readFileParam(calibFileNames.blueUp):"";
    calibPoints.blueLeftUp      = (QFile::exists(calibFileNames.blueLeftUp))?readFileParam(calibFileNames.blueLeftUp):"";
    calibPoints.blueLeft        = (QFile::exists(calibFileNames.blueLeft))?readFileParam(calibFileNames.blueLeft):"";
    calibPoints.blueLeftDown    = (QFile::exists(calibFileNames.blueLeftDown))?readFileParam(calibFileNames.blueLeftDown):"";
    calibPoints.blueDown        = (QFile::exists(calibFileNames.blueDown))?readFileParam(calibFileNames.blueDown):"";

    calibPoints.greenRightDown  = (QFile::exists(calibFileNames.greenRightDown))?readFileParam(calibFileNames.greenRightDown):"";
    calibPoints.greenRight      = (QFile::exists(calibFileNames.greenRight))?readFileParam(calibFileNames.greenRight):"";
    calibPoints.greenRightUp    = (QFile::exists(calibFileNames.greenRightUp))?readFileParam(calibFileNames.greenRightUp):"";
    calibPoints.greenUp         = (QFile::exists(calibFileNames.greenUp))?readFileParam(calibFileNames.greenUp):"";
    calibPoints.greenLeftUp     = (QFile::exists(calibFileNames.greenLeftUp))?readFileParam(calibFileNames.greenLeftUp):"";
    calibPoints.greenLeft       = (QFile::exists(calibFileNames.greenLeft))?readFileParam(calibFileNames.greenLeft):"";
    calibPoints.greenLeftDown   = (QFile::exists(calibFileNames.greenLeftDown))?readFileParam(calibFileNames.greenLeftDown):"";
    calibPoints.greenDown       = (QFile::exists(calibFileNames.greenDown))?readFileParam(calibFileNames.greenDown):"";

    calibPoints.redRightDown    = (QFile::exists(calibFileNames.redRightDown))?readFileParam(calibFileNames.redRightDown):"";
    calibPoints.redRight        = (QFile::exists(calibFileNames.redRight))?readFileParam(calibFileNames.redRight):"";
    calibPoints.redRightUp      = (QFile::exists(calibFileNames.redRightUp))?readFileParam(calibFileNames.redRightUp):"";
    calibPoints.redUp           = (QFile::exists(calibFileNames.redUp))?readFileParam(calibFileNames.redUp):"";
    calibPoints.redLeftUp       = (QFile::exists(calibFileNames.redLeftUp))?readFileParam(calibFileNames.redLeftUp):"";
    calibPoints.redLeft         = (QFile::exists(calibFileNames.redLeft))?readFileParam(calibFileNames.redLeft):"";
    calibPoints.redLeftDown     = (QFile::exists(calibFileNames.redLeftDown))?readFileParam(calibFileNames.redLeftDown):"";
    calibPoints.redDown         = (QFile::exists(calibFileNames.redDown))?readFileParam(calibFileNames.redDown):"";

    return calibPoints;

}



void genCalibXML::on_pbFiles_clicked()
{
    QDesktopServices::openUrl(QUrl(funcRemoveFileNameFromPath(_PATH_CALIBRATION_FILE)));
    this->close();
}
