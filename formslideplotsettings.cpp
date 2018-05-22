#include "formslideplotsettings.h"
#include "ui_formslideplotsettings.h"
#include <__common.h>

formSlidePlotSettings::formSlidePlotSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formSlidePlotSettings)
{
    ui->setupUi(this);

    reloadPlotSettings();
}

formSlidePlotSettings::~formSlidePlotSettings()
{
    delete ui;
}

void formSlidePlotSettings::on_buttonBox_rejected()
{
    this->close();
}

void formSlidePlotSettings::on_buttonBox_accepted(bool showMsg)
{
    QList<QString> lstFields, lstValues;

    lstFields << "minWave" << "maxWave";
    lstValues << QString::number(ui->spinBoxMinWave->value())
              << QString::number(ui->spinBoxMaxWave->value());
    funcSaveXML(_PATH_SLIDE_PLOT_SETTINGS,&lstFields,&lstValues,showMsg);
}

void formSlidePlotSettings::reloadPlotSettings()
{
    QList<QString> lstFields, lstValues;
    lstFields << "minWave" << "maxWave";
    if( readXMLIntoList(_PATH_SLIDE_PLOT_SETTINGS,lstFields,&lstValues) != _OK )
    {
        on_buttonBox_accepted(false);
        return (void)false;
    }
    ui->spinBoxMinWave->setValue( lstValues.at(0).toFloat(0) );
    ui->spinBoxMaxWave->setValue( lstValues.at(1).toFloat(0) );
}
