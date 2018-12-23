#include "selwathtocheck.h"
#include "ui_selwathtocheck.h"

#include <lstStructs.h>
#include <__common.h>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsLineItem>
#include <customline.h>
#include <customrect.h>

#include <QVector2D>

//#include <gencalibxml.h>

GraphicsView *globalGvValCal;

lstDoubleAxisCalibration doubAxisCalib;
lstDoubleAxisCalibration *daCalib;

selWathToCheck::selWathToCheck(QImage* origEditImg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selWathToCheck)
{
    ui->setupUi(this);

    fillLabelImg(_PATH_DISPLAY_IMAGE);

    daCalib = &doubAxisCalib;
    funcGetCalibration(daCalib);
    //funcPrintCalibration(daCalib);


    ui->douubleSpinWave->setMinimum(daCalib->minWavelength);
    ui->douubleSpinWave->setMaximum(daCalib->maxWavelength);

    this->internOrigEditImg = origEditImg;

}

selWathToCheck::~selWathToCheck()
{
    delete ui;
}

void selWathToCheck::showSqUsable(int x, int y, int w, int h , Qt::GlobalColor color)
{
    showGV();
    customRect *tmpRect = new customRect(QPoint(x,y),QPoint(w,h),this->internOrigEditImg);
    tmpRect->parameters.W = globalGvValCal->sceneRect().width();
    tmpRect->parameters.H = globalGvValCal->sceneRect().height();
    tmpRect->setPen(QPen(color));
    globalGvValCal->scene()->addItem(tmpRect);
    globalGvValCal->update();
}

void selWathToCheck::fillLabelImg(QString imgPath){
    QPixmap tmpImgScal;
    QPixmap tmpImg(imgPath);
    tmpImg.save(_PATH_DISPLAY_IMAGE);
    tmpImgScal = tmpImg.scaledToWidth(ui->labelImg->width());
    tmpImgScal = tmpImgScal.scaledToHeight(ui->labelImg->height());
    ui->labelImg->setPixmap(tmpImgScal);
    ui->labelImg->setAlignment(Qt::AlignCenter);
    ui->labelImg->setFrameShape(QFrame::NoFrame);
    this->setWindowTitle("Validating calibration");
    this->update();
}

void selWathToCheck::on_pbCentroids_clicked()
{
    showGV();
    //Centroides
    if( ui->checkBoxCentroids->isChecked() )
    {
        //showGV();
        drawAllCentoides();
    }

    //Centroides
    if( ui->checkBoxCentroidsCalc->isChecked() )
    {
        drawAllCalculatedCentoides();
    }

    //Limits
    if( ui->checkBoxLimits->isChecked() )
    {
        //showGV();
        drawAllLimits();
    }

    //Horizontal Linear Regression
    if( ui->checkBoxHorizLR->isChecked() )
    {
        //showGV();
        drawLinearRegression(true);
    }

    //Vertical Linear Regression
    if( ui->checkBoxVertLR->isChecked() )
    {
        //showGV();
        drawLinearRegression(false);
    }

    //Show calculated limits
    if( ui->checkBoxLimitsCalculated->isChecked() ){
        //showGV();
        showLimitCalculated();
    }

    //Show wavelength simulation
    if( ui->checkBoxWavelength->isChecked() ){
        //showGV();
        showWavelengthSimulation();
    }

    /*
    //Area to calculate sensitivities
    if( ui->checkBoxSensSamples->isChecked() )
    {
        showSensitivitiesArea();
    }
    */




    //Listen mouse events
    //connect( gvValCal, SIGNAL(signalMouseReleased(QMouseEvent)), this, SLOT(mousePresed(QMouseEvent)) );
    //gvValCal->funcShowWavelenLines(1);
}

/*
void selWathToCheck::showSensitivitiesArea()
{
    QString sourceHalogen;
    QVector2D origin;
    sourceHalogen = readFileParam( _PATH_LIMIT_S );
    origin.setX( sourceHalogen.split(",").at(0).toInt(0) );
    origin.setY( sourceHalogen.split(",").at(1).toInt(0) );






}
*/

void selWathToCheck::showWavelengthSimulation()
{

    strDiffProj diffProj;
    diffProj.wavelength = ui->douubleSpinWave->value();

    //Get usable area
    //int x1, y1, w, h;
    //x1 = daCalib->squareUsableX;
    //y1 = daCalib->squareUsableY;
    //w = daCalib->squareUsableW;
    //h = daCalib->squareUsableH;

    //Display the areas usable an the correspondign reflected area for selected wavelenght
    //..
    int x, y;
    //y = daCalib->squareUsableY;

    QImage tmpImg( _PATH_DISPLAY_IMAGE );

    //Horizontales
    for(x=1;x<=daCalib->squareUsableW;x++)
    {
        //Horizontal
        diffProj.x = x;
        diffProj.y = 1;//Row 1
        calcDiffProj( &diffProj, daCalib );              
        drawDiffProj( &diffProj );
        drawDiffProjIntoImage(&tmpImg,&diffProj);

        diffProj.x = x;
        diffProj.y = daCalib->squareUsableH;//row h
        calcDiffProj( &diffProj, daCalib );
        drawDiffProj( &diffProj );
        drawDiffProjIntoImage(&tmpImg,&diffProj);

    }


    //Verticales
    //x = daCalib->squareUsableX;
    for(y=1;y<=daCalib->squareUsableH;y++)
    {
        //Horizontal
        diffProj.x = 1;//Column 1
        diffProj.y = y;
        calcDiffProj( &diffProj, daCalib );
        drawDiffProj( &diffProj );
        drawDiffProjIntoImage(&tmpImg,&diffProj);

        diffProj.x = daCalib->squareUsableW;
        diffProj.y = y;
        calcDiffProj( &diffProj, daCalib );
        drawDiffProj( &diffProj );
        drawDiffProjIntoImage(&tmpImg,&diffProj);

    }

    tmpImg.save(_PATH_AUX_IMG);

    globalGvValCal->update();
}

/*
void selWathToCheck::calcDiffProj(strDiffProj *diffProj)
{
    int offsetX, offsetY;
    int origX, origY;

    origX   = diffProj->x;
    origY   = diffProj->y;
    offsetX = abs( daCalib->squareUsableX - origX );
    offsetY = abs( daCalib->squareUsableY - origY );

    //It calculates the jump
    int jumpX, jumpY;
    jumpX = floor(daCalib->LR.waveHorizA + (daCalib->LR.waveHorizB * diffProj->wavelength));
    jumpY = floor(daCalib->LR.waveVertA + (daCalib->LR.waveVertB * diffProj->wavelength));

    //Right
    diffProj->ry = floor(daCalib->LR.horizA + (daCalib->LR.horizB * (double)(origX + jumpX))) + offsetY;

    //Left
    diffProj->ly = floor(daCalib->LR.horizA + (daCalib->LR.horizB * (double)(origX - jumpX))) + offsetY;

    //Up
    diffProj->ux = floor( daCalib->LR.vertA + ( daCalib->LR.vertB * (double)(origY-jumpY)) ) + offsetX;

    //Down
    diffProj->dx = floor( daCalib->LR.vertA + ( daCalib->LR.vertB * (double)(origY+jumpY)) ) + offsetX;

    //Fits the original "y"
    diffProj->y  = floor(daCalib->LR.horizA + (daCalib->LR.horizB * (double)origX)) + offsetY;
    diffProj->x  = floor(daCalib->LR.vertA + (daCalib->LR.vertB * (double)origY)) + offsetX;

    diffProj->rx = diffProj->x + jumpX;
    diffProj->lx = diffProj->x - jumpX;
    diffProj->uy = diffProj->y - jumpY;
    diffProj->dy = diffProj->y + jumpY;

}
*/

void selWathToCheck::drawDiffProjIntoImage(QImage *img, strDiffProj *diffProj)
{
    //Zero
    img->setPixelColor(diffProj->x,diffProj->y,Qt::magenta);

    //Right
    img->setPixelColor(diffProj->rx,diffProj->ry,Qt::red);

    //Up
    img->setPixelColor(diffProj->ux,diffProj->uy,Qt::blue);

    //Left
    img->setPixelColor(diffProj->lx,diffProj->ly,Qt::green);

    //Down
    img->setPixelColor(diffProj->dx,diffProj->dy,Qt::yellow);

}


void selWathToCheck::drawDiffProj(strDiffProj *diffProj)
{
    //Zero
    QGraphicsEllipseItem *zeroPoint = new QGraphicsEllipseItem(diffProj->x,diffProj->y,1,1);
    zeroPoint->setPen(QPen(Qt::cyan));
    globalGvValCal->scene()->addItem(zeroPoint);

    //Right
    QGraphicsEllipseItem *rightPoint = new QGraphicsEllipseItem(diffProj->rx,diffProj->ry,1,1);
    rightPoint->setPen(QPen(Qt::red));
    globalGvValCal->scene()->addItem(rightPoint);

    //Left
    QGraphicsEllipseItem *leftPoint = new QGraphicsEllipseItem(diffProj->lx,diffProj->ly,1,1);
    leftPoint->setPen(QPen(Qt::green));
    globalGvValCal->scene()->addItem(leftPoint);

    //Up
    QGraphicsEllipseItem *upPoint = new QGraphicsEllipseItem(diffProj->ux,diffProj->uy,1,1);
    upPoint->setPen(QPen(Qt::blue));
    globalGvValCal->scene()->addItem(upPoint);

    //Down
    QGraphicsEllipseItem *downPoint = new QGraphicsEllipseItem(diffProj->dx,diffProj->dy,1,1);
    downPoint->setPen(QPen(Qt::yellow));
    globalGvValCal->scene()->addItem(downPoint);

}


void selWathToCheck::showLimitCalculated()
{
    //Show wavelength limits
    //..
    //QString msg;
    //msg = QString::number(daCalib->minWavelength) + "nm to " + QString::number(daCalib->maxWavelength) + "nm";
    //funcShowMsg("Spectral range", msg);

    //Prepare to display borders as lines
    //..
    QString source;
    customLine *minLine;
    customLine *maxLine;
    int x, y, sX, sY, min, max, W, H;
    W = globalGvValCal->scene()->width();
    H = globalGvValCal->scene()->height();
    min = round(daCalib->LR.waveHorizA +( daCalib->LR.waveHorizB * daCalib->minWavelength ));
    max = round(daCalib->LR.waveHorizA +( daCalib->LR.waveHorizB * daCalib->maxWavelength ));
    source = readAllFile(_PATH_LIMIT_S_HALOGEN);
    sX = source.split(",").at(0).toInt(0);
    sY = source.split(",").at(1).toInt(0);

    //Right
    x = sX+min;
    minLine = new customLine(QPoint(x,0),QPoint(x,H),QPen(Qt::blue));
    minLine->setToolTip( QString::number(daCalib->minWavelength) + "nm" );
    x = sX+max;
    maxLine = new customLine(QPoint(x,0),QPoint(x,W),QPen(Qt::red));
    maxLine->setToolTip( QString::number(daCalib->maxWavelength) + "nm" );
    globalGvValCal->scene()->addItem(minLine);
    globalGvValCal->scene()->addItem(maxLine);

    //Left
    x = sX-min;
    minLine = new customLine(QPoint(x,0),QPoint(x,H),QPen(Qt::blue));
    minLine->setToolTip( QString::number(daCalib->minWavelength) + "nm" );
    x = sX-max;
    maxLine = new customLine(QPoint(x,0),QPoint(x,W),QPen(Qt::red));
    maxLine->setToolTip( QString::number(daCalib->maxWavelength) + "nm" );
    globalGvValCal->scene()->addItem(minLine);
    globalGvValCal->scene()->addItem(maxLine);

    //Up
    min = round(daCalib->LR.waveVertA +( daCalib->LR.waveVertB * daCalib->minWavelength ));
    max = round(daCalib->LR.waveVertA +( daCalib->LR.waveVertB * daCalib->maxWavelength ));
    y = sY-min;
    minLine = new customLine(QPoint(0,y),QPoint(W,y),QPen(Qt::blue));
    minLine->setToolTip( QString::number(daCalib->minWavelength) + "nm" );
    y = sY-max;
    maxLine = new customLine(QPoint(0,y),QPoint(W,y),QPen(Qt::red));
    maxLine->setToolTip( QString::number(daCalib->maxWavelength) + "nm" );
    globalGvValCal->scene()->addItem(minLine);
    globalGvValCal->scene()->addItem(maxLine);

    //Down
    y = sY+min;
    minLine = new customLine(QPoint(0,y),QPoint(W,y),QPen(Qt::blue));
    minLine->setToolTip( QString::number(daCalib->minWavelength) + "nm" );
    y = sY+max;
    maxLine = new customLine(QPoint(0,y),QPoint(W,y),QPen(Qt::red));
    maxLine->setToolTip( QString::number(daCalib->maxWavelength) + "nm" );
    globalGvValCal->scene()->addItem(minLine);
    globalGvValCal->scene()->addItem(maxLine);

    QImage img(_PATH_DISPLAY_IMAGE);
    drawCentroid(_PATH_LIMIT_S_HALOGEN,Qt::magenta,&img);


}


void selWathToCheck::drawLinearRegression(bool horizontal){

    //genCalibXML tmpGenCal;

    //lstCalibFileNames calibPoints = tmpGenCal.fillLstCalibPoints();
    //strAllLinReg linRegRes = tmpGenCal.calcAllLinReg(&calibPoints);

    double a, b;
    if( horizontal )
    {
        a = daCalib->LR.horizA;
        b = daCalib->LR.horizB;
        int y;
        for(int x=1; x<globalGvValCal->scene()->width(); x++)
        {
           y = floor( a + (b*x) );
           globalGvValCal->scene()->addEllipse((qreal)x,(qreal)y,1,1,QPen(Qt::magenta));
        }
    }
    else
    {
        a = daCalib->LR.vertA;
        b = daCalib->LR.vertB;
        int x;
        for(int y=1; y<globalGvValCal->scene()->height(); y++)
        {
           x = floor( a + (b*y) );
           globalGvValCal->scene()->addEllipse((qreal)x,(qreal)y,1,1,QPen(Qt::cyan));
        }
    }
}


void selWathToCheck::drawAllLimits()
{
    drawLimit(_RIGHT);
    drawLimit(_LEFT);

    drawLimit(_ABOVE);
    drawLimit(_DOWN);

}

void selWathToCheck::drawLimit(int side){

    QString limSource = readAllFile(_PATH_LIMIT_S_HALOGEN);
    qreal realBluePos, realRedPos;
    QString limit;
    switch(side)
    {
        case _RIGHT:
            limit = readAllFile(_PATH_LIMIT_R);
            break;
        case _ABOVE:
            limit = readAllFile(_PATH_LIMIT_U);
            break;
        case _LEFT:
            limit = readAllFile(_PATH_LIMIT_L);
            break;
        case _DOWN:
            limit = readAllFile(_PATH_LIMIT_D);
            break;
    }
    realBluePos = limit.split(",").at(2).toInt();//Blue
    realRedPos  = limit.split(",").at(0).toInt();//Red

    customLine *limInfLine;
    customLine *limSupLine;
    int len2Blue, len2Red;
    double wavelenghtInf, wavelenghtSup;
    //strAllLinReg calibLR = tmpGenCal.getAllLR();
    if(side == _RIGHT || side == _LEFT)
    {
        len2Blue        = fabs((float)limSource.split(",").at(0).toInt() - realBluePos); //HalogenX - distanceBlue
        len2Red         = fabs((float)limSource.split(",").at(0).toInt() - realRedPos);  //HalogenX - distanceRed
        limInfLine      = new customLine(QPoint(realBluePos,0),QPoint(realBluePos,globalGvValCal->scene()->height()),QPen(Qt::blue));
        limSupLine      = new customLine(QPoint(realRedPos,0),QPoint(realRedPos,globalGvValCal->scene()->height()),QPen(Qt::red));
        wavelenghtInf   = daCalib->LR.deltaHorizA + (daCalib->LR.deltaHorizB * (double)len2Blue);
        wavelenghtSup   = daCalib->LR.deltaHorizA + (daCalib->LR.deltaHorizB * (double)len2Red);
        limInfLine->setToolTip(QString::number(wavelenghtInf) + "nm");
        limSupLine->setToolTip(QString::number(wavelenghtSup) + "nm");
    }
    if(side == _ABOVE || side == _DOWN)
    {
        int halogenSource_Y;
        halogenSource_Y = limSource.split(",").at(1).toInt();
        len2Blue        = fabs((float)halogenSource_Y - realBluePos); //HalogenY - distanceBlue
        len2Red         = fabs((float)halogenSource_Y - realRedPos);  //HalogenY - distanceRed
        limInfLine      = new customLine(QPoint(0,realBluePos),QPoint(globalGvValCal->scene()->width(),realBluePos),QPen(Qt::blue));
        limSupLine      = new customLine(QPoint(0,realRedPos),QPoint(globalGvValCal->scene()->width(),realRedPos),QPen(Qt::red));
        wavelenghtInf   = daCalib->LR.deltaVertA + (daCalib->LR.deltaVertB * (double)len2Blue);
        wavelenghtSup   = daCalib->LR.deltaVertA + (daCalib->LR.deltaVertB * (double)len2Red);
        limInfLine->setToolTip(QString::number(wavelenghtInf) + "nm");
        limSupLine->setToolTip(QString::number(wavelenghtSup) + "nm");


        //qDebug() << "side: " << side;
        //qDebug() << "halogenSource_Y: " << halogenSource_Y;
        //qDebug() << "realBluePos: " << realBluePos;
        //qDebug() << "realRedPos: " << realRedPos;
        //qDebug() << "len2Blue: " << len2Blue;
        //qDebug() << "len2Red: " << len2Red;
        //qDebug() << "limInfLine: " << limInfLine;
        //qDebug() << "limSupLine: " << limSupLine;
        //exit(0);



    }

    //qDebug() << "side: Right";
    //qDebug() << "hS_x: " << limSource.split(",").at(0).toInt() << "hS_y: " << limSource.split(",").at(1).toInt() ;
    //qDebug() << "realBluePos: " << realBluePos << "realRedPos: " << realRedPos;
    //qDebug() << "len2Blue: " << len2Blue << "len2Red: " << len2Red;
    //qDebug() << "wavelenghtInf: " << wavelenghtInf << "wavelenghtSup: " << wavelenghtSup;
    //exit(0);

    globalGvValCal->scene()->addItem(limInfLine);
    globalGvValCal->scene()->addItem(limSupLine);
    globalGvValCal->update();


}

void selWathToCheck::drawAllCentoides(){
    QImage img( _PATH_DISPLAY_IMAGE );
    if( funcShowMsgYesNo("Alert","Draw halogen?") )
    {
        drawCentroid("sourceHalogen",Qt::yellow,&img);
    }
    else
    {
        drawCentroid("source",Qt::magenta,&img);

        drawCentroid("bR",Qt::blue,&img);
        drawCentroid("gR",Qt::green,&img);
        drawCentroid("rR",Qt::red,&img);

        drawCentroid("bU",Qt::blue,&img);
        drawCentroid("gU",Qt::green,&img);
        drawCentroid("rU",Qt::red,&img);

        drawCentroid("bL",Qt::blue,&img);
        drawCentroid("gL",Qt::green,&img);
        drawCentroid("rL",Qt::red,&img);

        drawCentroid("bD",Qt::blue,&img);
        drawCentroid("gD",Qt::green,&img);
        drawCentroid("rD",Qt::red,&img);
    }
    img.save(_PATH_AUX_IMG);
}

void selWathToCheck::drawAllCalculatedCentoides()
{
    //---------------------------------------------
    //Get the calibration parameters
    //---------------------------------------------
    strDiffProj diffProj;
    funcGetCalibration(daCalib);
    double wavelengthRed    = readFileParam(_PATH_SETTINGS_RED_WAVELEN).toDouble();
    double wavelengthGreen  = readFileParam(_PATH_SETTINGS_GREEN_WAVELEN).toDouble();
    double wavelengthBlue   = readFileParam(_PATH_SETTINGS_BLUE_WAVELEN).toDouble();

    //---------------------------------------------
    //Get the Fluorescent centroid
    //---------------------------------------------
    QString fileContain;
    fileContain = readAllFile(_PATH_LIMIT_S_FLUORESCENT);
    if( fileContain.contains(_ERROR_FILE_NOTEXISTS) || fileContain.contains(_ERROR_FILE) ){
        qDebug() << "ERROR LOADING: _PATH_LIMIT_S_FLUORESCENT" << fileContain;
        return (void)NULL;
    }
    int sourceX, sourceY;
    sourceX = fileContain.split(",").at(0).toInt() - daCalib->squareUsableX;
    sourceY = fileContain.split(",").at(1).toInt() - daCalib->squareUsableY;
    //sourceX = sourceX - daCalib->squareUsableX;
    //sourceY = sourceY - daCalib->squareUsableY;

    //---------------------------------------------
    //Draw the Source Centroid
    //---------------------------------------------
    diffProj.x          = sourceX;
    diffProj.y          = sourceY;
    calcDiffProj( &diffProj, daCalib );
    QImage img( _PATH_DISPLAY_IMAGE );
    drawCentroid(diffProj.x,diffProj.y,Qt::magenta,&img);


    //---------------------------------------------
    //Draw centroid calculated
    //---------------------------------------------
    //Blue
    diffProj.x          = sourceX;
    diffProj.y          = sourceY;
    diffProj.wavelength = static_cast<float>(wavelengthRed);
    calcDiffProj( &diffProj, daCalib );
    drawCentroid(diffProj.rx,diffProj.ry,Qt::red,&img);
    drawCentroid(diffProj.ux,diffProj.uy,Qt::red,&img);
    drawCentroid(diffProj.lx,diffProj.ly,Qt::red,&img);
    drawCentroid(diffProj.dx,diffProj.dy,Qt::red,&img);

    //Green
    diffProj.x          = sourceX;
    diffProj.y          = sourceY;
    diffProj.wavelength = static_cast<float>(wavelengthGreen);
    calcDiffProj( &diffProj, daCalib );
    drawCentroid(diffProj.rx,diffProj.ry,Qt::green,&img);
    drawCentroid(diffProj.ux,diffProj.uy,Qt::green,&img);
    drawCentroid(diffProj.lx,diffProj.ly,Qt::green,&img);
    drawCentroid(diffProj.dx,diffProj.dy,Qt::green,&img);

    //Red
    diffProj.x              = sourceX;
    diffProj.y              = sourceY;
    diffProj.wavelength     = static_cast<float>(wavelengthBlue);
    calcDiffProj( &diffProj, daCalib );
    drawCentroid(diffProj.rx,diffProj.ry,Qt::blue,&img);
    drawCentroid(diffProj.ux,diffProj.uy,Qt::blue,&img);
    drawCentroid(diffProj.lx,diffProj.ly,Qt::blue,&img);
    drawCentroid(diffProj.dx,diffProj.dy,Qt::blue,&img);


    /*
    diffProj.wavelength =
    calcDiffProj( &diffProj, daCalib );*/


}


void selWathToCheck::drawCentroid(QString file, Qt::GlobalColor color, QImage *img)
{
    const int len = 15;

    QString fileContain;
    if(file == _PATH_LIMIT_S_HALOGEN)
    {
        fileContain = readAllFile(_PATH_LIMIT_S_HALOGEN);
    }
    else
    {
        fileContain = readAllFile(_PATH_CALIB + file + ".hypcam");
    }
    if( fileContain.contains(_ERROR_FILE_NOTEXISTS) || fileContain.contains(_ERROR_FILE) ){
        qDebug() << fileContain;
        return (void)NULL;
    }

    int x, y;
    x = fileContain.split(",").at(0).toInt(0);
    y = fileContain.split(",").at(1).toInt(0);
    QGraphicsLineItem *horLine = new QGraphicsLineItem(
                                                            (qreal)(x-len),
                                                            (qreal)y,
                                                            (qreal)(x+len),
                                                            (qreal)y
                                                      );
    QGraphicsLineItem *verLine = new QGraphicsLineItem(
                                                            (qreal)x,
                                                            (qreal)(y-len),
                                                            (qreal)x,
                                                            (qreal)(y+len)
                                                      );
    horLine->setPen(QPen(color));
    verLine->setPen(QPen(color));
    globalGvValCal->scene()->addItem(horLine);
    globalGvValCal->scene()->addItem(verLine);
    globalGvValCal->update();

    //Draw int real image
    int c, r;
    for( r=y-len; r<y+len; r++ )
        img->setPixelColor(x,r,color);
    for( c=x-len; c<x+len; c++ )
        img->setPixelColor(c,y,color);

}


void selWathToCheck::drawCentroid(int x, int y, Qt::GlobalColor color, QImage *img)
{
    const int len = 15;

    QGraphicsLineItem *horLine = new QGraphicsLineItem(
                                                            (qreal)(x-len),
                                                            (qreal)y,
                                                            (qreal)(x+len),
                                                            (qreal)y
                                                      );
    QGraphicsLineItem *verLine = new QGraphicsLineItem(
                                                            (qreal)x,
                                                            (qreal)(y-len),
                                                            (qreal)x,
                                                            (qreal)(y+len)
                                                      );
    horLine->setPen(QPen(color));
    verLine->setPen(QPen(color));
    globalGvValCal->scene()->addItem(horLine);
    globalGvValCal->scene()->addItem(verLine);
    globalGvValCal->update();

    //Draw int real image
    int c, r;
    for( r=y-len; r<y+len; r++ )
        img->setPixelColor(x,r,color);
    for( c=x-len; c<x+len; c++ )
        img->setPixelColor(c,y,color);

}

void selWathToCheck::showGV()
{
    //Set screem geometry
    QPixmap tmpPix(_PATH_DISPLAY_IMAGE);
    int screen2Work = (QApplication::desktop()->screenCount()>1)?1:-1;
    QRect screen = QApplication::desktop()->screenGeometry(screen2Work);
    int gvW = (tmpPix.width()<screen.width())?tmpPix.width():screen.width();
    int gvH = (tmpPix.height()<screen.height())?tmpPix.height():screen.height();

    //Fill image
    GraphicsView *gvValCal = new GraphicsView(this);
    globalGvValCal = gvValCal;
    QGraphicsScene *scene = new QGraphicsScene(0,0,tmpPix.width(),tmpPix.height());
    scene->setBackgroundBrush(tmpPix);
    gvValCal->setScene(scene);
    gvValCal->setGeometry(QRect(0,0,gvW,gvH));
    gvValCal->update();
    gvValCal->show();
}

void selWathToCheck::on_pbLoadImage_clicked()
{
    QString fileSelected;
    fileSelected = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select image..."),
                                                        "./snapshots/Calib/",
                                                        "(*.ppm *.RGB888 *.tif *.png *.jpg, *.jpeg);;"
                                                     );
    if( fileSelected.isEmpty() )
    {
        return (void)NULL;
    }

    fillLabelImg(fileSelected);


}
