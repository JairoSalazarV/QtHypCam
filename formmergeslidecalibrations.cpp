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

    //--------------------------------
    //Load OriginH into options
    //--------------------------------
    //Read Horizontal
    structSlideCalibration slideCalibration;
    funcReadHorHalfCalib(ui->txtHorPath->text().trimmed(), &slideCalibration);
    ui->spinBoxOriginH->setValue(slideCalibration.originH);
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
            CameraID.isEmpty()                              ||
            ui->txtHorPath->text().trimmed().isEmpty()      ||
            ui->txtVertPath->text().trimmed().isEmpty()     ||
            ui->txtAffineTrans->text().trimmed().isEmpty()
    ){
        funcShowMsgERROR_Timeout("All Fields are Required");
        return (void)false;
    }

    //----------------------------------------------
    //Get Half-Calibrations
    //----------------------------------------------
    //Read Horizontal
    structSlideCalibration slideCalibration;
    funcReadHorHalfCalib(ui->txtHorPath->text().trimmed(), &slideCalibration);
    //Update OriginH from User Settings
    slideCalibration.originH = ui->spinBoxOriginH->value();
    //Read Vertical
    float referenceX2 = 0;
    funcReadVertHalfCalib(
                            ui->txtVertPath->text().trimmed(),
                            &referenceX2,
                            &slideCalibration
                         );

    //--------------------------------
    //Load Affine Transformation
    //--------------------------------
    QTransform T;
    if( funcReadAffineTransXML( ui->txtAffineTrans->text().trimmed(), &T ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Affine Transformation");
        return (void)false;
    }

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
    slideCalibration.originY = round( (horB*referenceX2)+horA);
    slideCalibration.originX = round( (verB*slideCalibration.originY)+verA );

    //----------------------------------------------
    //Save Merged Calibration File
    //----------------------------------------------
    //Define Filename
    QString fullPathName(_PATH_SLIDE_CALIB_PATH);
    fullPathName.append("slideCam_");
    fullPathName.append(CameraID);
    fullPathName.append(".xml");
    //Save File
    if(
            funcSaveSlideCalibration(
                                        &fullPathName,
                                        &slideCalibration,
                                        &T
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
                                    structSlideCalibration* slideCalibration,
                                    QTransform *T
){

    //-----------------------------------
    //Define a Matrix with all coordinates
    //Pre-computed
    //-----------------------------------
    //Get maximum distance from origin
    float maxWavelength = _RASP_CAM_MAX_WAVELENGTH;
    int distPixFromLower;
    maxWavelength = maxWavelength - slideCalibration->originWave;
    distPixFromLower = round( funcApplyLR(maxWavelength,slideCalibration->wave2DistLR,true) );
    std::cout << "distPixFromLower: " << distPixFromLower << "px" << std::endl;
    //Calculate Coordinates
    //int originX, originY;
    //originX = slideCalibration->originX;
    //originY = slideCalibration->originY;

    //-----------------------------------
    //Fill Fixtures
    //-----------------------------------
    QList<QString> lstFixtures;
    lstFixtures << "imgW"       << "imgH"                
                << "originX"    << "originY"    << "originH"    << "originWave"
                << "maxWavelen"
                << "dist2WaveA" << "dist2WaveB"
                << "wave2DistA" << "wave2DistB"
                << "vertA"      << "vertB"
                << "horizA"     << "horizB"
                << "maxNumCols"
                << "Tm11"       << "Tm12"       << "Tm13"
                << "Tm21"       << "Tm22"       << "Tm23"
                << "Tm31"       << "Tm32"       << "Tm33" ;

    //-----------------------------------
    //Fill Values
    //-----------------------------------
    QList<QString> lstValues;
    lstValues   << QString::number(slideCalibration->imgW)
                << QString::number(slideCalibration->imgH)                
                << QString::number(slideCalibration->originX)
                << QString::number(slideCalibration->originY)
                << QString::number(slideCalibration->originH)                   
                << QString::number(slideCalibration->originWave)
                << QString::number(slideCalibration->maxWave)
                << QString::number(slideCalibration->dist2WaveLR.a)
                << QString::number(slideCalibration->dist2WaveLR.b)
                << QString::number(slideCalibration->wave2DistLR.a)
                << QString::number(slideCalibration->wave2DistLR.b)
                << QString::number(slideCalibration->vertLR.a)
                << QString::number(slideCalibration->vertLR.b)
                << QString::number(slideCalibration->horizLR.a)
                << QString::number(slideCalibration->horizLR.b)
                << QString::number(distPixFromLower)
                << QString::number(T->m11())
                << QString::number(T->m12())
                << QString::number(T->m13())
                << QString::number(T->m21())
                << QString::number(T->m22())
                << QString::number(T->m23())
                << QString::number(T->m31())
                << QString::number(T->m32())
                << QString::number(T->m33());

    //-----------------------------------
    //Save Slide Calibration File
    //-----------------------------------
    if( funcSaveXML(*pathDestine,&lstFixtures,&lstValues) != _OK )
    {
        return _ERROR;
    }

    return _OK;
}


void formMergeSlideCalibrations::on_pbAffineTrans_clicked()
{
    //--------------------------------
    //Define Transformation Origin
    //--------------------------------
    QString defaPath;
    defaPath.append(_PATH_LOCAL_SYNC_FOLDERS);
    defaPath.append(_PATH_LOCAL_FOLDER_VIDEOS);
    if( funcLetUserSelectFile( &defaPath ) != _OK )
    {
        return (void)false;
    }
    ui->txtAffineTrans->setText(defaPath);
}
