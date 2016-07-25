#include "selwathtocheck.h"
#include "ui_selwathtocheck.h"

#include <lstStructs.h>
#include <__common.h>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsLineItem>
#include <customline.h>

GraphicsView *globalGvValCal;

selWathToCheck::selWathToCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selWathToCheck)
{
    ui->setupUi(this);

    fillLabelImg(_PATH_DISPLAY_IMAGE);


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

    showGV();

    //Centroides
    if( ui->checkBoxCentroids->isChecked() )
    {
        drawAllCentoides();
    }

    //Limits
    if( ui->checkBoxLimits->isChecked() ){
        drawAllLimits();
    }




    //Listen mouse events
    //connect( gvValCal, SIGNAL(signalMouseReleased(QMouseEvent)), this, SLOT(mousePresed(QMouseEvent)) );
    //gvValCal->funcShowWavelenLines(1);
}


void selWathToCheck::drawAllLimits()
{
    drawLimit(_PATH_LIMIT_R,_RIGHT);
    drawLimit(_PATH_LIMIT_L,_LEFT);

    drawLimit(_PATH_LIMIT_U,_UP);
    drawLimit(_PATH_LIMIT_D,_DOWN);
}

void selWathToCheck::drawLimit(QString fileName, int side){
    QString limit;
    limit = readAllFile(fileName);
    if( fileIsValid(limit) < 1 )
    {
        funcShowFileError(fileIsValid(limit),fileName);
        return (void)NULL;
    }
    qreal limInf, limSup;
    limInf = limit.split(",").at(2).toInt(0);
    limSup = limit.split(",").at(0).toInt(0);
    customLine *limInfLine;
    customLine *limSupLine;
    if(side == _RIGHT || side == _LEFT)
    {
        limInfLine = new customLine(QPoint(limInf,0),QPoint(limInf,globalGvValCal->scene()->height()),QPen(Qt::blue));
        limSupLine = new customLine(QPoint(limSup,0),QPoint(limSup,globalGvValCal->scene()->height()),QPen(Qt::red));
        //QGraphicsLineItem *limInf = new QGraphicsLineItem( loc2,0,loc2,globalGvValCal->scene()->height() );
        //QGraphicsLineItem *limSup = new QGraphicsLineItem( loc1,0,loc1,globalGvValCal->scene()->height() );
    }
    if(side == _UP || side == _DOWN)
    {
        limInfLine = new customLine(QPoint(0,limInf),QPoint(globalGvValCal->scene()->width(),limInf),QPen(Qt::blue));
        limSupLine = new customLine(QPoint(0,limSup),QPoint(globalGvValCal->scene()->width(),limSup),QPen(Qt::red));
        //QGraphicsLineItem *limInf = new QGraphicsLineItem( 0,loc1,globalGvValCal->scene()->width(),loc1 );
        //QGraphicsLineItem *limSup = new QGraphicsLineItem( 0,loc2,globalGvValCal->scene()->width(),loc2 );
    }
    globalGvValCal->scene()->addItem(limInfLine);
    globalGvValCal->scene()->addItem(limSupLine);
    globalGvValCal->update();


}

void selWathToCheck::drawAllCentoides(){
    drawCentroid("source",Qt::magenta);

    drawCentroid("bR",Qt::blue);
    drawCentroid("rR",Qt::green);
    drawCentroid("gR",Qt::red);

    drawCentroid("bU",Qt::blue);
    drawCentroid("rU",Qt::green);
    drawCentroid("gU",Qt::red);

    drawCentroid("bL",Qt::blue);
    drawCentroid("rL",Qt::green);
    drawCentroid("gL",Qt::red);

    drawCentroid("bD",Qt::blue);
    drawCentroid("rD",Qt::green);
    drawCentroid("gD",Qt::red);
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
