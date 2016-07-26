#include "selwathtocheck.h"
#include "ui_selwathtocheck.h"

#include <lstStructs.h>
#include <__common.h>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsLineItem>
#include <customline.h>

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

}

selWathToCheck::~selWathToCheck()
{
    delete ui;
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

    //Centroides
    if( ui->checkBoxCentroids->isChecked() )
    {
        showGV();
        drawAllCentoides();
    }

    //Limits
    if( ui->checkBoxLimits->isChecked() )
    {
        showGV();
        drawAllLimits();
    }

    //Horizontal Linear Regression
    if( ui->checkBoxHorizLR->isChecked() )
    {
        showGV();
        drawLinearRegression(true);
    }

    //Vertical Linear Regression
    if( ui->checkBoxVertLR->isChecked() )
    {
        showGV();
        drawLinearRegression(false);
    }

    //Show limits calculated
    if( ui->checkBoxLimitsCalculated->isChecked() ){
        showLimitCalculated();
    }




    //Listen mouse events
    //connect( gvValCal, SIGNAL(signalMouseReleased(QMouseEvent)), this, SLOT(mousePresed(QMouseEvent)) );
    //gvValCal->funcShowWavelenLines(1);
}

void selWathToCheck::showLimitCalculated()
{
    QString msg;
    msg = QString::number(daCalib->minWavelength) + "nm to " + QString::number(daCalib->maxWavelength) + "nm";
    funcShowMsg("Spectral range", msg);
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
    fileContain = readAllFile(_PATH_CALIB + file + ".hypcam");
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
