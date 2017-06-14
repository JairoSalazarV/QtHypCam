#include "formndvisettings.h"
#include "ui_formndvisettings.h"

#include <__common.h>

formNDVISettings::formNDVISettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formNDVISettings)
{
    ui->setupUi(this);

    //....................................
    // Refresh parameters
    //....................................
    QString tmpParameter;
    //Threshold
    tmpParameter = readFileParam(_PATH_NDVI_THRESHOLD);
    if( tmpParameter.toDouble(0) >= -1 && tmpParameter.toDouble(0) <= 1  )
        ui->spinBoxThreshold->setValue(tmpParameter.toDouble(0));
    //Brilliant
    tmpParameter = readFileParam(_PATH_NDVI_BRILLIANT);
    if( tmpParameter.toInt(0) == 1 )
        ui->cbBrilliant->setChecked(true);
    else
        ui->cbBrilliant->setChecked(false);



}

formNDVISettings::~formNDVISettings()
{
    delete ui;
}

void formNDVISettings::on_buttonBox_accepted()
{
    QString tmpParameter;
    saveFile(_PATH_NDVI_THRESHOLD,QString::number(ui->spinBoxThreshold->value()));
    tmpParameter = (ui->cbBrilliant->isChecked())?"1":"0";
    saveFile(_PATH_NDVI_BRILLIANT,tmpParameter);
}
