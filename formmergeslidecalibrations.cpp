#include "formmergeslidecalibrations.h"
#include "ui_formmergeslidecalibrations.h"
#include <__common.h>
#include <QXmlStreamReader>

formMergeSlideCalibrations::formMergeSlideCalibrations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formMergeSlideCalibrations)
{
    ui->setupUi(this);
}

formMergeSlideCalibrations::~formMergeSlideCalibrations()
{
    delete ui;
}

void formMergeSlideCalibrations::on_pbHorizontal_clicked()
{
    //--------------------------------
    //Define Video Origin
    //--------------------------------
    QString defaVideoPath;
    defaVideoPath.append(_PATH_LOCAL_SYNC_FOLDERS);
    defaVideoPath.append(_PATH_LOCAL_FOLDER_VIDEOS);
    if( funcLetUserSelectFile( &defaVideoPath ) != _OK )
    {
        return (void)false;
    }
    ui->txtHorPath->setText(defaVideoPath);
}

void formMergeSlideCalibrations::on_pbVertical_clicked()
{
    //--------------------------------
    //Define Video Origin
    //--------------------------------
    QString defaVideoPath;
    defaVideoPath.append(_PATH_LOCAL_SYNC_FOLDERS);
    defaVideoPath.append(_PATH_LOCAL_FOLDER_VIDEOS);
    if( funcLetUserSelectFile( &defaVideoPath ) != _OK )
    {
        return (void)false;
    }
    ui->txtVertPath->setText(defaVideoPath);
}

void formMergeSlideCalibrations::on_pbMergeCalibration_clicked()
{
    //----------------------------------------------
    //Validate Camera ID
    //----------------------------------------------
    QString CameraID(ui->txtCameraID->text().trimmed());
    if(
            CameraID.isEmpty() ||
            ui->txtHorPath->text().trimmed().isEmpty() ||
            ui->txtVertPath->text().trimmed().isEmpty()
    ){
        funcShowMsgERROR_Timeout("All Fields Required");
        return (void)false;
    }

    //----------------------------------------------
    //Get Half-Calibrations
    //----------------------------------------------
    //Read Horizontal
    structSlideCalibration slideCalibration;
    funcReadHorHalfCalib(ui->txtHorPath->text().trimmed(), &slideCalibration);
    //Read Vertical
    funcReadVertHalfCalib(ui->txtVertPath->text().trimmed(), &slideCalibration);

    //---------------------------------------
    //Get Origin Point
    //---------------------------------------
    float horA, horB;
    float verA, verB;
    //Prefill Coordinates
    horA  = slideCalibration.horizLR.a;
    horB  = slideCalibration.horizLR.b;
    verA  = slideCalibration.vertLR.a;
    verB  = slideCalibration.vertLR.b;
    slideCalibration.originY = round( ((float)horB*(float)slideCalibration.x2)+(float)horA);
    slideCalibration.originX = round( ((float)verB*(float)slideCalibration.originY)+(float)verA );


    //----------------------------------------------
    //Save Merged Calibration File
    //----------------------------------------------
    //Define Filename
    QString fullPathName(_PATH_SLIDE_CALIB_PATH);
    fullPathName.append(CameraID);
    fullPathName.append(".xml");
    //Save File
    if(
            funcSaveSlideCalibration(
                                        &fullPathName,
                                        &slideCalibration
                                    ) != _OK
    ){
        funcShowMsgERROR_Timeout("Saving Slide Calibration File");
        return (void)false;
    }

    this->close();

}

int formMergeSlideCalibrations
    ::funcSaveSlideCalibration(
                                    QString* pathDestine,
                                    structSlideCalibration* slideCalibration
){
    QList<QString> lstFixtures;
    QList<QString> lstValues;

    //-----------------------------------
    //Fill Fixtures
    //-----------------------------------
    lstFixtures << "imgW"       << "imgH"
                << "x1"         << "y1"         << "x2"         << "y2"
                << "originX"    << "originY"    <<  "originH"
                << "waveA"      << "waveB"
                << "vertA"      << "vertB"
                << "horizA"     << "horizB";

    //-----------------------------------
    //Fill Values
    //-----------------------------------
    lstValues   << QString::number(slideCalibration->imgW)
                << QString::number(slideCalibration->imgH)
                << QString::number(slideCalibration->x1)
                << QString::number(slideCalibration->y1)
                << QString::number(slideCalibration->x2)
                << QString::number(slideCalibration->y2)
                << QString::number(slideCalibration->originX)
                << QString::number(slideCalibration->originY)
                << QString::number(slideCalibration->originH)
                << QString::number(slideCalibration->wavelengthLR.a)
                << QString::number(slideCalibration->wavelengthLR.b)
                << QString::number(slideCalibration->vertLR.a)
                << QString::number(slideCalibration->vertLR.b)
                << QString::number(slideCalibration->horizLR.a)
                << QString::number(slideCalibration->horizLR.b);

    //-----------------------------------
    //Save Slide Calibration File
    //-----------------------------------
    if( funcSaveXML(pathDestine,&lstFixtures,&lstValues) != _OK )
    {
        return _ERROR;
    }

    return _OK;
}

