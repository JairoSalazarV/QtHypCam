#include <__common.h>

#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>

#include <ifaddrs.h>
#include <netdb.h>

#include <QFile>
#include <QTextStream>

#include <QFileInfo>

#include <customQMatrix3x3.h>
#include <customQMatrix4x3.h>

#include <QDesktopServices>
#include <QFileDialog>

#include <iostream>
#include <fstream>

#include <QPixmap>
#include <graphicsview.h>
#include <QApplication>
#include <QDesktopWidget>

#include <lstcustoms.h>

#include <QInputDialog>

#include <QImage>

#include <QTimer>

#include <QIODevice>
#include <QWidget>

#include <cmath>

QPoint *calibPoint( QPoint *point, lstDoubleAxisCalibration *calib )
{
    point->setX( round( calib->LR.horizA + (calib->LR.horizB * point->x()) ) );
    point->setY( round( calib->LR.vertA + (calib->LR.vertB * point->y()) ) );
    return point;
}

int funcDeleteFile( QString fileName )
{
    QFile tmpFile( fileName );
    if( tmpFile.exists() )
    {
        tmpFile.remove();
        return _OK;//File exists and removed
    }
    else
    {
        return _FAILURE;//File does not exists but is equal to be removed
    }
    return _ERROR;//Error
}

QString funcRemoveFileNameFromPath( QString Path ){
    return QFileInfo(Path).absolutePath();
}


QString timeToQString( unsigned int totMilli )
{
    unsigned int h          = round(totMilli/(1000*60*60));
    totMilli                = totMilli - (h*(1000*60*60));
    unsigned int m          = round(totMilli/(1000*60));
    totMilli                = totMilli - (m*(1000*60));
    unsigned int s          = round(totMilli/1000);
    unsigned int u          = totMilli - (s*1000);
    QString timeElapsed     = "HH:MM:SS:U( ";
    timeElapsed            += (h>9)?QString::number(h)+":":"0"+QString::number(h)+":";
    timeElapsed            += (m>9)?QString::number(m)+":":"0"+QString::number(m)+":";
    timeElapsed            += (s>9)?QString::number(s)+":":"0"+QString::number(s)+":";
    timeElapsed            += QString::number(u);
    timeElapsed            += " )";
    return timeElapsed;
}


double vectorMax(double *vector, int len)
{
    int i;
    double max;
    max = 0;
    for(i=0;i<len;i++)
    {
        max = (max<vector[i])?vector[i]:max;
    }
    return max;
}

double vectorMaxQListQString(QList<QString> lst)
{
    int i;
    double max;
    max = 0;
    for(i=0;i<lst.size();i++)
    {
        max = (max<lst.at(i).toDouble())?lst.at(i).toDouble():max;
    }
    return max;
}

QImage bilinearInterpolationQImage(QImage img)
{
    QImage aux;
    aux = img;
    int x, y;
    QColor tmpPixel;
    QRgb Q11, Q12, Q21, Q22;
    int r, g, b;
    for(y=1; y<img.height()-1; y++)
    {
        for(x=1; x<img.width()-1; x++)
        {
            Q11 = img.pixel(x-1,y-1);
            Q12 = img.pixel(x+1,y-1);
            Q21 = img.pixel(x-1,y+1);
            Q22 = img.pixel(x+1,y+1);
            r = round( (qRed(Q11)+qRed(Q12)+qRed(Q21)+qRed(Q22)/4.0) );
            g = round( (qGreen(Q11)+qGreen(Q12)+qGreen(Q21)+qGreen(Q22)/4.0) );
            b = round( (qBlue(Q11)+qBlue(Q12)+qBlue(Q21)+qBlue(Q22)/4.0) );
            tmpPixel.setRed(r);
            tmpPixel.setGreen(g);
            tmpPixel.setBlue(b);
            aux.setPixel( x, y, tmpPixel.rgba() );
        }
    }
    return aux;
}

void funcPrintCalibration(lstDoubleAxisCalibration *calibSettings){

    qDebug() << "W" << calibSettings->W;
    qDebug() << "H" << calibSettings->H;

    qDebug() << "bkgPath" << calibSettings->bkgPath;

    qDebug() << "bigX" << calibSettings->bigX;
    qDebug() << "bigY" << calibSettings->bigY;
    qDebug() << "bigW" << calibSettings->bigW;
    qDebug() << "bigH" << calibSettings->bigH;

    qDebug() << "squareX" << calibSettings->squareX;
    qDebug() << "squareY" << calibSettings->squareY;
    qDebug() << "squareW" << calibSettings->squareW;
    qDebug() << "squareH" << calibSettings->squareH;

    qDebug() << "squarePixX: " << calibSettings->squarePixX;
    qDebug() << "squarePixY: " << calibSettings->squarePixY;
    qDebug() << "squarePixW: " << calibSettings->squarePixW;
    qDebug() << "squarePixH: " << calibSettings->squarePixH;

    qDebug() << "squareUsablePixX: " << calibSettings->squareUsableX;
    qDebug() << "squareUsablePixY: " << calibSettings->squareUsableY;
    qDebug() << "squareUsablePixW: " << calibSettings->squareUsableW;
    qDebug() << "squareUsablePixH: " << calibSettings->squareUsableH;

    qDebug() << "horizontalA: " << calibSettings->LR.horizA;
    qDebug() << "horizontalB: " << calibSettings->LR.horizB;
    qDebug() << "verticalA: " << calibSettings->LR.vertA;
    qDebug() << "verticalB: " << calibSettings->LR.vertB;

    qDebug() << "waveHorizA: " << calibSettings->LR.waveHorizA;
    qDebug() << "waveHorizB: " << calibSettings->LR.waveHorizB;
    qDebug() << "waveVertA: " << calibSettings->LR.waveVertA;
    qDebug() << "waveVertB: " << calibSettings->LR.waveVertB;

    qDebug() << "deltaHorizA: " << calibSettings->LR.deltaHorizA;
    qDebug() << "deltaHorizB: " << calibSettings->LR.deltaHorizB;
    qDebug() << "deltaVertA: " << calibSettings->LR.deltaVertA;
    qDebug() << "deltaVertB: " << calibSettings->LR.deltaVertB;

    qDebug() << "minWavelength: " << calibSettings->minWavelength;
    qDebug() << "maxWavelength: " << calibSettings->maxWavelength;
    qDebug() << "maxNumBand: " << calibSettings->maxNumBands;
    qDebug() << "minSpecRes: " << calibSettings->minSpecRes;

}

bool funcGetCalibration(lstDoubleAxisCalibration *doubAxisCal){

    QFile *xmlFile = new QFile(_PATH_CALIBRATION_FILE);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        funcShowMsg("ERROR","Opening _PATH_CALIBRATION_FILE");
        return false;
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
            if( xmlReader->name()=="bkgPath" )
                doubAxisCal->bkgPath = xmlReader->readElementText();

            if( xmlReader->name()=="W" )
                doubAxisCal->W = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="H" )
                doubAxisCal->H = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="bigX" )
                doubAxisCal->bigX = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="bigY" )
                doubAxisCal->bigY = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="bigW" )
                doubAxisCal->bigW = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="bigH" )
                doubAxisCal->bigH = xmlReader->readElementText().toFloat(0);

            if( xmlReader->name()=="squareX" )
                doubAxisCal->squareX = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="squareY" )
                doubAxisCal->squareY = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="squareW" )
                doubAxisCal->squareW = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="squareH" )
                doubAxisCal->squareH = xmlReader->readElementText().toFloat(0);

            if( xmlReader->name()=="squarePixX" )
                doubAxisCal->squarePixX = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="squarePixY" )
                doubAxisCal->squarePixY = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="squarePixW" )
                doubAxisCal->squarePixW = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="squarePixH" )
                doubAxisCal->squarePixH = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="squareUsablePixX" )
                doubAxisCal->squareUsableX = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="squareUsablePixY" )
                doubAxisCal->squareUsableY = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="squareUsablePixW" )
                doubAxisCal->squareUsableW = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="squareUsablePixH" )
                doubAxisCal->squareUsableH = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="horizontalA" )
                doubAxisCal->LR.horizA = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="horizontalB" )
                doubAxisCal->LR.horizB = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="verticalA" )
                doubAxisCal->LR.vertA = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="verticalB" )
                doubAxisCal->LR.vertB = xmlReader->readElementText().toFloat(0);

            if( xmlReader->name()=="waveHorizA" )
                doubAxisCal->LR.waveHorizA = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="waveHorizB" )
                doubAxisCal->LR.waveHorizB = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="waveVertA" )
                doubAxisCal->LR.waveVertA = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="waveVertB" )
                doubAxisCal->LR.waveVertB = xmlReader->readElementText().toFloat(0);

            if( xmlReader->name()=="deltaHorizA" )
                doubAxisCal->LR.deltaHorizA = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="deltaHorizB" )
                doubAxisCal->LR.deltaHorizB = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="deltaVertA" )
                doubAxisCal->LR.deltaVertA = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="deltaVertB" )
                doubAxisCal->LR.deltaVertB = xmlReader->readElementText().toFloat(0);

            if( xmlReader->name()=="minWavelength" )
                doubAxisCal->minWavelength = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="maxWavelength" )
                doubAxisCal->maxWavelength = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="maxNumBand" )
                doubAxisCal->maxNumBands = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="minSpecRes" )
                doubAxisCal->minSpecRes = xmlReader->readElementText().toFloat(0);

            if( xmlReader->name()=="Sr" )
                funcQStringToSensitivities( xmlReader->readElementText(), &doubAxisCal->Sr );
            if( xmlReader->name()=="Sg" )
                funcQStringToSensitivities( xmlReader->readElementText(), &doubAxisCal->Sg );
            if( xmlReader->name()=="Sb" )
                funcQStringToSensitivities( xmlReader->readElementText(), &doubAxisCal->Sb );

        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("_PATH_STARTING_SETTINGS Parse Error",xmlReader->errorString());
    }
    xmlReader->clear();
    xmlFile->close();

    return true;
}

void funcQStringToSensitivities(QString txt, QList<double> *p)
{
    int i;
    QList<QString> lstSensitivities;
    lstSensitivities = txt.split(",");
    //p = (double*)malloc(lstSensitivities.count()*sizeof(double));
    for(i=0; i<lstSensitivities.count(); i++)
    {
        p->append( lstSensitivities.at(i).toDouble(0) );
    }
}

void funcTransPix( calcAndCropSnap *calStruct, int w, int h, int W, int H ){
    //Extrapolate dimensions
    calStruct->X1   = round( ((float)W/(float)w)*(float)calStruct->x1 );
    calStruct->Y1   = round( ((float)H/(float)h)*(float)calStruct->y1 );
    calStruct->X2   = round( ((float)W/(float)w)*(float)calStruct->x2 );
    calStruct->Y2   = round( ((float)H/(float)h)*(float)calStruct->y2 );
    calStruct->lenW = abs(calStruct->X2-calStruct->X1);
    calStruct->lenH = abs(calStruct->Y2-calStruct->Y1);
}

colorAnalyseResult *funcAnalizeImage( QImage *img ){

    //Reserve memory
    //..
    colorAnalyseResult* colResults = (colorAnalyseResult*)malloc(sizeof(colorAnalyseResult));
    int aux = img->width() * sizeof(int);
    colResults->Red = (int*)malloc( aux );
    colResults->Green = (int*)malloc( aux );
    colResults->Blue = (int*)malloc( aux );
    memset( colResults->Red, '\0', aux );
    memset( colResults->Green, '\0', aux );
    memset( colResults->Blue, '\0', aux );

    //Procced to analyze the image
    //..
    int r,c,tmpMaxR=0,tmpMaxG=0,tmpMaxB=0,tmpMaxRx=0,tmpMaxGx=0,tmpMaxBx=0;
    QRgb pixel;
    colResults->maxRx   = 0;
    colResults->maxGx   = 0;
    colResults->maxBx   = 0;
    colResults->maxMax  = 0;
    //colResults->maxR = 0;
    //colResults->maxG = 0;
    //colResults->maxB = 0;
    for(r=0;r<img->height();r++){
        tmpMaxR = 0;
        tmpMaxG = 0;
        tmpMaxB = 0;
        tmpMaxRx = 0;
        tmpMaxGx = 0;
        tmpMaxBx = 0;
        for(c=0;c<img->width();c++){
            if(!img->valid(c,r)){
                qDebug() << "Invalid r: " << r << "c: "<<c;
                qDebug() << "img.width(): " << img->width();
                qDebug() << "img.height(): " << img->height();
                return colResults;
            }
            pixel = img->pixel(c,r);
            //Red
            if(tmpMaxR<qRed(pixel)){
                tmpMaxR     = qRed(pixel);
                tmpMaxRx    = c;
            }
            //Green
            if(tmpMaxG<qGreen(pixel)){
                tmpMaxG     = qGreen(pixel);
                tmpMaxGx    = c;
            }
            //Blue
            if(tmpMaxB<qBlue(pixel)){
                tmpMaxB     = qBlue(pixel);
                tmpMaxBx    = c;
            }
        }
        //Acumulate
        colResults->maxRx += tmpMaxRx;
        colResults->maxGx += tmpMaxGx;
        colResults->maxBx += tmpMaxBx;


    }
    //Mean
    colResults->maxRx = round( (float)colResults->maxRx / (float)img->height());
    colResults->maxGx = round( (float)colResults->maxGx / (float)img->height());
    colResults->maxBx = round( (float)colResults->maxBx / (float)img->height());

    tmpMaxRx = colResults->maxRx;
    tmpMaxGx = colResults->maxGx;
    tmpMaxBx = colResults->maxBx;
    //funcShowMsg("Result",
    //            "(" + QString::number(tmpMaxR)+","+QString::number(tmpMaxRx) + ")" +
    //            "(" + QString::number(tmpMaxG)+","+QString::number(tmpMaxGx) + ")" +
    //            "(" + QString::number(tmpMaxB)+","+QString::number(tmpMaxBx) + ")"
    //           );

    //Get the maxMax
    //..
    if( colResults->maxMax < colResults->maxR ){
        colResults->maxMax      = colResults->maxR;
        colResults->maxMaxx     = colResults->maxRx;
        colResults->maxMaxColor = 1;
    }
    if( colResults->maxMax < colResults->maxG ){
        colResults->maxMax      = colResults->maxG;
        colResults->maxMaxx     = colResults->maxGx;
        colResults->maxMaxColor = 2;
    }
    if( colResults->maxMax < colResults->maxB ){
        colResults->maxMax      = colResults->maxB;
        colResults->maxMaxx     = colResults->maxBx;
        colResults->maxMaxColor = 3;
    }

    return colResults;



    /*
    //Reserve memory
    //..
    colorAnalyseResult* colResults = (colorAnalyseResult*)malloc(sizeof(colorAnalyseResult));
    int aux = img->width() * sizeof(int);
    colResults->Red = (int*)malloc( aux );
    colResults->Green = (int*)malloc( aux );
    colResults->Blue = (int*)malloc( aux );
    memset( colResults->Red, '\0', aux );
    memset( colResults->Green, '\0', aux );
    memset( colResults->Blue, '\0', aux );

    //Procced to analyze the image
    //..
    int r, c;
    colResults->maxR    = 0;
    colResults->maxG    = 0;
    colResults->maxB    = 0;
    colResults->maxMax  = 0;
    QRgb pixel;
    for(c=0;c<img->width();c++){
        colResults->Red[c]   = 0;
        colResults->Green[c] = 0;
        colResults->Blue[c]  = 0;
        for(r=0;r<img->height();r++){
            if(!img->valid(QPoint(c,r))){
                qDebug() << "Invalid r: " << r << "c: "<<c;
                qDebug() << "img.width(): " << img->width();
                qDebug() << "img.height(): " << img->height();
                break;
                break;
            }
            pixel     = img->pixel(QPoint(c,r));
            colResults->Red[c]   += qRed(pixel);
            colResults->Green[c] += qGreen(pixel);
            colResults->Blue[c]  += qBlue(pixel);
        }
        colResults->Red[c]   = round((float)colResults->Red[c]/(float)img->height());
        colResults->Green[c] = round((float)colResults->Green[c]/(float)img->height());
        colResults->Blue[c]  = round((float)colResults->Blue[c]/(float)img->height());
        if( colResults->Red[c] > colResults->maxR ){
            colResults->maxR        = colResults->Red[c];
            colResults->maxRx       = c;
        }
        if( colResults->Green[c] > colResults->maxG ){
            colResults->maxG        = colResults->Green[c];
            colResults->maxGx       = c;
        }
        if( colResults->Blue[c] > colResults->maxB ){
            colResults->maxB        = colResults->Blue[c];
            colResults->maxBx       = c;
        }
    }

    //Get the maxMax
    //..
    if( colResults->maxMax < colResults->maxR ){
        colResults->maxMax      = colResults->maxR;
        colResults->maxMaxx     = colResults->maxRx;
        colResults->maxMaxColor = 1;
    }
    if( colResults->maxMax < colResults->maxG ){
        colResults->maxMax      = colResults->maxG;
        colResults->maxMaxx     = colResults->maxGx;
        colResults->maxMaxColor = 2;
    }
    if( colResults->maxMax < colResults->maxB ){
        colResults->maxMax      = colResults->maxB;
        colResults->maxMaxx     = colResults->maxBx;
        colResults->maxMaxColor = 3;
    }

    return colResults;
    */
}

/*
IplImage *funcGetImgFromCam( int usb, int stabMs ){
    //Turn on camera
    //..
    CvCapture* usbCam  = cvCreateCameraCapture( usb );
    cvSetCaptureProperty( usbCam,  CV_CAP_PROP_FRAME_WIDTH,  320*_FACT_MULT );
    cvSetCaptureProperty( usbCam,  CV_CAP_PROP_FRAME_HEIGHT, 240*_FACT_MULT );
    assert( usbCam );
    QtDelay(stabMs);

    //Create image
    //..
    IplImage *tmpCam = cvQueryFrame( usbCam );
    if( ( tmpCam = cvQueryFrame( usbCam ) ) ){
        //Get the image
        QString tmpName = "./snapshots/tmpUSB.png";
        if( _USE_CAM ){
            IplImage *imgRot = cvCreateImage(
                                                cvSize(tmpCam->height,tmpCam->width),
                                                tmpCam->depth,
                                                tmpCam->nChannels
                                            );
            cvTranspose(tmpCam,imgRot);
            cvTranspose(tmpCam,imgRot);
            cvTranspose(tmpCam,imgRot);

            //cv::imwrite( tmpName.toStdString(), cv::Mat(imgRot, true) );
            cv::imwrite( tmpName.toStdString(), cv::cvarrToMat(imgRot) );

            cvReleaseCapture(&usbCam);
            return imgRot;
        }else{
            cvReleaseCapture(&usbCam);
            return cvLoadImage(tmpName.toStdString().c_str(), 1);
        }
    }else{
        qDebug() << "ERROR retrieving image from usb(" << usb << ")";
    }
    cvReleaseCapture(&usbCam);
    return tmpCam;
}
*/

bool saveFile( const QString &fileName, QString contain ){
    QFile file(fileName);
    if(file.exists()){
        if(!file.remove()){
            funcShowMsgERROR("Removing file: " + fileName);
            return false;            
        }
    }
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << contain << endl;
        file.close();
    }else{
        funcShowMsgERROR("Opening file: " + fileName);
        return false;
    }
    return true;
}

QImage funcRotateImage(QString filePath, float rotAngle){
    QTransform transformation;
    transformation.rotate(rotAngle);
    QImage tmpImg(filePath);
    tmpImg = tmpImg.transformed(transformation);
    return tmpImg;
}

QString readAllFile( QString filePath ){
    QFile tmpFile(filePath);
    if( tmpFile.exists() )
    {
        tmpFile.open(QIODevice::ReadOnly);
        QTextStream tmpStream(&tmpFile);
        return tmpStream.readAll();
    }
    else
    {
        return _ERROR_FILE_NOTEXISTS;
    }
    return _ERROR_FILE;
}

int fileIsValid(QString fileContain)
{
    // return:
    //  1: exists and it is not empty
    // -1: empty file
    // -2: error reading
    // -3: it does note exist
    if( fileContain.isEmpty() )
    {
        return -1;
    }
    if( fileContain.contains(_ERROR_FILE) )
    {
        return -2;
    }
    if( fileContain.contains(_ERROR_FILE_NOTEXISTS) )
    {
        return -3;
    }
    return 1;
}

QString readFileParam( const QString &filePath, const QString &pathDefault, int* OK )
{
    *OK = _ERROR;
    QString fileContain;
    fileContain.clear();
    if( fileExists(filePath) )
    {
        fileContain = readAllFile(filePath);
        if( fileIsValid(fileContain) )
        {
            fileContain = fileContain.trimmed();
            fileContain.replace("\n","");
        }
        else
        {
            fileContain = pathDefault;
        }
    }else
    {
        saveFile(filePath,pathDefault);
        *OK = _FAILURE;
    }
    *OK = _OK;
    return fileContain;
}


int readFileParam(QString fileName, QString* tmpFileContain)
{
    *tmpFileContain = "";
    if( fileExists(fileName) )
    {
        *tmpFileContain = readAllFile(fileName);
        if( fileIsValid(*tmpFileContain) )
        {
            *tmpFileContain = tmpFileContain->trimmed();
            tmpFileContain->replace("\n","");
        }
        else
        {
            return _ERROR;
        }
    }else
    {
        saveFile(fileName,*tmpFileContain);
        return _FAILURE;
    }
    return _OK;
}

QString readFileParam(QString fileName){
    QString tmpFileContain = "";
    if( fileExists(fileName) )
    {
        tmpFileContain = readAllFile(fileName);
        if( fileIsValid(tmpFileContain) )
        {
            tmpFileContain = tmpFileContain.trimmed();
            tmpFileContain.replace("\n","");
        }
    }else
    {
        saveFile(fileName,tmpFileContain);
    }
    return tmpFileContain;
}






bool funGetSquareXML( QString fileName, squareAperture *squareParam ){

    QFile *xmlFile = new QFile( fileName );
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
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
        if(token == QXmlStreamReader::StartElement) {

            if( xmlReader->name()=="W" )
                squareParam->canvasW = xmlReader->readElementText().toInt();

            if( xmlReader->name()=="H" )
                squareParam->canvasH = xmlReader->readElementText().toInt();

            if( xmlReader->name()=="x" )
                squareParam->rectX = xmlReader->readElementText().toInt();

            if( xmlReader->name()=="y" )
                squareParam->rectY = xmlReader->readElementText().toInt();

            if( xmlReader->name()=="w" )
                squareParam->rectW = xmlReader->readElementText().toInt();

            if( xmlReader->name()=="h" )
                squareParam->rectH = xmlReader->readElementText().toInt();
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return false;
    }
    xmlReader->clear();
    xmlFile->close();
    return true;
}


bool funGetSlideSettingsXML( QString fileName, strReqImg *reqImg ){

    QFile *xmlFile = new QFile( fileName );
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
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
        if(token == QXmlStreamReader::StartElement) {

            if( xmlReader->name()=="degreeIni" )
                reqImg->slide.degreeIni = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="degreeEnd" )
                reqImg->slide.degreeEnd = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="degreeJump" )
                reqImg->slide.degreeJump = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="speed" )
                reqImg->slide.speed = xmlReader->readElementText().toInt(0);
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return false;
    }
    xmlReader->clear();
    xmlFile->close();
    return true;
}

bool funcGetRaspParamFromXML( structRaspcamSettings *raspcamSettings, QString filePath ){

    QFile *xmlFile = new QFile( filePath );
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        funcShowMsg("ERROR","Loading "+ filePath );
    }
    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);


    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        //If token is just StartDocument - go to next
        if(token == QXmlStreamReader::StartDocument) {
                continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if( xmlReader->name()=="AWB" ){                
                memcpy(
                            raspcamSettings->AWB,
                            xmlReader->readElementText().toStdString().c_str(),
                            sizeof(raspcamSettings->AWB)
                      );
            }
            if( xmlReader->name()=="Exposure" ){
                memcpy(
                            raspcamSettings->Exposure,
                            xmlReader->readElementText().toStdString().c_str(),
                            sizeof(raspcamSettings->Exposure)
                      );
            }


            if( xmlReader->name()=="Brightness" )
                raspcamSettings->Brightness = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Sharpness" )
                raspcamSettings->Sharpness = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Contrast" )
                raspcamSettings->Contrast = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Saturation" )
                raspcamSettings->Saturation = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="ShutterSpeedMs" )
                raspcamSettings->ShutterSpeedMs = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="SquareShutterSpeedMs" )
                raspcamSettings->SquareShutterSpeedMs = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="TimelapseDurationSecs" )
                raspcamSettings->TimelapseDurationSecs = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="TimelapseInterval_ms" )
                raspcamSettings->TimelapseInterval_ms = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="ISO" )
                raspcamSettings->ISO = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="ExposureCompensation" )
                raspcamSettings->ExposureCompensation = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Format" )
                raspcamSettings->Format = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Red" )
                raspcamSettings->Red = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Green" )
                raspcamSettings->Green = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Denoise" )
                raspcamSettings->Denoise = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="FullPhoto" )
                raspcamSettings->FullPhoto = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="ColorBalance" )
                raspcamSettings->ColorBalance = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Preview" )
                raspcamSettings->Preview = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="OneShot" )
                raspcamSettings->OneShot = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="TriggeringTimeSecs" )
                raspcamSettings->TriggeringTimeSecs = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="VideoDurationSecs" )
                raspcamSettings->VideoDurationSecs = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="CameraMp" )
                raspcamSettings->CameraMp = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Flipped" )
                raspcamSettings->Flipped = xmlReader->readElementText().toInt(0);
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg(filePath+" parse Error",xmlReader->errorString());
        return false;
    }
    xmlReader->clear();
    xmlFile->close();

    return true;

}

int connectSocket( structCamSelected *camSelected ){
    //Prepare command message
    int sockfd;
    //unsigned char bufferRead[sizeof(frameStruct)];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    qDebug() << "Comm IP: " << QString((char*)camSelected->IP);
    if (sockfd < 0){
        qDebug() << "connectSocket: opening socket";
        return -1;
    }
    //server = gethostbyname( ui->tableLstCams->item(tmpRow,1)->text().toStdString().c_str() );
    server = gethostbyname( (char*)camSelected->IP );
    if (server == NULL) {
        qDebug() << "connectSocket: no such host";
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(camSelected->tcpPort);
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        qDebug() << "connectSocket: connecting";
        return -1;
    }
    return sockfd;
}

void funcObtSettings( structSettings *lstSettings ){

    QFile *xmlFile = new QFile(_PATH_STARTING_SETTINGS);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        funcShowMsg("ERROR","Couldn't open _PATH_STARTING_SETTINGS to load settings for download");
        return (void)false;
    }
    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);


    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        //If token is just StartDocument - go to next
        if(token == QXmlStreamReader::StartDocument) {
                continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {            
            if( xmlReader->name()=="version" ){
                lstSettings->version = xmlReader->readElementText().toFloat();
            }
            if( xmlReader->name()=="tcpPort" ){
                lstSettings->tcpPort = xmlReader->readElementText().toInt(0);
            }
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("_PATH_STARTING_SETTINGS Parse Error",xmlReader->errorString());
    }
    xmlReader->clear();
    xmlFile->close();
}

void funcShowMsg(QString title, QString msg, QWidget *parent){
    QMessageBox yesNoMsgBox(parent);
    yesNoMsgBox.setWindowTitle(title);
    yesNoMsgBox.setText(msg);
    yesNoMsgBox.setDefaultButton(QMessageBox::Ok);
    yesNoMsgBox.exec();
}

void funcShowMsgERROR(QString msg, QWidget* parent){
    QMessageBox yesNoMsgBox(parent);
    yesNoMsgBox.setWindowTitle("ERROR");
    yesNoMsgBox.setText(msg);
    yesNoMsgBox.setDefaultButton(QMessageBox::Ok);
    yesNoMsgBox.exec();
}

int funcShowSelDir(const QString &title, QString startedPath, QString* dirSelected)
{
    *dirSelected = QFileDialog::getExistingDirectory(
                                                        NULL,
                                                        title,
                                                        startedPath,
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if( dirSelected->isEmpty() || dirSelected->isNull() )
    {
        dirSelected->clear();
        return _FAILURE;
    }
    dirSelected->append("/");
    //dirSelected = dirSelected;
    return _OK;
}

int funcShowSelDir(QString startedPath, QString* dirSelected)
{
    *dirSelected = QFileDialog::getExistingDirectory(
                                                        NULL,
                                                        "Open Directory",
                                                        startedPath,
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if( dirSelected->isEmpty() || dirSelected->isNull() )
    {
        dirSelected->clear();
        return _FAILURE;
    }
    dirSelected->append("/");
    //dirSelected = dirSelected;
    return _OK;
}

QString funcShowSelDir(QString path)
{
    QString dir = QFileDialog::getExistingDirectory(NULL, "Open Directory",
                                                 path,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    return dir;
}

int func_getFilenameFromUser(
                                QString* fileName,
                                QString lastPathSaved,
                                QString lastDefaultPath,
                                QWidget* parent
){
    //
    //Read the filename
    //
    QString lastPath = readFileParam(lastPathSaved);
    if( lastPath.isEmpty() )//First time using this parameter
    {
        lastPath = lastDefaultPath;
    }
    *fileName = QFileDialog::getSaveFileName(
                                                parent,
                                                "Save as...",
                                                lastPath,
                                                "Documents (*.*)"
                                            );
    if( fileName->isEmpty() )
    {
        qDebug() << "Filename not typed";
        return _ERROR;
    }
    else
    {
        lastPath = funcRemoveFileNameFromPath(*fileName);
        saveFile(lastPathSaved,lastPath);
    }

    return _OK;
}

QString funcRemoveImageExtension( QString imgName )
{
    imgName.replace(".png","");
    imgName.replace(".PNG","");

    imgName.replace(".jpg","");
    imgName.replace(".JPG","");

    imgName.replace(".jpeg","");
    imgName.replace(".JPEG","");

    imgName.replace(".gif","");
    imgName.replace(".GIF","");

    imgName.replace(".rgb888","");
    imgName.replace(".RGB888","");

    imgName.replace(".bmp","");
    imgName.replace(".BMP","");

    imgName.append( _FRAME_EXTENSION );

    return imgName;

}

int func_MergeImages(QImage* source, QImage* destine, QString type)
{
    if( source->width() != destine->width() || source->height() != destine->height() )
    {
        qDebug() << "Image Dimensions Mismatch";
        return _ERROR;
    }
    int x, y, r, g, b;
    for( x=0; x<source->width(); x++ )
    {
        for( y=0; y<source->height(); y++ )
        {
            if( type == "Average" )
            {
                r = round( (float)( qRed(source->pixel( x, y ))   + qRed(destine->pixel( x, y )) ) / 2.0 );
                g = round( (float)( qGreen(source->pixel( x, y )) + qGreen(destine->pixel( x, y )) ) / 2.0 );
                b = round( (float)( qBlue(source->pixel( x, y ))  + qBlue(destine->pixel( x, y )) ) / 2.0 );
            }
            if( type == "Maximum" )
            {
                r = std::max( qRed(source->pixel( x, y )),   qRed(destine->pixel( x, y )) );
                g = std::max( qGreen(source->pixel( x, y )), qGreen(destine->pixel( x, y )) );
                b = std::max( qBlue(source->pixel( x, y )),  qBlue(destine->pixel( x, y )) );
            }
            if( type == "Minimum" )
            {
                r = std::min( qRed(source->pixel( x, y )),   qRed(destine->pixel( x, y )) );
                g = std::min( qGreen(source->pixel( x, y )), qGreen(destine->pixel( x, y )) );
                b = std::min( qBlue(source->pixel( x, y )),  qBlue(destine->pixel( x, y )) );
            }
            destine->setPixel( x, y, qRgb(r,g,b) );
        }
    }
    return _OK;
}

int func_getMultiImages(QStringList* fileNames, QWidget* parent)
{
    //Get last path
    QString lastPath = readFileParam(_PATH_LAST_IMG_SAVED);
    if( lastPath.isEmpty() )lastPath = "./snapshots/";

    //Get lst Images
    QFileDialog dialog(parent);
    dialog.setDirectory(lastPath);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Images (*.png *.RGB888)");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    int dialogCode;
    dialogCode = dialog.exec();
    if( dialogCode )
        *fileNames = dialog.selectedFiles();
    if( dialogCode == QDialog::Accepted )
        return _OK;

    //Default
    return _FAILURE;
}

void funcShowFileError(int error, QString fileName){
    switch(error){
        case -1:
            funcShowMsg("ERROR","Empty file: " + fileName);
            break;
        case -2:
            funcShowMsg("ERROR","Unknow error in file: " + fileName);
            break;
        case -3:
            funcShowMsg("ERROR","File does not exists: " + fileName);
            break;
    }
}

void funcPrintFirst(int n, int max, char *buffer){
  QString Items;
  int i;
  Items.append("First: ");
  for(i=0;i<n;i++){
      Items.append(" "+ QString::number((int)((unsigned char)buffer[i])) +" ");
  }
  Items.append("\nLast: ");
  for(i=max-1;i>=max-n;i--){
    Items.append(" "+ QString::number((int)((unsigned char)buffer[i])) +" ");
  }
  Items.append("\n");
  funcShowMsg("First-Last",Items);
}

void funcObtainIP(char* host){
    FILE *f;
    char line[100] , *p , *c;
    f = fopen("/proc/net/route" , "r");
    while(fgets(line , 100 , f)){
        p = strtok(line , " \t");
        c = strtok(NULL , " \t");
        if(p!=NULL && c!=NULL){
            if(strcmp(c , "00000000") == 0){
                //printf("Default interface is : %s \n" , p);
                break;
            }
        }
    }

    //which family do we require , AF_INET or AF_INET6
    int fm = AF_INET;
    struct ifaddrs *ifaddr, *ifa;
    int family , s;
    if (getifaddrs(&ifaddr) == -1){
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    //Walk through linked list, maintaining head pointer so we can free list later
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
        if (ifa->ifa_addr == NULL){
            continue;
        }
        family = ifa->ifa_addr->sa_family;
        if(strcmp( ifa->ifa_name , p) == 0){
            if (family == fm) {
                s = getnameinfo( ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6) , host , NI_MAXHOST , NULL , 0 , NI_NUMERICHOST);
                if (s != 0) {
                    printf("getnameinfo() failed: %s\n", gai_strerror(s));
                    exit(EXIT_FAILURE);
                }
            }
            printf("\n");
        }
    }
    freeifaddrs(ifaddr);
}

bool funcIsIP( std::string ipCandidate ){
    std::string delimiter = ".";
    ipCandidate.append(delimiter);
    size_t pos = 0;
    std::string token;
    int ipVal;
    char *pEnd;
    u_int8_t numElems = 0;
    while ((pos = ipCandidate.find(delimiter)) != std::string::npos) {
        token = ipCandidate.substr(0, pos);
        if( !token.empty() ){
            ipVal = strtol ( token.c_str(), &pEnd, 10 );
            //funcShowMsg("IP",QString::number(ipVal));
            if( ipVal < 0 || ipVal > 255 ){
                return false;
            }
            numElems++;
        }
        ipCandidate.erase(0, pos + delimiter.length());
    }
    if( numElems < 4 ){
        return false;
    }else{
        if( strtol ( token.c_str(), &pEnd, 10 ) == 254 ){
            return false;
        }else{
            return true;
        }
    }
}


void QtDelay( unsigned int ms ){
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

int xyToIndex( int x, int y, int w)
{
    return ((y-1)*w) + x;
}


int funcShowMsgYesNo( QString title, QString msg, QWidget* parent){
    //int integerValue = 10;
    QMessageBox yesNoMsgBox(parent);
    yesNoMsgBox.setWindowTitle(title);
    yesNoMsgBox.setText(msg);
    //yesNoMsgBox.setInformativeText(QString(info).append(QVariant(integerValue).toString()));
    //yesNoMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    yesNoMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    switch (yesNoMsgBox.exec()){
        case QMessageBox::Yes: {
            return _YES;
            break;
        }
        case QMessageBox::No: {
            return _NO;
            break;
        }
    }
    return _NOT_ANSWER;
}


void funcRGB2XYZ(colSpaceXYZ *spaceXYZ, float Red, float Green, float Blue){
    memset(spaceXYZ,'\0',sizeof(colSpaceXYZ));
    //http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
    //qDebug() << "R: " << QString::number(Red);
    //qDebug() << "G: " << QString::number(Green);
    //qDebug() << "B: " << QString::number(Blue);
    spaceXYZ->X = (0.412453*Red) + (0.35758*Green) + (0.180423*Blue);
    spaceXYZ->Y = (0.212671*Red) + (0.71516*Green) + (0.072169*Blue);
    spaceXYZ->Z = (0.019334*Red) + (0.119193*Green) + (0.950227*Blue);
    float acum  = spaceXYZ->X + spaceXYZ->Y + spaceXYZ->Z;
    spaceXYZ->x = spaceXYZ->X / acum;
    spaceXYZ->y = spaceXYZ->Y / acum;
    spaceXYZ->z = spaceXYZ->Z / acum;
}

/*
QImage* IplImage2QImage(QImage* iplImg)
{
    int h = iplImg->height();
    int w = iplImg->width();
    //int channels = iplImg->nChannels;
    int channels = 3;
    QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
    char *data = iplImg->imageData;
    for (int y = 0; y < h; y++, data += iplImg->widthStep){
        for (int x = 0; x < w; x++){
            char r, g, b, a = 0;
            if (channels == 1){
                r = data[x * channels];
                g = data[x * channels];
                b = data[x * channels];
            }else if (channels == 3 || channels == 4){
                r = data[x * channels + 2];
                g = data[x * channels + 1];
                b = data[x * channels];
            }
            if (channels == 4){
                a = data[x * channels + 3];
                qimg->setPixel(x, y, qRgba(r, g, b, a));
            }else{
                qimg->setPixel(x, y, qRgb(r, g, b));
            }
        }
    }
    return qimg;

}*/

linearRegresion *funcCalcLinReg( float *X ){
    linearRegresion *linReg = (linearRegresion*)malloc(sizeof(linearRegresion));
    float mX, mY, aux1, aux2;
    float Y[]   = { 435.7, 546.1, 611.3 };
    //Calculate linear regression
    mX          = (X[0]+X[1]+X[2]) / 3.0;
    mY          = (Y[0]+Y[1]+Y[2]) / 3.0;
    aux1        = ((X[0]-mX)*(Y[0]-mY)) + ((X[1]-mX)*(Y[1]-mY)) + ((X[1]-mX)*(Y[1]-mY));
    aux2        = ((X[0]-mX)*(X[0]-mX)) + ((X[1]-mX)*(X[1]-mX)) + ((X[2]-mX)*(X[2]-mX));
    linReg->b   = aux1 / aux2;
    linReg->a   = mY-(linReg->b*mX);
    return linReg;
}

linearRegresion funcLinearRegression( double *X, double *Y, int numItems ){

    if(false)
    {
        for(int i=0; i<numItems; i++)
        {
            printf("%lf, %lf\n",X[i],Y[i]);
        }
    }


    linearRegresion linReg;
    double mX=0.0, mY=0.0, aux1=0.0, aux2=0.0;
    int i;
    //Mean
    for(i=0;i<numItems;i++)
    {
        mX += X[i];
        mY += Y[i];
    }
    mX /= (double)numItems;
    mY /= (double)numItems;
    //funcShowMsg("mX,mY",QString::number(mX)+", "+QString::number(mY));
    //
    for(i=0;i<numItems;i++)
    {
        aux1 += (X[i]-mX)*(Y[i]-mY);
        aux2 += (X[i]-mX)*(X[i]-mX);
    }    
    linReg.b   = aux1 / aux2;
    linReg.a   = mY-(linReg.b*mX);

    //printf("linReg->b: %lf \n",linReg.b);
    //printf("linReg->a: %lf \n",linReg.a);
    //printf("aux1: %lf \n",aux1);
    //printf("aux2: %lf \n",aux2);
    //printf("mX: %lf \n",mX);
    //printf("mY: %lf \n",mY);


    return linReg;
}


bool saveBinFile(unsigned long datasize, unsigned char *dataPtr, QString directory){
    QFile DummyFile(directory);
    if(DummyFile.open(QIODevice::WriteOnly)) {
        qint64 bytesWritten = DummyFile.write(reinterpret_cast<const char*>(dataPtr), datasize);
        if (bytesWritten < (qint64)datasize) {
            return false;
        }
        DummyFile.close();
    }
    return true;
}

int saveBinFile_From_u_int8_T( std::string fileName, uint8_t *data, size_t len)
{
    std::ofstream fp;
    fp.open( fileName, std::ios::out | std::ios::binary );
    fp.write((char*)data, len);
    fp.close();
    return _OK;
}

int fileExists( QString fileName )
{
    //Receive a filename and return
    //1: file exists
    //0: file does not exist
    //-1: fileName empty
    //-2: Unknow error
    if( fileName.isEmpty() )return -1;

    QFile tmpFile( fileName );
    if( tmpFile.exists() )
        return 1;
    else
        return 0;

    return -2;
}

int funcExecuteCommand( QString command )
{
    //Execute raspistill
    FILE* pipe;
    pipe = popen(command.toStdString().c_str(), "r");
    pclose(pipe);


    return _OK;
}


QString funcExecuteCommandAnswer( char* command )
{
    //Execute command
    QString result;
    result = "";//idMsg to send
    FILE* pipe;
    pipe = popen(command, "r");
    char bufferComm[100];
    try
    {
      while (!feof(pipe))
      {
        if (fgets(bufferComm, frameBodyLen, pipe) != NULL)
        {
          result.append( bufferComm );
        }
      }
    }
    catch (...)
    {
      pclose(pipe);
      throw;
    }
    pclose(pipe);
    return result;
}

/*
void funcSourcePixToDiffPix(strDiffPix *diffPix, lstDoubleAxisCalibration *calSett ){
    diffPix->rightY = (float)calSett->rightLinRegA  + ( (float)calSett->rightLinRegB    * (float)diffPix->x );
    diffPix->upY    = (float)calSett->upLinRegA     + ( (float)calSett->upLinRegB       * (float)diffPix->x );
    diffPix->leftY  = (float)calSett->leftLinRegA   + ( (float)calSett->leftLinRegB     * (float)diffPix->x );
    diffPix->downY  = (float)calSett->downLinRegA   + ( (float)calSett->downLinRegB     * (float)diffPix->x );

    qDebug() << "inside: funcSourcePixToDiffPix";
    qDebug() << "x: " << diffPix->x;

    qDebug() << "calSett->rightLinRegA: " << calSett->rightLinRegA;
    qDebug() << "calSett->rightLinRegB: " << calSett->rightLinRegB;
    qDebug() << "calSett->rightY: " << diffPix->rightY;

}
*/


double funcDet2x2(double **M){
    return (M[0][0] * M[1][1]) - (M[1][0]*M[0][1]);
}


customQMatrix3x3 matMultiply(QMatrix3x4 *M1, QMatrix4x3 *M2)
{
    int M, N, C;
    M = 3;
    C = 4;
    N = 3;
    customQMatrix3x3 auxP;
    int i, j, c;
    for(j=0;j<M;j++)
    {
        for(i=0;i<N;i++)
        {
            auxP.operator ()(j,i) = 0;
            for(c=0;c<C;c++)
            {
                auxP.operator ()(j,i) += M1->operator ()(c,i) * M2->operator ()(j,c);
            }
        }
    }
    return auxP;
}

QMatrix3x4 matMultiply(customQMatrix3x3 *M1, QMatrix3x4 *M2)
{
    //MxC x CxN
    int M, N, C;
    M = 3;
    C = 3;
    N = 4;
    QMatrix3x4 auxP;
    int i, j, c;
    for(i=0;i<M;i++)//Final Row
    {
        for(j=0;j<N;j++)//Final Col
        {
            auxP.operator ()(j,i) = 0;
            for(c=0;c<C;c++)//Calc col
            {
                auxP.operator ()(j,i) += M1->operator ()(c,i) * M2->operator ()(j,c);
            }
        }
    }
    return auxP;
}

QVector3D matMultiply(QMatrix3x4 *M1, QVector4D *M2)
{
    QVector3D res;
    int items;
    double acum;
    items = 3;
    for( int i=0; i<items; i++ )
    {
        acum = 0.0;
        acum += (M1->operator ()(0,i) * M2->x());
        acum += (M1->operator ()(1,i) * M2->y());
        acum += (M1->operator ()(2,i) * M2->z());
        acum += (M1->operator ()(3,i) * M2->w());
        if(i==0)res.setX(acum);
        if(i==1)res.setY(acum);
        if(i==2)res.setZ(acum);
    }
    return res;
}


void funcOpenFolder(QString path)
{
    QDesktopServices::openUrl(QUrl(path));
}

int funcAccountFilesInDir(QString Dir)
{
    int numFiles = 0;
    QDir dir(Dir);
    if ( dir.exists() )
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files, QDir::DirsFirst))
        {
            numFiles++;
        }
    }
    return numFiles;
}

QList<QFileInfo> funcListFilesInDir(QString Dir)
{
    //Dir: Path of interes
    //Return lstFiles
    QList<QFileInfo> lstFiles;
    QDir dir(Dir);
    if ( dir.exists() )
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files, QDir::DirsFirst))
        {
            lstFiles.append(info);
        }
    }
    return lstFiles;
}

QList<QFileInfo> funcListFilesInDirSortByNumberName(QString Dir)
{
    //Dir: Path of interes
    //Return lstFiles
    QList<QFileInfo> lstFilesInfo;
    QList<double> lstFilesNames;
    QDir dir(Dir);
    QString tmpFilename;
    if ( dir.exists() )
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files, QDir::DirsFirst))
        {
            lstFilesNames.append(info.fileName().replace(".png","").toDouble());
        }
    }
    std::sort(lstFilesNames.begin(), lstFilesNames.end());
    int i;

    for(i=0;i<lstFilesNames.size();i++)
    {
        QFileInfo tmpFileInfo(Dir + "/" + QString::number(lstFilesNames.at(i)) + ".png");
        lstFilesInfo.append(tmpFileInfo);
    }
    return lstFilesInfo;
}

QList<QFileInfo> funcListFilesInDir(QString Dir, QString Suffix)
{
    Suffix = Suffix.replace(".","");

    //Only add files with suffix provided
    QList<QFileInfo> lstFiles;
    QDir dir(Dir);
    if ( dir.exists() )
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files, QDir::Time | QDir::Name | QDir::Reversed))
        {
            if( info.completeSuffix() == Suffix )
            {
                lstFiles.append(info);
            }
        }
    }
    return lstFiles;
}

QList<QFileInfo> funcFilterFilelist(QList<QFileInfo> lstFiles, QString suffix)
{
    int i;
    QList<QFileInfo> lstResult;
    for( i=0; i<lstFiles.size(); i++ )
    {
        if( lstFiles.at(i).completeSuffix() == suffix )
        {
            lstResult.append(lstFiles.at(i));
        }
    }
    return lstResult;
}

void calcDiffProj(strDiffProj *diffProj, lstDoubleAxisCalibration *daCalib)
{
    //int offsetX, offsetY;
    int origX, origY;

    origX   = diffProj->x + daCalib->squareUsableX;
    origY   = diffProj->y + daCalib->squareUsableY;



    //offsetX = abs( daCalib->squareUsableX - origX );
    //offsetY = abs( daCalib->squareUsableY - origY );

    //It calculates the jump
    int jumpX, jumpY;
    jumpX = floor(daCalib->LR.waveHorizA + (daCalib->LR.waveHorizB * diffProj->wavelength));
    jumpY = floor(daCalib->LR.waveVertA + (daCalib->LR.waveVertB * diffProj->wavelength));

    //Right
    diffProj->ry = floor(daCalib->LR.horizA + (daCalib->LR.horizB * (double)(origX + jumpX))) + diffProj->y;

    //Left
    diffProj->ly = floor(daCalib->LR.horizA + (daCalib->LR.horizB * (double)(origX - jumpX))) + diffProj->y;

    //Up
    diffProj->ux = floor( daCalib->LR.vertA + ( daCalib->LR.vertB * (double)(origY-jumpY)) ) + diffProj->x;

    //Down
    diffProj->dx = floor( daCalib->LR.vertA + ( daCalib->LR.vertB * (double)(origY+jumpY)) ) + diffProj->x;

    //Fits the original "y"
    diffProj->y  = floor(daCalib->LR.horizA + (daCalib->LR.horizB * (double)origX)) + diffProj->y;
    diffProj->x  = floor(daCalib->LR.vertA + (daCalib->LR.vertB * (double)origY)) + diffProj->x;

    diffProj->rx = diffProj->x + jumpX;
    diffProj->lx = diffProj->x - jumpX;
    diffProj->uy = diffProj->y - jumpY;
    diffProj->dy = diffProj->y + jumpY;

}


void funcClearDirFolder(QString path)
{
    QDir dir(path);
    if (dir.exists())
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            QFile::remove(info.absoluteFilePath());
        }
    }
    else
    {
        dir.mkdir(path);
    }
}

int funcPrintRectangle(QString title, squareAperture* rectangle)
{
    qDebug() << "";
    qDebug() << title;
    qDebug() << "W: " << rectangle->canvasW;
    qDebug() << "H: " << rectangle->canvasH;
    qDebug() << "x: " << rectangle->rectX;
    qDebug() << "y: " << rectangle->rectY;
    qDebug() << "w: " << rectangle->rectW;
    qDebug() << "h: " << rectangle->rectH;
    return 1;
}

extern void* funcAllocInteger3DMatrixMemo( int rows, int cols, int layers, int*** M )
{
    int i, j;

    //Check settings
    if( rows<=0 || cols<=0 || layers<=0 )
    {
        std::cout << "ERROR on funcAllocInteger3DMatrixMemo: mismatch parameters" << std::endl;
        return (void*)NULL;
    }

    //Free Matrix
    if( M != NULL )
    {
        for( i=0; i<rows; i++ )
        {
            if( M[i] != NULL )
            {
                for( j=0; j<cols; j++ )
                {
                    if( M[i][j] != NULL )
                    {
                        free( M[i][j] ) ;
                    }
                }
                free( M[i] );
            }
        }
        free(M);
    }

    //Allocate Matrix
    M = (int***)malloc(rows*sizeof(int**));
    if( M == NULL )
    {
        std::cout << "ERROR Allocating M" << std::endl;
        return (void*)NULL;
    }
    for( i=0; i<rows; i++ )
    {
        M[i] = (int**)malloc(cols*sizeof(int*));
        if( M[i] == NULL )
        {
            std::cout << "ERROR Allocating M[i]" << std::endl;
            return (void*)NULL;
        }
        for( j=0; j<cols; j++ )
        {
            M[i][j] = (int*)malloc(layers*sizeof(int));
            if( M[i][j] == NULL )
            {
                std::cout << "ERROR Allocating M[i][j]" << std::endl;
                return (void*)NULL;
            }
        }
    }
    return M;
}


void displayImageFullScreen( const QImage &tmpImg )
{
    //Set screem geometry
    QPixmap tmpPix = QPixmap::fromImage(tmpImg);
    int screen2Work = (QApplication::desktop()->screenCount()>1)?1:-1;
    QRect screen = QApplication::desktop()->screenGeometry(screen2Work);
    int gvW = (tmpPix.width()<screen.width())?tmpPix.width():screen.width();
    int gvH = (tmpPix.height()<screen.height())?tmpPix.height():screen.height();

    //Fill image
    GraphicsView *gvValCal = new GraphicsView(NULL);
    QGraphicsScene *scene = new QGraphicsScene(0,0,tmpPix.width(),tmpPix.height());
    scene->setBackgroundBrush(tmpPix);
    gvValCal->setScene(scene);
    gvValCal->setGeometry(QRect(0,0,gvW,gvH));
    gvValCal->update();
    gvValCal->show();
}

void funcNDVI(
                QImage* imgToNDVI,
                double lowerBound,
                int brilliant,
                QString Infrared,
                QString Red
){
    //......................................
    // Get Infrared Weight
    //......................................
    QString infraredWeight;
    double infraredWeightRatio;
    infraredWeight = readFileParam( _PATH_NDVI_INFRARED_WEIGHT );
    infraredWeightRatio = infraredWeight.toDouble(0);
    if( infraredWeightRatio <= 0.0 )
    {
        funcShowMsg("FAIL","infraredWeightRatio WRONG!, setted to 1.0");
        infraredWeightRatio = 1.0;
    }

    //......................................
    // Get Minimum Value
    //......................................
    QString stringMinimumValue;
    double minimumValue;
    stringMinimumValue = readFileParam( _PATH_NDVI_MIN_VALUE );
    minimumValue = stringMinimumValue.toDouble(0);
    if( minimumValue < 1.0 || minimumValue > 255.0 )
    {
        funcShowMsg("FAIL","minimumValue WRONG!, setted to 1.0");
        minimumValue = 1.0;
    }

    //......................................
    // Validate lower bound
    //......................................
    if( lowerBound < -1.0 || lowerBound > 1.0 )
    {
        lowerBound = 0.0;
        funcShowMsg("Alert!","Lower bound fixed to 0.0");
    }

    //......................................
    // Apply Classic NDVI
    //......................................
    int x, y;
    double NDVI, maxNDVI, minNDVI;
    QRgb tmpPixel;
    maxNDVI = 0.0;
    minNDVI = 1.0;
    //range   = 1.0 - lowerBound;
    double infraredSensed;
    double redSensed;
    for( x=0; x<imgToNDVI->width(); x++ )
    {
        for( y=0; y<imgToNDVI->height(); y++ )
        {
            //Get wavelengths
            tmpPixel        = imgToNDVI->pixel(x,y);
            infraredSensed  = funcGetPixelColor( &tmpPixel, &Infrared );
            redSensed       = funcGetPixelColor( &tmpPixel, &Red );
            infraredSensed  = (double)infraredSensed * infraredWeightRatio;

            //Validate measuremente
            if( infraredSensed > minimumValue && redSensed > minimumValue )
                NDVI        = (infraredSensed-redSensed)/(infraredSensed+redSensed);
            else
                NDVI        = -1.0;


            //Uses threshold
            NDVI    = (NDVI>lowerBound)?NDVI:-1.0;

            /*
            //Draw pixel
            if( NDVI >= -1.0 && NDVI < -0.2 )
                imgToNDVI->setPixel(x,y,qRgb(0,0,0));       //Negro
            if( NDVI >= -0.2 && NDVI < -0.1 )
                imgToNDVI->setPixel(x,y,qRgb(255,0,0));     //Rojo brillante
            if( NDVI >= -0.1 && NDVI < 0.0 )
                imgToNDVI->setPixel(x,y,qRgb(180,0,0));     //Rojo Sangre
            if( NDVI >= 0.0 && NDVI < 0.1 )
                imgToNDVI->setPixel(x,y,qRgb(90,0,0));      //Rojo Opaco
            if( NDVI >= 0.1 && NDVI < 0.2 )
                imgToNDVI->setPixel(x,y,qRgb(255,255,0));   //Amarillo Brillante
            if( NDVI >= 0.2 && NDVI < 0.3 )
                imgToNDVI->setPixel(x,y,qRgb(255,150,0));   //Amarillo Ocre
            if( NDVI >= 0.3 && NDVI < 0.4 )
                imgToNDVI->setPixel(x,y,qRgb(255,80,0));    //Verde Oliva
            if( NDVI >= 0.4 && NDVI < 0.5 )
                imgToNDVI->setPixel(x,y,qRgb(0,255,255));   //Azul Brillante
            if( NDVI >= 0.5 && NDVI < 0.6 )
                imgToNDVI->setPixel(x,y,qRgb(0,125,125));   //Azul Opaco
            if( NDVI >= 0.6 && NDVI < 0.7 )
                imgToNDVI->setPixel(x,y,qRgb(0,80,80));     //Azul obscuro
            if( NDVI >= 0.7 && NDVI < 0.8 )
                imgToNDVI->setPixel(x,y,qRgb(60,255,0));    //Verde Brillante
            if( NDVI >= 0.8 && NDVI < 0.9 )
                imgToNDVI->setPixel(x,y,qRgb(30,140,0));    //Verde Opaco
            if( NDVI >= 0.9 && NDVI < 1.0 )
                imgToNDVI->setPixel(x,y,qRgb(12,60,0));     //Verde militar*/

            //Draw pixel
            if( NDVI >= -1.0 && NDVI < -0.33 )
                imgToNDVI->setPixel(x,y,qRgb(255,0,0));
            if( NDVI >= -0.33 && NDVI < -0.1 )
                imgToNDVI->setPixel(x,y,qRgb(180,0,0));
            if( NDVI >= -0.1 && NDVI < 0.0 )
                imgToNDVI->setPixel(x,y,qRgb(50,0,0));
            if( NDVI >= 0.0 && NDVI < 0.1 )
                imgToNDVI->setPixel(x,y,qRgb(0,50,0));
            if( NDVI >= 0.1 && NDVI < 0.2 )
                imgToNDVI->setPixel(x,y,qRgb(0,100,0));
            if( NDVI >= 0.2 && NDVI < 0.3 )
                imgToNDVI->setPixel(x,y,qRgb(0,150,0));
            if( NDVI >= 0.3 && NDVI < 0.4 )
                imgToNDVI->setPixel(x,y,qRgb(0,200,0));
            if( NDVI >= 0.4 )
                imgToNDVI->setPixel(x,y,qRgb(0,255,0));




            //Save maximum
            //NDVI        = (NDVI>=lowerBound)?NDVI:0.0;
            maxNDVI     = (NDVI>maxNDVI)?NDVI:maxNDVI;
            minNDVI     = (NDVI<minNDVI)?NDVI:minNDVI;
        }
    }
    qDebug() << "maxNDVI: " << maxNDVI;
    qDebug() << "minNDVI: " << minNDVI;

    //......................................
    // Remark identified plant pixels
    //......................................
    /*
    if( brilliant )
    {
        int curve;
        curve = 255 - round( maxNDVI*255.0 );
        qDebug() << "curve: " << curve;
        if( curve > 5 )
        {
            for( x=0; x<imgToNDVI->width(); x++ )
            {
                for( y=0; y<imgToNDVI->height(); y++ )
                {
                    tmpPixel    = imgToNDVI->pixel(x,y);
                    NDVI        = qRed(tmpPixel) + curve;
                    if( qRed(tmpPixel) > 0 )
                        imgToNDVI->setPixel(x,y,qRgb(NDVI,0,0));
                }
            }
        }
    }
    */
}

int funcGetPixelColor( QRgb* Pixel, QString* Infrared )
{
    int tmpValue;
    tmpValue = 0;
    if( *Infrared == "Red" )
        tmpValue = qRed(*Pixel);
    if( *Infrared == "Green" )
        tmpValue = qGreen(*Pixel);
    if( *Infrared == "Blue" )
        tmpValue = qBlue(*Pixel);
    return tmpValue;
}

int funcReadAnalysePlot( structAnalysePlotSaved* structPlotSaved )
{

    QString tmpParameter = readFileParam( _PATH_SLIDE_FLUORESCENT );
    if( tmpParameter.isEmpty() )
    {
        qDebug() << _PATH_SLIDE_FLUORESCENT << " is empty";
        return _ERROR;
    }

    structPlotSaved->red        = tmpParameter.split(",").at(0).toInt(0);
    structPlotSaved->green      = tmpParameter.split(",").at(1).toInt(0);
    structPlotSaved->blue       = tmpParameter.split(",").at(2).toInt(0);
    structPlotSaved->canvasW    = tmpParameter.split(",").at(3).toInt(0);
    structPlotSaved->canvasH    = tmpParameter.split(",").at(4).toInt(0);
    structPlotSaved->originalW  = tmpParameter.split(",").at(5).toInt(0);
    structPlotSaved->originalH  = tmpParameter.split(",").at(6).toInt(0);

    return _OK;
}

void rotateQImage(QImage* tmpImg, int degree)
{
    QTransform transformation;
    transformation.rotate(degree);
    *tmpImg = tmpImg->transformed(transformation);
}

QRect screenResolution(QWidget* reference)
{
    return QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(reference));
}

int* imageDecriptor(QImage* img, bool horizontal)
{
    int x, y;
    int* descriptor;
    int acum;
    if( horizontal == true )
    {
        descriptor = (int*)malloc(img->width()*sizeof(int));
        for( x=0; x<img->width(); x++ )
        {
            acum = 0;
            for( y=0; y<img->height(); y++ )
            {
                acum += pixelMaxValue( img->pixel(x,y) );
            }
            descriptor[x] = acum;
        }
    }
    else
    {
        descriptor = (int*)malloc(img->height()*sizeof(int));
        for( y=0; y<img->height(); y++ )
        {
            acum = 0;
            for( x=0; x<img->width(); x++ )
            {
                acum += pixelMaxValue( img->pixel(x,y) );
            }
            descriptor[y] = acum;
        }
    }
    return descriptor;
}

int pixelMaxValue( QRgb pixel )
{
    int maxVal;
    maxVal = (qRed(pixel)>=qGreen(pixel))?qRed(pixel):qGreen(pixel);
    maxVal = (maxVal>=qBlue(pixel))?maxVal:qBlue(pixel);
    return maxVal;
}

int pixelMaxValue( const QColor &color )
{
    int maxVal;
    maxVal = (color.red()>=color.green())?color.red():color.green();
    maxVal = (maxVal>=color.blue())?maxVal:color.blue();
    return maxVal;
}

QPoint imageSimilarity2D(QImage* img1, QImage* img2, float maxShift, bool horizontal)
{
    //maxShift is the percentage allowed to explore similarity

    //-----------------------------------------------------
    //Containers and Variables
    //-----------------------------------------------------
    QPoint shift2D;
    shift2D.setX(0);
    shift2D.setY(0);
    if( img1->width() < 1 || img1->height() < 1 )
        return shift2D;


    int k, n, pixelRange, numShifts, zeroError, kFix;
    pixelRange  = (horizontal==true)?round(img1->width()*maxShift):round(img1->height()*maxShift);
    numShifts   = (2*pixelRange)+1;
    n           = (horizontal==true)?img1->width():img1->height();

    //-----------------------------------------------------
    //Shifting
    //-----------------------------------------------------
    int kStart, kEnd;
    float minError, tmpError, shiftFix;
    kStart      = numShifts*(-1);
    kEnd        = numShifts;
    zeroError   = squareImageDifferenece( img1, img2, 0, horizontal );
    kFix        = (float)zeroError / (float)n;
    minError    = zeroError;
    for( k=kStart; k<kEnd; k++ )
    {
        shiftFix    = abs(k) * kFix;
        tmpError    = squareImageDifferenece( img1, img2, k, horizontal ) + shiftFix;
        if( tmpError < minError )
        {
            minError    = tmpError;
            shift2D.setY(k);
        }
    }

    return shift2D;
}

int squareImageDifferenece(QImage* img1, QImage* img2, int k, bool horizontal)
{
    int x, y, n, start, end, acum;
    QRgb pixelLeft, pixelRight;
    acum = 0;
    if(horizontal==true)
    {
        //...........................................
        //Horizontal
        //...........................................
        if( abs(k)>=img1->width() )
        {
            return 0;
        }
        n       = img1->width();
        end     = (k>=0)?n:n+k;
        start   = (k>=0)?k:0;
        for( x=start; x<end; x++ )
        {
            for( y=0; y<img1->height(); y++ )
            {
                pixelLeft   = img1->pixel( x, y );
                pixelRight  = img2->pixel( x-k, y );
                acum       += (qRed(pixelLeft)-qRed(pixelRight))*(qRed(pixelLeft)-qRed(pixelRight));
                acum       += (qGreen(pixelLeft)-qGreen(pixelRight))*(qGreen(pixelLeft)-qGreen(pixelRight));
                acum       += (qBlue(pixelLeft)-qBlue(pixelRight))*(qBlue(pixelLeft)-qBlue(pixelRight));
            }
        }
    }
    else
    {
        //...........................................
        //Vertical
        //...........................................
        if( abs(k)>=img1->height() )
        {
            return 0;
        }
        n       = img1->height();
        end     = (k>=0)?n:n+k;
        start   = (k>=0)?k:0;
        for( y=start; y<end; y++ )
        {
            for( x=0; x<img1->width(); x++ )
            {
                pixelLeft   = img1->pixel( x, y );
                pixelRight  = img2->pixel( x, y-k );
                acum       += (qRed(pixelLeft)-qRed(pixelRight))*(qRed(pixelLeft)-qRed(pixelRight));
                acum       += (qGreen(pixelLeft)-qGreen(pixelRight))*(qGreen(pixelLeft)-qGreen(pixelRight));
                acum       += (qBlue(pixelLeft)-qBlue(pixelRight))*(qBlue(pixelLeft)-qBlue(pixelRight));
            }
        }
    }
    return acum;
}

int vectorSimilarity(int* v1, int* v2, int n, float lang)
{
    float* c    = vectorCrossCorrelation(v1,v2,n,lang);
    return c[0]-n;
}

int* vectorConvolution(int* v1, int* v2, int n)
{

    //[Comment]: Apply Convolution, v1=x(n) and v2 = h(n)
    //           v1 and v3 have same dimensions
    //           v3[0] contains the position with the bigger value
    //           v3[1-n] contains the convolution result

    int numElem = 2*n;
    //qDebug() << "numElem: " << numElem;

    int i, j, maxAcum, aux, v1Pos, v2Pos, cPos;
    maxAcum  = 0;
    cPos     = 1;

    int* v3 = (int*)malloc(numElem*sizeof(int));
    memset( v3, '\0', numElem*sizeof(int));

    //Outside-Left to the Overlapped
    for( i=0; i<n; i++ )
    {
        aux = 0;
        for( j=0; j<=i; j++ )
        {
            v1Pos   = i-j;      //->
            v2Pos   = j;    //<-
            aux    += v1[v1Pos] * v2[v2Pos];
            //qDebug() << v1[v1Pos] << " x " << v2[v2Pos] << " -> " << aux;
        }
        //qDebug() << "---";
        //qDebug() << "i: " << i << " val: " << aux;
        v3[cPos] = aux;
        if( aux>maxAcum )
        {
            maxAcum  = aux;
            v3[0]    = cPos;
        }
        cPos++;
    }

    /*
    for( i=0; i<numElem; i++ )
    {
        qDebug() << v3[i];
    }
    exit(0);*/


    //Overlapped to Outside-Right
    for( i=1; i<n; i++ )
    {
        aux = 0;
        for( j=0; j<n-i; j++ )
        {
            v1Pos   = i+j;      //->
            v2Pos   = n-1-j;        //<-
            aux    += v1[v1Pos] * v2[v2Pos];
            //qDebug() << v1[v1Pos] << " x " << v2[v2Pos] << " -> " << aux;
        }
        //qDebug() << "---";
        //qDebug() << "i: " << i << " val: " << aux;
        v3[cPos] = aux;
        if( aux>maxAcum )
        {
            maxAcum  = aux;
            v3[0]    = cPos;
        }
        cPos++;
    }

    /*
    for( i=0; i<numElem; i++ )
    {
        qDebug() << v3[i];
    }
    exit(0);*/

    return v3;
}


float* vectorCrossCorrelation(int* v1, int* v2, int n, float lang)
{
    int k;
    int numElem = (2*n)+1;
    float* v3 = (float*)malloc(numElem*sizeof(float));
    memset( v3, '\0', numElem*sizeof(float));
    //lang = (lang>0.5)?0.5:lang;

    //---------------------------------------------
    //Calculate Correlation Zero Shift
    //---------------------------------------------
    float zeroCorr;
    zeroCorr        = vectorCorrelation(v1,v1,n,0);
    int maxShift    = floor((float)n*lang);
    maxShift        = ( maxShift > n-1 )?n-1:maxShift;
    int minShift    = maxShift*(-1);
    float maxCorr   = 0.0;
    for( k=minShift; k<=maxShift; k++ )
    {
        v3[n+k] = (k!=0)?vectorCorrelation(v1,v2,n,k,zeroCorr):zeroCorr;
        if( v3[n+k] > maxCorr )
        {
            maxCorr = v3[n+k];
            v3[0]   = n+k;
        }
    }

    /*
    for( k=0; k<=((2*n)-1); k++ )
    {
        qDebug() << "k: " << k-n << " v3: " << v3[k];
    }
    exit(0);*/


    return v3;
}

float vectorSimpleCorrelation(int* v1, int* v2, int n, int k)
{
    if( k>=n || k<((-1)*n) )
        return 0.0;
    int vectorAux[n];
    memset( vectorAux, '\0', (sizeof(int)*n) );

    //--------------------------------------------
    //Shift vectorAux=v2 if Required
    //--------------------------------------------
    int i, acum, begin, end;
    if( k>0  ){memcpy(vectorAux,&v2[k],(sizeof(int)*(n-k)));begin=0;end=n-k;}
    if( k<0  ){k=abs(k);memcpy(&vectorAux[k],v2,(sizeof(int)*(n-k)));begin=k;end=n;}
    if( k==0 ){memcpy( vectorAux, v2, (sizeof(int)*n) );begin=0;end=n;}

    //qDebug() << "begin: " << begin;
    //qDebug() << "end: " << end;

    //--------------------------------------------
    //Multiply v1 and v2=vectorAux
    //--------------------------------------------
    acum = 0;
    for( i=begin; i<end; i++ )
    {        
        acum += v1[i] * vectorAux[i];
        //qDebug() << v1[i] << " x " << vectorAux[i] << ": " << acum;
    }

    //--------------------------------------------
    //Calculate the Result
    //--------------------------------------------
    return (1.0/(float)n) * (float)acum;
}


float vectorCorrelation(int* v1, int* v2, int n, int k)
{
    //Calculate Raw Correlation
    float zeroCorr, corr, error;
    corr        = vectorSimpleCorrelation(v1,v2,n,k);

    //Fix Shipping Errro
    k = abs(k);
    zeroCorr    = vectorSimpleCorrelation(v1,v2,n,0);
    error       = 0;
    if( k != 0 )
    {
        error = ((float)k/(float)n)*zeroCorr;
        //qDebug() << "zeroCorr: " << zeroCorr << " corr: " << corr << " error: " << error;
        corr += error;
        //qDebug() << "corr final: " << corr;
    }
    //exit(0);

    return corr;
}

float vectorCorrelation(int* v1, int* v2, int n, int k, float zeroCorr)
{
    //Calculate Raw Correlation
    float corr, error;
    corr        = vectorSimpleCorrelation(v1,v2,n,k);

    //Fix Shipping Error
    k = abs(k);
    error       = 0;
    if( k != 0 )
    {
        error = ((float)k/(float)n)*zeroCorr;
        corr += error;
    }

    return corr;
}


int calcSlideExtraW(structSlideHypCube* slideSett)
{
    return floor((float)slideSett->width * slideSett->extraW);
}

QString funcGetParam(QString label)
{
    bool ok;
    return QInputDialog::getText(NULL, "Input required...",
                                                        label+":", QLineEdit::Normal,
                                                        "", &ok);
}

QString funcGetParam(QString label, QString defaultValue)
{
    bool ok;
    return QInputDialog::getText(
                                    NULL,
                                    "Input required...",
                                    label+":", QLineEdit::Normal,
                                    defaultValue,
                                    &ok
                                );
}

int funcSaveXML(
                    const QString &fileName,
                    QList<QString>* lstFixtures,
                    QList<QString>* lstValues,
                    bool notify
){
    //Receives filename only, without path
    //[path=null][filename].xml

    //-----------------------------------
    //Validate XML contain
    //-----------------------------------
    if( lstFixtures->size() != lstValues->size() )
    {
        funcShowMsgERROR(
                            "Fixtures("+
                            QString::number(lstFixtures->size())+
                            ") and Values("+
                            QString::number(lstValues->size())+
                            ") have diferent size for: " + fileName
                        );
        return _ERROR;
    }

    //-----------------------------------
    //Save File XML
    //-----------------------------------
    //Validate Filename
    QString fileNameFixed;
    fileNameFixed.append(fileName);
    funcGuaranteeExtension(&fileNameFixed,"xml");
    //Prepare file contain
    QString tmpContain;
    tmpContain.append( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" );
    tmpContain.append("<Variables>\n");
    int i;
    for(i=0; i<lstFixtures->size(); i++)
    {
        tmpContain.append("\t");
        tmpContain.append("<"+ lstFixtures->at(i) +">");
        tmpContain.append(lstValues->at(i));
        tmpContain.append("</"+ lstFixtures->at(i) +">\n");
    }
    tmpContain.append("</Variables>");

    //Save file
    if( saveFile( fileNameFixed, tmpContain ) == false )
    {
        //funcShowMsg("ERROR","Saving XML");
        return _ERROR;
    }
    else
    {
        if( notify == true )
        {
            funcShowMsg("Success","XML saved successfully");
        }
    }
    return _OK;
}

void funcGuaranteeExtension(QString* filename, QString extension)
{
    //Asumes that extension = "ext" without "."
    QFileInfo fi(*filename);
    *filename = filename->replace("."+fi.suffix(),"") + "." + extension.replace(".","");
}

int funcReadLineFromXML(QString* filePath, structLine* tmpLine)
{
    //---------------------------------------
    //Extract XML
    //---------------------------------------
    QFile *xmlFile = new QFile(*filePath);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        funcShowMsg("ERROR","Opening: "+*filePath);
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
            //if( xmlReader->name()=="canvasW" )
            //    tmpLine->canvasW = xmlReader->readElementText().trimmed().toInt(0);
            //if( xmlReader->name()=="canvasH" )
            //    tmpLine->canvasH = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="imgW" )
                tmpLine->originalW = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="imgH" )
                tmpLine->originalH = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="x1" )
                tmpLine->x1 = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="y1" )
                tmpLine->y1 = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="x2" )
                tmpLine->x2 = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="y2" )
                tmpLine->y2 = xmlReader->readElementText().trimmed().toInt(0);
            //if( xmlReader->name()=="m" )
            //    tmpLine->m = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="colorR" )
                tmpLine->colorR = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="colorG" )
                tmpLine->colorG = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="colorB" )
                tmpLine->colorB = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="wavelength" )
                tmpLine->wavelength = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="orientation" )
                tmpLine->oritation = xmlReader->readElementText().trimmed().toInt(0);
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();

    return _OK;
}

int funcLetUserSelectFile(
                            QString* filePath,
                            const QString &title,
                            const QString &pathLocation,
                            const QString &defaultLastPath,
                            QWidget* parent
){
    //filePath:         File output, filename selected by the user
    //title:            Showed to User, what kind of file is the user selecting
    //pathLocation:     Where is saved the last path location saved
    //pathOfInterest:   If it is the first time, what path will be saved as default
    //parent:           In order to use this Dialog
    QString lastPath = readFileParam(pathLocation);
    if( lastPath.isEmpty() )//First time using this parameter
    {
        lastPath = defaultLastPath;
    }

    //Select image
    //..

    *filePath = QFileDialog::getOpenFileName(
                                                parent,
                                                title,
                                                lastPath,
                                                "(*.*);;"
                                             );
    if( filePath->isEmpty() )
    {
        return _ERROR;
    }
    else
    {
        //Save Folder in order to Speed up File Selection
        lastPath = funcRemoveFileNameFromPath(*filePath);
        //funcShowMsg("pathLocation",*pathLocation);
        //funcShowMsg("lastPath",lastPath);
        saveFile(pathLocation,lastPath);
    }
    return _OK;
}

int funcLetUserSelectFile(
                            QString* filePath,
                            const QString &title,
                            QWidget* parent
){
    QString lastPath = readFileParam(_PATH_LAST_PATH_OPENED);
    if( lastPath.isEmpty() )//First time using this parameter
    {
        lastPath = "./";
    }

    //Select image
    //..

    *filePath = QFileDialog::getOpenFileName(
                                                parent,
                                                title,
                                                lastPath,
                                                "(*.*);;"
                                             );
    if( filePath->isEmpty() )
    {
        return _ERROR;
    }
    else
    {
        //Save Folder in order to Speed up File Selection
        lastPath = funcRemoveFileNameFromPath(*filePath);
        saveFile(_PATH_LAST_PATH_OPENED,lastPath);
    }
    return _OK;
}

int funcLetUserDefineFile(
                            QString* filePath,
                            QString title,
                            QString extension,
                            QString* pathLocation,
                            QString* defaultLastPath,
                            QWidget* parent
){
    //filePath:         File output, filename selected by the user
    //title:            Showed to User, what kind of file is the user selecting
    //pathLocation:     Where is saved the last path location saved
    //defaultLastPath:  If it is the first time, what path will be saved as default
    //parent:           In order to use this Dialog

    QString lastPath = readFileParam(*pathLocation);
    if( lastPath.isEmpty() )//First time using this parameter
    {
        lastPath = defaultLastPath->toStdString().c_str();
    }

    //Select image
    //..
    *filePath = QFileDialog::getSaveFileName(
                                                    parent,
                                                    title,
                                                    lastPath,
                                                    QWidget::tr("Documents (*.*)")
                                            );
    if( filePath->isEmpty() )
    {
        return _ERROR;
    }
    else
    {
        //Guarantee Extension
        funcGuaranteeExtension(filePath,extension);

        //Save last file open
        saveFile(*pathLocation,*filePath);

        //Save Folder in order to Speed up File Selection
        lastPath = funcRemoveFileNameFromPath(*filePath);
        saveFile(*pathLocation,lastPath);
    }

    return _OK;
}

void funcShowMsg_Timeout(QString title, QString msg, QMessageBox::Icon iconSelected, QWidget *parent, int ms)
{
    QMessageBox *msgBox         = new QMessageBox(iconSelected,title,msg,NULL);
    QTimer *msgBoxCloseTimer    = new QTimer(parent);
    msgBoxCloseTimer->setInterval(ms);
    msgBoxCloseTimer->setSingleShot(true);
    parent->connect(msgBoxCloseTimer, SIGNAL(timeout()), msgBox, SLOT(reject()));
    msgBoxCloseTimer->start();
    msgBox->exec();
}

void funcShowMsgERROR_Timeout(QString msg, QWidget* parent, int ms)
{
    QMessageBox *msgBox         = new QMessageBox(QMessageBox::Warning,"ERROR",msg,NULL);
    QTimer *msgBoxCloseTimer    = new QTimer(parent);
    msgBoxCloseTimer->setInterval(ms);
    msgBoxCloseTimer->setSingleShot(true);
    parent->connect(msgBoxCloseTimer, SIGNAL(timeout()), msgBox, SLOT(reject()));
    msgBoxCloseTimer->start();
    msgBox->exec();
}


void funcShowMsgSUCCESS_Timeout(QString msg, QWidget* parent, int ms)
{
    QMessageBox *msgBox         = new QMessageBox(QMessageBox::Information,"SUCCESS",msg,NULL);
    QTimer *msgBoxCloseTimer    = new QTimer(parent);
    msgBoxCloseTimer->setInterval(ms);
    msgBoxCloseTimer->setSingleShot(true);
    parent->connect(msgBoxCloseTimer, SIGNAL(timeout()), msgBox, SLOT(reject()));
    msgBoxCloseTimer->start();
    msgBox->exec();
}

void mouseCursorWait(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void mouseCursorReset(){
    QApplication::restoreOverrideCursor();
}

void funcExportLineToXML(structLine* tmpLine, const QString filePath)
{
    //----------------------------------------
    //Save line parameters
    //----------------------------------------
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures.clear();
    lstValues.clear();
    lstFixtures << "canvasW" << "canvasH" << "x1" << "y1" << "x2" << "y2" << "m" << "colorR" << "colorG" << "colorB" << "wavelength" << "orientation";
    lstValues.append(QString::number(tmpLine->canvasW));
    lstValues.append(QString::number(tmpLine->canvasH));
    lstValues.append(QString::number(tmpLine->x1));
    lstValues.append(QString::number(tmpLine->y1));
    lstValues.append(QString::number(tmpLine->x2));
    lstValues.append(QString::number(tmpLine->y2));
    //lstValues.append(QString::number(tmpLine->m));
    lstValues.append(QString::number(tmpLine->colorR));
    lstValues.append(QString::number(tmpLine->colorG));
    lstValues.append(QString::number(tmpLine->colorB));
    lstValues.append(QString::number(tmpLine->wavelength));
    lstValues.append(QString::number(tmpLine->oritation));

    //Save line
    if( funcSaveXML(filePath,&lstFixtures,&lstValues) != _OK )
    {
        funcShowMsgERROR("Saving Line Parameters");
    }
}

/*
int funcCalc_X_SlopToPoint(int newY, structLine* internLine)
{
    int newX;
    float origX, origY, m;
    origX   = (float)internLine->x1;
    origY   = (float)internLine->y1;
    m       = (float)internLine->m;
    newX    = round( ((newY-origY) / m) + origX );
    //std::cout << "((" << newY << " - " << origY << ") / " << m << ") + " << origX << " = " << newX << std::endl;
    return newX;
}

int funcCalc_Y_SlopToPoint(int newX, structLine* internLine)
{
    int newY;
    float origX, origY, m;
    origX   = (float)internLine->x1;
    origY   = (float)internLine->y1;
    m       = (float)internLine->m;
    newY    = round( (m*(newX - origX))+origY );
    //std::cout << m << "( " << newX << " - " << origX << ") + " << origY << std::endl;
    return newY;
}*/

/*
int funcMergeSlideCalib(
                            const QString &vertPath,
                            const QString &horizPath,
                            structSlideCalibration* slideCalibration
){
    //------------------------------------------------
    //Read Calibration Parts
    //------------------------------------------------
    int referenceX2 = 0;
    funcReadHorHalfCalib(horizPath,slideCalibration);
    funcReadVertHalfCalib(vertPath,&referenceX2,slideCalibration);

    //------------------------------------------------
    //Merge Calibration Parts
    //------------------------------------------------
}*/

int funcReadVertHalfCalib(
                            const QString &filePath,
                            float* referenceX2,
                            structSlideCalibration* slideCalibration
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
        if(token == QXmlStreamReader::StartElement) {
            if( xmlReader->name()=="imgW" )
                slideCalibration->imgW = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="imgH" )
                slideCalibration->imgH = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="lowerBoundX2" )
                *referenceX2 = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="lowerBoundWave" )
                slideCalibration->originWave = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="higherBoundWave" )
                slideCalibration->maxWave = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="dist2WaveA" )
                slideCalibration->dist2WaveLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="dist2WaveB" )
                slideCalibration->dist2WaveLR.b = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="wave2DistA" )
                slideCalibration->wave2DistLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="wave2DistB" )
                slideCalibration->wave2DistLR.b = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="vertA" )
                slideCalibration->vertLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="vertB" )
                slideCalibration->vertLR.b = xmlReader->readElementText().toFloat(0);
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();
    return _OK;
}

int funcReadHorHalfCalib(
                            const QString &filePath,
                            structSlideCalibration* slideCalibration
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
        if(token == QXmlStreamReader::StartElement) {
            if( xmlReader->name()=="H" )
                slideCalibration->originH = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="a" )
                slideCalibration->horizLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="b" )
                slideCalibration->horizLR.b = xmlReader->readElementText().toFloat(0);
        }
    }

    //Set translation
    //slideCalibration->translation.setMatrix( m11, m12, m13, m21, m22, m23, m31, m32, m33 );

    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();
    return _OK;
}

int funcReadAffineTransXML( const QString &filePath, QTransform* T )
{

    QFile *xmlFile = new QFile( filePath );
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return _ERROR;
    }
    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

    //Parse the XML until we reach end of it
    double m11=0.0, m12=0.0, m13=0.0, m21=0.0, m22=0.0, m23=0.0, m31=0.0, m32=0.0, m33=0.0;
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if( xmlReader->name()=="Tm11" )
                m11 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm12" )
                m12 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm13" )
                m13 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm21" )
                m21 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm22" )
                m22 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm23" )
                m23 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm31" )
                m31 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm32" )
                m32 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm33" )
                m33 = xmlReader->readElementText().toDouble(0);
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();


    //Set translation
    T->setMatrix( m11, m12, m13, m21, m22, m23, m31, m32, m33 );

    return _OK;
}

cameraResolution* getCamRes( int megapixels )
{
    //cameraResolution* camRes;
    //camRes = (cameraResolution*)malloc(sizeof(cameraResolution));

    if( megapixels == 5 )
    {
        //#define _BIG_WIDTH 2592 //2592 | 640 | 320
        //#define _BIG_HEIGHT 1944 //1944 | 480 | 240
        camRes->width   = _RASP_CAM_5MP_IMAGE_W;
        camRes->height  = _RASP_CAM_5MP_IMAGE_H;
        camRes->videoW  = _RASP_CAM_5MP_VIDEO_W; //1920 | 1640
        camRes->videoH  = _RASP_CAM_5MP_VIDEO_H; //1080 | 1232
    }
    else
    {
        if( megapixels == 8 )
        {
            //https://www.raspberrypi.org/forums/viewtopic.php?t=145815
            camRes->width   = _RASP_CAM_8MP_IMAGE_W;
            camRes->height  = _RASP_CAM_8MP_IMAGE_H;
            camRes->videoW  = _RASP_CAM_8MP_VIDEO_W; //1920 | 1640
            camRes->videoH  = _RASP_CAM_8MP_VIDEO_H; //1080 | 1232
        }
    }

    return camRes;

}

int scaleLen( int len, int canvasLen, int originalLen )
{
    return round(((float)(len) / (float)canvasLen)*(float)originalLen);
}

int funcReadSlideCalib( const QString &filePath, structSlideCalibration* slideCalibration )
{
    //------------------------------------------
    //Read File from XML
    //------------------------------------------
    QFile *xmlFile = new QFile( filePath );
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return _ERROR;
    }
    QXmlStreamReader *xmlReader = new QXmlStreamReader(xmlFile);

    double m11=0.0, m12=0.0, m13=0.0, m21=0.0, m22=0.0, m23=0.0, m31=0.0, m32=0.0, m33=0.0;


    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if( xmlReader->name()=="imgW" )
                slideCalibration->imgW = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="imgH" )
                slideCalibration->imgH = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="originX" )
                slideCalibration->originX = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="originY" )
                slideCalibration->originY = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="originH" )
                slideCalibration->originH = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="originWave" )
                slideCalibration->originWave = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="maxWavelen" )
                slideCalibration->maxWave = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="dist2WaveA" )
                slideCalibration->dist2WaveLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="dist2WaveB" )
                slideCalibration->dist2WaveLR.b = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="wave2DistA" )
                slideCalibration->wave2DistLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="wave2DistB" )
                slideCalibration->wave2DistLR.b = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="vertA" )
                slideCalibration->vertLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="vertB" )
                slideCalibration->vertLR.b = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="horizA" )
                slideCalibration->horizLR.a = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="horizB" )
                slideCalibration->horizLR.b = xmlReader->readElementText().toFloat(0);
            if( xmlReader->name()=="maxNumCols" )
                slideCalibration->maxNumCols = xmlReader->readElementText().toInt(0);
            if( xmlReader->name()=="Tm11" )
                m11 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm12" )
                m12 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm13" )
                m13 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm21" )
                m21 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm22" )
                m22 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm23" )
                m23 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm31" )
                m31 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm32" )
                m32 = xmlReader->readElementText().toDouble(0);
            if( xmlReader->name()=="Tm33" )
                m33 = xmlReader->readElementText().toDouble(0);
            //-------------------------------------------------------------------
            if( xmlReader->name()=="originalR" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.originalR );
            }
            if( xmlReader->name()=="originalG" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.originalG );
            }
            if( xmlReader->name()=="originalB" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.originalB );
            }
            if( xmlReader->name()=="originalH" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.originalH );
            }
            //-------------------------------------------------------------------
            if( xmlReader->name()=="normedOrigR" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.normedOrigR );
            }
            if( xmlReader->name()=="normedOrigG" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.normedOrigG );
            }
            if( xmlReader->name()=="normedOrigB" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.normedOrigB );
            }
            //-------------------------------------------------------------------
            if( xmlReader->name()=="normedRalfR" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.normedRalfR );
            }
            if( xmlReader->name()=="normedRalfG" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.normedRalfG );
            }
            if( xmlReader->name()=="normedRalfB" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.normedRalfB );
            }
            if( xmlReader->name()=="normedRalfH" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.normedRalfH );
            }
            //-------------------------------------------------------------------
            if( xmlReader->name()=="wR" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.wR );
            }
            if( xmlReader->name()=="wG" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.wG );
            }
            if( xmlReader->name()=="wB" )
            {
                slideCalibration->sensitivities.filled = 1;
                funcCSVToFloat( xmlReader->readElementText().trimmed(),
                                &slideCalibration->sensitivities.wB );
            }
            //-------------------------------------------------------------------
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();

    //Save Transformation
    slideCalibration->translation.setMatrix(m11,m12,m13,m21,m22,m23,m31,m32,m33);

    return _OK;

}

int funcCSVToFloat(const QString &CSV, QList<float>* lst )
{
    QList<QString> lstSlides;
    lstSlides = CSV.trimmed().split(",");
    int i;
    for(i=0; i<lstSlides.size(); i++)
    {
        lst->append( lstSlides.at(i).trimmed().toFloat(0) );
    }
    return _OK;
}

float funcApplyLR(
                    const float &coordinate,
                    const linearRegresion &LR,
                    bool print
){
    if( print == true )
    {

        std::cout << "(" << coordinate << "*" << LR.b << ") + " << LR.a << " = " << ((coordinate * LR.b) + LR.a) << std::endl;
    }
    return (coordinate * LR.b) + LR.a;
}

QPoint funcGetCoor(
                        int tmpX,
                        int tmpY,
                        const structSlideCalibration &slideCalibration
){
    //It Receives
    //tmpX: [0-maxNumCols-1]
    //tmpY: [0-originH-1]
    int x, y;
    //x = slideCalibration.originX + tmpX;
    //y = slideCalibration.originY + tmpY;

    //Before Affine Transformation
    y     = round(
                    funcApplyLR(
                                    (float)(tmpX+slideCalibration.originX),
                                    slideCalibration.horizLR,
                                    false
                                )
                 )+tmpY;

    x     = round(
                    funcApplyLR(
                                    (float)(tmpY+slideCalibration.originY),
                                    slideCalibration.vertLR,
                                    false
                                )
                 )+tmpX;

    //std::cout << "x: " << x << " y: " << y << std::endl;

    return QPoint(x,y);
}

int funcGetPixQE(
                        const int &x,
                        const int &y,
                        float*  pixQE,
                        const QImage &tmpImg,
                        const structSlideCalibration &slideCalibration
){
    //---------------------------------------------
    // Get Pixel Location
    //---------------------------------------------    
    QPoint pixLocation = funcGetCoor( x, y, slideCalibration );
    if(
            pixLocation.x() > tmpImg.width()    ||
            pixLocation.y() > tmpImg.height()
    ){
        std::cout << "x: "  << x << " y: " << y
                  << " xx: " << pixLocation.x() << " yy: " << pixLocation.y()
                  << " w: "  << tmpImg.width()  << " h: " << tmpImg.height()
                  << std::endl;

        return _ERROR;
    }
    QColor pixel  = tmpImg.pixelColor( pixLocation.x(), pixLocation.y() );
    //std::cout   << "original x: " << *x << " y: " << *y
    //            << " scaled  x: " << pixLocation.x() << " y: " << pixLocation.y() << std::endl;

    //---------------------------------------------
    // Calculate pixel's QE
    // Temporally, QE is the max value sensed
    //---------------------------------------------
    *pixQE = (float)pixelMaxValue(pixel);

    return _OK;
}

int funcGetQEArea(
                    const QPoint &p1,
                    const QPoint &p2,
                    int** tmpSubarea,
                    const QImage &img,
                    const structSlideCalibration &slideCalibration
){
    int row, col;
    int W, H;
    //get the hight
    H = p2.y() - p1.y();
    W = p2.x() - p1.x();
    if( H<=0 || W<=0 )
    {
        std::cout << "Points H Wrong" << std::endl;
        return _ERROR;
    }

    //---------------------------
    //Copy QE
    //---------------------------
    float tmpQE;
    int tmpPosX;
    for(row=0; row<H; row++)
    {
        for(col=0; col<W; col++)
        {
            tmpPosX = col + p1.x();
            if(
                    funcGetPixQE(
                                    tmpPosX,
                                    row,
                                    &tmpQE,
                                    img,
                                    slideCalibration
                                ) != _OK
            ){
                funcShowMsgERROR(
                                    "Reading QE: "+
                                    QString::number(row) +
                                    ", " +
                                    QString::number( col + p1.x() )
                                );
                return _ERROR;
            }

            //Save QE
            //std::cout << "tmpQE: " << tmpQE << " H: " << H << " W: " << W  << " col: " << col << " row: " << row << " tmpPosX: " << tmpPosX << std::endl;
            if( tmpQE < 0 || tmpQE >255 )
            {
                std::cout << "tmpQE: " << tmpQE << " H: " << H << " W: " << W  << " col: " << col << " row: " << row << " tmpPosX: " << tmpPosX << std::endl;
                funcShowMsgERROR("tmpQE: "+QString::number(tmpQE));
                exit(0);
            }
            tmpSubarea[row][col] = round(tmpQE);
        }
    }

    return _OK;
}

int funcLetUserSelectDirectory(
                                    const QString &pathToLastFilenameParam,
                                    QString* pathDirContainer
){
    //pathToLastFilenameParam: File path containing the last dirpath selected
    //pathDirContainer: Used to return the new dirpath selected

    QString originPath = readAllFile(pathToLastFilenameParam.trimmed()).trimmed();

    //Get Directory
    if( funcShowSelDir(originPath, pathDirContainer) != _OK )
    {
        return _ERROR;
    }

    //Update Original Path
    saveFile(pathToLastFilenameParam,*pathDirContainer);

    return _OK;
}

int funcGetTranslation( QTransform* tmpTrans, QWidget *parent )
{
    //-------------------------------------
    //Read lines
    //-------------------------------------
    QString lowerLinePath, upperLinePath;
    //Read Lower Line
    if(
            funcLetUserSelectFile(
                                    &lowerLinePath,
                                    "Select Horizontal Lower Bound...",
                                    _PATH_LAST_LINE_OPENED,
                                    "./XML/lines/",
                                    parent
                                 ) != _OK
    ){
        return _ERROR;
    }

    //Read Upper Line
    if(
            funcLetUserSelectFile(
                                    &upperLinePath,
                                    "Select Horizontal Upper Bound...",
                                    _PATH_LAST_LINE_OPENED,
                                    "./XML/lines/",
                                    parent
                                 ) != _OK
    ){
        return _ERROR;
    }

    //-------------------------------------
    //Read Lines FromHDD
    //-------------------------------------
    structLine lowerLine, upperLine;
    funcReadLineFromXML(&lowerLinePath,&lowerLine);
    funcReadLineFromXML(&upperLinePath,&upperLine);


    if( funcLines2Translation(tmpTrans, lowerLine, upperLine) != _OK )
    {
        return _ERROR;
    }

    return _OK;
}


int funcLines2Translation(
                            QTransform* tmpTrans,
                            const structLine &lowerLine,
                            const structLine &upperLine
){
    //-------------------------------------
    //Define the quad transformation
    //-------------------------------------
    //Original Points
    QVector<QPointF> originPoints;
    originPoints.append( QPointF(upperLine.x1,upperLine.y1) );
    originPoints.append( QPointF(upperLine.x2,upperLine.y2) );
    originPoints.append( QPointF(lowerLine.x1,lowerLine.y1) );
    originPoints.append( QPointF(lowerLine.x2,lowerLine.y2) );
    //Destine Points
    QVector<QPointF> destinePoints;
    destinePoints.append( QPointF(upperLine.x1,upperLine.y1) );
    destinePoints.append( QPointF(upperLine.x2,upperLine.y1) );
    destinePoints.append( QPointF(lowerLine.x1,lowerLine.y1) );
    destinePoints.append( QPointF(lowerLine.x2,lowerLine.y1) );
    //Transformation Quads
    QPolygonF originQuad(originPoints);
    QPolygonF destineQuad(destinePoints);
    //std::cout << "x11: " << originPoints.at(0).x() << " y11: " << originPoints.at(0).y() << std::endl;
    //std::cout << "x12: " << originPoints.at(1).x() << " y12: " << originPoints.at(1).y() << std::endl;
    //std::cout << "x21: " << destinePoints.at(0).x() << " y21: " << destinePoints.at(0).y() << std::endl;
    //std::cout << "x22: " << destinePoints.at(1).x() << " y22: " << destinePoints.at(1).y() << std::endl;

    //-------------------------------------
    //Build Transformation
    //-------------------------------------
    //Vanashing Point
    if( tmpTrans->quadToQuad(originQuad,destineQuad,*tmpTrans) == false )
    {
        funcShowMsgERROR_Timeout("Transformation does not exists");
        return _ERROR;
    }
    //Rotate
    float degree;
    QString tmpRotation;
    tmpRotation = readAllFile(_PATH_LAST_ONEAXIS_ROTATION).trimmed();
    degree = (tmpRotation.isEmpty())?0.0:tmpRotation.toFloat(0);
    tmpTrans->rotate(degree);

    return _OK;
}

int funcGetImagesFromFolder(
                                const QString &Dir,
                                QList<QImage>* lstTransImages,
                                QList<QFileInfo>* lstImagePaths,
                                QProgressBar *progBar
){
    //--------------------------------------------
    //Read Slide Calibration
    //--------------------------------------------
    //Get Default Slide Calibration Path
    QString defaCalibPath;
    defaCalibPath = readAllFile(_PATH_SLIDE_ACTUAL_CALIB_PATH).trimmed();
    if( defaCalibPath.isEmpty() )
    {
        funcShowMsg("Alert!","Please, Set Default Slide Calibration");
        return _ERROR;
    }
    //Read Default Calibration Path
    structSlideCalibration slideCalibration;
    if( funcReadSlideCalib( defaCalibPath, &slideCalibration ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Default Slide Calibration File");
        return _ERROR;
    }

    //--------------------------------------------
    //Define Max Wavelength
    //--------------------------------------------
    double maxWavelen;
    maxWavelen = readAllFile(_PATH_SLIDE_MAX_WAVELENGTH).trimmed().toDouble(0);
    if( maxWavelen < _RASP_CAM_MIN_WAVELENGTH || maxWavelen > _RASP_CAM_MAX_WAVELENGTH )
    {
        maxWavelen = _RASP_CAM_MAX_WAVELENGTH;
    }

    //--------------------------------------------
    //Get List of Files in Default Directory
    //--------------------------------------------
    //List All Files in Directory
    *lstImagePaths = funcListFilesInDir( Dir );
    std::cout << "numFrames: " << lstImagePaths->size() << std::endl;
    if( lstImagePaths->size() < 3 )
    {
        funcShowMsgERROR_Timeout("Number of Files Insufficient");
        mouseCursorReset();
        return _ERROR;
    }

    //-------------------------------------------------
    //Get into Memory Transformed Frames
    //-------------------------------------------------
    int i, progVal;
    //Update Image Displayed
    if( progBar != Q_NULLPTR )
    {
        progBar->setValue(0);
        progBar->setVisible(true);
        progBar->update();
    }
    mouseCursorWait();
    *lstImagePaths  = funcListFilesInDir( Dir );
    for( i=0; i<lstImagePaths->size(); i++ )
    {
        //std::cout << "MOD-> i: " << i << " Modified: " << lstFrames.at(i).absoluteFilePath().toStdString() << std::endl;
        lstTransImages->append( QImage( lstImagePaths->at(i).absoluteFilePath() ) );
        //Update Progress Bar
        progVal = round( ( (float)(i) / (float)lstImagePaths->size() ) * 100.0 );
        if( progBar != Q_NULLPTR )
        {
            progBar->setValue(progVal);
            progBar->update();
        }
    }
    mouseCursorReset();

    //-------------------------------------------------
    //Reset Progress Bar
    //-------------------------------------------------
    if( progBar != Q_NULLPTR )
    {
        progBar->setValue(0);
        progBar->setVisible(false);
        progBar->update();
    }

    return _OK;
}


float funcPixelToQE(
                        const QColor &originPixColor,
                        const int &wavePos,
                        const structSlideSensitivities &slideSens,
                        const float &wS
){
    //--------------------------------------------------
    //Denoise Pixel Color
    //--------------------------------------------------

    //Spectral Denoising
    float originQE;
    originQE    = ((float)originPixColor.red() * slideSens.wR.at(wavePos))    +
                  ((float)originPixColor.green() * slideSens.wG.at(wavePos))  +
                  ((float)originPixColor.blue() * slideSens.wB.at(wavePos));
    //std::cout << "originQE: " << originQE << " wS: " << wS << std::endl;
    originQE    = originQE * wS;
    originQE    = (originQE>255.0)?255.0:originQE;

    return originQE;

}


QList<double> getNormedFunction( QString fileName )
{
    QString contain;
    contain = readFileParam(fileName);
    QList<QString> irradiation;
    QList<double> function;
    irradiation = contain.split(",");
    int i;
    double max;
    max = irradiation.at(0).toDouble(0);
    irradiation.removeAt(0);
    for(i=0;i<irradiation.count();i++)
    {
        function.append((irradiation.at(i).toDouble(0) / max)*0.75 );
        //printf("%.4f,",function.at(i));
    }
    return function;
}

/*
int funcGetSpectrallyDenoisedPixel(
                                    const QColor &originColor,
                                    const int &wavePos,
                                    const float &wS,
                                    const structSlideSensitivities &slideSens
){

    //Spectral Denoising
    float tmpColor;
    tmpColor    = ((float)originColor.red() * slideSens.wR.at(wavePos))    +
                  ((float)originColor.green() * slideSens.wG.at(wavePos))  +
                  ((float)originColor.blue() * slideSens.wB.at(wavePos));
    tmpColor    = tmpColor * wS;
    tmpColor    = (tmpColor>255.0)?255.0:tmpColor;

    //Return Color
    return round(tmpColor);

}*/

int funcSlideDenoiseDefineSensorToUse(
                                        strDenoisedColorSelected* denColSel,
                                        const int &wavePos,
                                        const structSlideSensitivities &slideSens
){

    denColSel->colorID  = _BLUE;
    denColSel->wS       = 1.0;

    float wR    = slideSens.normedRalfR.at(wavePos);
    float wG    = slideSens.normedRalfG.at(wavePos);
    float wB    = slideSens.normedRalfB.at(wavePos);
    float tmpX  = 0;
    if( (wR >= wG) && (wR >= wB) )
    {
        denColSel->colorID    = _RED;
        tmpX                = (1.0 - wR);
        tmpX                = (tmpX<1.0)?tmpX:0;
    }else if( (wG >= wR) && (wG >= wB) )
    {
        denColSel->colorID  = _GREEN;
        tmpX                = (1.0 - wG);
        tmpX                = (tmpX<1.0)?tmpX:0;
    }else if( (wB >= wR) && (wB >= wG) )
    {
        denColSel->colorID  = _BLUE;
        tmpX                = (1.0 - wB);
        tmpX                = (tmpX<1.0)?tmpX:0;
    }

    //denColSel->wS       = (pow(tmpX,4)*16.03168)    +
    //                      (pow(tmpX,3)*-18.27273)   +
    //                      (pow(tmpX,2)*6.73246)     +
    //                      (tmpX*-0.11645)           +
    //                      1;

    if(tmpX>0.2)
    {
        denColSel->wS       = (pow(tmpX,3)*3.86029)     +
                              (pow(tmpX,2)*-3.05151)    +
                              (tmpX*1.31828)            +
                              1;
    }
    else
    {
        denColSel->wS       = 1.0;
    }


    //denColSel->wS           = (denColSel->wS<1.71)?1.71:denColSel->wS;

    //std::cout << "denColSel->wS: " << denColSel->wS << std::endl;

    return _OK;
}

void funcGetMaximumSensitivities(
                                    structSlideCalibration* mainSlideCalibration,
                                    QWidget* parent
){
    //------------------------------------------------------
    //Load Slide Calibration Settings Set as Default
    //------------------------------------------------------
    QString calibPath = readAllFile(_PATH_SLIDE_ACTUAL_CALIB_PATH).trimmed();
    if( funcReadSlideCalib( calibPath, mainSlideCalibration ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Slide Calibration File: "+calibPath,parent);
        return (void)false;
    }

    //------------------------------------------------------
    //Get Max Sensitivities
    //------------------------------------------------------
    int i;
    float tmpMaxSR  = 0;
    float tmpMaxSG  = 0;
    float tmpMaxSB  = 0;
    float tmpMaxMax = 0;
    float tmpVal;
    for( i=0; i<mainSlideCalibration->sensitivities.originalR.size(); i++ )
    {
        //Red
        tmpVal      = mainSlideCalibration->sensitivities.normedRalfR.at(i);
        tmpMaxSR    = (tmpVal > tmpMaxSR)?tmpVal:tmpMaxSR;
        //Green
        tmpVal      = mainSlideCalibration->sensitivities.normedRalfG.at(i);
        tmpMaxSG    = (tmpVal > tmpMaxSG)?tmpVal:tmpMaxSG;
        //Blue
        tmpVal      = mainSlideCalibration->sensitivities.normedRalfB.at(i);
        tmpMaxSB    = (tmpVal > tmpMaxSB)?tmpVal:tmpMaxSB;
    }
    //maxMax
    tmpMaxMax   = (tmpMaxSR>=tmpMaxSG)?tmpMaxSR:(tmpMaxSG>=tmpMaxSB)?tmpMaxSG:tmpMaxSB;
    //Save
    mainSlideCalibration->sensitivities.maximumColors.maxSR   = tmpMaxSR;
    mainSlideCalibration->sensitivities.maximumColors.maxSG   = tmpMaxSG;
    mainSlideCalibration->sensitivities.maximumColors.maxSB   = tmpMaxSB;
    mainSlideCalibration->sensitivities.maximumColors.maxMaxS = tmpMaxMax;

}


int func_DirExistOrCreateIt( const QList<QString> &lstFolders , QWidget* parent )
{
    for( int i=0; i<lstFolders.size(); i++ )
    {
        if( !QDir( lstFolders.at(i) ).exists() )
        {
            QString tmpMsg;
            tmpMsg.append("Create Folder ");
            tmpMsg.append(lstFolders.at(i));
            tmpMsg.append("?");
            if( funcShowMsgYesNo("Folder not Found",tmpMsg,parent) )
            {
                QDir().mkdir( lstFolders.at(i) );
            }
            else
            {
                return _ERROR;
            }
        }
    }
    return _OK;
}


int readHypCubeParameters( const QString &cubePath, strCubeParameters* cubeParam )
{
    //---------------------------------------------------
    //Prepare Parameters of Interest
    //---------------------------------------------------
    QList<QString> lstParameters;
    QList<QString> lstValues;
    lstParameters   << "hypcubeW"          << "hypcubeH"               << "hypcubeL"
                    << "initWavelength"    << "spectralResolution";

    //---------------------------------------------------
    //Read Cube Parameters
    //---------------------------------------------------
    if( readXMLIntoList( cubePath, lstParameters, &lstValues ) != _OK )
    {
        return _ERROR;
    }

    //---------------------------------------------------
    //Fill Parameters
    //---------------------------------------------------
    int i = 0;
    cubeParam->W                = lstValues.at(i++).toInt(0);
    cubeParam->H                = lstValues.at(i++).toInt(0);
    cubeParam->L                = lstValues.at(i++).toInt(0);
    cubeParam->initWavelength   = lstValues.at(i++).toFloat(0);
    cubeParam->spectralRes      = lstValues.at(i++).toFloat(0);


    return _OK;
}

int readXMLIntoList(
                        const QString &xmlPath,
                        const QList<QString> &lstParameters,
                        QList<QString>* lstValues
){
    //---------------------------------------------------
    //Read File XML
    //---------------------------------------------------
    QFile *xmlFile = new QFile(xmlPath);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        funcShowMsg("ERROR","Opening "+xmlPath);
        return _ERROR;
    }
    QXmlStreamReader* xmlReader = new QXmlStreamReader(xmlFile);

    //---------------------------------------------------
    //Read Fields
    //---------------------------------------------------
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
            if( lstParameters.indexOf(xmlReader->name().toString()) != -1 )
            {
                lstValues->append( xmlReader->readElementText() );
            }
        }
    }
    if(xmlReader->hasError())
    {
        xmlReader->clear();
        xmlFile->close();
        funcShowMsg(xmlPath+" Parse Error",xmlReader->errorString());
        return _FAILURE;
    }
    xmlReader->clear();
    xmlFile->close();

    return _OK;
}

void msg( std::string msg )
{
    std::cout << msg << std::endl;
}

QPoint originFromCalibration(
                                const structHorizontalCalibration &tmpHorizlCal,
                                const structVerticalCalibration &tmpVertCal
){
    QPoint origin;
    float horA, horB;
    float verA, verB;
    int H;
    //Prefill Coordinates
    horA  = tmpHorizlCal.a;
    horB  = tmpHorizlCal.b;
    verA  = tmpVertCal.vertLR.a;
    verB  = tmpVertCal.vertLR.b;
    H     = tmpVertCal.imgH;

    //Found the lowest error between the Vertical lower bound
    //and the Y-value calculated using Horizontal LR
    int vertX, vertY, horizY, tmpError, minError;
    minError = H;
    for( vertY=0; vertY<H; vertY++ )
    {
        vertX       = round( ( vertY * verB ) + verA );
        horizY      = round( ( vertX * horB ) + horA );
        tmpError    = horizY - vertY;
        if( tmpError>=0 && tmpError<minError )
        {
            minError = tmpError;
            origin.setX( vertX );
            origin.setY( horizY );
        }
        if( tmpError < 0 )
        {
            break;
        }
    }
    return origin;
}

int funcReadVerticalCalibration(
                                        QString* filePath,
                                        structVerticalCalibration* vertCal
){
    //---------------------------------------
    //Extract XML
    //---------------------------------------
    QFile *xmlFile = new QFile(*filePath);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        funcShowMsg("ERROR","Opening: "+*filePath);
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
            if( xmlReader->name()=="imgW" )
                vertCal->imgW = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="imgH" )
                vertCal->imgH = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="x1" )
                vertCal->x1 = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="y1" )
                vertCal->y1 = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="x2" )
                vertCal->x2 = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="y2" )
                vertCal->y2 = xmlReader->readElementText().trimmed().toInt(0);
            if( xmlReader->name()=="lowerBoundWave" )
                vertCal->minWave = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="higherBoundWave" )
                vertCal->maxWave = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="wavelengthA" )
                vertCal->wavelengthLR.a = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="wavelengthB" )
                vertCal->wavelengthLR.b = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="dist2WaveA" )
                vertCal->dist2WaveLR.a = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="dist2WaveB" )
                vertCal->dist2WaveLR.b = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="wave2DistA" )
                vertCal->wave2DistLR.a = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="wave2DistB" )
                vertCal->wave2DistLR.b = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="vertA" )
                vertCal->vertLR.a = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="vertB" )
                vertCal->vertLR.b = xmlReader->readElementText().trimmed().toFloat(0);
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("Parse Error",xmlReader->errorString());
        return _ERROR;
    }
    xmlReader->clear();
    xmlFile->close();

    return _OK;
}
