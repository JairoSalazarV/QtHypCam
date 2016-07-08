#include "gencalibxml.h"
#include "ui_gencalibxml.h"
#include <__common.h>
#include <QFileDialog>

genCalibXML::genCalibXML(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::genCalibXML)
{
    ui->setupUi(this);
    autoLoadCentroids();
}

genCalibXML::~genCalibXML()
{
    delete ui;
}

void genCalibXML::funcGetFilePath(QPushButton *tmpPb){
    QString auxQstring;
    auxQstring = QFileDialog::getOpenFileName(
                                                    this,
                                                    tr("Select setting..."),
                                                    "./settings/Calib/",
                                                    "(*.hypcam);;"
                                             );
    if( !auxQstring.trimmed().isEmpty() ){
        setButton(tmpPb,auxQstring);
    }
}

void genCalibXML::autoLoadCentroids(){
    //Source
    setButton(ui->pbSource,"./settings/Calib/source.hypcam",false);
    //Blues
    setButton(ui->pbBlueRightDown,"./settings/Calib/bRD.hypcam",false);
    setButton(ui->pbBlueRight,"./settings/Calib/bR.hypcam",false);
    setButton(ui->pbBlueRightUp,"./settings/Calib/bRU.hypcam",false);
    setButton(ui->pbBlueUp,"./settings/Calib/bU.hypcam",false);
    setButton(ui->pbBlueLeftUp,"./settings/Calib/bLU.hypcam",false);
    setButton(ui->pbBlueLeft,"./settings/Calib/bL.hypcam",false);
    setButton(ui->pbBlueLeftDown,"./settings/Calib/bLD.hypcam",false);
    setButton(ui->pbBlueDown,"./settings/Calib/bD.hypcam",false);
    //Greens
    setButton(ui->pbGreenRightDown,"./settings/Calib/gRD.hypcam",false);
    setButton(ui->pbGreenRight,"./settings/Calib/gR.hypcam",false);
    setButton(ui->pbGreenRightUp,"./settings/Calib/gRU.hypcam",false);
    setButton(ui->pbGreenUp,"./settings/Calib/gU.hypcam",false);
    setButton(ui->pbGreenLeftUp,"./settings/Calib/gLU.hypcam",false);
    setButton(ui->pbGreenLeft,"./settings/Calib/gL.hypcam",false);
    setButton(ui->pbGreenLeftDown,"./settings/Calib/gLD.hypcam",false);
    setButton(ui->pbGreenDown,"./settings/Calib/gD.hypcam",false);
    //Reds
    setButton(ui->pbRedRightDown,"./settings/Calib/rRD.hypcam",false);
    setButton(ui->pbRedRight,"./settings/Calib/rR.hypcam",false);
    setButton(ui->pbRedRightUp,"./settings/Calib/rRU.hypcam",false);
    setButton(ui->pbRedUp,"./settings/Calib/rU.hypcam",false);
    setButton(ui->pbRedLeftUp,"./settings/Calib/rLU.hypcam",false);
    setButton(ui->pbRedLeft,"./settings/Calib/rL.hypcam",false);
    setButton(ui->pbRedLeftDown,"./settings/Calib/rLD.hypcam",false);
    setButton(ui->pbRedDown,"./settings/Calib/rD.hypcam",false);
}

void genCalibXML::setButton(QPushButton *tmpPb, QString tip, bool doAlert){
    QFile tmpfile(tip);
    if( !tmpfile.exists() ){
        if( doAlert ){
            funcShowMsg("Alert","File does not exist: "+tip);
        }
        return (void)NULL;
    }
    tmpPb->setToolTip(tip);
    tmpPb->setStyleSheet("width:27px;background-color:#000;color:#FFF;");
}


void genCalibXML::on_pbSource_clicked()
{
    funcGetFilePath(ui->pbSource);
}

void genCalibXML::on_pbBlueRight_clicked()
{
    funcGetFilePath(ui->pbBlueRight);
}

void genCalibXML::on_pbGreenRight_clicked()
{
    funcGetFilePath(ui->pbGreenRight);
}

void genCalibXML::on_pbRedRight_clicked()
{
    funcGetFilePath(ui->pbRedRight);
}

void genCalibXML::on_pbBlueRightUp_clicked()
{
    funcGetFilePath(ui->pbBlueRightUp);
}

void genCalibXML::on_pbGreenRightUp_clicked()
{
    funcGetFilePath(ui->pbGreenRightUp);
}

void genCalibXML::on_pbRedRightUp_clicked()
{
    funcGetFilePath(ui->pbRedRightUp);
}

void genCalibXML::on_pbBlueUp_clicked()
{
    funcGetFilePath(ui->pbBlueUp);
}

void genCalibXML::on_pbGreenUp_clicked()
{
    funcGetFilePath(ui->pbGreenUp);
}

void genCalibXML::on_pbRedUp_clicked()
{
    funcGetFilePath(ui->pbRedUp);
}

void genCalibXML::on_pbBlueLeftUp_clicked()
{
    funcGetFilePath(ui->pbBlueLeftUp);
}

void genCalibXML::on_pbGreenLeftUp_clicked()
{
    funcGetFilePath(ui->pbGreenLeftUp);
}

void genCalibXML::on_pbRedLeftUp_clicked()
{
    funcGetFilePath(ui->pbRedLeftUp);
}

void genCalibXML::on_pbBlueLeft_clicked()
{
    funcGetFilePath(ui->pbBlueLeft);
}

void genCalibXML::on_pbGreenLeft_clicked()
{
    funcGetFilePath(ui->pbGreenLeft);
}

void genCalibXML::on_pbRedLeft_clicked()
{
    funcGetFilePath(ui->pbRedLeft);
}

void genCalibXML::on_pbBlueLeftDown_clicked()
{
    funcGetFilePath(ui->pbBlueLeftDown);
}

void genCalibXML::on_pbGreenLeftDown_clicked()
{
    funcGetFilePath(ui->pbGreenLeftDown);
}

void genCalibXML::on_pbRedLeftDown_clicked()
{
    funcGetFilePath(ui->pbRedLeftDown);
}

void genCalibXML::on_pbBlueDown_clicked()
{
    funcGetFilePath(ui->pbBlueDown);
}

void genCalibXML::on_pbRedDown_clicked()
{
    funcGetFilePath(ui->pbRedDown);
}

void genCalibXML::on_pbGreenDown_clicked()
{
    funcGetFilePath(ui->pbGreenDown);
}

void genCalibXML::on_pbBlueRightDown_clicked()
{
    funcGetFilePath(ui->pbBlueRightDown);
}

void genCalibXML::on_pbGreenRightDown_clicked()
{
    funcGetFilePath(ui->pbGreenRightDown);
}

void genCalibXML::on_pbRedRightDown_clicked()
{
    funcGetFilePath(ui->pbRedRightDown);
}
