#include "formhypercubeanalysis.h"
#include "ui_formhypercubeanalysis.h"

#include <QVBoxLayout>
#include <QMainWindow>
#include <QGraphicsLineItem>
#include <QPen>

#include <QFormLayout>
#include <formslideplotsettings.h>



QWidget *parentHypercubeAnalysis;

formHypercubeAnalysis::formHypercubeAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formHypercubeAnalysis)
{    
    ui->setupUi(this);

    ui->progressBar->setVisible(false);

    parentHypercubeAnalysis = qobject_cast<QMainWindow*>(parent);

    ui->slideTmpImg->setEnabled(false);

}

formHypercubeAnalysis::~formHypercubeAnalysis()
{
    delete ui;
}

/*
void formHypercubeAnalysis::refreshGVGeometry()
{
    //Calc Coordinates Sizes
    int tmpW, tmpH, frameW, frameH;
    QRect screenRes = screenResolution(parentHypercubeAnalysis);
    frameW  = 30;
    frameH  = 80;
    tmpW    = screenRes.width() - frameW;
    tmpH    = round( ((float)screenRes.height() - frameH) * 0.4 );

    //Modify Geometries
    mainGV->setFixedSize(tmpW,mainGV->height());
    plotGV->setFixedSize(tmpW,tmpH);

    mainGV->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    mainGV->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    plotGV->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    plotGV->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    std::cout << "refreshGVGeometry" << std::endl;
}*/

void formHypercubeAnalysis::updateSignature(QMouseEvent* e)
{
    int cubeX, cubeY;
    cubeX = round( ((float)e->x() / (float)slideHypCube->width() ) * (float)slideHypCube->cubeParam.W );
    cubeY = round( ((float)e->y() / (float)slideHypCube->height()) * (float)slideHypCube->cubeParam.H );
    //std::cout << "coor(" << e->x() << ", " << e->y() << ")"
    //          << " cube(" << cubeX  << ", " << cubeY  << ")" << std::endl;


}

void formHypercubeAnalysis::drawnAxis()
{
    int frameW = 40;
    int frameH = 25;
    //X-Axix
    gvPlot->scene()->addLine( 0, gvPlot->height()-frameH, gvPlot->width(), gvPlot->height()-frameH, QPen(QBrush(Qt::white),2.0) );
    //Y-Axis
    gvPlot->scene()->addLine( frameW, 0, frameW, gvPlot->height(), QPen(QBrush(Qt::white),2.0) );

}


void formHypercubeAnalysis::on_slideTmpImg_valueChanged(const int &value)
{
    updateLabel(value);
    updateSlidePicture(value);
}

void formHypercubeAnalysis::updateSlidePicture(const int &l)
{
    gvImg->scene()->clear();
    gvImg->scene()->addPixmap(QPixmap::fromImage(slideHypCube->lstCubeThumbs.at(l)));
    gvImg->update();
    QtDelay(15);
}

void formHypercubeAnalysis::updateLabel(const int &value)
{
    float tmpWavelen;
    tmpWavelen = slideHypCube->cubeParam.initWavelength + ( slideHypCube->cubeParam.spectralRes * value);
    ui->labelImgNumber->setText( "Wavelength: " + QString::number(tmpWavelen) + " nm" );
}

void formHypercubeAnalysis::on_pbCubeToImg_clicked()
{
    formSlidePlotSettings* tmpForm = new formSlidePlotSettings(this);
    tmpForm->setModal(true);
    tmpForm->show();
}

void formHypercubeAnalysis::on_pbLoadCube_clicked()
{
    //------------------------------------------------
    //Load Hypercube
    //------------------------------------------------

    connect(
                slideHypCube,
                SIGNAL(signalProgBarValue(int,QString)),
                this,
                SLOT(updateProgressBar(int,QString))
           );

    //Load Hypercube
    if( slideHypCube->loadHypercube(this) != _OK )
    {
        return (void)false;
    }
    ui->slideTmpImg->setMaximum( slideHypCube->cubeParam.L-1 );
    ui->slideTmpImg->setMinimum( 0 );
    updateProgressBar(101,"");
    updateLabel(0);

    //---------------------------------------------------------
    //Create Layout and Set Image into GV
    //---------------------------------------------------------
    int frameH = 125;
    //Add to layout
    QFormLayout* tmpLayout = new QFormLayout(this);
    tmpLayout->addWidget(gvImg);
    this->setLayout(tmpLayout);
    //Put Image into GV
    QImage tmpImg = slideHypCube->slideImgFromCube();
    QGraphicsScene* imgScene = new QGraphicsScene(this);
    gvImg->setScene(imgScene);
    gvImg->scene()->addPixmap(QPixmap::fromImage(tmpImg));
    gvImg->setGeometry(QRect(0,0,tmpImg.width(),tmpImg.height()));
    gvImg->setMouseTracking(true);
    gvImg->setVisible(true);
    gvImg->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvImg->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvImg->move(10,frameH);

    //---------------------------------------------------------
    //Create Layout and Set Image into GV
    //---------------------------------------------------------
    int frameH2 = 30;
    //Add to layout
    QRect screenRes = screenResolution(parentHypercubeAnalysis);
    QFormLayout* tmpLayout2 = new QFormLayout(this);
    tmpLayout2->addWidget(gvPlot);
    this->setLayout(tmpLayout2);
    QGraphicsScene* imgScene2 = new QGraphicsScene(this);
    gvPlot->setScene(imgScene2);
    gvPlot->scene()->setBackgroundBrush(QBrush(QColor(45,45,45)));
    gvPlot->setGeometry(QRect(0,0,tmpImg.width(),round(screenRes.height()*0.45)));
    gvPlot->setMouseTracking(true);
    gvPlot->setVisible(true);
    gvPlot->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvPlot->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvPlot->move(10,frameH+frameH2+tmpImg.height());

    ui->slideTmpImg->setEnabled(true);
    drawnAxis();

    //---------------------------------------------------------
    //Connect
    //---------------------------------------------------------
    connect(
                gvImg,
                SIGNAL(signalMouseMove(QMouseEvent*)),
                this,
                SLOT(updateSignature(QMouseEvent*))
           );
    //std::cout << "t2" << std::endl;


}

void formHypercubeAnalysis::updateProgressBar(int value, QString label)
{
    if( value > 100 || value < 0 )
    {
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(false);
        ui->labelProgressBar->setText("");
        return (void)true;
    }

    if( value >= 0 && value <= 100 )
    {
        ui->labelProgressBar->setText(label);
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(value);
        ui->progressBar->update();
        if( ui->progressBar->value() != value )
        {
            QtDelay(15);
        }
    }
}

void formHypercubeAnalysis::resizeEvent( QResizeEvent * event )
{
    event = event;
    std::cout << "ResizeEvent" << std::endl;
}

