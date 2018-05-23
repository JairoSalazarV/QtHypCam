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

void formSlidePlotSettings::on_buttonBox_accepted()
{
    QList<QString> lstFields, lstValues;

    lstFields << "minWave"  << "maxWave"
              << "XFrame"   << "YFrame"
              << "XStep"    << "YStep";
    lstValues << QString::number(ui->spinBoxMinWave->value())
              << QString::number(ui->spinBoxMaxWave->value())
              << QString::number(ui->spinBoxXFrame->value())
              << QString::number(ui->spinBoxYFrame->value())
              << QString::number(ui->spinBoxXStep->value())
              << QString::number(ui->spinBoxYStep->value());
    funcSaveXML(_PATH_SLIDE_PLOT_SETTINGS,&lstFields,&lstValues,true);
}

void formSlidePlotSettings::reloadPlotSettings()
{
    strSlidePlotSettings tmpSlideSettings;
    readPlotSettings(&tmpSlideSettings);
    ui->spinBoxMinWave->setValue( tmpSlideSettings.minWave );
    ui->spinBoxMaxWave->setValue( tmpSlideSettings.maxWave );
    ui->spinBoxXFrame->setValue ( tmpSlideSettings.XFrame );
    ui->spinBoxYFrame->setValue ( tmpSlideSettings.YFrame );
    ui->spinBoxXStep->setValue  ( tmpSlideSettings.XStep );
    ui->spinBoxYStep->setValue  ( tmpSlideSettings.YStep );
}

void formSlidePlotSettings::readPlotSettings( strSlidePlotSettings* slideSettings )
{
    //-------------------------------------------------
    //Read Slide Plot Settings
    //-------------------------------------------------
    QList<QString> lstFields, lstValues;
    lstFields << "minWave"  << "maxWave"
              << "XFrame"   << "YFrame"
              << "XStep"    << "YStep";
    if( readXMLIntoList(_PATH_SLIDE_PLOT_SETTINGS,lstFields,&lstValues) != _OK )
    {
        on_buttonBox_accepted();
        return (void)false;
    }
    int i=0;
    slideSettings->minWave  = lstValues.at(i++).toFloat(0);
    slideSettings->maxWave  = lstValues.at(i++).toFloat(0);
    slideSettings->XFrame   = lstValues.at(i++).toFloat(0);
    slideSettings->YFrame   = lstValues.at(i++).toFloat(0);
    slideSettings->XStep    = lstValues.at(i++).toFloat(0);
    slideSettings->YStep    = lstValues.at(i++).toFloat(0);

    //-------------------------------------------------
    //Calc X-Axis For Wavelength
    //-------------------------------------------------


}
