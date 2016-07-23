#include "validatecalibration.h"
#include "ui_validatecalibration.h"

#include <__common.h>
#include <lstStructs.h>
#include <QDesktopWidget>
#include <QGridLayout>


validateCalibration::validateCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::validateCalibration)
{
    ui->setupUi(this);
    int offset;
    offset = 1000;

    //Set screem geometry
    QPixmap tmpPix(_PATH_DISPLAY_IMAGE);
    int screen2Work = (QApplication::desktop()->screenCount()>1)?1:-1;
    QRect screen = QApplication::desktop()->screenGeometry(screen2Work);
    int gvW = (tmpPix.width()<screen.width())?tmpPix.width():screen.width();
    int gvH = (tmpPix.height()<screen.height())?tmpPix.height():screen.height();
    this->setGeometry(QRect(0,0,gvW,gvH));

    //Fill image
    GraphicsView *gvValCal = new GraphicsView(this);
    QGraphicsScene *scene = new QGraphicsScene(0,0,tmpPix.width(),tmpPix.height());
    scene->setBackgroundBrush(tmpPix);
    gvValCal->setScene(scene);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(gvValCal,0,offset);
    this->setLayout(layout);
    this->update();

    //Listen mouse events
    //connect( gvValCal, SIGNAL(signalMouseReleased(QMouseEvent)), this, SLOT(mousePresed(QMouseEvent)) );
    gvValCal->funcShowWavelenLines(1);


}

validateCalibration::~validateCalibration()
{
    delete ui;
}
