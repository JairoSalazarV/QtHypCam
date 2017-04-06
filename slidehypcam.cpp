#include "slidehypcam.h"
#include "ui_slidehypcam.h"

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <slidehypcam.h>
#include <__common.h>

#include <mainwindow.h>
#include <rasphypcam.h>

#include <arduinomotor.h>

#include <QThread>

#include <formslidesettings.h>

slideHypCam::slideHypCam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::slideHypCam)
{
    ui->setupUi(this);

    //motor = new arduinoMotorSerial();

    //serialPortConnect();
    //motor.funcMotorGoToZero(&serialPort);
    //serialPort.close();

}

slideHypCam::~slideHypCam()
{
    delete ui;
}



void slideHypCam::on_actionpbGetSlideCube_triggered()
{
    //Instances MainWindow
    MainWindow* internMainWin       = new MainWindow(this);
    structCamSelected* camSelected  = internMainWin->funcGetCamSelected();

    //Check if camara is selected
    if( camSelected->tcpPort == 0 )
    {
        funcShowMsg("ALERT","Connect to the camera");
        return (void)NULL;
    }

    //Change mouse pointer
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //Obtains camera resolution selected on MainWindow
    camRes = internMainWin->getCamRes();

    //
    //Start motor rotation in an independen thread
    //
    arduinoMotor* motor = new arduinoMotor();
    motor->setAWalk( 0, 180, 60, 800 );
    motor->start();

    //
    //Obtain imagery and put into RAM
    //
    int numImgs;
    u_int8_t** lstImgs = internMainWin->funcGetSLIDESnapshot( &numImgs, true );
    delete[] lstImgs;
    fflush(stdout);

    //Restores mouse pointer
    QApplication::restoreOverrideCursor();

}



void slideHypCam::on_actionSlide_settings_triggered()
{
    formSlideSettings* tmpForm = new formSlideSettings(this);
    tmpForm->setModal(true);
    tmpForm->show();
}
