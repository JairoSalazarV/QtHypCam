#include "slidehypcam.h"
#include "ui_slidehypcam.h"

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <slidehypcam.h>

QSerialPort* serial;



slideHypCam::slideHypCam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::slideHypCam)
{
    ui->setupUi(this);

    //
    //Connect to the serial port
    //
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &slideHypCam::readSerialPort);
    serial->setBaudRate(9600);
    serial->setPortName("ttyUSB0");
    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Connected";
    } else {
        qDebug() << "Error: " << serial->errorString();
    }

    //
    //Send test
    //
    serial->write("2");



}

slideHypCam::~slideHypCam()
{
    delete ui;
}

void slideHypCam::on_pbSendSerial_clicked()
{
    //
    //Close Serial Port
    //
    if (serial->isOpen())
        serial->close();


}

void slideHypCam::readSerialPort()
{
    QByteArray data = serial->readAll();
    qDebug() << QString(data);
}
