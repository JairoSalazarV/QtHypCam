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
    structSlideStrSens slideSensitivities;
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
                                structSlideStrSens* slideStrSens
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
            if( xmlReader->name()=="normedOrigR" )
                slideStrSens->normedOrigR.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="normedOrigG" )
                slideStrSens->normedOrigG.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="normedOrigB" )
                slideStrSens->normedOrigB.append( xmlReader->readElementText().trimmed() );

            if( xmlReader->name()=="normedRalfR" )
                slideStrSens->normedRalfR.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="normedRalfG" )
                slideStrSens->normedRalfG.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="normedRalfB" )
                slideStrSens->normedRalfB.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="normedRalfH" )
                slideStrSens->normedRalfH.append( xmlReader->readElementText().trimmed() );

            if( xmlReader->name()=="originalR" )
                slideStrSens->originalR.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="originalG" )
                slideStrSens->originalG.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="originalB" )
                slideStrSens->originalB.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="originalH" )
                slideStrSens->originalH.append( xmlReader->readElementText().trimmed() );

            if( xmlReader->name()=="wR" )
                slideStrSens->wR.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="wG" )
                slideStrSens->wG.append( xmlReader->readElementText().trimmed() );
            if( xmlReader->name()=="wB" )
                slideStrSens->wB.append( xmlReader->readElementText().trimmed() );

        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();

    slideStrSens->filled = 1;

    return _OK;
}

int formMergeSlideCalibrations
    ::funcSaveSlideCalibration(
                                    QString* pathDestine,
                                    structSlideCalibration* slideCalibration,
                                    structSlideStrSens *slideStrSens,
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

    if( slideStrSens->filled )
    {
        lstFixtures << "normedOrigR"    << "normedOrigG"    << "normedOrigB"
                    << "normedRalfR"    << "normedRalfG"    << "normedRalfB"    << "normedRalfH"
                    << "originalR"      << "originalG"      << "originalB"      << "originalH"
                    << "wR"             << "wG"             << "wB";
        lstValues   << slideStrSens->normedOrigR
                    << slideStrSens->normedOrigG
                    << slideStrSens->normedOrigB
                    << slideStrSens->normedRalfR
                    << slideStrSens->normedRalfG
                    << slideStrSens->normedRalfB
                    << slideStrSens->normedRalfH
                    << slideStrSens->originalR
                    << slideStrSens->originalG
                    << slideStrSens->originalB
                    << slideStrSens->originalH
                    << slideStrSens->wR
                    << slideStrSens->wG
                    << slideStrSens->wB;
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
