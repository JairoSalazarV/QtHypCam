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

//#include <gencalibxml.h>

GraphicsView *globalGvValCal;

lstDoubleAxisCalibration doubAxisCalib;
lstDoubleAxisCalibration *daCalib;

selWathToCheck::selWathToCheck(QWidget *parent) :
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

}

selWathToCheck::~selWathToCheck()
{
    delete ui;
}

void selWathToCheck::showSqUsable(int x, int y, int w, int h , Qt::GlobalColor color)
{
    showGV();
    customRect *tmpRect = new customRect(QPoint(x,y),QPoint(w,h));
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

    //Show limits calculated
    if( ui->checkBoxLimitsCalculated->isChecked() ){
        //showGV();
        showLimitCalculated();
    }

    //Show wavelength simulation
    if( ui->checkBoxWavelength->isChecked() ){
        //showGV();
        showWavelengthSimulation();
    }




    //Listen mouse events
    //connect( gvValCal, SIGNAL(signalMouseReleased(QMouseEvent)), this, SLOT(mousePresed(QMouseEvent)) );
    //gvValCal->funcShowWavelenLines(1);
}

void selWathToCheck::showWavelengthSimulation()
{

    strDiffProj diffProj;

    //Get usable area
    int x1, y1, w, h;
    x1 = daCalib->squareUsableX;
    y1 = daCalib->squareUsableY;
    w = daCalib->squareUsableW;
    h = daCalib->squareUsableH;

    //Display the areas usable an the correspondign reflected area for selected wavelenght
    //..
    int x, y;
    y = daCalib->squareUsableY;

    //Horizontales
    for(x=x1;x<=(x1+w);x++)
    {
        //Horizontal
        diffProj.x = x;
        diffProj.y = y;
        calcDiffProj( &diffProj );
        drawDiffProj( &diffProj );

        diffProj.x = x;
        diffProj.y = y + daCalib->squareUsableH;
        calcDiffProj( &diffProj );
        drawDiffProj( &diffProj );

    }

    //Verticales
    x = daCalib->squareUsableX;
    for(y=y1;y<=(y1+h);y++)
    {
        //Horizontal
        diffProj.x = x;
        diffProj.y = y;
        calcDiffProj( &diffProj );
        drawDiffProj( &diffProj );

        diffProj.x = x + daCalib->squareUsableW;
        diffProj.y = y;
        calcDiffProj( &diffProj );
        drawDiffProj( &diffProj );

    }





    globalGvValCal->update();
}

void selWathToCheck::calcDiffProj(strDiffProj *diffProj)
{
    double wave;
    int offsetX, offsetY;
    int origX, origY;
    wave = ui->douubleSpinWave->value();

    origX   = diffProj->x;
    origY   = diffProj->y;
    offsetX = abs( daCalib->squareUsableX - origX );
    offsetY = abs( daCalib->squareUsableY - origY );

    //It calculates the jump
    int jumpX, jumpY;
    jumpX = floor(daCalib->LR.waveHorizA + (daCalib->LR.waveHorizB * wave));
    jumpY = floor(daCalib->LR.waveVertA + (daCalib->LR.waveVertB * wave));

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
    source = readAllFile(_PATH_LIMIT_S);
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

    drawCentroid(_PATH_LIMIT_S,Qt::magenta);


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

    drawLimit(_UP);
    drawLimit(_DOWN);

}

void selWathToCheck::drawLimit(int side){

    QString limSource = readAllFile(_PATH_LIMIT_S);
    qreal limInf, limSup;
    QString limit;
    switch(side)
    {
        case _RIGHT:
            limit = readAllFile(_PATH_LIMIT_R);
            break;
        case _UP:
            limit = readAllFile(_PATH_LIMIT_U);
            break;
        case _LEFT:
            limit = readAllFile(_PATH_LIMIT_L);
            break;
        case _DOWN:
            limit = readAllFile(_PATH_LIMIT_D);
            break;
    }
    limInf = limit.split(",").at(2).toInt(0);
    limSup = limit.split(",").at(0).toInt(0);


    customLine *limInfLine;
    customLine *limSupLine;
    int len1, len2;
    double wavelenghtInf, wavelenghtSup;
    //strAllLinReg calibLR = tmpGenCal.getAllLR();
    if(side == _RIGHT || side == _LEFT)
    {
        len1            = abs(limSource.split(",").at(0).toInt(0) - limInf);
        len2            = abs(limSource.split(",").at(0).toInt(0) - limSup);        
        limInfLine      = new customLine(QPoint(limInf,0),QPoint(limInf,globalGvValCal->scene()->height()),QPen(Qt::blue));
        limSupLine      = new customLine(QPoint(limSup,0),QPoint(limSup,globalGvValCal->scene()->height()),QPen(Qt::red));
        wavelenghtInf   = daCalib->LR.deltaHorizA + (daCalib->LR.deltaHorizB * (double)len1);
        wavelenghtSup   = daCalib->LR.deltaHorizA + (daCalib->LR.deltaHorizB * (double)len2);
        limInfLine->setToolTip(QString::number(wavelenghtInf) + "nm");
        limSupLine->setToolTip(QString::number(wavelenghtSup) + "nm");
    }
    if(side == _UP || side == _DOWN)
    {
        len1            = abs(limSource.split(",").at(1).toInt(0) - limInf);
        len2            = abs(limSource.split(",").at(1).toInt(0) - limSup);
        limInfLine      = new customLine(QPoint(0,limInf),QPoint(globalGvValCal->scene()->width(),limInf),QPen(Qt::blue));
        limSupLine      = new customLine(QPoint(0,limSup),QPoint(globalGvValCal->scene()->width(),limSup),QPen(Qt::red));
        wavelenghtInf   = daCalib->LR.deltaVertA + (daCalib->LR.deltaVertB * (double)len1);
        wavelenghtSup   = daCalib->LR.deltaVertA + (daCalib->LR.deltaVertB * (double)len2);
        limInfLine->setToolTip(QString::number(wavelenghtInf) + "nm");
        limSupLine->setToolTip(QString::number(wavelenghtSup) + "nm");
    }
    globalGvValCal->scene()->addItem(limInfLine);
    globalGvValCal->scene()->addItem(limSupLine);
    globalGvValCal->update();


}

void selWathToCheck::drawAllCentoides(){
    drawCentroid("source",Qt::magenta);

    drawCentroid("bR",Qt::blue);
    drawCentroid("gR",Qt::green);
    drawCentroid("rR",Qt::red);

    drawCentroid("bU",Qt::blue);
    drawCentroid("gU",Qt::green);
    drawCentroid("rU",Qt::red);

    drawCentroid("bL",Qt::blue);
    drawCentroid("gL",Qt::green);
    drawCentroid("rL",Qt::red);

    drawCentroid("bD",Qt::blue);
    drawCentroid("gD",Qt::green);
    drawCentroid("rD",Qt::red);
}

void selWathToCheck::drawCentroid(QString file, Qt::GlobalColor color)
{
    const int len = 15;

    QString fileContain;
    if(file == _PATH_LIMIT_S)
    {
        fileContain = readAllFile(_PATH_LIMIT_S);
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
