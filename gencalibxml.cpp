#include "gencalibxml.h"
#include "ui_gencalibxml.h"

genCalibXML::genCalibXML(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::genCalibXML)
{
    ui->setupUi(this);
}

genCalibXML::~genCalibXML()
{
    delete ui;
}
