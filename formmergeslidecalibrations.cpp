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
    defaVideoPath.append(_PATH_LAST_PATH_OPENED);
    //defaVideoPath.append(_PATH_LOCAL_FOLDER_VIDEOS);
    if( funcLetUserSelectFile( &defaVideoPath, "Select HORIZONTAL Settings", this ) != _OK )
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
    defaVideoPath.append(_PATH_LAST_PATH_OPENED);
    //defaVideoPath.append(_PATH_LOCAL_FOLDER_VIDEOS);
    if( funcLetUserSelectFile( &defaVideoPath, "Select VERTICAL Settings", this ) != _OK )
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

    //----------------------------------------------
    //Load Affine Transformation
    //----------------------------------------------
    QTransform T;
    if( funcReadAffineTransXML( ui->txtAffineTrans->text().trimmed(), &T ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Affine Transformation");
        return (void)false;
    }

    //----------------------------------------------
    //Load Sensitivities If was Selected
    //----------------------------------------------
    structSlideSensitivities slideSensitivities;
    if( !ui->txtHorPath->text().trimmed().isEmpty() )
    {        
        funcReadSensitivities( ui->txtSensitivities->text().trimmed(), &slideSensitivities );
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
                                        &slideSensitivities,
                                        &T
                                    ) != _OK
    ){
        funcShowMsgERROR_Timeout("Saving Slide Calibration File");
        return (void)false;
    }

    this->close();

}

int formMergeSlideCalibrations
    ::funcReadSensitivities(
                                const QString &filePath,
                                structSlideSensitivities* slideSensitivities
){
    QFile *xmlFile = new QFile( filePath );
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return _ERROR;
    }
    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement)
        {
            if( xmlReader->name()=="ralphSR" )
                slideSensitivities->ralphSR.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="ralphSG" )
                slideSensitivities->ralphSG.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="ralphSB" )
                slideSensitivities->ralphSB.append( xmlReader->readElementText().trimmed() );

            if( xmlReader->name()=="wSR" )
                slideSensitivities->wSR.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="wSG" )
                slideSensitivities->wSG.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="wSB" )
                slideSensitivities->wSB.append( xmlReader->readElementText().trimmed() );

            if( xmlReader->name()=="originalSR" )
                slideSensitivities->originalSR.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="originalSG" )
                slideSensitivities->originalSG.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="originalSB" )
                slideSensitivities->originalSB.append( xmlReader->readElementText().trimmed() );
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();

    slideSensitivities->filled = 1;

    return _OK;
}

int formMergeSlideCalibrations
    ::funcSaveSlideCalibration(
                                    QString* pathDestine,
                                    structSlideCalibration* slideCalibration,
                                    structSlideSensitivities* slideSensitivities,
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
    if( slideSensitivities->filled )
    {
        lstFixtures << "ralphSR"        << "ralphSG"        << "ralphSB"
                    << "wSR"            << "wSG"            << "wSB"
                    << "originalSR"     << "originalSG"     << "originalSB";
    }

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

    if( slideSensitivities->filled )
    {
        lstValues   << slideSensitivities->ralphSR
                    << slideSensitivities->ralphSG
                    << slideSensitivities->ralphSB
                    << slideSensitivities->wSR
                    << slideSensitivities->wSG
                    << slideSensitivities->wSB
                    << slideSensitivities->originalSR
                    << slideSensitivities->originalSG
                    << slideSensitivities->originalSB;
    }

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
    QString defaPath;//_PATH_CALIB_LAST_OPEN
    defaPath.append(_PATH_LAST_PATH_OPENED);
    if( funcLetUserSelectFile( &defaPath, "Select TRANSFORMATION Settings", this ) != _OK )
    {
        return (void)false;
    }
    ui->txtAffineTrans->setText(defaPath);
}

void formMergeSlideCalibrations::on_pbSensitivities_clicked()
{
    //--------------------------------
    //Define Transformation Origin
    //--------------------------------
    QString defaPath;
    defaPath.append(_PATH_LAST_PATH_OPENED);
    if( funcLetUserSelectFile( &defaPath, "Select SENSITIVITIES Settings", this ) != _OK )
    {
        return (void)false;
    }
    ui->txtSensitivities->setText(defaPath);
}
