#include "rotationfrm.h"
#include "ui_rotationfrm.h"
#include <__common.h>

#define ROT_FACTOR 120.0

rotationFrm::rotationFrm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rotationFrm)
{
    ui->setupUi(this);
}

rotationFrm::~rotationFrm()
{
    delete ui;
}

void rotationFrm::on_pbSaveRotation_clicked()
{
    if( saveFile(_PATH_LAST_ROTATION,QString::number(ui->spinAngle->value())) ){
        funcShowMsg("Success","Rotation saved");
    }else{
        funcShowMsg("ERROR", "Saving rotation file");
    }
}

void rotationFrm::on_spinAngle_valueChanged(double arg1)
{
    emit angleChanged((float)arg1);
}

void rotationFrm::on_pbLoadLastRotation_clicked()
{
    if( funcShowMsgYesNo("Alert","Do you want to load the lastest angle?") ){
        QString lastAngFileContain = readAllFile(_PATH_LAST_ROTATION);
        lastAngFileContain.replace("\n","");
        double lastAngle = lastAngFileContain.toFloat(0);
        ui->spinAngle->setValue(lastAngle);
        on_spinAngle_valueChanged(lastAngle);
    }
}
