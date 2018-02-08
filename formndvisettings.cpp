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

    //Weight
    tmpParameter = readFileParam(_PATH_NDVI_INFRARED_WEIGHT);
    if( tmpParameter.toDouble(0) > 0.0  )
        ui->spinBoxWeightRatio->setValue(tmpParameter.toDouble(0));

    //Minimum Value
    tmpParameter = readFileParam(_PATH_NDVI_MIN_VALUE);
    if( tmpParameter.toDouble(0) > 0.0 && tmpParameter.toDouble(0) <= 255.0 )
        ui->spinBoxMinimumValue->setValue(tmpParameter.toDouble(0));

    //Brilliant
    tmpParameter = readFileParam(_PATH_NDVI_BRILLIANT);
    if( tmpParameter.toInt(0) == 1 )
        ui->cbBrilliant->setChecked(true);
    else
        ui->cbBrilliant->setChecked(false);

    //Infrared
    tmpParameter = readFileParam(_PATH_NDVI_IR_CHANEL).trimmed();
    ui->comboBoxInfrared->setCurrentText(tmpParameter);

    //Red
    tmpParameter = readFileParam(_PATH_NDVI_RED_CHANEL).trimmed();
    ui->comboBoxRed->setCurrentText(tmpParameter);



}

formNDVISettings::~formNDVISettings()
{
    delete ui;
}

void formNDVISettings::on_buttonBox_accepted()
{
    QString tmpParameter;
    saveFile(_PATH_NDVI_THRESHOLD,QString::number(ui->spinBoxThreshold->value()));
    saveFile(_PATH_NDVI_INFRARED_WEIGHT,QString::number(ui->spinBoxWeightRatio->value()));
    saveFile(_PATH_NDVI_MIN_VALUE,QString::number(ui->spinBoxMinimumValue->value()));
    saveFile(_PATH_NDVI_IR_CHANEL,ui->comboBoxInfrared->itemText(ui->comboBoxInfrared->currentIndex()));
    saveFile(_PATH_NDVI_RED_CHANEL,ui->comboBoxRed->itemText(ui->comboBoxRed->currentIndex()));
    tmpParameter = (ui->cbBrilliant->isChecked())?"1":"0";
    saveFile(_PATH_NDVI_BRILLIANT,tmpParameter);
}
