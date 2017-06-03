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

#include <QRgb>

#include <mainwindow.h>

bool isExportable = false;
MainWindow *mainW;
cameraResolution* genCalibXMLcamRes;


genCalibXML::genCalibXML(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::genCalibXML)
{
    ui->setupUi(this);    
    autoLoadCentroids();

    disableButtons();

    mainW = qobject_cast<MainWindow*>(parent);

    genCalibXMLcamRes = mainW->getCamRes();

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

    calibFileNames.limR             = "./settings/Calib/limR.hypcam";
    calibFileNames.limU             = "./settings/Calib/limU.hypcam";
    calibFileNames.limL             = "./settings/Calib/limL.hypcam";
    calibFileNames.limD             = "./settings/Calib/limD.hypcam";
    calibFileNames.sourceHalogen    = "./settings/Calib/sourceHalogen.hypcam";


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
    //Limits
    acum=setButton(ui->pbRedDown,calibFileNames.limR,false)?++i:i;
    acum=setButton(ui->pbRedDown,calibFileNames.limU,false)?++i:i;
    acum=setButton(ui->pbRedDown,calibFileNames.limL,false)?++i:i;
    acum=setButton(ui->pbRedDown,calibFileNames.limD,false)?++i:i;
    acum=setButton(ui->pbRedDown,calibFileNames.sourceHalogen,false)?++i:i;


    if(acum==17)
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

    //qDebug() << "a:x1: " << x1;
    //qDebug() << "a:y1: " << y1;

    //Offset
    double offsetX, offsetY;
    QPoint Corner = getSquareCorner(genCalibXMLcamRes);
    //qDebug() << "squareCorner.x(): " << Corner.x();
    //qDebug() << "squareCorner.y(): " << Corner.y();
    //qDebug() << "source.x(): " << centroids->source.split(",").at(0).toFloat(0);
    //qDebug() << "source.y(): " << centroids->source.split(",").at(1).toFloat(0);

    //Offset respect to the left-top corner
    offsetX     = (Corner.x() - centroids->source.split(",").at(0).toFloat(0))*1;
    offsetY     = (Corner.y() - centroids->source.split(",").at(1).toFloat(0))*1;
    //qDebug() << "offsetX: " << offsetX;
    //qDebug() << "offsetY: " << offsetY;

    //--------------------------------------------
    //Horizontal
    //--------------------------------------------
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

    /*
    for(int i = 0; i<7; i++)
        printf("%d ", (int)pointsX[i]);
    printf("\n");
    for(int i = 0; i<7; i++)
        printf("%d ", (int)pointsY[i]);
    printf("\n");
    */

    linearRegresion horB = funcLinearRegression( pointsX, pointsY, 7 );
    linRegRes.horizA = offsetY + horB.a;
    linRegRes.horizB = horB.b;

    //--------------------------------------------
    //Vertical
    //--------------------------------------------
    int xPos = 1;
    int yPos = 0;
    pointsX[0] = (double)centroids->redDown.split(",").at(xPos).toInt(0);
    pointsX[1] = (double)centroids->greenDown.split(",").at(xPos).toInt(0);
    pointsX[2] = (double)centroids->blueDown.split(",").at(xPos).toInt(0);
    pointsX[3] = (double)centroids->source.split(",").at(xPos).toInt(0);
    pointsX[4] = (double)centroids->blueUp.split(",").at(xPos).toInt(0);
    pointsX[5] = (double)centroids->greenUp.split(",").at(xPos).toInt(0);
    pointsX[6] = (double)centroids->redUp.split(",").at(xPos).toInt(0);

    pointsY[0] = (double)centroids->redDown.split(",").at(yPos).toInt(0);
    pointsY[1] = (double)centroids->greenDown.split(",").at(yPos).toInt(0);
    pointsY[2] = (double)centroids->blueDown.split(",").at(yPos).toInt(0);
    pointsY[3] = (double)centroids->source.split(",").at(yPos).toInt(0);
    pointsY[4] = (double)centroids->blueUp.split(",").at(yPos).toInt(0);
    pointsY[5] = (double)centroids->greenUp.split(",").at(yPos).toInt(0);
    pointsY[6] = (double)centroids->redUp.split(",").at(yPos).toInt(0);


    linearRegresion verB = funcLinearRegression( pointsX, pointsY, 7 );
    linRegRes.vertA = offsetX + verB.a;
    linRegRes.vertB = verB.b;

    if( false )
    {
        for(int i = 0; i<7; i++)
            printf("%d, %d\n", (int)pointsX[i], (int)pointsY[i]);
        printf("\n");
        //exit(0);
    }



    //--------------------------------------------
    //Wavelength horizontal
    //--------------------------------------------
    double auxL, auxR, aux;
    aux  = centroids->source.split(",").at(0).toFloat(0);//SourceX

    //Delta source X
    pointsX[0] = 0.0;
    pointsY[0] = 0.0;
    //Delta blue X
    //auxL = (double)abs(aux - centroids->blueLeft.split(",").at(0).toFloat(0));
    auxL = (double)fabs( aux - centroids->blueLeft.split(",").at(0).toFloat(0) );//SourceX-blueLeft
    auxR = (double)fabs(aux - centroids->blueRight.split(",").at(0).toFloat(0));//SourceX-blueRight
    pointsX[1] = (double)_BLUE_WAVELENGHT;
    pointsY[1] = (auxL+auxR) / 2.0;
    //Delta green X
    auxL = (double)fabs(aux - centroids->greenLeft.split(",").at(0).toFloat(0));
    auxR = (double)fabs(aux - centroids->greenRight.split(",").at(0).toFloat(0));
    pointsX[2] = (double)_GREEN_WAVELENGHT;
    pointsY[2] = (auxL+auxR) / 2.0;
    //Delta red X
    auxL = (double)fabs(aux - centroids->redLeft.split(",").at(0).toFloat(0));
    auxR = (double)fabs(aux - centroids->redRight.split(",").at(0).toFloat(0));
    pointsX[3] = (double)_RED_WAVELENGHT;
    pointsY[3] = (auxL+auxR) / 2.0;

    linearRegresion waveXB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.waveHorizA = waveXB.a;
    linRegRes.waveHorizB = waveXB.b;

    //--------------------------------------------
    //Wavelength vertical
    //--------------------------------------------
    double auxU, auxD;
    aux         = centroids->source.split(",").at(1).toFloat(0);
    //Delta source X
    pointsX[0]  = 0.0;
    pointsY[0]  = 0.0;
    //Delta blue X
    auxU        = (double)fabs(aux - centroids->blueUp.split(",").at(1).toFloat(0));
    auxD        = (double)fabs(aux - centroids->blueDown.split(",").at(1).toFloat(0));
    pointsX[1]  = (double)_BLUE_WAVELENGHT;
    pointsY[1]  = (auxU+auxD) / 2.0;
    //Delta green X
    auxU        = (double)fabs(aux - centroids->greenUp.split(",").at(1).toFloat(0));
    auxD        = (double)fabs(aux - centroids->greenDown.split(",").at(1).toFloat(0));
    pointsX[2]  = (double)_GREEN_WAVELENGHT;
    pointsY[2]  = (auxU+auxD) / 2.0;
    //Delta red X
    auxU        = (double)fabs(aux - centroids->redUp.split(",").at(1).toFloat(0));
    auxD        = (double)fabs(aux - centroids->redDown.split(",").at(1).toFloat(0));
    pointsX[3]  = (double)_RED_WAVELENGHT;
    pointsY[3]  = (auxU+auxD) / 2.0;

    linearRegresion waveYB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.waveVertA = waveYB.a;
    linRegRes.waveVertB = waveYB.b;


    //--------------------------------------------
    //Delta horizontal
    //--------------------------------------------
    aux  = centroids->source.split(",").at(0).toFloat(0);//Source X
    //Delta source X
    pointsX[0] = 0.0;
    pointsY[0] = 0.0;
    //Delta blue X
    auxL = (double)fabs(aux - centroids->blueLeft.split(",").at(0).toFloat(0));
    auxR = (double)fabs(aux - centroids->blueRight.split(",").at(0).toFloat(0));
    pointsY[1] = (double)_BLUE_WAVELENGHT;
    pointsX[1] = (auxL+auxR) / 2.0;
    //Delta green X
    auxL = (double)fabs(aux - centroids->greenLeft.split(",").at(0).toFloat(0));
    auxR = (double)fabs(aux - centroids->greenRight.split(",").at(0).toFloat(0));
    pointsY[2] = (double)_GREEN_WAVELENGHT;
    pointsX[2] = (auxL+auxR) / 2.0;
    //Delta red X
    auxL = (double)fabs(aux - centroids->redLeft.split(",").at(0).toFloat(0));
    auxR = (double)fabs(aux - centroids->redRight.split(",").at(0).toFloat(0));
    pointsY[3] = (double)_RED_WAVELENGHT;
    pointsX[3] = (auxL+auxR) / 2.0;

    linearRegresion deltaXB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.deltaHorizA = deltaXB.a;
    linRegRes.deltaHorizB = deltaXB.b;

    //--------------------------------------------
    //Delta vertical
    //--------------------------------------------
    aux         = centroids->source.split(",").at(1).toFloat(0);
    //Delta source X
    pointsY[0]  = 0.0;
    pointsX[0]  = 0.0;
    //Delta blue X
    auxU        = (double)fabs(aux - centroids->blueUp.split(",").at(1).toFloat(0));
    auxD        = (double)fabs(aux - centroids->blueDown.split(",").at(1).toFloat(0));
    pointsY[1]  = (double)_BLUE_WAVELENGHT;
    pointsX[1]  = (auxU+auxD) / 2.0;
    //Delta green X
    auxU        = (double)fabs(aux - centroids->greenUp.split(",").at(1).toFloat(0));
    auxD        = (double)fabs(aux - centroids->greenDown.split(",").at(1).toFloat(0));
    pointsY[2]  = (double)_GREEN_WAVELENGHT;
    pointsX[2]  = (auxU+auxD) / 2.0;
    //Delta red X
    auxU        = (double)fabs(aux - centroids->redUp.split(",").at(1).toFloat(0));
    auxD        = (double)fabs(aux - centroids->redDown.split(",").at(1).toFloat(0));
    pointsY[3]  = (double)_RED_WAVELENGHT;
    pointsX[3]  = (auxU+auxD) / 2.0;

    linearRegresion deltaYB = funcLinearRegression( pointsX, pointsY, 4 );
    linRegRes.deltaVertA = deltaYB.a;
    linRegRes.deltaVertB = deltaYB.b;

    return linRegRes;
}

strAllLinReg genCalibXML::getAllLR(){
    lstCalibFileNames calibPoints = fillLstCalibPoints();
    //QVector2D sqUsableCorner = getSqUsableIni();
    //return calcAllLinReg( &calibPoints, sqUsableCorner.x(), sqUsableCorner.y() );

    return calcAllLinReg( &calibPoints );
}

/*
QPoint genCalibXML::squareCorner(){

    QPoint corner;
    corner.setX(-1);
    corner.setY(-1);

    //Square aperture
    //..
    squareAperture *sqApert = (squareAperture*)malloc(sizeof(squareAperture));
    //if( !funGetSquareXML( _PATH_SQUARE_APERTURE, sqApert ) )
    if( mainW->rectangleInPixelsFromSquareXML(_PATH_SQUARE_APERTURE, sqApert) == 0 )
    {
        funcShowMsg("ERROR","Loading 02/Junio/2017 _PATH_SQUARE_APERTURE");
        mainW->mouseCursorReset();
        return corner;
    }


    //Usable area
    //..
    squareAperture *sqSquareUsable = (squareAperture*)malloc(sizeof(squareAperture));
    //if( !funGetSquareXML( _PATH_SQUARE_APERTURE, sqApert ) )
    if( mainW->rectangleInPixelsFromSquareXML(_PATH_SQUARE_USABLE, sqSquareUsable) == 0 )
    {
        funcShowMsg("ERROR","Loading 02/Junio/2017 _PATH_SQUARE_USABLE");
        mainW->mouseCursorReset();
        return corner;
    }

    qDebug() << "sqApert->rectX: " << sqApert->rectX;
    qDebug() << "sqSquareUsable->rectX: " << sqSquareUsable->rectX;
    corner.setX( sqApert->rectX - sqSquareUsable->rectX );
    corner.setY( sqApert->rectY - sqSquareUsable->rectY );
    return corner;

}*/

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
    //qDebug() << "Aqui3-3";
    return fontier;
}

void genCalibXML::on_pbGenCal_clicked()
{
    if( isExportable )
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        //lstCalibFileNames calibPoints = fillLstCalibPoints();
        QString newFileCon = "";

        QString sourcePath = readFileParam(_PATH_CALBKG);

        /*
        //Region of interes
        //..
        squareAperture *regOfInteres = (squareAperture*)malloc(sizeof(squareAperture));
        if( !funGetSquareXML( _PATH_SQUARE_USABLE, regOfInteres ) )
        {
            funcShowMsg("ERROR","Loading _PATH_REGION_OF_INTERES");
            return (void)false;
        }
        double xB,yB,wB,hB;
        xB = (double)regOfInteres->rectX / (double)regOfInteres->canvasW;
        yB = (double)regOfInteres->rectY / (double)regOfInteres->canvasH;
        wB = (double)regOfInteres->rectW / (double)regOfInteres->canvasW;
        hB = (double)regOfInteres->rectH / (double)regOfInteres->canvasH;*/

        //return (void)NULL;

        //Square aperture
        //..
        //cameraResolution* camRes = (cameraResolution*)malloc(sizeof(cameraResolution));
        //camRes = mainW->getCamRes();
        squareAperture *sqApert = (squareAperture*)malloc(sizeof(squareAperture));
        //if( !funGetSquareXML( _PATH_SQUARE_APERTURE, sqApert ) )
        if( rectangleInPixelsFromSquareXML(_PATH_SQUARE_APERTURE, sqApert, genCalibXMLcamRes) == 0 )
        {
            funcShowMsg("ERROR","Loading _PATH_SQUARE_APERTURE");
            mainW->mouseCursorReset();
            return (void)false;
        }
        /*
        //Calculates the position expected in the received image
        //qDebug() << "camRes->width: " << camRes->width;
        int auxSqX, auxSqY, auxSqW, auxSqH, auxBigX, auxBigY;
        auxSqX  = round((float)camRes->width  * ((float)sqApert->rectX / (float)sqApert->canvasW));
        auxSqY  = round((float)camRes->height * ((float)sqApert->rectY / (float)sqApert->canvasH));
        auxSqW  = round((float)camRes->width  * ((float)sqApert->rectW / (float)sqApert->canvasW));
        auxSqH  = round((float)camRes->height * ((float)sqApert->rectH / (float)sqApert->canvasH));
        auxBigX = round((float)camRes->width  * ((float)regOfInteres->rectX / (float)regOfInteres->canvasW));
        auxBigY = round((float)camRes->height * ((float)regOfInteres->rectY / (float)regOfInteres->canvasH));
        //In reference to the final position
        auxSqX -= auxBigX;
        auxSqY -= auxBigY;  */

        //Usable area
        //..
        squareAperture *sqUsable = (squareAperture*)malloc(sizeof(squareAperture));
        //if( !funGetSquareXML( _PATH_SQUARE_USABLE, sqUsable ) )
        if( rectangleInPixelsFromSquareXML(_PATH_SQUARE_USABLE,sqUsable,genCalibXMLcamRes) == 0 )
        {
            funcShowMsg("ERROR","Loading _PATH_SQUARE_USABLE");
            mainW->mouseCursorReset();
            return (void)false;
        }
        /*
        //Calculates the position expected in the received image
        int auxSqUsableX, auxSqUsableY, auxSqUsableW, auxSqUsableH;
        float areaInterW, areaInterH;
        areaInterW = (float)camRes->width * wB;
        areaInterH = (float)camRes->height * hB;
        auxSqUsableX  = round((float)areaInterW * ((float)sqUsable->rectX / (float)sqUsable->canvasW));
        auxSqUsableY  = round((float)areaInterH * ((float)sqUsable->rectY / (float)sqUsable->canvasH));
        auxSqUsableW  = round((float)areaInterW * ((float)sqUsable->rectW / (float)sqUsable->canvasW));
        auxSqUsableH  = round((float)areaInterH * ((float)sqUsable->rectH / (float)sqUsable->canvasH));*/

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
        qDebug() << "minWavelength: " << minWavelength;
        qDebug() << "maxWavelength: " << maxWavelength;        

        //Square aperture as percentage
        //..
        /*
        double xs,ys,ws,hs;
        xs = (double)sqApert->rectX / (double)sqApert->canvasW;
        ys = (double)sqApert->rectY / (double)sqApert->canvasH;
        ws = (double)sqApert->rectW / (double)sqApert->canvasW;
        hs = (double)sqApert->rectH / (double)sqApert->canvasH;*/

        //Calculate MIN num of bands
        //..
        QVector2D spectralResolution;
        spectralResolution = calcSpectralResolution();
        QString maxNumBand, minSpecRes;
        maxNumBand  = QString::number(spectralResolution.x());
        minSpecRes  = QString::number(spectralResolution.y());        

        //Calculates the sensivities and save into HDD
        //..
        QString Sr, Sg, Sb;
        lstDoubleAxisCalibration daCalibGenCal;        
        daCalibGenCal.LR = getAllLR();
        daCalibGenCal.maxNumBands   = spectralResolution.x();
        daCalibGenCal.minSpecRes    = spectralResolution.y();
        daCalibGenCal.minWavelength = waveLim.x();
        daCalibGenCal.maxWavelength = waveLim.y();        

        /*daCalibGenCal.squareUsableX = auxSqUsableX;
        daCalibGenCal.squareUsableY = auxSqUsableY;
        daCalibGenCal.squareUsableW = auxSqUsableW;
        daCalibGenCal.squareUsableH = auxSqUsableH;*/        
        calculateAndSaveSensitivities(&daCalibGenCal);
        Sr = readFileParam( _PATH_RED_SENSITIV );
        Sg = readFileParam( _PATH_GREEN_SENSITIV );
        Sb = readFileParam( _PATH_BLUE_SENSITIV );



        //It creates the XML file
        //..
        newFileCon.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");

        newFileCon.append("<calib>\n");

            newFileCon.append("    <bkgPath>"+ sourcePath                                       + "</bkgPath>\n");

            newFileCon.append("    <origin>(0,0)=(left,up)</origin>\n");

            newFileCon.append("    <W>"+ QString::number(genCalibXMLcamRes->width)                            + "</W>\n");
            newFileCon.append("    <H>"+ QString::number(genCalibXMLcamRes->height)                           + "</H>\n");

            /*newFileCon.append("    <bigX>"+ QString::number( xB )                               + "</bigX>\n");
            newFileCon.append("    <bigY>"+ QString::number( yB )                               + "</bigY>\n");
            newFileCon.append("    <bigW>"+ QString::number( wB )                               + "</bigW>\n");
            newFileCon.append("    <bigH>"+ QString::number( hB )                               + "</bigH>\n");*/

            //newFileCon.append("    <squareCanvasW>"+ QString::number( sqApert->canvasW )       + "</squareCanvasW>\n");
            //newFileCon.append("    <squareCanvasH>"+ QString::number( sqApert->canvasH )       + "</squareCanvasH>\n");
            newFileCon.append("    <squareX>"+ QString::number( sqApert->rectX )               + "</squareX>\n");
            newFileCon.append("    <squareY>"+ QString::number( sqApert->rectY )               + "</squareY>\n");
            newFileCon.append("    <squareW>"+ QString::number( sqApert->rectW )               + "</squareW>\n");
            newFileCon.append("    <squareH>"+ QString::number( sqApert->rectH )               + "</squareH>\n");

            /*newFileCon.append("    <squarePixX>"+ QString::number( auxSqX )                     + "</squarePixX>\n");
            newFileCon.append("    <squarePixY>"+ QString::number( auxSqY )                     + "</squarePixY>\n");
            newFileCon.append("    <squarePixW>"+ QString::number( auxSqW )                     + "</squarePixW>\n");
            newFileCon.append("    <squarePixH>"+ QString::number( auxSqH )                     + "</squarePixH>\n");*/

            //newFileCon.append("    <squareCanvasW>"+ QString::number( sqUsable->canvasW )      + "</squareCanvasW>\n");
            //newFileCon.append("    <squareCanvasH>"+ QString::number( sqUsable->canvasH )      + "</squareCanvasH>\n");
            newFileCon.append("    <squareUsableX>"+ QString::number( sqUsable->rectX )         + "</squareUsableX>\n");
            newFileCon.append("    <squareUsableY>"+ QString::number( sqUsable->rectY )         + "</squareUsableY>\n");
            newFileCon.append("    <squareUsableW>"+ QString::number( sqUsable->rectW )         + "</squareUsableW>\n");
            newFileCon.append("    <squareUsableH>"+ QString::number( sqUsable->rectH )         + "</squareUsableH>\n");
            //newFileCon.append("    <squareUsableX1>"+ QString::number( auxSqUsableX1 )          + "</squareUsableX1>\n");
            //newFileCon.append("    <squareUsableY1>"+ QString::number( auxSqUsableY1 )          + "</squareUsableY1>\n");

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

            newFileCon.append("    <minWavelength>"     + minWavelength                         + "</minWavelength>\n");
            newFileCon.append("    <maxWavelength>"     + maxWavelength                         + "</maxWavelength>\n");
            newFileCon.append("    <maxNumBand>"        + maxNumBand                            + "</maxNumBand>\n");
            newFileCon.append("    <minSpecRes>"        + minSpecRes                            + "</minSpecRes>\n");

            newFileCon.append("    <Sr>"    + Sr                                                + "</Sr>\n");
            newFileCon.append("    <Sg>"    + Sg                                                + "</Sg>\n");
            newFileCon.append("    <Sb>"    + Sb                                                + "</Sb>\n");




        newFileCon.append("</calib>\n");

        QApplication::restoreOverrideCursor();

        //Save file
        if( saveFile(_PATH_CALIBRATION_FILE,newFileCon) )
        {
            funcShowMsg(" ","File saved");
        }
        else
        {
            funcShowMsg("ERROR","Saving file");
        }

    }
    else
    {
        funcShowMsg("Lack","Incomplete Calibrations Points");
    }

}

void genCalibXML::calculateAndSaveSensitivities(lstDoubleAxisCalibration *daCalibGenCal)
{    
    //It is required offsets because, the calcDiffProj was created to
    //manage coordinates based on square aperture and apply a transformation
    //that moves the centroide's coordinates by applying LR.
    //int offsetX, offsetY;
    //offsetX = 2;
    //offsetY = 5;

    //--------------------------------------------------
    //Get source centroide
    //--------------------------------------------------
    //QString sourceHalogen;
    QVector2D origin;
    //sourceHalogen = readFileParam( _PATH_LIMIT_S );
    QPoint leftTopCorner = getSquareCorner(genCalibXMLcamRes);
    origin.setX(leftTopCorner.x());
    origin.setY(leftTopCorner.y());
    //origin.setX( sourceHalogen.split(",").at(0).toInt(0) - daCalibGenCal->squareUsableX - offsetX );
    //origin.setY( sourceHalogen.split(",").at(1).toInt(0) - daCalibGenCal->squareUsableY - offsetY );

    //--------------------------------------------------
    //Account all sensitivities
    //--------------------------------------------------
    double actWave;
    strDiffProj diffProj;    
    QImage img( _PATH_DISPLAY_IMAGE );
    QImage imgMod( _PATH_DISPLAY_IMAGE );
    QRgb tmpPix;
    int r, c, tmpX, tmpY, numWaves, range;
    numWaves = 0;
    double response[4][daCalibGenCal->maxNumBands];
    double sensitiv[4][daCalibGenCal->maxNumBands];
    double sensNorm[3][daCalibGenCal->maxNumBands];

    memset(response[0],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(response[1],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(response[2],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(response[3],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));

    memset(sensitiv[0],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(sensitiv[1],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(sensitiv[2],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(sensitiv[3],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));

    memset(sensNorm[0],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(sensNorm[1],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));
    memset(sensNorm[2],'\0',(daCalibGenCal->maxNumBands*sizeof(double)));

    range = 0;
    for( r=origin.y()-range; r<=origin.y()+range; r++ )
    {
        for( c=origin.x()-range; c<=origin.x()+range; c++ )
        {
            actWave     = daCalibGenCal->minWavelength;
            numWaves = 0;
            while( actWave < daCalibGenCal->maxWavelength )
            {

                //--------------------------------------------------
                //Calculates the projection
                //--------------------------------------------------
                diffProj.x          = c;
                diffProj.y          = r;
                diffProj.wavelength = actWave;
                calcDiffProj(&diffProj, daCalibGenCal);

                //--------------------------------------------------
                //Accumulates diffraction values sensed
                //--------------------------------------------------
                tmpPix = img.pixel( diffProj.rx, diffProj.ry );//Right
                response[0][numWaves] += (double)qRed(tmpPix);
                response[1][numWaves] += (double)qGreen(tmpPix);
                response[2][numWaves] += (double)qBlue(tmpPix);
                //qDebug() << "right \t r: " << qRed(tmpPix)<< "\tg: " << qGreen(tmpPix)<< "\tb: " << qBlue(tmpPix);

                tmpPix = img.pixel( diffProj.ux, diffProj.uy );//Up
                response[0][numWaves] += (double)qRed(tmpPix);
                response[1][numWaves] += (double)qGreen(tmpPix);
                response[2][numWaves] += (double)qBlue(tmpPix);
                //qDebug() << "Up \t r: " << qRed(tmpPix)<< "\tg: " << qGreen(tmpPix)<< "\tb: " << qBlue(tmpPix);

                tmpPix = img.pixel( diffProj.lx, diffProj.ly );//Left
                response[0][numWaves] += (double)qRed(tmpPix);
                response[1][numWaves] += (double)qGreen(tmpPix);
                response[2][numWaves] += (double)qBlue(tmpPix);
                //qDebug() << "Left \t r: " << qRed(tmpPix)<< "\tg: " << qGreen(tmpPix)<< "\tb: " << qBlue(tmpPix);

                tmpPix = img.pixel( diffProj.dx, diffProj.dy );//Down
                response[0][numWaves] += (double)qRed(tmpPix);
                response[1][numWaves] += (double)qGreen(tmpPix);
                response[2][numWaves] += (double)qBlue(tmpPix);
                //qDebug() << "Down \t r: " << qRed(tmpPix)<< "\tg: " << qGreen(tmpPix)<< "\tb: " << qBlue(tmpPix);
                //qDebug() << "";

                //--------------------------------------------------
                //Draw into the real image
                //--------------------------------------------------
                tmpX = diffProj.x; tmpY = diffProj.y;
                imgMod.setPixelColor(tmpX,tmpY,Qt::magenta);

                tmpX = diffProj.rx; tmpY = diffProj.ry;
                imgMod.setPixelColor(tmpX,tmpY,Qt::red);

                tmpX = diffProj.ux; tmpY = diffProj.uy;
                imgMod.setPixelColor(tmpX,tmpY,Qt::green);

                tmpX = diffProj.lx; tmpY = diffProj.ly;
                imgMod.setPixelColor(tmpX,tmpY,Qt::blue);

                tmpX = diffProj.dx; tmpY = diffProj.dy;
                imgMod.setPixelColor(tmpX,tmpY,Qt::yellow);

                actWave += daCalibGenCal->minSpecRes;
                numWaves++;


                //exit(2);

            }
        }
    }

    //Get halogen function
    //..
    QList<double> halogenFunction;
    halogenFunction = getNormedFunction( _PATH_HALOGEN_FUNCTION );

    //It calcultes the average sensitivities
    //..
    QString sensitivities;
    QString redResponse, greenResponse, blueResponse;
    QString redSensivility, greenSensivility, blueSensivility;
    QString redSenNorm, greenSenNorm, blueSenNorm;
    QString halogenIrradiance;
    int i, repeated, idWave;
    idWave = round(daCalibGenCal->minWavelength);
    repeated = (1+(range*2))*(1+(range*2))*4;
    for(i=0; i<numWaves; i++)
    {
        response[0][i]      = ((double)response[0][i] / (double)repeated) / 255.0;
        response[1][i]      = ((double)response[1][i] / (double)repeated) / 255.0;
        response[2][i]      = ((double)response[2][i] / (double)repeated) / 255.0;
        response[3][i]      = response[0][i] + response[1][i] + response[2][i];

        sensitiv[0][i]      = response[0][i] / halogenFunction.at(idWave+i);
        sensitiv[1][i]      = response[1][i] / halogenFunction.at(idWave+i);
        sensitiv[2][i]      = response[2][i] / halogenFunction.at(idWave+i);
        sensitiv[3][i]      = sensitiv[0][i] + sensitiv[1][i] + sensitiv[2][i];

        sensNorm[0][i]      = sensitiv[0][i] / sensitiv[3][i];
        sensNorm[1][i]      = sensitiv[1][i] / sensitiv[3][i];
        sensNorm[2][i]      = sensitiv[2][i] / sensitiv[3][i];

        if(i==0)
        {
            redResponse         = QString::number(response[0][i]);
            greenResponse       = QString::number(response[1][i]);
            blueResponse        = QString::number(response[2][i]);

            redSensivility      = QString::number(sensitiv[0][i]);
            greenSensivility    = QString::number(sensitiv[1][i]);
            blueSensivility     = QString::number(sensitiv[2][i]);

            sensitivities       = QString::number(sensitiv[0][i]+sensitiv[1][i]+sensitiv[2][i]);

            redSenNorm          = QString::number(sensNorm[0][i]);
            greenSenNorm        = QString::number(sensNorm[1][i]);
            blueSenNorm         = QString::number(sensNorm[2][i]);

            halogenIrradiance   = QString::number(halogenFunction.at(idWave+i));
        }
        else
        {
            redResponse.append(","+QString::number(response[0][i]));
            greenResponse.append(","+QString::number(response[1][i]));
            blueResponse.append(","+QString::number(response[2][i]));

            redSensivility.append(","+QString::number(sensitiv[0][i]));
            greenSensivility.append(","+QString::number(sensitiv[1][i]));
            blueSensivility.append(","+QString::number(sensitiv[2][i]));

            sensitivities.append(","+QString::number(sensitiv[0][i]+sensitiv[1][i]+sensitiv[2][i]));

            redSenNorm.append(","+QString::number(sensNorm[0][i]));
            greenSenNorm.append(","+QString::number(sensNorm[1][i]));
            blueSenNorm.append(","+QString::number(sensNorm[2][i]));

            halogenIrradiance.append(","+QString::number(halogenFunction.at(idWave+i)));
        }
    }
    //Save backup
    saveFile(_PATH_RED_RESPONSE,redResponse);
    saveFile(_PATH_GREEN_RESPONSE,greenResponse);
    saveFile(_PATH_BLUE_RESPONSE,blueResponse);

    saveFile(_PATH_RED_SENSITIV,redSensivility);
    saveFile(_PATH_GREEN_SENSITIV,greenSensivility);
    saveFile(_PATH_BLUE_SENSITIV,blueSensivility);

    saveFile(_PATH_RGB_SENSIVILITIES,sensitivities);

    saveFile(_PATH_RED_SENS_NORM,redSenNorm);
    saveFile(_PATH_GREEN_SENS_NORM,greenSenNorm);
    saveFile(_PATH_BLUE_SENS_NORM,blueSenNorm);

    saveFile(_PATH_HALOGEN_IRRADIATION,halogenIrradiance);

    img.save(_PATH_IMG_SENSIBILITIES);
    imgMod.save(_PATH_AUX_IMG);

}

QList<double> genCalibXML::getNormedFunction( QString fileName )
{
    QString contain;
    contain = readFileParam(fileName);
    QList<QString> irradiation;
    QList<double> function;
    irradiation = contain.split(",");
    int i;
    double max;
    max = irradiation.at(0).toDouble(0);
    irradiation.removeAt(0);
    for(i=0;i<irradiation.count();i++)
    {
        function.append(irradiation.at(i).toDouble(0) / max );
    }
    return function;
}

QVector2D genCalibXML::getSqUsableIni(){

    QVector2D result;
    result.setX(-1);
    result.setY(-1);


    //Region of interes
    //..
    squareAperture *regOfInteres = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SQUARE_USABLE, regOfInteres ) )
    {
        funcShowMsg("ERROR","Loading _PATH_REGION_OF_INTERES");
        return result;
    }
    double wB,hB;
    wB = (double)regOfInteres->rectW / (double)regOfInteres->canvasW;
    hB = (double)regOfInteres->rectH / (double)regOfInteres->canvasH;

    //Usable area
    //..
    squareAperture *sqUsable = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SQUARE_USABLE, sqUsable ) )
    {
        funcShowMsg("ERROR","Loading _PATH_SQUARE_USABLE");
        return result;
    }
    //Calculates the position expected in the received image
    int auxSqUsableX, auxSqUsableY;//, auxSqUsableW, auxSqUsableH;
    float areaInterW, areaInterH;
    areaInterW = (float)genCalibXMLcamRes->width * wB;
    areaInterH = (float)genCalibXMLcamRes->height * hB;
    auxSqUsableX  = round((float)areaInterW * ((float)sqUsable->rectX / (float)sqUsable->canvasW));
    auxSqUsableY  = round((float)areaInterH * ((float)sqUsable->rectY / (float)sqUsable->canvasH));
    //auxSqUsableW  = round((float)areaInterW * ((float)sqUsable->rectW / (float)sqUsable->canvasW));
    //auxSqUsableH  = round((float)areaInterH * ((float)sqUsable->rectH / (float)sqUsable->canvasH));
    result.setX(auxSqUsableX);
    result.setY(auxSqUsableY);
    return result;
}

QVector2D genCalibXML::calcSpectralResolution()
{
    QVector2D results;
    int minPixel, maxPixel, deltaHoriz, deltaVert, numPixels;
    QVector2D waveLim;
    strAllLinReg LR = getAllLR();
    waveLim = getWavelengthFrontiers();
    float waveRange;

    //Horizontal number of pixels
    minPixel    = round(LR.waveHorizA + (LR.waveHorizB * waveLim.x()));
    maxPixel    = round(LR.waveHorizA + (LR.waveHorizB * waveLim.y()));
    deltaHoriz  = abs(minPixel-maxPixel);
    qDebug() << "Pixel Horiz range: " << minPixel << " to " << maxPixel;

    //Vertical number of pixels
    minPixel    = round(LR.waveVertA + (LR.waveVertB * waveLim.x()));
    maxPixel    = round(LR.waveVertA + (LR.waveVertB * waveLim.y()));
    deltaVert   = abs(minPixel-maxPixel);
    qDebug() << "Pixel Vert range: " << minPixel << " to " << maxPixel;

    qDebug() << "Wave range: " << waveLim.x() << " to " << waveLim.y();


    //It concludes
    waveRange = fabs(waveLim.x()-waveLim.y());
    numPixels = (deltaHoriz < deltaVert)?deltaHoriz:deltaVert;

    results.setX((float)(numPixels+1));
    results.setY(waveRange/(float)numPixels);

    return results;
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

void genCalibXML::on_pbClearCalib_clicked()
{
    if( funcShowMsgYesNo("Alert!","Are you shure to delete calibration files?") )
    {
        funcClearDirFolder(_PATH_CALIBRATION);
        funcClearDirFolder(_PATH_CALIBRATION_RESPONCES);
        funcClearDirFolder(_PATH_CALIBRATION_IMAGES);

        QDir dir(_PATH_CALIBRATION);
        dir.mkdir(_PATH_CALIBRATION_RESPONCES);
        dir.mkdir(_PATH_CALIBRATION_IMAGES);
        this->close();
    }
}
