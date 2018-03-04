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
    lstFixtures << "vertCanvasW" << "vertCanvasH"
                << "vertX1"      << "vertY1"
                << "vertX2"      << "vertY2"
                << "vertWaveA"   << "vertWaveB"
                << "vertVertA"   << "vertVertB";

    lstFixtures << "horCanvasW" << "horCanvasH"
                << "H"
                << "a" << "b";

    //-----------------------------------
    //Fill Values
    //-----------------------------------
    lstValues   << QString::number(slideCalibration->vertical.canvasW)
                << QString::number(slideCalibration->vertical.canvasH)
                << QString::number(slideCalibration->vertical.x1)
                << QString::number(slideCalibration->vertical.y1)
                << QString::number(slideCalibration->vertical.x2)
                << QString::number(slideCalibration->vertical.y2)
                << QString::number(slideCalibration->vertical.wavelengthLR.a)
                << QString::number(slideCalibration->vertical.wavelengthLR.b)
                << QString::number(slideCalibration->vertical.vertLR.a)
                << QString::number(slideCalibration->vertical.vertLR.b);

    lstValues   << QString::number(slideCalibration->horizontal.canvasW)
                << QString::number(slideCalibration->horizontal.canvasH)
                << QString::number(slideCalibration->horizontal.H)
                << QString::number(slideCalibration->horizontal.a)
                << QString::number(slideCalibration->horizontal.b);

    //-----------------------------------
    //Save Slide Calibration File
    //-----------------------------------
    if( funcSaveXML(pathDestine,&lstFixtures,&lstValues) != _OK )
    {
        return _ERROR;
    }

    return _OK;
}

