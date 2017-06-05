#include "formsquareaperturesettings.h"
#include "ui_formsquareaperturesettings.h"
#include <__common.h>
#include <lstpaths.h>

formSquareApertureSettings::formSquareApertureSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formSquareApertureSettings)
{
    ui->setupUi(this);

    QString tmpParam;

    tmpParam = readFileParam(_PATH_SETTINGS_RED_WAVELEN);
    ui->doubleSpinBoxRed->setValue(tmpParam.toDouble(0));

    tmpParam = readFileParam(_PATH_SETTINGS_GREEN_WAVELEN);
    ui->doubleSpinBoxGreen->setValue(tmpParam.toDouble(0));

    tmpParam = readFileParam(_PATH_SETTINGS_BLUE_WAVELEN);
    ui->doubleSpinBoxBlue->setValue(tmpParam.toDouble(0));

    tmpParam = readFileParam(_PATH_SETTINGS_EM_ITERATIONS);
    ui->spinBoxEMIterations->setValue(tmpParam.toInt(0));

}

formSquareApertureSettings::~formSquareApertureSettings()
{
    delete ui;
}

void formSquareApertureSettings::on_buttonBox_accepted()
{
    if( funcShowMsgYesNo("Alert!","Overwrite wavelenghts?") )
    {
        saveFile(_PATH_SETTINGS_RED_WAVELEN,QString::number(ui->doubleSpinBoxRed->value()));
        saveFile(_PATH_SETTINGS_GREEN_WAVELEN,QString::number(ui->doubleSpinBoxGreen->value()));
        saveFile(_PATH_SETTINGS_BLUE_WAVELEN,QString::number(ui->doubleSpinBoxBlue->value()));
        saveFile(_PATH_SETTINGS_EM_ITERATIONS,QString::number(ui->spinBoxEMIterations->value()));
    }
}
