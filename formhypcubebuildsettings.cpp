#include "formhypcubebuildsettings.h"
#include "ui_formhypcubebuildsettings.h"
#include <QXmlStreamReader>

formHypcubeBuildSettings::formHypcubeBuildSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formHypcubeBuildSettings)
{
    ui->setupUi(this);

    //Refresh Last Settings
    if( fileExists( _PATH_SLIDE_EXPORTING_HYPCUBE ) )
    {
        structExportHypcubeSettings hypcubeSettings;
        if( funcReadSettings(&hypcubeSettings) == _OK )
        {
            ui->spinboxSpectralResolution->setValue(hypcubeSettings.spectralResolution);
            ui->spinboxSpatialResolution->setValue(hypcubeSettings.spatialResolution);
            ui->spinboxSpatialOverlap->setValue(hypcubeSettings.spatialOverlap);
            if( hypcubeSettings.flip == 1 )
                ui->cbFlip->setChecked(true);
            else
                ui->cbFlip->setChecked(false);
        }
    }
}

formHypcubeBuildSettings::~formHypcubeBuildSettings()
{
    delete ui;
}

void formHypcubeBuildSettings::on_pbSave_clicked()
{
    //----------------------------------------------
    //Prepare File Contain
    //----------------------------------------------
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures.clear();
    lstValues.clear();
    lstFixtures << "spectralResolution" << "spatialResolution"
                << "spatialOverlap" << "flip";
    lstValues << QString::number(ui->spinboxSpectralResolution->value())
              << QString::number(ui->spinboxSpatialResolution->value())
              << QString::number(ui->spinboxSpatialOverlap->value()) ;
    if( ui->cbFlip->isChecked() )
        lstValues << "1";
    else
        lstValues << "0";

    //----------------------------------------------
    //Save XML
    //----------------------------------------------
    if( funcSaveXML(_PATH_SLIDE_EXPORTING_HYPCUBE,&lstFixtures,&lstValues) != _OK )
    {
        funcShowMsgERROR("Saving Settings");
    }
    else
    {
        this->close();
    }
}

int formHypcubeBuildSettings::funcReadSettings(structExportHypcubeSettings* hypcubeSettings)
{
    memset(hypcubeSettings,'\0',sizeof(structExportHypcubeSettings));

    QFile *xmlFile = new QFile(_PATH_SLIDE_EXPORTING_HYPCUBE);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        funcShowMsg("ERROR","Opening _PATH_SLIDE_EXPORTING_HYPCUBE",this);
        return _ERROR;
    }
    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);


    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError())
    {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        //If token is just StartDocument - go to next
        if(token == QXmlStreamReader::StartDocument)
        {
                continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement)
        {
            if( xmlReader->name()=="spectralResolution" )
                hypcubeSettings->spectralResolution = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="spatialResolution" )
                hypcubeSettings->spatialResolution = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="spatialOverlap" )
                hypcubeSettings->spatialOverlap = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="flip" )
                hypcubeSettings->flip = xmlReader->readElementText().toInt(0);
        }
    }
    if(xmlReader->hasError())
    {
        funcShowMsg("_PATH_STARTING_SETTINGS Parse Error",xmlReader->errorString());
        xmlReader->clear();
        xmlFile->close();
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();

    return _OK;
}
