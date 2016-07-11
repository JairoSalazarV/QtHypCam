#include "__common.h"

#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>

#include <ifaddrs.h>
#include <netdb.h>

#include <QFile>
#include <QTextStream>


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
            cv::imwrite( tmpName.toStdString(), cv::Mat(imgRot, true) );
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

bool saveFile( QString fileName, QString contain ){
    QFile file(fileName);
    if(file.exists()){
        if(!file.remove()){
            return false;
        }
    }
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << contain << endl;
        file.close();
    }else{
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
    tmpFile.open(QIODevice::ReadOnly);
    QTextStream tmpStream(&tmpFile);
    return tmpStream.readAll();
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

            if( xmlReader->name()=="width" )
                squareParam->width = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="height" )
                squareParam->height = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="x1" )
                squareParam->x1 = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="y1" )
                squareParam->y1 = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="x2" )
                squareParam->x2 = xmlReader->readElementText().toInt(0);

            if( xmlReader->name()=="y2" )
                squareParam->y2 = xmlReader->readElementText().toInt(0);
        }
    }
    if(xmlReader->hasError()) {
        funcShowMsg("raspcamSettings.xml Parse Error",xmlReader->errorString());
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
            if( xmlReader->name()=="ShutterSpeed" )
                raspcamSettings->ShutterSpeed = xmlReader->readElementText().toInt(0);
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

    QFile *xmlFile = new QFile("settings.xml");
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        funcShowMsg("ERROR","Couldn't open settings.xml to load settings for download");
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
        funcShowMsg("settings.xml Parse Error",xmlReader->errorString());
    }
    xmlReader->clear();
    xmlFile->close();
}

void funcShowMsg(QString title, QString msg){
    QMessageBox yesNoMsgBox;
    yesNoMsgBox.setWindowTitle(title);
    yesNoMsgBox.setText(msg);
    yesNoMsgBox.setDefaultButton(QMessageBox::Ok);
    yesNoMsgBox.exec();
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


int funcShowMsgYesNo( QString title, QString msg ){
    //int integerValue = 10;
    QMessageBox yesNoMsgBox;
    yesNoMsgBox.setWindowTitle(title);
    yesNoMsgBox.setText(msg);
    //yesNoMsgBox.setInformativeText(QString(info).append(QVariant(integerValue).toString()));
    //yesNoMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    yesNoMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    switch (yesNoMsgBox.exec()){
        case QMessageBox::Yes: {
            return 1;
            break;
        }
        case QMessageBox::No: {
            return 0;
            break;
        }
    }
    return 0;
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


QImage* IplImage2QImage(IplImage *iplImg){
    int h = iplImg->height;
    int w = iplImg->width;
    int channels = iplImg->nChannels;
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

}

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



bool saveBinFile(unsigned long datasize, unsigned char *dataPtr, QString directory){
    QFile DummyFile(directory);
    if(DummyFile.open(QIODevice::WriteOnly)) {
        qint64 bytesWritten = DummyFile.write(reinterpret_cast<const char*>(dataPtr), datasize);
        if (bytesWritten < datasize) {
            return false;
        }
        DummyFile.close();
    }
    return true;
}

