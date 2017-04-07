#include "formslidesettings.h"
#include "ui_formslidesettings.h"

#include <lstStructs.h>
#include <__common.h>

strReqImg* reqImg;

formSlideSettings::formSlideSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formSlideSettings)
{
    ui->setupUi(this);


    //Fit speed, duration and other parameters
    reqImg = (strReqImg*)malloc(sizeof(strReqImg));
    if( !funGetSlideSettingsXML( _PATH_SLIDE_SETTINGS, reqImg ) ){
        funcShowMsg("ERROR","Loading Slide Settings");
        this->close();
    }
    else
    {
        ui->spinBoxIni->setValue( reqImg->slide.degreeIni );
        ui->spinBoxEnd->setValue( reqImg->slide.degreeEnd );
        ui->spinBoxJump->setValue( reqImg->slide.degreeJump );
        ui->spinBoxSpeed->setValue( reqImg->slide.speed );
    }


}

formSlideSettings::~formSlideSettings()
{
    delete ui;
}

void formSlideSettings::on_pbClose_clicked()
{
    this->close();
}

void formSlideSettings::on_pbSave_clicked()
{
    QString tmpSettings;

    tmpSettings = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
    tmpSettings.append("<Variables>\n");

    tmpSettings.append("\t<degreeIni>");
        tmpSettings.append(QString::number(ui->spinBoxIni->value()));
    tmpSettings.append("</degreeIni>\n");

    tmpSettings.append("\t<degreeEnd>");
        tmpSettings.append(QString::number(ui->spinBoxEnd->value()));
    tmpSettings.append("</degreeEnd>\n");

    tmpSettings.append("\t<degreeJump>");
        tmpSettings.append(QString::number(ui->spinBoxJump->value()));
    tmpSettings.append("</degreeJump>\n");

    tmpSettings.append("\t<speed>");
        tmpSettings.append(QString::number(ui->spinBoxSpeed->value()));
    tmpSettings.append("</speed>\n");

    tmpSettings.append("</Variables>");

    if( saveFile( _PATH_SLIDE_SETTINGS, tmpSettings ) == true )
    {
        funcShowMsg("Success","File saved");
        this->close();
    }
    else
        funcShowMsg("ERROR","Saving settings");

}
