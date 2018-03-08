/*

    ->Jun/05/2017 Ready to get one shot snapshot

    //----------------------------------------
    //Merge branch into master
    //----------------------------------------
    git checkout seotweaks
    git merge -s ours master
    git checkout master
    git merge seotweaks

    ->June/13/2017 Remote Terminal Command Execution Added

    //----------------------------------------
    //WORK TO DO
    //----------------------------------------
    //Integrate sensitivities at hypercube building

*/





#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "__common.h"
#include "hypCamAPI.h"

#include <unistd.h>
#include <netdb.h>
#include <QFile>
#include <fstream>
#include <math.h>

//#include <netinet/in.h>
#include <QDebug>
//#include <QPixmap>
#include <QFileInfo>
#include <QFileDialog>
#include <QGraphicsPixmapItem>

#include "graphicsview.h"

#include <QRect>
#include <QRgb>
#include <QProgressBar>

#include <QFormLayout>

#include <selwathtocheck.h>

#include <QImageReader>

#include <formsquareaperturesettings.h>

#include <lstcustoms.h>

//OpenCV
/*
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
*/

/*
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/videoio/videoio_c.h>

#include <opencv2/video/tracking_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <highgui.h>
#include <cv.h>
#include <opencv2/core/core_c.h>
#include <imgproc.hpp>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "/usr/local/include/opencv2/videoio/videoio_c.h"
*/

/*
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/videoio.hpp>
*/




#include <hypCamAPI.h>

#include <rasphypcam.h>


//Custom
#include <customline.h>
#include <customrect.h>
#include <selcolor.h>
#include <gencalibxml.h>
#include <rotationfrm.h>
#include <recparamfrm.h>
//#include <generatehypercube.h>
//#include <validatecalibration.h>
#include <selwathtocheck.h>

#include <chosewavetoextract.h>
#include <QRgb>

#include <slidehypcam.h>

#include <QtSerialPort/QSerialPort>
#include <slidehypcam.h>

#include <QThread>
#include <arduinomotor.h>

#include <formslidesettings.h>

#include <lstfilenames.h>

#include <QMediaPlayer>
#include <QVideoProbe>
#include <QVideoWidget>
#include <QAbstractVideoSurface>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <formndvisettings.h>

#include <formobtainfolder.h>

#include <formbuildslidehypecubepreview.h>

#include <formtimertxt.h>
#include <QMessageBox>


#include <formgenlinearregression.h>

#include <formslidelinearregression.h>

#include <formmergeslidecalibrations.h>

structSettings *lstSettings = (structSettings*)malloc(sizeof(structSettings));

structCamSelected *camSelected = (structCamSelected*)malloc(sizeof(structCamSelected));

structRaspcamSettings *raspcamSettings = (structRaspcamSettings*)malloc(sizeof(structRaspcamSettings));

GraphicsView *myCanvas;
GraphicsView *canvasSpec;
GraphicsView *canvasCalib;
GraphicsView *canvasAux;

customLine *globalCanvHLine;
customLine *globalCanvVLine;
//customLine *globalTmpLine;

//QString imgPath = "/media/jairo/56A3-A5C4/DatosAVIRIS/CrearHSI/MyDatasets/Philips/HojasFotoVsHojasBiomasaJun2016/200Id/CROPED/100.tif";
//QString impPath = "./imgResources/CIE.png";

QList<QPair<int,int>> *lstBorder;
QList<QPair<int,int>> *lstSelPix;
QList<QPair<int,int>> *lstPixSelAux;

//int tmpRect[4];
calcAndCropSnap calStruct;
bool globaIsRotated;

qint64 numFrames;
//QList<QImage> lstFrames;



QThread* progBarThread;


int*** tmpHypercube;
static QList<QFileInfo> lstImages;
GraphicsView* graphViewSmall;
const int frameX    = 40;       //left and right
const int frameY    = 30;       //up and down
const int rangeInit = 300;      //Wavelength(nm) at the origin
const int rangeEnd  = 1100;     //Wavelength(nm) max limit
const int rangeStep = 50;       //Wavelength(nm) between slides

QImage* globalEditImg;
QImage* globalBackEditImg;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    /*
    int a[]     = {4,2,-1,3,-2,-6,-5,4,5};
    int b[]     = {-4,1,3,7,4,-2,-8,-2,1};
    int n       = 9;
    float* c    = vectorCrossCorrelation(a,b,9,0.4);
    int shift   = c[0]-n;
    qDebug() << "shift: " << shift;
    exit(0);
    */










    //ui->actionValidCal->trigger();

    funcObtSettings( lstSettings );

    progBarThread = new QThread(this);

    /*
    int algo = 4;
    int *p;
    p = &algo;
    qDebug() << "bool: " << sizeof(bool);
    qDebug() << "char: " << sizeof(char);
    qDebug() << "int8: " << sizeof(u_int8_t);
    qDebug() << "int: " << sizeof(int);
    qDebug() << "int*: " << sizeof(p);
    printf("Puntero %p\n",p);
    printf("Puntero %d\n",p);
    printf("Puntero %d\n",*p);
    */


    /*
    //Fill IP prefix
    //if(_PRELOAD_IP){
        char cIP[15];
        funcObtainIP( cIP );
        QString qsIP(cIP);
        QStringList lstIP = qsIP.split('.');
        qsIP = lstIP.at(0) + "." +lstIP.at(1) + "."+lstIP.at(2) + ".";
        ui->txtIp->setText(qsIP);
    //}
    */


    //Initialize global settings
    camSelected->isConnected    = false;
    camSelected->On             = false;
    camSelected->stream         = false;

    //Initialize camera parameters
    ui->txtCamParamXMLName->setText("raspcamSettings");
    funcGetRaspParamFromXML( raspcamSettings, _PATH_RASPICAM_SETTINGS );
    funcIniCamParam( raspcamSettings );


    //Create Graphic View Widget
    myCanvas = new GraphicsView;
    canvasSpec = new GraphicsView;
    canvasCalib = new GraphicsView;
    canvasAux = new GraphicsView;
    //QString imgPath = "/media/jairo/56A3-A5C4/DatosAVIRIS/CrearHSI/MyDatasets/Philips/HojasFotoVsHojasBiomasaJun2016/25Id/15.png";
    //funcPutImageIntoGV( myCanvas, imgPath );

    //Initialize points container for free-hand pen tool
    lstBorder = new QList<QPair<int,int>>;
    lstSelPix = new QList<QPair<int,int>>;
    lstPixSelAux = new QList<QPair<int,int>>;

    /*
    //Connect to image
    lstBorder = new QList<QPair<int,int>>;
    lstSelPix = new QList<QPair<int,int>>;
    connect(
                myCanvas,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcAddPoint(QMouseEvent*) )
           );
    ui->progBar->setVisible(false);

    //Connect to calib double axis
    connect(
                canvasCalib,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcBeginRect(QMouseEvent*) )
           );
    connect(
                canvasCalib,
                SIGNAL( signalMouseReleased(QMouseEvent*) ),
                this,
                SLOT( funcCalibMouseRelease(QMouseEvent*) )
           );

    //Connect to spec
    connect(
                canvasSpec,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcBeginRect(QMouseEvent*) )
           );
    connect(
                canvasSpec,
                SIGNAL( signalMouseReleased(QMouseEvent*) ),
                this,
                SLOT( funcSpectMouseRelease(QMouseEvent*) )
           );
    */

    //Fill the lastsnapshots path as default
    //QString lastSnapPath = readAllFile( _PATH_LAST_SNAPPATH );
    //lastSnapPath.replace("\n","");
    //ui->txtSnapPath->setText(lastSnapPath);

    //Set layout into spectometer
    QFormLayout *layout = new QFormLayout;
    ui->tab_5->setLayout(layout);

    if(_USE_CAM)ui->sbSpecUsb->setValue(1);
    else ui->sbSpecUsb->setValue(0);

    //Enable-Disable buttoms
    //..
    //ui->toolBarDraw->setEnabled(false);

    ui->progBar->setValue(0);
    ui->progBar->setVisible(false);

    ui->pbShutdown->setDisabled(true);

    disableAllToolBars();

    //loadImageIntoCanvasEdit(_PATH_DISPLAY_IMAGE, false);

    //
    //Try to connect to the last IP
    //
    QString lastIP = readAllFile( _PATH_LAST_IP );
    lastIP.replace("\n","");
    ui->txtIp->setText(lastIP);

    if( funcShowMsgYesNo("Welcome!","Auto connect?") ){
        ui->pbAddIp->click();
        ui->tableLstCams->selectRow(0);
        ui->pbConnect->click();
    }


    //
    // Show Last Preview
    //
    QString lastFileOpen = readFileParam(_PATH_LAST_USED_IMG_FILENAME);
    globalEditImg = new QImage(lastFileOpen);
    updateDisplayImage(globalEditImg);



    //
    //Allocate memory to the hypercube
    //
    //funcAllocInteger3DMatrixMemo( 3, 3, 3, tmpbe, true );

    //on_actionSlide_Linear_Regression_triggered();

    /*
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/408nm.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/438nm.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/492nm.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/550nm.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/586nm.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/620nm.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/712nm.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/horizontalLower.xml");
    funcDrawLineIntoCanvasEdit("./XML/lines/slideV1_002/horizontalUpper.xml");*/

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disableAllToolBars(){
    //ui->toolBarDraw->setVisible(false);
}

void MainWindow::funcSpectMouseRelease( QMouseEvent *e){
    funcEndRect(e, canvasSpec );
    funcDrawLines(0,0,0,0);
}

void MainWindow::funcCalibMouseRelease( QMouseEvent *e){
    funcEndRect( e, canvasCalib );
    //ui->pbClearCalScene->setText("Clear line");
}

void MainWindow::funcAddPoint( QMouseEvent *e ){

    //Extract pixel's coordinates
    QPair<int,int> tmpPixSel;
    tmpPixSel.first = e->x();
    tmpPixSel.second = e->y();
    lstPixSelAux->append(tmpPixSel);

    //Update politope
    funcUpdatePolitope();

}

bool MainWindow::funcUpdatePolitope(){

    int i, x1, y1, x2, y2, x0, y0;

    //If is the first point
    if( lstPixSelAux->count() < 2 ){
        x1  = lstPixSelAux->at(0).first - 3;//Point +/- error
        y1  = lstPixSelAux->at(0).second - 4;//Point +/- error
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(x1, y1, 5, 5 );
        item->setPen( QPen(Qt::white) );
        item->setBrush( QBrush(Qt::red) );
        canvasAux->scene()->addItem(item);
        return true;
    }

    //--------------------------------------------
    // Fill the border points
    //--------------------------------------------

    //Last two points
    i   = lstPixSelAux->count()-1;
    x0  = lstPixSelAux->at(0).first;
    y0  = lstPixSelAux->at(0).second;
    x1  = lstPixSelAux->at(i-1).first;
    y1  = lstPixSelAux->at(i-1).second;
    x2  = lstPixSelAux->at(i).first;
    y2  = lstPixSelAux->at(i).second;

    //Draw the line
    if( abs(x0-x2)<=6 && abs(y0-y2)<=6 ){//Politope closed
        funcCreateLine(false,x1,y1,x0,y0);
    }else{//Add line
        funcCreateLine(true,x1,y1,x2,y2);
    }

    //--------------------------------------------
    // Fill pixels inside politope
    //--------------------------------------------
    if( abs(x0-x2)<=6 && abs(y0-y2)<=6 ){
        funcFillFigure();
    }










    return true;
}

void MainWindow::funcFillFigure(){
    // Clear points
    //...
    //qDebug() << "Polytope closed: " << QString::number(lstBorder->count()) ;
    //while(ui->tableLstPoints->rowCount() > 0){
    //    ui->tableLstPoints->removeRow(0);
    //}
    //funcPutImageIntoGV( canvasAux, imgPath );
    canvasAux->scene()->clear();

    // Get max and min Y
    //...
    int i, minX, maxX, minY, maxY;
    minX = minY = INT_MAX;
    maxX = maxY = -1;
    for(i=0;i<lstBorder->count();i++){
        minY = ( lstBorder->at(i).second < minY )?lstBorder->at(i).second:minY;
        maxY = ( lstBorder->at(i).second > maxY )?lstBorder->at(i).second:maxY;
    }

    //Run range in Y
    int tmpY;
    //bool entro;
    QPair<int,int> tmpPair;
    for(tmpY=minY; tmpY<=maxY; tmpY++){
        //qDebug() << "tmpY: "<< QString::number(tmpY);
        //Get range in X
        //entro = false;
        minX = INT_MAX;
        maxX = -1;
        for(i=0;i<lstBorder->count();i++){
            if( lstBorder->at(i).second == tmpY ){
                //entro = true;
                minX = ( lstBorder->at(i).first < minX )?lstBorder->at(i).first:minX;
                maxX = ( lstBorder->at(i).first > maxX )?lstBorder->at(i).first:maxX;
                lstBorder->removeAt(i);
                //qDebug() << "minX: "<< QString::number(minX)<< "maxX: "<< QString::number(maxX);
            }
        }
        //qDebug() << "Linw: minX= "<< QString::number(minX)<< "maxX= "<< QString::number(maxX);

        //Draw line in X
        for(i=minX;i<=maxX;i++){
            tmpPair.first  = i;
            tmpPair.second = tmpY;
            lstSelPix->append( tmpPair );
            funcDrawPointIntoCanvas( tmpPair.first, tmpPair.second, 1, 1, "#FF0000", "#FFFFFF" );
        }
        //funcShowMsg("","jeha");
    }

    //Clear border
    lstBorder->clear();

    //Redrawn pixels
    for(i=0;i<lstSelPix->count();i++){
        funcDrawPointIntoCanvas(
                                    lstSelPix->at(i).first,
                                    lstSelPix->at(i).second,
                                    1,
                                    1,
                                    "#FF0000",
                                    "#FFFFFF"
                                );
    }

    lstPixSelAux->clear();

     disconnect(
                canvasCalib,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcAddPoint(QMouseEvent*) )
           );

     mouseCursorReset();


}





void MainWindow::funcCreateLine(bool drawVertex,
    int x1,
    int y1,
    int x2,
    int y2
){

    QPair<int,int> tmpPair;

    //Drawing the vertex
    if( drawVertex ){
        tmpPair.first = x2-3;
        tmpPair.second = y2-4;
        lstBorder->append(tmpPair);
        funcDrawPointIntoCanvas(tmpPair.first, tmpPair.second, 5, 5, "#FF0000", "#FFFFFF");
        //QGraphicsEllipseItem* item = new QGraphicsEllipseItem(tmpPair.first, tmpPair.second, 5, 5 );
        //item->setPen( QPen(QColor("#FFFFFF")) );
        //item->setBrush( QBrush(QColor("#FF0000")) );
        //myCanvas->scene()->addItem(item);
    }

    //Variables
    int i, yIni, yEnd;
    float m,b;

    //Compute slope
    m = (float)(y2-y1) / (float)(x2-x1);
    b = (float)y1-(m*(float)x1);

    //Obtain points in the border
    int xIni = (x1 <= x2)?x1:x2;
    int xEnd = (xIni == x1)?x2:x1;
    int lastY = -1, j = -1;
    if(xIni==xEnd){
        yIni = (y1<=y2)?y1:y2;
        yEnd = (y1==yIni)?y2:y1;
        for( j=yIni; j<=yEnd; j++ ){
            funcDrawPointIntoCanvas(xIni, j, 1, 1, "#FF0000", "#FFFFFF");
            //QGraphicsEllipseItem* item = new QGraphicsEllipseItem( xIni, j, 1, 1 );
            //item->setPen( QPen(QColor("#FF0000")) );
            //myCanvas->scene()->addItem(item);

            tmpPair.first = xIni;
            tmpPair.second = j;
            lstBorder->append(tmpPair);
        }
    }else{
        for( i=xIni; i<=xEnd; i++ ){
            //Add discrete value
            tmpPair.first = i;
            tmpPair.second = floor( (m*i) + b );
            lstBorder->append(tmpPair);

            funcDrawPointIntoCanvas(tmpPair.first, tmpPair.second, 1, 1, "#FF0000", "#FFFFFF");
            //QGraphicsEllipseItem* item = new QGraphicsEllipseItem( tmpPair.first, tmpPair.second, 1, 1 );
            //item->setPen( QPen(QColor("#FF0000")) );
            //myCanvas->scene()->addItem(item);

            //Complete the line
            if( (i == xIni) ){
                lastY = tmpPair.second;
            }else{
                yIni = (tmpPair.second <= lastY)?tmpPair.second:lastY;
                yEnd = (yIni == tmpPair.second)?lastY:tmpPair.second;
                if( abs(yIni-yEnd) > 1 ){
                    lastY = tmpPair.second;
                    for( j=yIni; j<=yEnd; j++ ){
                        tmpPair.first = i;
                        tmpPair.second = j;
                        lstBorder->append(tmpPair);

                        //QGraphicsEllipseItem* item = new QGraphicsEllipseItem( i, j, 1, 1 );
                        //item->setPen( QPen(QColor("#FF0000")) );
                        //myCanvas->scene()->addItem(item);
                        funcDrawPointIntoCanvas(i,j,1,1, "#FF0000", "#FFFFFF");

                        //funcShowMsg("","Ja");
                    }
                }
            }
        }
    }


}

void MainWindow::funcDrawPointIntoCanvas(
                                            int x,
                                            int y,
                                            int w,
                                            int h,
                                            QString color = "#FF0000",
                                            QString lineColor = "#FFFFFF"
){
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem( x, y, w, h );
    item->setPen( QPen(QColor(color)) );
    canvasAux->scene()->addItem(item);
    item->setBrush( QBrush(QColor(lineColor)) );
}

void MainWindow::funcAddPoit2Graph(
                                        GraphicsView *tmpCanvas,
                                        int x,
                                        int y,
                                        int w,
                                        int h,
                                        QColor color,
                                        QColor lineColor
){
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem( x, y, w, h );
    item->setPen( QPen(color) );
    tmpCanvas->scene()->addItem(item);
    item->setBrush( QBrush(lineColor) );
}


void MainWindow::funcIniCamParam( structRaspcamSettings *raspcamSettings )
{    
    QList<QString> tmpList;

    //Set AWB: off,auto,sun,cloud,shade,tungsten,fluorescent,incandescent,flash,horizon
    tmpList<<"none"<<"off"<<"auto"<<"sun"<<"cloud"<<"shade"<<"tungsten"<<"fluorescent"<<"incandescent"<<"flash"<<"horizon";
    ui->cbAWB->clear();
    ui->cbAWB->addItems( tmpList );
    ui->cbAWB->setCurrentText((char*)raspcamSettings->AWB);
    tmpList.clear();

    //Set Exposure: off,auto,night,nightpreview,backlight,spotlight,sports,snow,beach,verylong,fixedfps,antishake,fireworks
    tmpList<<"none"<<"off"<<"auto"<<"night"<<"nightpreview"<<"backlight"<<"spotlight"<<"sports"<<"snow"<<"beach"<<"verylong"<<"fixedfps"<<"antishake"<<"fireworks";
    ui->cbExposure->clear();
    ui->cbExposure->addItems( tmpList );
    ui->cbExposure->setCurrentText((char*)raspcamSettings->Exposure);
    tmpList.clear();

    //Gray YUV420
    //if( raspcamSettings->Format == 1 )ui->rbFormat1->setChecked(true);
    //if( raspcamSettings->Format == 2 )ui->rbFormat2->setChecked(true);

    //Brightness
    //ui->slideBrightness->setValue( raspcamSettings->Brightness );
    //ui->labelBrightness->setText( "Brightness: " + QString::number(raspcamSettings->Brightness) );

    //Sharpness
    //ui->slideSharpness->setValue( raspcamSettings->Sharpness );
    //ui->labelSharpness->setText( "Sharpness: " + QString::number(raspcamSettings->Sharpness) );

    //Contrast
    //ui->slideContrast->setValue( raspcamSettings->Contrast );
    //ui->labelContrast->setText( "Contrast: " + QString::number(raspcamSettings->Contrast) );

    //Saturation
    //ui->slideSaturation->setValue( raspcamSettings->Saturation );
    //ui->labelSaturation->setText( "Saturation: " + QString::number(raspcamSettings->Saturation) );

    //DiffractionShuterSpeed
    ui->spinBoxShuterSpeed->setValue( raspcamSettings->ShutterSpeedMs );

    //SquareShuterSpeed
    ui->spinBoxSquareShuterSpeed->setValue(raspcamSettings->SquareShutterSpeedMs);

    //Timelapse Duration
    ui->spinBoxTimelapseDuration->setValue( raspcamSettings->TimelapseDurationSecs );

    //Timelapse Interval
    ui->spinBoxTimelapse->setValue( raspcamSettings->TimelapseInterval_ms );

    //Video Duration
    ui->spinBoxVideoDuration->setValue( raspcamSettings->VideoDurationSecs );

    //ISO
    ui->slideISO->setValue( raspcamSettings->ISO );
    ui->labelISO->setText( "ISO: " + QString::number(raspcamSettings->ISO) );

    //ExposureCompensation
    //ui->slideExpComp->setValue( raspcamSettings->ExposureCompensation );
    //ui->labelExposureComp->setText( "Exp. Comp.: " + QString::number(raspcamSettings->ExposureCompensation) );

    //RED
    //qDebug() << "Red: " << raspcamSettings->Red;
    //ui->slideRed->setValue( raspcamSettings->Red );
    //ui->labelRed->setText( "Red: " + QString::number(raspcamSettings->Red) );

    //GREEN
    //ui->slideGreen->setValue( raspcamSettings->Green );
    //ui->labelGreen->setText( "Green: " + QString::number(raspcamSettings->Green) );

    //PREVIEW
    //if( raspcamSettings->Preview )ui->cbPreview->setChecked(true);
    //else ui->cbPreview->setChecked(false);

    //TRIGGER TIME
    ui->slideTriggerTime->setValue(raspcamSettings->TriggeringTimeSecs);
    ui->labelTriggerTime->setText("Trigger time: " + QString::number(raspcamSettings->TriggeringTimeSecs)+"secs");

    //DENOISE EFX
    if( raspcamSettings->Denoise )ui->cbDenoise->setChecked(true);
    else ui->cbDenoise->setChecked(false);

    //FULL PHOTO
    //if( raspcamSettings->FullPhoto )ui->cbFullPhoto->setChecked(true);
    //else ui->cbFullPhoto->setChecked(false);

    //COLORBALANCE EFX
    if( raspcamSettings->ColorBalance )ui->cbColorBalance->setChecked(true);
    else ui->cbColorBalance->setChecked(false);

    //CAMERA RESOLUTION
    if( raspcamSettings->CameraMp == 5 )ui->radioRes5Mp->setChecked(true);
    else ui->radioRes8Mp->setChecked(true);

    //FLIPPED
    if( raspcamSettings->Flipped )ui->cbFlipped->setChecked(true);
    else ui->cbFlipped->setChecked(false);


}

/*
void MainWindow::on_pbGetVideo_clicked()
{

    //Prepare socket variables
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    frameStruct *frame2Send     = (frameStruct*)malloc(sizeof(frameStruct));
    portno = lstSettings->tcpPort;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        //error("ERROR opening socket");
    }
    server = gethostbyname( "192.168.1.69" );
    //server = gethostbyname( "10.0.5.126" );
    if (server == NULL) {
        //fprintf(stderr,"ERROR, not such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        //error("ERROR connecting");
    }

    //Request file
    frame2Send->header.idMsg          = (char)4;  // Header: Id instruction
    frame2Send->header.consecutive    = 1;        // Header: Consecutive
    frame2Send->header.numTotMsg      = 1;        // Header: Total number of message to send
    frame2Send->header.bodyLen        = 0;        // Header: Usable message lenght
    int tmpFrameLen = sizeof(frameHeader)+frame2Send->header.bodyLen;
    n = ::write(sockfd,frame2Send,tmpFrameLen);
    if (n < 0){
        qDebug() << "writing to socket";
    }

    //Receibing ack with file len
    unsigned int fileLen;
    unsigned char bufferRead[frameBodyLen];
    n = read(sockfd,bufferRead,frameBodyLen);
    memcpy(&fileLen,&bufferRead,sizeof(unsigned int));
    //funcShowMsg("FileLen n("+QString::number(n)+")",QString::number(fileLen));

    if(fileLen>0){
        //Receive File
        unsigned char tmpFile[fileLen];
        if( funcReceiveFile( sockfd, fileLen, bufferRead, tmpFile ) ){

            //Save file
            std::ofstream myFile ("yoRec2.jpg", std::ios::out | std::ios::binary);
            myFile.write((char*)&tmpFile, fileLen);

            //It finishes succesfully
            funcShowMsg("OK","Successfull reception");

        }else{
            funcShowMsg("ERROR","File does not received");
        }

        ::close(sockfd);

    }else{
        funcShowMsg("Alert","File is empty");
    }


}
*/


bool MainWindow::funcReceiveFile(
                                    int sockfd,
                                    unsigned int fileLen,
                                    unsigned char *bufferRead,
                                    unsigned char *tmpFile
){

    qDebug() << "Inside funcReceiveFile sockfd: " << sockfd;



    //Requesting file
    int i, n;

    n = ::write(sockfd,"sendfile",8);
    if (n < 0){
        qDebug() << "ERROR: writing to socket";
        return false;
    }



    //Receive file parts
    unsigned int numMsgs = (fileLen>0)?floor( (float)fileLen / (float)frameBodyLen ):0;
    numMsgs = ((numMsgs*frameBodyLen)<fileLen)?numMsgs+1:numMsgs;
    unsigned int tmpPos = 0;
    memset(tmpFile,'\0',fileLen);
    qDebug() << "Receibing... " <<  QString::number(numMsgs) << " messages";
    qDebug() << "fileLen: " << fileLen;

    //Receive the last
    if(numMsgs==0){
        //Receives the unik message
        bzero(bufferRead,frameBodyLen);
        //qDebug() << "R1";
        //n = read(sockfd,tmpFile,fileLen);
    }else{

        //ui->progBar->setVisible(true);
        ui->progBar->setRange(0,numMsgs);
        ui->progBar->setValue(0);

        funcActivateProgBar();

        for(i=1;i<=(int)numMsgs;i++){
            //printf("Msg: %d\n",i);

            ui->progBar->setValue(i);
            bzero(bufferRead,frameBodyLen);
            n = read(sockfd,bufferRead,frameBodyLen);
            //qDebug() << "n: " << n;
            if(n!=(int)frameBodyLen&&i<(int)numMsgs){
                qDebug() << "ERROR, message " << i << "WRONG";
                return false;
            }
            //Append message to file
            memcpy( &tmpFile[tmpPos], bufferRead, frameBodyLen );
            tmpPos += n;
            //Request other part
            if( i<(int)numMsgs ){
                //qDebug() << "W2";
                QtDelay(2);
                n = ::write(sockfd,"sendpart",8);                
                if(n<0){
                    qDebug() << "ERROR: Sending part-file request";
                    return false;
                }
            }
        }
        ui->progBar->setValue(0);
        ui->progBar->setVisible(false);
        ui->progBar->update();
        QtDelay(30);

    }

    //qDebug() << "tmpPos: " << tmpPos;


    return true;
}

void MainWindow::funcActivateProgBar(){
    //mouseCursorReset();
    ui->progBar->setVisible(true);
    ui->progBar->setValue(0);
    ui->progBar->update();
    QtDelay(50);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_pbAddIp_clicked()
{
    //Validating IP
    if( !funcIsIP( ui->txtIp->text().toStdString() ) ){
        funcShowMsg("ERROR","Invalid IP address");
        ui->txtIp->setFocus();
    }else{
        //Checks if IP is in the list and remove it if exists
        int i;
        for(i=0;i<ui->tableLstCams->rowCount();i++){
            if( ui->tableLstCams->item(i,1)->text().trimmed() == ui->txtIp->text().trimmed() ){
                ui->tableLstCams->removeRow(i);
            }
        }

        //Add IP
        camSettings * tmpCamSett = (camSettings*)malloc(sizeof(camSettings));
        tmpCamSett->idMsg = (char)0;
        funcValCam(
                        ui->txtIp->text().trimmed().toStdString(),
                        lstSettings->tcpPort,
                        tmpCamSett
                  );
        if( tmpCamSett->idMsg < 1 ){
            funcShowMsg("ERROR","Camera does not respond at "+ui->txtIp->text());
            ui->txtIp->setFocus();
        }else{
            ui->tableLstCams->insertRow( ui->tableLstCams->rowCount() );
            ui->tableLstCams->setItem(
                                            ui->tableLstCams->rowCount()-1,
                                            0,
                                            new QTableWidgetItem(QString(tmpCamSett->Alias))
                                      );
            ui->tableLstCams->setItem(
                                            ui->tableLstCams->rowCount()-1,
                                            1,
                                            new QTableWidgetItem(ui->txtIp->text().trimmed())
                                      );
            ui->tableLstCams->setColumnWidth(0,150);
            ui->tableLstCams->setColumnWidth(1,150);
            //funcShowMsg("Success","Camera detected at "+ui->txtIp->text().trimmed());
        }
    }
}

void MainWindow::on_pbSearchAll_clicked()
{
    //Clear table
    while( ui->tableLstCams->rowCount() > 0 ){
        ui->tableLstCams->removeRow(0);
    }

    //Obtain IP list
    QString result = "";//idMsg to send
    FILE* pipe = popen("arp", "r");
    char bufferComm[frameBodyLen];
    try {
      while (!feof(pipe)) {
        if (fgets(bufferComm, frameBodyLen, pipe) != NULL){
          result.append( bufferComm );
        }
      }
    } catch (...) {
      pclose(pipe);
      throw;
    }
    pclose(pipe);

    //Check IPs candidates
    camSettings *tmpCamSett = (camSettings*)malloc(sizeof(camSettings));
    QStringList ipsCandidates = result.split("\n");
    QStringList candIP;
    int i;


    int sockfd, n, tmpFrameLen;
    tmpFrameLen = sizeof(camSettings);
    struct sockaddr_in serv_addr;
    struct hostent *server;
    unsigned char bufferRead[tmpFrameLen];

    for( i=0;i<ipsCandidates.count();i++ ){
        if( !ipsCandidates.at(i).contains("unreachable") &&
            !ipsCandidates.at(i).contains("incomplete") ){
            candIP = ipsCandidates.at(i).split("ether");

            if( funcIsIP( candIP.at(0).toStdString() ) &&
                candIP.at(0) != ui->txtIp->text().trimmed() + "254"
            ){

                qDebug() << "IP: " << candIP.at(0).trimmed();

                tmpCamSett->idMsg = (char)0;
                memset(tmpCamSett,'\0',sizeof(camSettings));

                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd < 0){
                    qDebug() << "opening socket";
                }else{
                    server = gethostbyname( candIP.at(0).trimmed().toStdString().c_str() );
                    if (server == NULL) {
                        qDebug() << "Not such host";
                    }else{


                        bzero((char *) &serv_addr, sizeof(serv_addr));
                        serv_addr.sin_family = AF_INET;
                        bcopy((char *)server->h_addr,
                            (char *)&serv_addr.sin_addr.s_addr,
                            server->h_length);
                        serv_addr.sin_port = htons(lstSettings->tcpPort);




                        if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
                            qDebug() << "connecting";
                        }else{
                            //Request camera settings
                            char tmpIdMsg = (char)1;


                            n = ::write(sockfd,&tmpIdMsg,1);
                            if (n < 0){
                                qDebug() << "writing to socket";
                            }else{
                                //Receibing ack with file len
                                n = read(sockfd,bufferRead,tmpFrameLen);
                                if (n < 0){
                                    qDebug() << "reading socket";
                                }else{
                                    memcpy(tmpCamSett,&bufferRead,tmpFrameLen);

                                    if( tmpCamSett->idMsg > 0 ){
                                        ui->tableLstCams->insertRow( ui->tableLstCams->rowCount() );
                                        ui->tableLstCams->setItem(
                                                                        ui->tableLstCams->rowCount()-1,
                                                                        0,
                                                                        new QTableWidgetItem(QString(tmpCamSett->Alias))
                                                                  );
                                        ui->tableLstCams->setItem(
                                                                        ui->tableLstCams->rowCount()-1,
                                                                        1,
                                                                        new QTableWidgetItem(candIP.at(0).trimmed())
                                                                  );
                                        ui->tableLstCams->setColumnWidth(0,150);
                                        ui->tableLstCams->setColumnWidth(1,150);
                                    }







                                }
                            }
                        }
                    }

                }



                ::close(sockfd);





















            }




        }
    }
}

void MainWindow::on_pbSendComm_clicked()
{
    if( !camSelected->isConnected){
        funcShowMsg("Alert","Camera not connected");
        return (void)NULL;
    }
    if( ui->txtCommand->text().isEmpty() ){
        funcShowMsg("Alert","Empty command");
        return (void)NULL;
    }
    if( ui->tableLstCams->rowCount() == 0 ){
        funcShowMsg("Alert","Not camera detected");
        return (void)NULL;
    }
    ui->tableLstCams->setFocus();

    //Prepare message to send
    frameStruct *frame2send = (frameStruct*)malloc(sizeof(frameStruct));
    memset(frame2send,'\0',sizeof(frameStruct));
    if( !ui->checkBlind->isChecked() ){
        frame2send->header.idMsg = (unsigned char)2;
    }else{
        frame2send->header.idMsg = (unsigned char)3;
    }
    frame2send->header.numTotMsg = 1;
    frame2send->header.consecutive = 1;
    frame2send->header.trigeredTime = 0;
    frame2send->header.bodyLen   = ui->txtCommand->text().length();
    bzero(frame2send->msg,ui->txtCommand->text().length()+1);
    memcpy(
                frame2send->msg,
                ui->txtCommand->text().toStdString().c_str(),
                ui->txtCommand->text().length()
          );

    //Prepare command message
    int sockfd, n, tmpFrameLen;
    //unsigned char bufferRead[sizeof(frameStruct)];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    qDebug() << "Comm IP: " << QString((char*)camSelected->IP);
    if (sockfd < 0){
        qDebug() << "opening socket";
        return (void)NULL;
    }
    //server = gethostbyname( ui->tableLstCams->item(tmpRow,1)->text().toStdString().c_str() );
    server = gethostbyname( (char*)camSelected->IP );
    if (server == NULL) {
        qDebug() << "Not such host";
        return (void)NULL;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(camSelected->tcpPort);
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        qDebug() << "ERROR: connecting to socket";
        return (void)NULL;
    }


    //Request command result
    qDebug() << "idMsg: " << (int)frame2send->header.idMsg;
    qDebug() << "command: " << frame2send->msg;
    qDebug() << "tmpFrameLen: " << sizeof(frameHeader) + ui->txtCommand->text().length();
    tmpFrameLen = sizeof(frameHeader) + ui->txtCommand->text().length();
    n = ::write(sockfd,frame2send,tmpFrameLen+1);
    if(n<0){
        qDebug() << "ERROR: Sending command";
        return (void)NULL;
    }

    //Receibing ack with file len
    unsigned int fileLen;
    unsigned char bufferRead[frameBodyLen];
    n = read(sockfd,bufferRead,frameBodyLen);
    memcpy(&fileLen,&bufferRead,sizeof(unsigned int));
    fileLen = (fileLen<frameBodyLen)?frameBodyLen:fileLen;
    qDebug() << "fileLen: " << fileLen;
    //funcShowMsg("FileLen n("+QString::number(n)+")",QString::number(fileLen));

    //Receive File
    unsigned char tmpFile[fileLen];
    funcReceiveFile( sockfd, fileLen, bufferRead, tmpFile );
    qDebug() <<tmpFile;
    ::close(sockfd);

    //Show command result
    std::string tmpTxt((char*)tmpFile);
    qDebug() << "Get: " << (char*)tmpFile;
    ui->txtCommRes->setText( QString(tmpTxt.c_str()) ) ;

}

void MainWindow::on_pbConnect_clicked()
{
    ui->tableLstCams->setFocus();
    int numRow = ui->tableLstCams->rowCount();    
    if( numRow > 0 ){
        int rowSelected = ui->tableLstCams->currentRow();
        qDebug() << "CurrentRow: " << QString::number(rowSelected);
        if(rowSelected >= 0){
            if( camSelected->isConnected == true ){
                qDebug() << "Disconnecting";
                camSelected->isConnected = false;
                camSelected->tcpPort = 0;
                memset(camSelected->IP,'\0',sizeof(camSelected->IP));
                ui->pbConnect->setIcon( QIcon(":/new/icons/imagenInte/right.gif") );
                ui->pbSnapshot->setEnabled(false);
                //ui->pbGetSlideCube->setEnabled(false);
                ui->pbShutdown->setDisabled(true);
            }else{
                qDebug() << "Connecting: ";                
                camSelected->tcpPort = lstSettings->tcpPort;
                memcpy(
                            camSelected->IP,
                            ui->tableLstCams->item(rowSelected,1)->text().trimmed().toStdString().c_str(),
                            sizeof(camSelected->IP)
                      );
                camSelected->isConnected = true;
                ui->pbConnect->setIcon( QIcon(":/new/icons/imagenInte/close.png") );
                ui->pbSnapshot->setEnabled(true);
                //ui->pbGetSlideCube->setEnabled(true);
                ui->pbShutdown->setDisabled(false);
                qDebug() << "IP->: " << QString((char*)camSelected->IP);
                //Save last IP
                saveFile( _PATH_LAST_IP, QString((char*)camSelected->IP) );
            }
        }else{
            ui->pbSnapshot->setEnabled(false);
            funcShowMsg("ERROR", "Camara not selected");            
        }
    }else{
        funcShowMsg("ERROR", "Empty cam list");
        ui->pbSnapshot->setEnabled(false);
    }

}

/*
void MainWindow::on_pbCamTurnOn_clicked()
{
    unsigned char v[20];
    v[0] = 'H';
    v[1] = 'o';
    v[2] = 'l';
    v[3] = 'a';
    std::string v2;
    v2.assign((char*)v);
    qDebug() << "Size: " << v2.size();

}
*/




/*
void MainWindow::on_pbStartVideo_clicked()
{    
    if( !camSelected->isConnected ){
        funcShowMsg("Lack","Select and connect a camera");
    }else{
        //Save camera settings
        //..
        saveRaspCamSettings( "tmp" );

        //Play
        //..
        if( camSelected->stream ){//Playing video
            camSelected->stream = false;
            //ui->pbStartVideo->setIcon( QIcon(":/new/icons/imagenInte/player_play.svg") );
            //ui->pbStartVideo->setToolTip("Play");
            qDebug() << "Paused";
        }else{
            if( ui->cbOneShot->isChecked() ){
                funcUpdateVideo( 100, 2.3 );
            }else{
                camSelected->stream = true;
                ui->pbStartVideo->setIcon( QIcon(":/new/icons/imagenInte/pause.png") );
                ui->pbStartVideo->setToolTip("Pause");
                qDebug() << "Play";
                QtDelay(100);
                while( camSelected->stream ){
                    funcUpdateVideo( 100, 2.3 );
                    QtDelay(700);
                }
            }
        }
    }

}
*/

void MainWindow::progBarTimer( int ms )
{

    //this->moveToThread(progBarThread);
    ui->progBar->setVisible(true);
    ui->progBar->setMinimum(0);
    ui->progBar->setMaximum(100);
    ui->progBar->setValue(0);
    ui->progBar->update();

    int step = floor( (float)ms/100.0);
    int i;
    for( i=0; i<=100; i++ )
    {
        ui->progBar->setValue(i);
        ui->progBar->update();
        QThread::msleep(step);
    }
    i=0;

    ui->progBar->setValue(0);
    ui->progBar->update();
    ui->progBar->setVisible(false);
    ui->progBar->update();
}

//unsigned char *MainWindow::funcGetRemoteImg( strReqImg *reqImg, bool saveImg ){
bool MainWindow::funcGetRemoteImg( strReqImg *reqImg, bool saveImg ){

    //Open socket
    int n;
    int sockfd = connectSocket( camSelected );
    unsigned char bufferRead[frameBodyLen];
    qDebug() << "Socket opened";
    //Require photo size
    //QtDelay(5);
    n = ::write(sockfd,reqImg,sizeof(strReqImg));
    qDebug() << "Img request sent";        

    //Receive ACK with the camera status
    memset(bufferRead,'\0',3);
    n = read(sockfd,bufferRead,2);
    if( bufferRead[1] == 1 ){//Begin the image adquisition routine
        qDebug() << "Camera OK";
        ::close(sockfd);

        progBarUpdateLabel("Stabilizing camera",0);
        progBarTimer( (reqImg->raspSett.TriggeringTimeSecs + 1) * 1000 );

        //Get File
        if( saveImg )
        {
            progBarUpdateLabel("Stabilizing remote camera",0);
            obtainImageFile( _PATH_REMOTE_SNAPSHOT, _PATH_IMAGE_RECEIVED );
        }

    }else{//Do nothing becouse camera is not accessible
        qDebug() << "ERROR turning on the camera";
        ::close(sockfd);
        return false;
    }
    n=n;

    /*
    //Receive photo's size
    QtDelay(1000);
    int fileLen;
    memset(bufferRead,'\0',sizeof(int)+1);
    n = read(sockfd,bufferRead,sizeof(int));

    memcpy(&fileLen,&bufferRead,sizeof(int));
    qDebug() << "Receiving fileLen: " << QString::number(fileLen);
    */

    /*
    //Receive File photogram
    int buffLen = ceil((float)fileLen/(float)frameBodyLen)*frameBodyLen;
    unsigned char *tmpFile = (unsigned char*)malloc(buffLen);
    QtDelay(60);
    if( funcReceiveFile( sockfd, fileLen, bufferRead, tmpFile ) ){
        qDebug() << "Frame received";
    }else{
        qDebug() << "ERROR: Frame does not received";
    }

    //Save a backup of the image received
    //..
    if( saveImg )
    {
        if(!saveBinFile( (unsigned long)fileLen, tmpFile,_PATH_IMAGE_RECEIVED))
        {
            qDebug()<< "ERROR: saving image-file received";
        }
    }
    */


    //Close socket and return

    //return tmpFile;

    funcLabelProgBarHide();

    return true;
}

void MainWindow::progBarUpdateLabel( QString txt, int color )
{
    ui->labelProgBar->setVisible(true);
    ui->labelProgBar->setText(txt);
    if( color == 0 )
        ui->labelProgBar->setStyleSheet("QLabel { color : black; }");
    if( color == 1 )
        ui->labelProgBar->setStyleSheet("QLabel { color : white; }");

    ui->labelProgBar->update();
    QtDelay(10);
}

void MainWindow::funcLabelProgBarHide()
{
    ui->labelProgBar->setVisible(false);
    ui->labelProgBar->update();
}

structRaspcamSettings MainWindow::funcFillSnapshotSettings( structRaspcamSettings raspSett ){
    //Take settings from gui ;D
    //raspSett.width                 = ui->slideWidth->value();
    //raspSett.height                = ui->slideHeight->value();
    memcpy(
                raspSett.AWB,
                ui->cbAWB->currentText().toStdString().c_str(),
                sizeof(raspSett.AWB)
          );
    //raspSett.Brightness            = ui->slideBrightness->value();
    //raspSett.Contrast              = ui->slideContrast->value();
    memcpy(
                raspSett.Exposure,
                ui->cbExposure->currentText().toStdString().c_str(),
                sizeof(raspSett.Exposure)
          );
    //raspSett.ExposureCompensation     = ui->slideExpComp->value();
    //raspSett.Format                   = ( ui->rbFormat1->isChecked() )?1:2;
    //raspSett.Green                    = ui->slideGreen->value();
    raspSett.ISO                        = ui->slideISO->value();
    //raspSett.Red                      = ui->slideRed->value();
    //raspSett.Saturation               = ui->slideSaturation->value();
    //raspSett.Sharpness                = ui->slideSharpness->value();
    raspSett.ShutterSpeedMs             = ui->spinBoxShuterSpeed->value();
    raspSett.SquareShutterSpeedMs       = ui->spinBoxSquareShuterSpeed->value();
    raspSett.Denoise                    = (ui->cbDenoise->isChecked())?1:0;
    raspSett.ColorBalance               = (ui->cbColorBalance->isChecked())?1:0;
    raspSett.TriggeringTimeSecs         = ui->slideTriggerTime->value();

    return raspSett;
}

unsigned char * MainWindow::funcObtVideo( unsigned char saveLocally ){

    int n;

    qDebug() << "Dentro 1";
    //Open socket
    int sockfd = connectSocket( camSelected );
    unsigned int fileLen;
    unsigned char bufferRead[frameBodyLen];

    //Require photo size
    qDebug() << "Dentro 2";
    unsigned char tmpInstRes[2];
    tmpInstRes[0] = (unsigned char)6;//Request photo size
    tmpInstRes[1] = saveLocally;
    n = ::write(sockfd,&tmpInstRes,2);

    unsigned char *tmpFile;

    if( saveLocally==1 ){

        //Receive photo's size
        qDebug() << "Dentro 3";
        n = read(sockfd,bufferRead,frameBodyLen);
        memcpy(&fileLen,&bufferRead,sizeof(unsigned int));
        qDebug() << "fileLen: " << QString::number(fileLen);

        //Receive File photogram
        qDebug() << "Dentro 4";
        tmpFile = (unsigned char*)malloc(fileLen);

        if( funcReceiveFile( sockfd, fileLen, bufferRead, tmpFile ) ){
            qDebug() << "Big image received";
        }else{
            qDebug() << "ERROR: Big image does not received";
        }
    }else{
        tmpFile = (unsigned char*)malloc(2);
    }

    //Close socket
    ::close(sockfd);
    n=n;

    return tmpFile;
}

bool MainWindow::funcUpdateVideo( unsigned int msSleep, int sec2Stab ){

    msSleep = msSleep;
    mouseCursorWait();
    this->update();

    //Set required image's settings
    //..
    strReqImg *reqImg   = (strReqImg*)malloc(sizeof(strReqImg));
    reqImg->idMsg       = (unsigned char)7;    
    reqImg->stabSec     = sec2Stab;    
    reqImg->raspSett    = funcFillSnapshotSettings( reqImg->raspSett );

    qDebug() << "reqImg->raspSett.TriggerTime: " << reqImg->raspSett.TriggeringTimeSecs;

    //Define photo's region
    //..
    //QString tmpSquare2Load = (ui->cbPreview->isChecked())?_PATH_REGION_OF_INTERES:_PATH_SQUARE_APERTURE;
    if( !funGetSquareXML( _PATH_SQUARE_APERTURE2, &reqImg->sqApSett ) ){
        funcShowMsg("ERROR","Loading squareAperture.xml");
        return false;
    }
    reqImg->needCut     = true;
    reqImg->squApert    = true;
    //Calculate real number of columns of the required photo
    reqImg->sqApSett.rectX  = round( ((float)reqImg->sqApSett.rectX/(float)reqImg->sqApSett.canvasW) * (float)camRes->width);
    reqImg->sqApSett.rectY  = round( ((float)reqImg->sqApSett.rectY/(float)reqImg->sqApSett.canvasH) * (float)camRes->height);
    reqImg->sqApSett.rectW  = round( ((float)reqImg->sqApSett.rectW/(float)reqImg->sqApSett.canvasW) * (float)camRes->width);
    reqImg->sqApSett.rectH  = round( ((float)reqImg->sqApSett.rectH/(float)reqImg->sqApSett.canvasH) * (float)camRes->height);

    //It save the received image
    funcGetRemoteImg( reqImg, true );
    QImage tmpImg(_PATH_IMAGE_RECEIVED);

    //tmpImg.save("./Results/tmpCropFromSave.ppm");
    ui->labelVideo->setPixmap( QPixmap::fromImage(tmpImg) );
    ui->labelVideo->setFixedWidth( tmpImg.width() );
    ui->labelVideo->setFixedHeight( tmpImg.height() );
    ui->labelVideo->update();

    //Delay in order to refresh actions applied    
    this->update();
    return true;
}







/*
void MainWindow::on_pbSaveImage_clicked()
{    
    int n;
    bool tmpSaveLocaly = funcShowMsgYesNo("Question","Bring into this PC?");
    //Stop streaming
    ui->pbStartVideo->click();
    ui->pbStartVideo->setEnabled(false);
    ui->pbStartVideo->update();

    //Open socket
    int tmpFrameLen = 2+sizeof(structRaspcamSettings);;
    int sockfd = connectSocket( camSelected );

    //Turn off camera
    unsigned char tmpInstRes[tmpFrameLen];
    tmpInstRes[0] = (unsigned char)5;//Camera instruction
    tmpInstRes[1] = (unsigned char)2;
    n = ::write(sockfd,&tmpInstRes,2);
    bzero(tmpInstRes,tmpFrameLen);
    n = read(sockfd,tmpInstRes,2);
    ::close(sockfd);

    //Turn on camera with new parameters
    sockfd = connectSocket( camSelected );
    //raspcamSettings->width  = _BIG_WIDTH;//2592, 640
    //raspcamSettings->height = _BIG_HEIGHT;//1944, 480
    tmpInstRes[0] = 5;
    tmpInstRes[1] = 1;
    memcpy( &tmpInstRes[2], raspcamSettings, sizeof(structRaspcamSettings) );
    n = ::write(sockfd,&tmpInstRes,tmpFrameLen);
    bzero(tmpInstRes,tmpFrameLen);
    n = read(sockfd,tmpInstRes,2);
    //qDebug() << "n: " << QString::number(n);
    qDebug() << "instRes: " << tmpInstRes[1];
    if( tmpInstRes[1] == (unsigned char)0 ){
        funcShowMsg("ERROR","Camera did not completed the instruction");
    }
    ::close(sockfd);


    if( tmpSaveLocaly ){

        //Get big file
        unsigned char *tmpFile = funcObtVideo( (unsigned char)1 );

        //Take file name
        char tmpFileName[16+14];
        snprintf(tmpFileName, 16+14, "tmpImages/%lu.ppm", time(NULL));

        std::ofstream outFile ( tmpFileName, std::ios::binary );
        //outFile<<"P6\n"<<raspcamSettings->width<<" "<<raspcamSettings->height<<" 255\n";
        //outFile.write( (char*)tmpFile, 3*raspcamSettings->width*raspcamSettings->height );
        outFile.close();
        delete tmpFile;
        qDebug() << "Big image saved locally";

    }else{

        //Send instruction: Save image in HypCam
        sockfd = connectSocket( camSelected );
        tmpInstRes[0] = (unsigned char)6;//Camera instruction
        tmpInstRes[1] = (unsigned char)2;
        n = ::write(sockfd,&tmpInstRes,2);
        bzero(tmpInstRes,tmpFrameLen);
        n = read(sockfd,tmpInstRes,2);
        qDebug() << "Image saved into HypCam";
        ::close(sockfd);

    }

    qDebug() << "Before turn off";

    //Turn off camera
    sockfd = connectSocket( camSelected );
    tmpInstRes[0] = (unsigned char)5;//Camera instruction
    tmpInstRes[1] = (unsigned char)2;
    n = ::write(sockfd,&tmpInstRes,2);
    bzero(tmpInstRes,tmpFrameLen);
    n = read(sockfd,tmpInstRes,2);
    ::close(sockfd);
    qDebug() << "Camera Turned off";

    //Turn on camera with new parameters
    sockfd = connectSocket( camSelected );
    funcSetCam(raspcamSettings);
    tmpInstRes[0] = 5;
    tmpInstRes[1] = 1;
    memcpy( &tmpInstRes[2], raspcamSettings, sizeof(structRaspcamSettings) );
    n = ::write(sockfd,&tmpInstRes,tmpFrameLen);
    bzero(tmpInstRes,tmpFrameLen);
    n = read(sockfd,tmpInstRes,2);
    qDebug() << "instRes: " << tmpInstRes[1];
    if( tmpInstRes[1] == (unsigned char)0 ){
        funcShowMsg("ERROR","Camera did not completed the instruction");
    }
    ::close(sockfd);

    //Start streaming
    ui->pbStartVideo->setEnabled(true);
    ui->pbStartVideo->click();

    n=n;
}
*/

/*
void MainWindow::on_slideShuterSpeed_valueChanged(int value)
{
    QString qstrVal = QString::number(value + ui->slideShuterSpeedSmall->value());
    ui->labelShuterSpeed->setText( "Diffraction Shuter Speed: " + qstrVal );
}
*/
void MainWindow::on_slideISO_valueChanged(int value)
{
    ui->labelISO->setText( "ISO: " + QString::number(value) );
}


void MainWindow::on_pbSaveRaspParam_clicked()
{
    if( ui->txtCamParamXMLName->text().isEmpty() ){
        funcShowMsg("Lack","Type the scenario's name");
    }else{

        QString tmpName = ui->txtCamParamXMLName->text();
        tmpName.replace(".xml","");
        tmpName.replace(".XML","");
        qDebug() << tmpName;

        bool saveFile = false;
        if( QFileInfo::exists( "./XML/camPerfils/" + tmpName + ".xml" ) ){
            if( funcShowMsgYesNo("Alert","Replace existent file?") ){
                QFile file( "./XML/camPerfils/" + tmpName + ".xml" );
                file.remove();
                saveFile = true;
            }
        }else{
            saveFile = true;
        }

        if( saveFile ){
            if( saveRaspCamSettings( tmpName ) ){
                funcShowMsg("Success","File stored successfully");
            }else{
                funcShowMsg("ERROR","Saving raspcamsettings");
            }
        }
    }
}

bool MainWindow::saveRaspCamSettings( QString tmpName ){

    //----------------------------------------------
    //Conditional variables
    //----------------------------------------------
    if( tmpName.isEmpty() )return false;

    tmpName = tmpName.replace(".xml","");
    tmpName = tmpName.replace(".XML","");

    //Resolution
    int tmpResInMp = -1;
    if( ui->radioRes5Mp->isChecked() )
    {
        tmpResInMp = 5;
    }
    else
    {
        tmpResInMp = 8;
    }

    //----------------------------------------------
    //Prepare file contain
    //----------------------------------------------
    QString newFileCon = "";
    QString flipped = (ui->cbFlipped->isChecked())?"1":"0";
    QString denoiseFlag = (ui->cbDenoise->isChecked())?"1":"0";
    QString colbalFlag = (ui->cbColorBalance->isChecked())?"1":"0";
    newFileCon.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    newFileCon.append("<settings>\n");
    newFileCon.append("    <AWB>"+ ui->cbAWB->currentText() +"</AWB>\n");
    newFileCon.append("    <Exposure>"+ ui->cbExposure->currentText() +"</Exposure>\n");
    newFileCon.append("    <Denoise>"+ denoiseFlag +"</Denoise>\n");
    newFileCon.append("    <ColorBalance>"+ colbalFlag +"</ColorBalance>\n");
    newFileCon.append("    <TriggeringTimeSecs>"+ QString::number( ui->slideTriggerTime->value() ) +"</TriggeringTimeSecs>\n");
    newFileCon.append("    <ShutterSpeedMs>"+ QString::number( ui->spinBoxShuterSpeed->value() ) +"</ShutterSpeedMs>\n");
    newFileCon.append("    <SquareShutterSpeedMs>"+ QString::number( ui->spinBoxSquareShuterSpeed->value() ) +"</SquareShutterSpeedMs>\n");
    newFileCon.append("    <TimelapseDurationSecs>"+ QString::number( ui->spinBoxTimelapseDuration->value() ) +"</TimelapseDurationSecs>\n");
    newFileCon.append("    <TimelapseInterval_ms>"+ QString::number( ui->spinBoxTimelapse->value() ) +"</TimelapseInterval_ms>\n");
    newFileCon.append("    <VideoDurationSecs>"+ QString::number( ui->spinBoxVideoDuration->value() ) +"</VideoDurationSecs>\n");
    newFileCon.append("    <ISO>"+ QString::number( ui->slideISO->value() ) +"</ISO>\n");
    newFileCon.append("    <CameraMp>"+ QString::number( tmpResInMp ) +"</CameraMp>\n");
    newFileCon.append("    <Flipped>"+ flipped +"</Flipped>\n");
    newFileCon.append("</settings>");

    //----------------------------------------------
    //Save
    //----------------------------------------------
    QFile newFile( "./XML/camPerfils/" + tmpName + ".xml" );
    if(newFile.exists())newFile.remove();
    if ( newFile.open(QIODevice::ReadWrite) ){
        QTextStream stream( &newFile );
        stream << newFileCon << endl;
        newFile.close();
    }else{
        return false;
    }
    return true;
}

void MainWindow::on_pbObtPar_clicked()
{

    QString filePath = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select XML..."),
                                                        "./XML/camPerfils/",
                                                        "(*.xml);;"
                                                     );
    if( !filePath.isEmpty() ){
        QStringList tmpList = filePath.split("/");
        tmpList = tmpList.at( tmpList.count()-1 ).split(".");
        ui->txtCamParamXMLName->setText( tmpList.at( 0 ) );

        funcGetRaspParamFromXML( raspcamSettings, filePath );
        funcIniCamParam( raspcamSettings );
    }
}



bool MainWindow::funcSetCam( structRaspcamSettings *raspcamSettings ){


        memcpy(
                    raspcamSettings->AWB,
                    ui->cbAWB->currentText().toStdString().c_str(),
                    sizeof(raspcamSettings->AWB)
              );

        memcpy(
                    raspcamSettings->Exposure,
                    ui->cbExposure->currentText().toStdString().c_str(),
                    sizeof(raspcamSettings->Exposure)
              );


        //raspcamSettings->width = ui->slideWidth->value();

        //raspcamSettings->height = ui->slideHeight->value();

        //raspcamSettings->Brightness = ui->slideBrightness->value();

        //raspcamSettings->Sharpness = ui->slideSharpness->value();

        //raspcamSettings->Contrast = ui->slideContrast->value();

        //raspcamSettings->Saturation = ui->slideSaturation->value();

        raspcamSettings->ShutterSpeedMs = ui->spinBoxShuterSpeed->value();

        raspcamSettings->ISO = ui->slideISO->value();

        //raspcamSettings->ExposureCompensation = ui->slideExpComp->value();

        //raspcamSettings->Red = ui->slideRed->value();

        //raspcamSettings->Green = ui->slideGreen->value();



    /*
    memcpy(raspcamSettings->AWB,"AUTO\0",5);            // OFF,AUTO,SUNLIGHT,CLOUDY,TUNGSTEN,FLUORESCENT,INCANDESCENT,FLASH,HORIZON
    memcpy(raspcamSettings->Exposure,"FIREWORKS\0",6);      // OFF,AUTO,NIGHT,NIGHTPREVIEW,BACKLIGHT,SPOTLIGHT,SPORTS,SNOW,BEACH,VERYLONG,FIXEDFPS,ANTISHAKE,FIREWORKS

    raspcamSettings->width                  = 384;      // 1280 to 2592
    raspcamSettings->height                 = 288;      // 960 to 1944
    raspcamSettings->Brightness             = 50;       // 0 to 100
    raspcamSettings->Sharpness              = 0;        // -100 to 100
    raspcamSettings->Contrast               = 0;        // -100 to 100
    raspcamSettings->Saturation             = 0;        // -100 to 100
    raspcamSettings->ShutterSpeed           = 1000*75;  // microsecs (max 330000)
    raspcamSettings->ISO                    = 400;      // 100 to 800
    raspcamSettings->ExposureCompensation   = 0;        // -10 to 10
    raspcamSettings->Format                 = 2;        // 1->raspicam::RASPICAM_FORMAT_GRAY | 2->raspicam::RASPICAM_FORMAT_YUV420
    raspcamSettings->Red                    = 0;        // 0 to 8 set the value for the RED component of white balance
    raspcamSettings->Green                  = 0;        // 0 to 8 set the value for the GREEN component of white balance
    */
    return true;
}


void MainWindow::funcGetSnapshot()
{

    //Getting calibration
    //..
    lstDoubleAxisCalibration daCalib;
    funcGetCalibration(&daCalib);

    //Save path
    //..
    //saveFile(_PATH_LAST_SNAPPATH,ui->txtSnapPath->text());

    //Save lastest settings
    if( saveRaspCamSettings( _PATH_LAST_SNAPPATH ) == false ){
        funcShowMsg("ERROR","Saving last snap-settings");
    }

    //Set required image's settings
    //..
    strReqImg *reqImg       = (strReqImg*)malloc(sizeof(strReqImg));
    memset(reqImg,'\0',sizeof(strReqImg));
    reqImg->idMsg           = (unsigned char)7;
    reqImg->raspSett        = funcFillSnapshotSettings( reqImg->raspSett );

    //Define photo's region
    //..
    reqImg->needCut     = false;
    reqImg->fullFrame   = true;
    reqImg->imgCols     = camRes->width;//2592 | 640
    reqImg->imgRows     = camRes->height;//1944 | 480
    //It saves image into HDD: _PATH_IMAGE_RECEIVED

    if( funcGetRemoteImg( reqImg, true ) )
    {
        //Display image
        QImage imgAperture( _PATH_IMAGE_RECEIVED );

        //Invert phot if required
        //if( _INVERTED_CAMERA )
        //    imgAperture = funcRotateImage( _PATH_DISPLAY_IMAGE , 180 );
        imgAperture.save( _PATH_DISPLAY_IMAGE );

        //Display image into qlabel
        updateDisplayImage(&imgAperture);
    }
    else funcShowMsg("ERROR","Camera respond with error");
    funcLabelProgBarHide();

}

void MainWindow::mergeSnapshot(QImage *diff, QImage *aper, lstDoubleAxisCalibration *daCalib )
{
    int x1, y1;
    int row, col;

    x1 = daCalib->squarePixX;
    y1 = daCalib->squarePixY;

    for(row=0;row<aper->height();row++)
    {
        for(col=0;col<aper->width();col++)
        {
            diff->setPixel(x1+col,y1+row,aper->pixel(col,row));
        }
    }

    diff->save( _PATH_DISPLAY_IMAGE );
}


/*
void MainWindow::getRemoteImgByPartsAndSave( strReqImg *reqImg )
{
    //It saves image into HDD: _PATH_IMAGE_RECEIVED
    funcGetRemoteImg( reqImg, true );
}
*/

/*
void MainWindow::calcRectangles(
                                    QList<QRect> *lstRect,
                                    strDiffProj  *p11Min,
                                    strDiffProj  *p12Min,
                                    strDiffProj  *p21Min,
                                    strDiffProj  *p22Min,
                                    strDiffProj  *p11Max,
                                    strDiffProj  *p12Max,
                                    strDiffProj  *p21Max,
                                    strDiffProj  *p22Max
                               ){

}
*/

void MainWindow::calcDiffPoints(
                                    float wave,
                                    strDiffProj *p11,
                                    strDiffProj *p12,
                                    strDiffProj *p21,
                                    strDiffProj *p22,
                                    lstDoubleAxisCalibration *daCalib
                                ){

    p11->wavelength     = wave;
    p12->wavelength     = wave;
    p21->wavelength     = wave;
    p22->wavelength     = wave;

    p11->x              = 1;                    //1-index
    p11->y              = 1;                    //1-index
    p12->x              = daCalib->squarePixW;  //1-index
    p12->y              = 1;                    //1-index
    p21->x              = 1;                    //1-index
    p21->y              = daCalib->squarePixH;  //1-index
    p22->x              = daCalib->squarePixW;  //1-index
    p22->y              = daCalib->squarePixH;  //1-index

    calcDiffProj( p11, daCalib );
    calcDiffProj( p12, daCalib );
    calcDiffProj( p21, daCalib );
    calcDiffProj( p22, daCalib );

}


void MainWindow::getMaxCalibRect( QRect *rect, lstDoubleAxisCalibration *calib )
{
    //It defines points in the rectangle
    QPoint p11, p12, p21, p22;
    p11.setX(rect->x());                    p11.setY(rect->y());
    p12.setX(rect->x()+rect->width());      p12.setY(rect->y());
    p21.setX(rect->x());                    p21.setY(rect->y()+rect->height());
    p22.setX(rect->x()+rect->width());      p22.setY(rect->y()+rect->height());

    //It calibrates rectangle
    calibPoint( &p11, calib );      calibPoint( &p12, calib );
    calibPoint( &p21, calib );      calibPoint( &p22, calib );

    //Defines rectangle calibrated
    int x1, y1, x2, y2, w, h;
    x1 = (p11.x()<p21.x())?p11.x():p21.x();
    y1 = (p11.y()<p12.y())?p11.y():p12.y();
    x2 = (p12.x()>p22.x())?p12.x():p22.x();
    y2 = (p21.y()>p22.y())?p21.y():p22.y();
    w  = abs(x2 - x1);
    h  = abs(y2 - y1);

    //Set calibrated rectangle
    rect->setX(x1);
    rect->setY(y1);
    rect->setWidth(w);
    rect->setHeight(h);

}

//QString MainWindow::getFilenameForRecImg()
//{
    //QString tmpFileName = ui->txtSnapPath->text().trimmed();
    //tmpFileName.append(QString::number(time(NULL)));
   // tmpFileName.append(".RGB888");
    //QFile::copy(_PATH_IMAGE_RECEIVED, tmpFileName);
    //return tmpFileName;
//}



void MainWindow::updateDisplayImage(QImage* tmpImg)
{
    mouseCursorWait();

    //Update Image Preview
    updatePreviewImage(tmpImg);

    //Update Edit View
    updateImageCanvasEdit(tmpImg);

    mouseCursorReset();

}

void MainWindow::updateDisplayImage(QString fileName)
{
    mouseCursorWait();

    //Load Image
    auxQstring = fileName;
    QImage tmpImg(auxQstring);

    //Update Image Preview
    updatePreviewImage(&tmpImg);

    //Update Edit View
    updateImageCanvasEdit(&tmpImg);

    mouseCursorReset();
}

void MainWindow::updatePreviewImage(QString* fileName)
{
    //
    //Display Snapshot
    //
    QImage tmpImg( *fileName );
    int maxW, maxH;
    maxW = (_FRAME_THUMB_W<tmpImg.width())?_FRAME_THUMB_W:tmpImg.width();
    maxH = (_FRAME_THUMB_H<tmpImg.height())?_FRAME_THUMB_H:tmpImg.height();
    QImage tmpThumb = tmpImg.scaledToHeight(maxH);
    tmpThumb = tmpThumb.scaledToWidth(maxW);
    ui->labelVideo->setPixmap( QPixmap::fromImage(tmpThumb) );
    ui->labelVideo->setFixedSize( tmpThumb.width(), tmpThumb.height() );
}

void MainWindow::updatePreviewImage(QImage* tmpImg)
{
    //
    //Display Snapshot
    //
    int maxW, maxH;
    maxW = (_FRAME_THUMB_W<tmpImg->width())?_FRAME_THUMB_W:tmpImg->width();
    maxH = (_FRAME_THUMB_H<tmpImg->height())?_FRAME_THUMB_H:tmpImg->height();
    QImage tmpThumb = tmpImg->scaledToHeight(maxH);
    tmpThumb = tmpThumb.scaledToWidth(maxW);
    ui->labelVideo->setPixmap( QPixmap::fromImage(tmpThumb) );
    ui->labelVideo->setFixedSize( tmpThumb.width(), tmpThumb.height() );
}

void MainWindow::on_pbSnapshot_clicked()
{

    /*
    mouseCursorWait();

    camRes = getCamRes();



    funcGetSnapshot();


    mouseCursorReset();
    */

    mouseCursorWait();

    if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,false) )
    {
        qDebug() << "ERROR: Taking Full Area";
        return (void)NULL;
    }

    //
    //Timer
    //
    int triggeringTime;
    triggeringTime = ui->slideTriggerTime->value();
    if( triggeringTime > 0 )
    {
        formTimerTxt* timerTxt = new formTimerTxt(this,"Remainning Time to Shoot...",triggeringTime);
        timerTxt->setModal(true);
        timerTxt->show();
        QtDelay(200);
        timerTxt->startMyTimer(triggeringTime);
    }

    //QImage snapShot = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
    *globalEditImg = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
    saveFile(_PATH_LAST_USED_IMG_FILENAME,_PATH_IMAGE_RECEIVED);
    updateDisplayImage(globalEditImg);
    //snapShot.save(_PATH_DISPLAY_IMAGE);

    funcResetStatusBar();
    mouseCursorReset();


    //
    // Show Last Preview
    //
    //updateDisplayImageReceived();


}





void MainWindow::funcPutImageIntoGV(QGraphicsView *gvContainer, QString imgPath){    
    QPixmap pim(imgPath);
    QGraphicsScene *scene = new QGraphicsScene(0,0,pim.width(),pim.height());
    scene->setBackgroundBrush(pim);
    gvContainer->setScene(scene);
    gvContainer->resize(pim.width(),pim.height());
    gvContainer->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    gvContainer->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    /*
    QHBoxLayout *layout = new QHBoxLayout;
    ui->tab_3->setLayout(layout);
    ui->tab_3->layout()->addWidget(gvContainer);
    layout->setEnabled(false);
    */
}


/*
void MainWindow::on_pbPtsClearAll_clicked()
{
    lstPixSelAux->clear();

    canvasAux->scene()->clear();

    lstBorder->clear();
    lstSelPix->clear();


}*/


/*
void MainWindow::on_pbSavePixs_clicked()
{
    if( lstSelPix->isEmpty() ){
        funcShowMsg("LACK","Not pixels to export");
    }else{
        int i;
        QString filePath = "./Results/lstPixels.txt";
        QFile fileLstPixels(filePath);
        if (!fileLstPixels.open(QIODevice::WriteOnly | QIODevice::Text)){
            funcShowMsg("ERROR","Creating file fileLstPixels");
        }else{
            QTextStream streamLstPixels(&fileLstPixels);
            for( i=0; i<lstSelPix->count(); i++ ){
                streamLstPixels << QString::number(lstSelPix->at(i).first) << " "<< QString::number(lstSelPix->at(i).second) << "\n";
            }
        }
        fileLstPixels.close();
        funcShowMsg("Success","List of pixels exported into: "+filePath);
    }
}
*/


/*
bool MainWindow::on_pb2XY_clicked()
{
    //Validate that exist pixel selected
    //..
    if( lstSelPix->count()<1){
        funcShowMsg("Lack","Not pixels selected");
        return false;
    }

    //Create xycolor space
    //..
    GraphicsView *xySpace = new GraphicsView(this);
    funcPutImageIntoGV(xySpace, "./imgResources/CIEManual.png");
    xySpace->setWindowTitle( "XY color space" );
    xySpace->show();

    //Transform each pixel from RGB to xy and plot it
    //..
    QImage tmpImg(imgPath);
    QRgb tmpPix;
    colSpaceXYZ *tmpXYZ = (colSpaceXYZ*)malloc(sizeof(colSpaceXYZ));
    int tmpOffsetX = -13;
    int tmpOffsetY = -55;
    int tmpX, tmpY;
    int i;
    for( i=0; i<lstSelPix->count(); i++ ){
        tmpPix = tmpImg.pixel( lstSelPix->at(i).first, lstSelPix->at(i).second );
        funcRGB2XYZ( tmpXYZ, (float)qRed(tmpPix), (float)qGreen(tmpPix), (float)qBlue(tmpPix) );
        //funcRGB2XYZ( tmpXYZ, 255.0, 0, 0  );
        tmpX = floor( (tmpXYZ->x * 441.0) / 0.75 ) + tmpOffsetX;
        tmpY = 522 - floor( (tmpXYZ->y * 481.0) / 0.85 ) + tmpOffsetY;
        funcAddPoit2Graph( xySpace, tmpX, tmpY, 1, 1,
                           QColor(qRed(tmpPix),qGreen(tmpPix),qBlue(tmpPix)),
                           QColor(qRed(tmpPix),qGreen(tmpPix),qBlue(tmpPix))
                         );
    }

    //Save image plotted
    //..
    QPixmap pixMap = QPixmap::grabWidget(xySpace);
    pixMap.save("./Results/Miscelaneas/RGBPloted.png");

    return true;
}
*/


/*
void MainWindow::on_pbLoadImg_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"./imgResources/",tr("Image files (*.*)"));
    imgPath = fileNames.at(0);
    funcPutImageIntoGV( myCanvas, imgPath );
    //funcShowMsg("",fileNames.at(0));
}
*/

/*
void MainWindow::on_pbUpdCut_clicked()
{

    //Set Canvas dimensions
    //..
    int maxW = 640;
    int maxH = 480;

    QString imgPath = "./imgResources/tmpBigToCut.ppm";

    if( funcShowMsgYesNo("Alert","Get remote image?") ){
        //Set required image's settings
        //..
        ui->progBar->setVisible(true);
        strReqImg *reqImg       = (strReqImg*)malloc(sizeof(strReqImg));
        reqImg->idMsg           = (unsigned char)7;
        reqImg->needCut         = false;
        reqImg->stabSec         = 3;
        reqImg->imgCols         = _BIG_WIDTH;//2592 | 640
        reqImg->imgRows         = _BIG_HEIGHT;//1944 | 480
        reqImg->raspSett        = funcFillSnapshotSettings( reqImg->raspSett );
        unsigned char *tmpFrame = funcGetRemoteImg( reqImg );
        std::ofstream outFile ( imgPath.toStdString(), std::ios::binary );
        outFile<<"P6\n"<<reqImg->imgCols<<" "<<reqImg->imgRows<<" 255\n";
        outFile.write( (char*)tmpFrame, 3*reqImg->imgCols*reqImg->imgRows );
        outFile.close();
        //delete tmpFrame;
        qDebug() << "Big image saved locally";
        ui->progBar->setVisible(false);
    }

    //Load image
    //..
    QImage imgOrig(imgPath);
    QImage tmpImg = imgOrig.scaled(QSize(640,480),Qt::KeepAspectRatio);
    QGraphicsScene *scene = new QGraphicsScene(0,0,tmpImg.width(),tmpImg.height());
    scene->setBackgroundBrush( QPixmap::fromImage(tmpImg) );
    ui->gvCut->setScene( scene );

    //Set UI
    //..
    ui->gvCut->setFixedSize( maxW, maxH );
    ui->gvCut->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->gvCut->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->slideCutPosX->setMaximum( maxW );
    ui->slideCutPosY->setMaximum(maxH);
    ui->slideCutWX->setMaximum( maxW );
    ui->slideCutWY->setMaximum( maxH );
    //ui->slideCutWX->setMinimum(5);
    //ui->slideCutWY->setMinimum(5);
    ui->slideCutWX->setEnabled(true);
    ui->slideCutWY->setEnabled(true);
    ui->slideCutPosX->setEnabled(true);
    ui->slideCutPosY->setEnabled(true);
    ui->pbSaveSquare->setEnabled(true);
    ui->pbSaveBigSquare->setEnabled(true);


}



void MainWindow::funcSetLines(){
    int xPos    = ui->slideCutPosX->value();
    int xW      = ui->slideCutWX->value();
    int yPos    = ui->slideCutPosY->value();
    int yW      = ui->slideCutWY->value();
    ui->gvCut->scene()->clear();
    ui->gvCut->scene()->addLine(xPos,1,xPos,ui->gvCut->height(),QPen(Qt::red));
    ui->gvCut->scene()->addLine(xPos+xW,1,xPos+xW,ui->gvCut->height(),QPen(Qt::red));
    ui->gvCut->scene()->addLine(1,ui->gvCut->height()-yPos,ui->gvCut->width(),ui->gvCut->height()-yPos,QPen(Qt::red));
    ui->gvCut->scene()->addLine(1,ui->gvCut->height()-yPos-yW,ui->gvCut->width(),ui->gvCut->height()-yPos-yW,QPen(Qt::red));
}
*/

/*
void MainWindow::on_slideCutPosX_valueChanged(int xpos)
{
    xpos = xpos;
    funcSetLines();
}

void MainWindow::on_slideCutWX_valueChanged(int value)
{
    value = value;
    funcSetLines();
}

void MainWindow::on_slideCutWY_valueChanged(int value)
{
    value = value;
    funcSetLines();
}

void MainWindow::on_slideCutPosY_valueChanged(int value)
{
    value = value;
    funcSetLines();
}
*/
/*
void MainWindow::on_pbSaveSquare_clicked()
{
    if( funcShowMsgYesNo("Alert","Do you want to replace the setting?") == 1 ){
        if( funcSaveRect ( "./XML/squareAperture.xml" ) ){
            funcShowMsg("Success","Settings saved");
        }else{
            funcShowMsg("ERROR","It can NOT save settings");
        }
    }
}
*/

/*
bool MainWindow::funcSaveRect( QString fileName ){
    QString tmpContain;
    tmpContain.append( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" );
    tmpContain.append("<Variables>\n");
    tmpContain.append("\t<width>"+ QString::number(ui->gvCut->width()) +"</width>\n");
    tmpContain.append("\t<height>"+ QString::number(ui->gvCut->height()) +"</height>\n");
    tmpContain.append("\t<x1>"+ QString::number( ui->slideCutPosX->value() ) +"</x1>\n");
    tmpContain.append("\t<y1>"+ QString::number( ui->gvCut->height() - ui->slideCutPosY->value() - ui->slideCutWY->value() ) +"</y1>\n");
    tmpContain.append("\t<x2>"+ QString::number( ui->slideCutPosX->value() + ui->slideCutWX->value() ) +"</x2>\n");
    tmpContain.append("\t<y2>"+ QString::number( ui->gvCut->height() - ui->slideCutPosY->value() ) +"</y2>\n");
    tmpContain.append("</Variables>");
    if( !saveFile( fileName, tmpContain ) ){
        return false;
    }
    return true;
}
*/

/*
void MainWindow::on_pbSaveBigSquare_clicked()
{
    if( funcShowMsgYesNo("Alert","Do you want to replace the setting?") == 1 ){
        if( funcSaveRect ( "./XML/bigSquare.xml" ) ){
            funcShowMsg("Success","Settings saved");
        }else{
            funcShowMsg("ERROR","It can NOT save settings");
        }
    }
}
*/

void MainWindow::on_pbSpecSnapshot_clicked()
{
    /*
    //Turn on camera
    //..
    CvCapture* usbCam  = cvCreateCameraCapture( ui->sbSpecUsb->value() );
    cvSetCaptureProperty( usbCam,  CV_CAP_PROP_FRAME_WIDTH,  320*_FACT_MULT );
    cvSetCaptureProperty( usbCam,  CV_CAP_PROP_FRAME_HEIGHT, 240*_FACT_MULT );
    assert( usbCam );

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

            cv::imwrite( tmpName.toStdString(), cv::cvarrToMat(imgRot) );
            //cv::imwrite( tmpName.toStdString(), cv::Mat(imgRot, true) );
        }
        //Create canvas
        //Display accum
        //..        

        //Show image
        QPixmap pix(tmpName);        
        pix = pix.scaledToHeight( _GRAPH_HEIGHT );
        QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
        canvasSpec->setBackgroundBrush(pix);
        canvasSpec->setScene( scene );
        canvasSpec->resize(pix.width(),pix.height());
        canvasSpec->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        canvasSpec->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        ui->tab_5->layout()->addWidget(canvasSpec);
        ui->tab_5->layout()->setEnabled(false);
        ui->tab_5->layout()->setAlignment(Qt::AlignLeft);
        ui->gridColors->setAlignment(Qt::AlignLeft);
        ui->gridColors->setAlignment(Qt::AlignLeft);


        ui->slideRedLen->setMaximumWidth(pix.width());
        ui->slideGreenLen->setMaximumWidth(pix.width());
        ui->slideBlueLen->setMaximumWidth(pix.width());

        pix.save("./snapshots/tmpThumb.png");




    }else{
        funcShowMsg("ERROR", "cvQueryFrame empty");
    }
    cvReleaseCapture(&usbCam);

    //Reset slides
    ui->slideRedLen->setValue(0);
    ui->slideGreenLen->setValue(0);
    ui->slideBlueLen->setValue(0);

    //Reset lines
    funcUpdateColorSensibilities();
    funcDrawLines(0,0,0,0);
    */
}

void MainWindow::funcBeginRect(QMouseEvent* e){
    calStruct.x1 = e->x();
    calStruct.y1 = e->y();
    qDebug() << "funcBeginRect";
}

void MainWindow::funcEndRect(QMouseEvent* e, GraphicsView *tmpCanvas){
    calStruct.x2 = e->x();
    calStruct.y2 = e->y();
    ui->pbSpecCut->setEnabled(true);
    qDeleteAll(tmpCanvas->scene()->items());
    int x1, y1, x2, y2, w, h;
    x1 = (calStruct.x1<=e->x())?calStruct.x1:e->x();
    x2 = (calStruct.x1>=e->x())?calStruct.x1:e->x();
    y1 = (calStruct.y1<=e->y())?calStruct.y1:e->y();
    y2 = (calStruct.y1>=e->y())?calStruct.y1:e->y();
    w  = abs(x2-x1);
    h  = abs(y2-y1);
    QPoint p1, p2;
    p1.setX(x1);   p1.setY(y1);
    p2.setX(w);    p2.setY(h);
    customRect* tmpRect = new customRect(p1,p2,globalEditImg);
    tmpRect->mapToScene(p1.x(),p1.y(),p2.x(),p2.y());
    //customRect* tmpRect = new customRect(this);
    tmpRect->parameters.W = canvasCalib->scene()->width();
    tmpRect->parameters.H = canvasCalib->scene()->height();
    tmpRect->setPen( QPen(Qt::red) );
    tmpCanvas->scene()->addItem(tmpRect);    
    tmpRect->setFocus();
    tmpRect->parameters.movible = true;
    tmpRect->parameters.canvas = tmpCanvas;
    tmpRect->parameters.backgroundPath = auxQstring;
}

void MainWindow::funcAnalizeAreaSelected(QPoint p1, QPoint p2){
    p1 = p1;
    p2 = p2;
}

void MainWindow::on_chbRed_clicked()
{
    funcUpdateColorSensibilities();
    funcDrawLines(0,0,0,0);
}

void MainWindow::on_chbGreen_clicked()
{
    funcUpdateColorSensibilities();
    funcDrawLines(0,0,0,0);
}

void MainWindow::on_chbBlue_clicked()
{
    funcUpdateColorSensibilities();
    funcDrawLines(0,0,0,0);
}

void MainWindow::funcUpdateColorSensibilities(){
    //Accumulate values in each color
    //..
    canvasSpec->scene()->clear();
    QImage tmpImg( "./snapshots/tmpThumb.png" );
    int Red[tmpImg.width()];memset(Red,'\0',tmpImg.width());
    int Green[tmpImg.width()];memset(Green,'\0',tmpImg.width());
    int Blue[tmpImg.width()];memset(Blue,'\0',tmpImg.width());
    int r, c, maxR, maxG, maxB, xR = 0, xG = 0, xB = 0;
    maxR = 0;
    maxG = 0;
    maxB = 0;
    QRgb pixel;
    for(c=0;c<tmpImg.width();c++){
        Red[c]   = 0;
        Green[c] = 0;
        Blue[c]  = 0;
        for(r=0;r<tmpImg.height();r++){
            if(!tmpImg.valid(QPoint(c,r))){
                qDebug() << "Invalid r: " << r << "c: "<<c;
                qDebug() << "tmpImg.width(): " << tmpImg.width();
                qDebug() << "tmpImg.height(): " << tmpImg.height();
                return (void)NULL;
                close();
            }
            pixel     = tmpImg.pixel(QPoint(c,r));
            Red[c]   += qRed(pixel);
            Green[c] += qGreen(pixel);
            Blue[c]  += qBlue(pixel);
        }
        Red[c]   = round((float)Red[c]/tmpImg.height());
        Green[c] = round((float)Green[c]/tmpImg.height());
        Blue[c]  = round((float)Blue[c]/tmpImg.height());
        if( Red[c] > maxR ){
            maxR = Red[c];
            xR = c;
        }
        if( Green[c] > maxG ){
            maxG = Green[c];
            xG = c;
        }
        if( Blue[c] > maxB ){
            maxB = Blue[c];
            xB = c;
        }
        //qDebug() << "xR: " << xR << "xG: " << xG << "xB: " << xB;
    }

    //Move slides
    //..
    ui->slideRedLen->setMaximum(tmpImg.width());
    ui->slideGreenLen->setMaximum(tmpImg.width());
    ui->slideBlueLen->setMaximum(tmpImg.width());

    ui->slideRedLen->setValue(xR);
    ui->slideGreenLen->setValue(xG);
    ui->slideBlueLen->setValue(xB);

    //qDebug() << "c" << c << "maxR:"<<maxR<<" maxG:"<<maxG<<" maxB:"<<maxB;
    //qDebug() << "c" << c << "xR:"<<xR<<" xG:"<<xG<<" xB:"<<xB;
    //qDebug() << "tmpImg.width(): " << tmpImg.width();
    //qDebug() << "tmpImg.height(): " << tmpImg.height();

    //Draw lines and locate lines
    //..
    int tmpPoint1, tmpPoint2, tmpHeight;
    //tmpHeight = _GRAPH_HEIGHT;
    tmpHeight = tmpImg.height();
    for(c=1;c<tmpImg.width();c++){
        if( ui->chbRed->isChecked() ){
            tmpPoint1 = tmpHeight-Red[c-1];
            tmpPoint2 = tmpHeight-Red[c];
            //tmpPoint1 = tmpImg.height()-Red[c-1];
            //tmpPoint2 = tmpImg.height()-Red[c];
            canvasSpec->scene()->addLine( c, tmpPoint1, c+1, tmpPoint2, QPen(QColor("#FF0000")) );
        }
        if( ui->chbGreen->isChecked() ){
            tmpPoint1 = tmpHeight-Green[c-1];
            tmpPoint2 = tmpHeight-Green[c];
            canvasSpec->scene()->addLine( c, tmpPoint1, c+1, tmpPoint2, QPen(QColor("#00FF00")) );
        }
        if( ui->chbBlue->isChecked() ){
            tmpPoint1 = tmpHeight-Blue[c-1];
            tmpPoint2 = tmpHeight-Blue[c];
            canvasSpec->scene()->addLine( c, tmpPoint1, c+1, tmpPoint2, QPen(QColor("#0000FF")) );
        }        
    }
}

void MainWindow::funcDrawLines(int flag, int xR, int xG, int xB){
    //Draw lines
    //..

    ui->slideRedLen->setEnabled(true);
    ui->slideGreenLen->setEnabled(true);
    ui->slideBlueLen->setEnabled(true);

    if( flag == 0 ){//Get x from slides
        xR = ui->slideRedLen->value();
        xG = ui->slideGreenLen->value();
        xB = ui->slideBlueLen->value();
    }

    //qDebug() << "flag:" << flag << "xR:" << xR << " xG:" << xG << "xB: " << xB;

    canvasSpec->scene()->addLine( xR,1,xR,canvasSpec->height(),QPen(QColor("#FF0000")) );
    canvasSpec->scene()->addLine( xG,1,xG,canvasSpec->height(),QPen(QColor("#00FF00")) );
    canvasSpec->scene()->addLine( xB,1,xB,canvasSpec->height(),QPen(QColor("#0000FF")) );

    ui->slideRedLen->setValue(xR);
    ui->slideGreenLen->setValue(xG);
    ui->slideBlueLen->setValue(xB);

}

void MainWindow::on_pbSpecCut_clicked()
{
    //Prepare variables
    //..
    int w, h, W, H;
    QPixmap tmpPix("./snapshots/tmpUSB.png");
    W = tmpPix.width();
    H = tmpPix.height();
    w = canvasSpec->width();
    h = canvasSpec->height();
    //Extrapolate dimensions
    qDebug() << calStruct.x1 << ", " << calStruct.y1;
    qDebug() << calStruct.x2 << ", " << calStruct.y2;
    calStruct.X1 = round( ((float)W/(float)w)*(float)calStruct.x1 );
    calStruct.Y1 = round( ((float)H/(float)h)*(float)calStruct.y1 );
    calStruct.X2 = round( ((float)W/(float)w)*(float)calStruct.x2 );
    calStruct.Y2 = round( ((float)H/(float)h)*(float)calStruct.y2 );
    calStruct.lenW = abs(calStruct.X2-calStruct.X1);
    calStruct.lenH = abs(calStruct.Y2-calStruct.Y1);
    qDebug() << calStruct.X1 << ", " << calStruct.Y1;
    qDebug() << calStruct.X2 << ", " << calStruct.Y2;
    qDebug() << "lenW=" << calStruct.lenW;
    qDebug() << "lenH=" << calStruct.lenH;

    //Crop image
    //..
    //qDeleteAll(canvasSpec->scene()->items());
    QPixmap cropped = tmpPix.copy( QRect( calStruct.X1, calStruct.Y1, calStruct.lenW, calStruct.lenH ) );
    cropped.save("./snapshots/tmpThumb.png");
    cropped.save("./snapshots/tmpUSB.png");
    QGraphicsScene *scene = new QGraphicsScene(0,0,cropped.width(),cropped.height());
    canvasSpec->setBackgroundBrush(cropped);
    canvasSpec->setCacheMode(QGraphicsView::CacheNone);
    canvasSpec->setScene( scene );
    canvasSpec->resize(cropped.width(),cropped.height());
    canvasSpec->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasSpec->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->tab_5->layout()->addWidget(canvasSpec);
    ui->tab_5->layout()->setEnabled(false);
    ui->tab_5->layout()->setAlignment(Qt::AlignLeft);
    //Set slide max
    ui->slideRedLen->setMaximumWidth(cropped.width());
    ui->slideGreenLen->setMaximumWidth(cropped.width());
    ui->slideBlueLen->setMaximumWidth(cropped.width());



    //Update lines
    //..
    funcUpdateColorSensibilities();

    //Update lines
    //..
    funcDrawLines(0,0,0,0);

    //int tmpCanvW = _GRAPH_HEIGHT;
    int tmpCanvW = cropped.height();
    canvasSpec->resize(QSize(cropped.width(),tmpCanvW));


    ui->pbViewBack->setEnabled(true);
    ui->pbSnapCal->setEnabled(true);



}

void MainWindow::on_slideRedLen_sliderReleased()
{
    int x1, x2, x3;
    x1 = ui->slideRedLen->value();
    x2 = ui->slideGreenLen->value();
    x3 = ui->slideBlueLen->value();
    funcUpdateColorSensibilities();
    funcDrawLines(1,x1,x2,x3);
    qDebug() << "Slide released";
}


void MainWindow::on_slideBlueLen_sliderReleased()
{
    int x1, x2, x3;
    x1 = ui->slideRedLen->value();
    x2 = ui->slideGreenLen->value();
    x3 = ui->slideBlueLen->value();
    funcUpdateColorSensibilities();
    funcDrawLines(1,x1,x2,x3);
    qDebug() << "Slide released";
}

void MainWindow::on_slideGreenLen_sliderReleased()
{
    int x1, x2, x3;
    x1 = ui->slideRedLen->value();
    x2 = ui->slideGreenLen->value();
    x3 = ui->slideBlueLen->value();
    funcUpdateColorSensibilities();
    funcDrawLines(1,x1,x2,x3);
    qDebug() << "Slide released";
}


void MainWindow::on_pbViewBack_clicked()
{
    //canvasSpec->scene()->clear();
    //QGraphicsScene *scene = new QGraphicsScene(0,0,canvasSpec->scene()->width(),canvasSpec->scene()->height());
    //QPixmap backBlack("./imgResources/backBlack.png");
    //scene->setBackgroundBrush(backBlack);
    //canvasSpec->setScene(scene);
    canvasSpec->scene()->invalidate(1,1,200,100);

    canvasSpec->setStyleSheet("background-color: black;");


}

void MainWindow::on_pbSnapCal_clicked()
{
    /*
    QString imgCropPath = "./snapshots/tmp/calibCrop.png";

    //Get image from camera
    //..
    QString imgPath = "./snapshots/tmp/calib.png";
    if( _USE_CAM ){
        IplImage *imgCal = funcGetImgFromCam( ui->sbSpecUsb->value(), 500 );

        cv::imwrite( imgPath.toStdString(), cv::cvarrToMat(imgCal) );
        //cv::imwrite( imgPath.toStdString(), cv::Mat(imgCal, true) );
    }

    //Cut image
    //..
    QPixmap tmpOrigPix(imgPath);
    QPixmap tmpImgCrop = tmpOrigPix.copy( QRect( calStruct.X1, calStruct.Y1, calStruct.lenW, calStruct.lenH ) );
    tmpImgCrop.save(imgCropPath);

    //Analyze the image
    //..
    QImage *tmpImg = new QImage(imgCropPath);
    colorAnalyseResult *imgSumary = funcAnalizeImage(tmpImg);

    //Calculate wavelen
    //..
    //int tmpX;
    float tmpWave;
    float pixX[] = {
                        (float)ui->slideBlueLen->value(),
                        (float)ui->slideGreenLen->value(),
                        (float)ui->slideRedLen->value()
                   };
    linearRegresion *linReg = funcCalcLinReg( pixX );
    tmpWave = linReg->a + ( linReg->b * (float)imgSumary->maxMaxx );

    //Display scene with value calculated
    //..
    GraphicsView *wavelen = new GraphicsView(this);
    funcPutImageIntoGV(wavelen, imgCropPath);
    wavelen->setWindowTitle( "x("+ QString::number(imgSumary->maxMaxx) +
                             ") | Wavelen: " +
                             QString::number(tmpWave) );
    QString tmpColor = "#FF0000";
    if(imgSumary->maxMaxColor==2)tmpColor="#00FF00";
    if(imgSumary->maxMaxColor==3)tmpColor="#0000FF";
    wavelen->scene()->addLine(
                                    imgSumary->maxMaxx,
                                    1,
                                    imgSumary->maxMaxx,
                                    tmpImg->height(),
                                    QPen( QColor( tmpColor ) )
                             );
    wavelen->show();
    */
}







/*
void MainWindow::on_pbObtPar_2_clicked()
{
    //Select image
    //..
    auxQstring = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select image..."),
                                                        "./snapshots/Calib/",
                                                        "(*.ppm *.RGB888);;"
                                                     );
    if( auxQstring.isEmpty() ){
        return (void)NULL;
    }    

    //Create a copy of the image selected
    //..
    QImage origImg(auxQstring);
    qDebug() << "auxQstring: " << auxQstring;
    if( !origImg.save(_PATH_DISPLAY_IMAGE) ){
        funcShowMsg("ERROR","Creating image to display");
        return (void)NULL;
    }

    //Rotate if requires
    //..
    if( funcShowMsgYesNo("Alert","Rotate using saved rotation?") == 1 ){
        float rotAngle = getLastAngle();
        doImgRotation( rotAngle );
        globaIsRotated = true;
    }else{
        globaIsRotated = false;
    }

    //Refresh image in scene
    //..
    //Show image
    reloadImage2Display();

    //Load layout
    QLayout *layout = new QVBoxLayout();
    layout->addWidget(canvasCalib);
    layout->setEnabled(false);
    ui->tab_6->setLayout(layout);


    //qDebug() << "tres";
    //ui->tab_6->layout()->addWidget(canvasCalib);
    //qDebug() << "cuatro";
    //ui->tab_6->layout()->setEnabled(false);
    //ui->tab_6->layout()->setAlignment(Qt::AlignLeft);


    //refreshGvCalib( auxQstring );

    //It enables slides
    //..
    //ui->containerCalSave->setEnabled(true);
    ui->toolBarDraw->setEnabled(true);
    ui->toolBarDraw->setVisible(true);
    //ui->slide2AxCalThre->setEnabled(true);



}*/



void MainWindow::refreshGvCalib( QString fileName )
{
    //Add image to graphic view
    //
    QImage imgOrig(fileName);
    QImage tmpImg = imgOrig.scaled(QSize(640,480),Qt::KeepAspectRatio);
    QGraphicsScene *scene = new QGraphicsScene(0,0,tmpImg.width(),tmpImg.height());
    canvasCalib->originalW  = tmpImg.width();
    canvasCalib->originalH  = tmpImg.height();
    canvasCalib->scene()->setBackgroundBrush( QPixmap::fromImage(tmpImg) );
    canvasCalib->resize(tmpImg.width(),tmpImg.height());
    scene->setBackgroundBrush( QPixmap::fromImage(tmpImg) );
    canvasCalib->setScene( scene );
    canvasCalib->resize(tmpImg.width(),tmpImg.height());
    //canvasCalib->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //canvasCalib->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

/*
void MainWindow::on_slide2AxCalPos_valueChanged(int value)
{
    value = value;
    updateCalibLine();
}*/

void MainWindow::updateCalibLine(){
    /*
    canvasCalib->scene()->clear();

    //Creates white line
    //..
    int x1,x2,y;
    y = canvasCalib->height() - ui->slide2AxCalPos->value();
    x1 = 0;
    x2 = canvasCalib->width();
    float rotAngle = -1.0*((float)ui->slide2AxCalRot->value()/5.0);
    canvasCalib->scene()->addLine(x1,y,x2,y,QPen(QColor("#FAE330")));
    ui->slide2AxCalPos->setToolTip( QString::number(ui->slide2AxCalPos->value()) );
    ui->slide2AxCalRot->setToolTip( QString::number(rotAngle) );
    ui->pbClearCalScene->setText("Clear line");
    */

}

/*
void MainWindow::on_slide2AxCalThre_valueChanged(int value)
{

    //Rotate image if requested
    //..
    if(globaIsRotated){
        float rotAngle = readAllFile( "./settings/calib/rotation.hypcam" ).trimmed().toFloat(0);
        QImage imgRot = funcRotateImage(auxQstring, rotAngle);
        imgRot.save(_PATH_DISPLAY_IMAGE);
    }
    //Apply threshold to the image
    //..
    QImage *imgThre = new QImage(auxQstring);
    funcImgThreshold( value, imgThre );
    QString tmpFilePaht = _PATH_DISPLAY_IMAGE;
    if( imgThre->save(tmpFilePaht) ){
        QtDelay(100);
        QPixmap pix(tmpFilePaht);
        pix = pix.scaledToHeight(_GRAPH_CALIB_HEIGHT);
        canvasCalib->setBackgroundBrush(pix);
        ui->slide2AxCalThre->setValue(value);
        ui->slide2AxCalThre->setToolTip(QString::number(value));
        ui->slide2AxCalThre->update();
        QtDelay(20);
    }

}
*/

void MainWindow::funcImgThreshold( int threshold, QImage *tmpImage ){
    int r, c;
    QRgb pix;
    for(r=0;r<tmpImage->height();r++){
        for(c=0;c<tmpImage->width();c++){
            pix = tmpImage->pixel(c,r);
            if( qRed(pix)<=threshold && qGreen(pix)<=threshold && qBlue(pix)<=threshold ){
                tmpImage->setPixel(QPoint(c,r),0);
            }
        }
    }
}


/*
void MainWindow::on_pbCalSaveTop_clicked()
{

    //Update image view
    //..
    QImage tmpImg( auxQstring );
    funcUpdateImgView( &tmpImg );

    //Crop image
    //..
    funcTransPix(
                    &calStruct,
                    canvasCalib->width(),
                    canvasCalib->height(),
                    tmpImg.width(),
                    tmpImg.height()
                );

    //Analize square selected
    //..
    QImage imgCrop = tmpImg.copy(
                                    calStruct.X1,
                                    calStruct.Y1,
                                    calStruct.lenW,
                                    calStruct.lenH
                                );
    colorAnalyseResult *tmpRes = funcAnalizeImage( &imgCrop );


    //Draw fluorescent RGB pixels
    //..
    int tmpX;
    //Clear scene
    canvasCalib->scene()->clear();
    //Regresh rec
    canvasCalib->scene()->addRect(calStruct.x1,calStruct.y1,calStruct.x2-calStruct.x1,calStruct.y2-calStruct.y1,QPen(QColor("#FF0000")));
    //Red
    tmpX = round(
                    (float)((calStruct.X1 + tmpRes->maxRx)*canvasCalib->width()) /
                    (float)tmpImg.width()
                );
    customLine *redLine = new customLine(QPoint(tmpX,0),QPoint(tmpX,canvasCalib->height()),QPen(Qt::red));
    canvasCalib->scene()->addItem(redLine);
    redLine->setToolTip("Red");
    redLine->parameters.name = "Vertical-Red-Right-Line";
    redLine->parameters.orientation = 2;
    redLine->parameters.lenght = canvasCalib->height();
    redLine->parameters.movible = false;
    //Green
    tmpX = round(
                    (float)((calStruct.X1 + tmpRes->maxGx)*canvasCalib->width()) /
                    (float)tmpImg.width()
                );
    customLine *GreenLine = new customLine(QPoint(tmpX,0),QPoint(tmpX,canvasCalib->height()),QPen(Qt::green));
    canvasCalib->scene()->addItem(GreenLine);
    GreenLine->setToolTip("Green");
    GreenLine->parameters.name = "Vertical-Green-Right-Line";
    GreenLine->parameters.orientation = 2;
    GreenLine->parameters.lenght = canvasCalib->height();
    GreenLine->parameters.movible = false;
    //Blue
    tmpX = round(
                    (float)((calStruct.X1 + tmpRes->maxBx)*canvasCalib->width()) /
                    (float)tmpImg.width()
                );
    customLine *BlueLine = new customLine(QPoint(tmpX,0),QPoint(tmpX,canvasCalib->height()),QPen(Qt::blue));
    canvasCalib->scene()->addItem(BlueLine);
    BlueLine->setToolTip("Blue");
    BlueLine->parameters.name = "Vertical-Blue-Right-Line";
    BlueLine->parameters.orientation = 2;
    BlueLine->parameters.lenght = canvasCalib->height();
    BlueLine->parameters.movible = false;



}
*/

/*
void MainWindow::funcUpdateImgView(QImage *tmpImg){
    //Applies rotation
    //..

    //Applies threshold
    //..
    if( ui->slide2AxCalThre->value()>0 ){
        funcImgThreshold( ui->slide2AxCalThre->value(), tmpImg );
    }
}
*/

void MainWindow::on_pbSpecLoadSnap_clicked()
{
    //Select image
    //..
    auxQstring = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select image..."),
                                                        "./snapshots/Calib/",
                                                        "(*.ppm);;"
                                                     );
    if( auxQstring.isEmpty() ){
        return (void)NULL;
    }

    //Rotate if requires
    //..
    if( funcShowMsgYesNo("Alert","Rotate using saved rotation?") == 1 ){
        float rotAngle = readAllFile( "./settings/calib/rotation.hypcam" ).trimmed().toFloat(0);
        QImage imgRot = funcRotateImage(auxQstring, rotAngle);
        imgRot.save(auxQstring);
    }


    //Create canvas
    //Display accum
    //..

    //Show image
    QPixmap pix(auxQstring);
    pix.save("./snapshots/tmpUSB.png");
    pix = pix.scaledToHeight( _GRAPH_HEIGHT );
    QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
    canvasSpec->setBackgroundBrush(pix);
    canvasSpec->setScene( scene );
    canvasSpec->resize(pix.width(),pix.height());
    canvasSpec->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasSpec->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->tab_5->layout()->addWidget(canvasSpec);
    ui->tab_5->layout()->setEnabled(false);
    ui->tab_5->layout()->setAlignment(Qt::AlignLeft);
    ui->gridColors->setAlignment(Qt::AlignLeft);
    ui->gridColors->setAlignment(Qt::AlignLeft);


    ui->slideRedLen->setMaximumWidth(pix.width());
    ui->slideGreenLen->setMaximumWidth(pix.width());
    ui->slideBlueLen->setMaximumWidth(pix.width());

    pix.save("./snapshots/tmpThumb.png");


}

void MainWindow::on_actionRect_triggered()
{
    /*
    selColor *selCol = new selColor(this);
    connect(selCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addRect2Calib(QString)));
    selCol->setModal(true);
    selCol->exec();
    disconnect(selCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addRect2Calib(QString)));
    */

    clearFreeHandPoligon();

    //Change mouse's cursor
    addRect2Calib("#F00");
    QApplication::setOverrideCursor(Qt::CrossCursor);
    ResetGraphToolBar("Rectangle");
    //Connect to calib double axis
    connect(
                canvasCalib,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcBeginRect(QMouseEvent*) )
           );
    connect(
                canvasCalib,
                SIGNAL( signalMouseReleased(QMouseEvent*) ),
                this,
                SLOT( funcCalibMouseRelease(QMouseEvent*) )
           );


}

void MainWindow::ResetGraphToolBar( QString toEnable ){
    //Disable all
    //..



    if( toEnable=="Rectangle" ){

    }
}

void MainWindow::on_actionCircle_triggered()
{
    selColor *selCol = new selColor(this);
    connect(selCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addCircle2Calib(QString)));
    selCol->setModal(true);
    selCol->exec();
    disconnect(selCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addCircle2Calib(QString)));
}

void MainWindow::on_actionHorizontalLine_triggered()
{
    selColor *selHCol = new selColor(this);
    connect(selHCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addHorLine2Calib(QString)));
    selHCol->setModal(true);
    selHCol->exec();
    disconnect(selHCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addHorLine2Calib(QString)));
}

void MainWindow::on_actionVerticalLine_triggered()
{
    selColor *selVCol = new selColor(this);
    connect(selVCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addVertLine2Calib(QString)));
    selVCol->setModal(true);
    selVCol->exec();
    disconnect(selVCol, SIGNAL(signalColorSelected(QString)), this, SLOT(addVertLine2Calib(QString)));
}

void MainWindow::addRect2Calib(QString colSeld){
    qDebug() << "Rect: " << colSeld;

}

void MainWindow::addCircle2Calib(QString colSeld){
    qDebug() << "Circle: " << colSeld;

}

void MainWindow::addVertLine2Calib(QString colSeld){
    int x = round( canvasCalib->width() / 2 );
    QPoint p1(x,0);
    QPoint p2(x, canvasCalib->height());
    customLine *tmpVLine = new customLine(p1, p2, QPen(QColor(colSeld)));
    tmpVLine->parameters.originalW      = canvasCalib->originalW;
    tmpVLine->parameters.originalH      = canvasCalib->originalH;
    tmpVLine->parameters.orientation    = _VERTICAL;
    tmpVLine->parentSize.setWidth(canvasCalib->width());
    tmpVLine->parentSize.setHeight(canvasCalib->height());
    globalCanvVLine = tmpVLine;
    canvasCalib->scene()->addItem( globalCanvVLine );
    canvasCalib->update();
}

void MainWindow::addHorLine2Calib(QString colSeld){
    int y = round( canvasCalib->height() / 2 );
    QPoint p1(0,y);
    QPoint p2( canvasCalib->width(), y);
    customLine *tmpHLine = new customLine(p1, p2, QPen(QColor(colSeld)));
    tmpHLine->parameters.orientation = _HORIZONTAL;
    tmpHLine->parameters.originalW  = canvasCalib->originalW;
    tmpHLine->parameters.originalH  = canvasCalib->originalH;
    tmpHLine->parentSize.setWidth(canvasCalib->width());
    tmpHLine->parentSize.setHeight(canvasCalib->height());
    globalCanvHLine = tmpHLine;
    canvasCalib->scene()->addItem( globalCanvHLine );
    canvasCalib->update();
}







void MainWindow::on_actionClear_triggered()
{
    //Clear scene
    canvasCalib->scene()->clear();

    //Clear image
    //globalEditImg = globalBackEditImg;
    //updateDisplayImage(globalBackEditImg);

    clearFreeHandPoligon();

    clearRectangle();

    //Mouse
    mouseCursorReset();

}

void MainWindow::clearFreeHandPoligon(){
    //Clear Free-hand points
    lstBorder->clear();
    lstSelPix->clear();
    lstPixSelAux->clear();

    //Disconnect signals
    disconnect(
               canvasAux,
               SIGNAL( signalMousePressed(QMouseEvent*) ),
               this,
               SLOT( funcAddPoint(QMouseEvent*) )
          );
    canvasAux->update();
}

void MainWindow::clearRectangle(){
    disconnect(
                canvasCalib,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcBeginRect(QMouseEvent*) )
           );
    disconnect(
                canvasCalib,
                SIGNAL( signalMouseReleased(QMouseEvent*) ),
                this,
                SLOT( funcCalibMouseRelease(QMouseEvent*) )
           );
    canvasCalib->update();
}

void MainWindow::on_actionSelection_triggered()
{
    //Change cursor
    QApplication::restoreOverrideCursor();

    //Disconnect
    //..
    //Rectangle
    disconnect(
                canvasCalib,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcBeginRect(QMouseEvent*) )
           );
    disconnect(
                canvasCalib,
                SIGNAL( signalMouseReleased(QMouseEvent*) ),
                this,
                SLOT( funcCalibMouseRelease(QMouseEvent*) )
           );

}

void MainWindow::on_actionDrawToolbar_triggered()
{
    disableAllToolBars();
    ui->toolBarDraw->setVisible(true);
}

void MainWindow::on_pbExpPixs_tabBarClicked(int index)
{
    disableAllToolBars();
    switch(index){
        case 3:
            ui->toolBarDraw->setVisible(true);
            break;
    }
}


/*
void MainWindow::on_slide2AxCalThre_sliderReleased()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //int value = ui->slide2AxCalThre->value();
    //Rotate image if requested
    //..
    if(globaIsRotated){
        float rotAngle = readAllFile( "./settings/calib/rotation.hypcam" ).trimmed().toFloat(0);
        QImage imgRot = funcRotateImage(auxQstring, rotAngle);
        imgRot.save(_PATH_DISPLAY_IMAGE);
    }
    //Apply threshold to the image
    //..
    QImage *imgThre = new QImage(auxQstring);
    funcImgThreshold( value, imgThre );
    QString tmpFilePaht = _PATH_DISPLAY_IMAGE;
    if( imgThre->save(tmpFilePaht) ){
        QtDelay(100);
        QPixmap pix(tmpFilePaht);
        pix = pix.scaledToHeight(_GRAPH_CALIB_HEIGHT);
        canvasCalib->setBackgroundBrush(pix);
       // ui->slide2AxCalThre->setValue(value);
        //ui->slide2AxCalThre->setToolTip(QString::number(value));
        //ui->slide2AxCalThre->update();
        //QtDelay(20);
    }
    QApplication::restoreOverrideCursor();
}
*/

void MainWindow::on_actionDoubAxisDiff_triggered()
{
    genCalibXML *genCalib = new genCalibXML(this);
    genCalib->setModal(true);
    genCalib->show();

}


void MainWindow::on_slideTriggerTime_valueChanged(int value)
{
    ui->labelTriggerTime->setText( "Trigger at: " + QString::number(value) + " secs" );
}

void MainWindow::on_actionRotateImg_triggered()
{
    DrawVerAndHorLines( canvasCalib, Qt::magenta );
    rotationFrm *doRot = new rotationFrm(this);
    doRot->setModal(false);
    connect(doRot,SIGNAL(angleChanged(float)),this,SLOT(doImgRotation(float)));
    doRot->show();
    doRot->move(QPoint(this->width(),0));
    doRot->raise();
    doRot->update();
}

void MainWindow::doImgRotation( float angle ){
    QTransform transformation;
    transformation.rotate(angle);
    QImage tmpImg(auxQstring);
    tmpImg = tmpImg.transformed(transformation);
    tmpImg.save(_PATH_DISPLAY_IMAGE);
    //reloadImage2Display();
    DrawVerAndHorLines( canvasCalib, Qt::magenta );
}

void MainWindow::DrawVerAndHorLines(GraphicsView *tmpCanvas, Qt::GlobalColor color){
    QPoint p1(0,(tmpCanvas->height()/2));
    QPoint p2(tmpCanvas->width(),(tmpCanvas->height()/2));
    customLine *hLine = new customLine(p1,p2,QPen(color));
    tmpCanvas->scene()->addItem(hLine);
    p1.setX((tmpCanvas->width()/2));
    p1.setY(0);
    p2.setX((tmpCanvas->width()/2));
    p2.setY(tmpCanvas->height());
    customLine *vLine = new customLine(p1,p2,QPen(color));
    tmpCanvas->scene()->addItem(vLine);
    tmpCanvas->update();
}

/*
void MainWindow::reloadImage2Display(){
    //Load image to display
    QPixmap pix( auxQstring );
    QRect calibArea = ui->pbExpPixs->geometry();
    int maxW, maxH;
    maxW = calibArea.width() - 3;
    maxH = calibArea.height() - 25;
    pix = pix.scaledToHeight(maxH);
    if( pix.width() > maxW )
        pix = pix.scaledToWidth(maxW);
    //It creates the scene to be loaded into Layout
    QGraphicsScene *sceneCalib = new QGraphicsScene(0,0,pix.width(),pix.height());    
    canvasCalib->setBackgroundBrush(QBrush(Qt::black));
    canvasCalib->setBackgroundBrush(pix);    
    canvasCalib->setScene( sceneCalib );
    canvasCalib->resize(pix.width(),pix.height());
    canvasCalib->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasCalib->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasCalib->update();
    //qDebug() << "CanvasSceneW: "<< canvasCalib->scene()->width();
    //qDebug() << "CanvasSceneH: "<< canvasCalib->scene()->height();
    //qDebug() << "CanvasW: "<< canvasCalib->width();
    //qDebug() << "CanvasH: "<< canvasCalib->height();
}*/

void MainWindow::on_actionLoadCanvas_triggered()
{

    QString lastPath = readFileParam(_PATH_LAST_IMG_OPEN);
    if( lastPath.isEmpty() )//First time using this parameter
    {
        lastPath = "./snapshots/Calib/";
    }

    //Select image
    //..
    auxQstring = QFileDialog::getOpenFileName(
                                                this,
                                                tr("Select image..."),
                                                lastPath,
                                                "(*.ppm *.RGB888 *.tif *.png *.jpg *.jpeg *.JPEG *.JPG *.bmp);;"
                                             );
    if( auxQstring.isEmpty() )
    {
        return (void)NULL;
    }
    else
    {
        //Save last file open
        saveFile(_PATH_LAST_USED_IMG_FILENAME,auxQstring);

        //Save Folder in order to Speed up File Selection
        lastPath = funcRemoveFileNameFromPath(auxQstring);
        saveFile(_PATH_LAST_IMG_OPEN,lastPath);
    }

    //Load Image Selected
    globalEditImg       = new QImage(auxQstring);
    globalBackEditImg   = new QImage(auxQstring);

    //Update Thumb and Edit Canvas
    updateDisplayImage(globalEditImg);

    //Save Image to Display Path
    //mouseCursorWait();
    //globalEditImg->save(_PATH_DISPLAY_IMAGE);
    //mouseCursorReset();

}

void MainWindow::updateImageCanvasEdit(QString fileName){
    //
    //Refresh image in scene
    //
    //Load layout    
    QLayout *layout = new QVBoxLayout();
    layout->addWidget(canvasCalib);
    layout->setEnabled(false);
    ui->tab_6->setLayout(layout);

    //
    //It enables slides
    //
    ui->toolBarDraw->setEnabled(true);
    ui->toolBarDraw->setVisible(true);

    //
    //Load image to display
    //
    QPixmap pix( fileName );
    canvasCalib->originalW  = pix.width();
    canvasCalib->originalH  = pix.height();
    QRect calibArea = ui->pbExpPixs->geometry();
    int maxW, maxH;
    maxW = calibArea.width() - 3;
    maxH = calibArea.height() - 25;
    pix = pix.scaledToHeight(maxH);
    if( pix.width() > maxW )
        pix = pix.scaledToWidth(maxW);
    //It creates the scene to be loaded into Layout
    QGraphicsScene *sceneCalib = new QGraphicsScene(0,0,pix.width(),pix.height());    
    canvasCalib->setBackgroundBrush(QBrush(Qt::black));
    canvasCalib->setBackgroundBrush(pix);
    canvasCalib->setScene( sceneCalib );
    canvasCalib->resize(pix.width(),pix.height());
    canvasCalib->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasCalib->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasCalib->update();

}

void MainWindow::updateImageCanvasEdit(QImage* origImg)
{
    //
    //Refresh image in scene
    //
    //Load layout
    QLayout *layout = new QVBoxLayout();
    layout->addWidget(canvasCalib);
    layout->setEnabled(false);
    ui->tab_6->setLayout(layout);

    //
    //It enables slides
    //
    ui->toolBarDraw->setEnabled(true);
    ui->toolBarDraw->setVisible(true);

    //
    //Load image to display
    //
    QPixmap pix = QPixmap::fromImage(*origImg);
    QRect calibArea = ui->pbExpPixs->geometry();
    int maxW, maxH;
    maxW = calibArea.width() - 3;
    maxH = calibArea.height() - 25;
    pix = pix.scaledToHeight(maxH);
    if( pix.width() > maxW )
        pix = pix.scaledToWidth(maxW);
    //It creates the scene to be loaded into Layout
    QGraphicsScene *sceneCalib = new QGraphicsScene(0,0,pix.width(),pix.height());
    canvasCalib->originalW = origImg->width();
    canvasCalib->originalH = origImg->height();
    canvasCalib->setBackgroundBrush(QBrush(Qt::black));
    canvasCalib->setBackgroundBrush(pix);
    canvasCalib->setScene( sceneCalib );
    canvasCalib->resize(pix.width(),pix.height());
    canvasCalib->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasCalib->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvasCalib->update();
}

void MainWindow::on_actionApplyThreshold_triggered()
{
    recParamFrm *recParam = new recParamFrm(this);
    recParam->setModal(false);
    connect(recParam,SIGNAL(paramGenerated(QString)),this,SLOT(applyThreshol2Scene(QString)));
    recParam->setWindowTitle("Type the threshold [0-255]");
    recParam->show();
    recParam->raise();
    recParam->update();
}

void MainWindow::applyThreshol2Scene(QString threshold){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    int value = threshold.toInt(0);
    //Apply threshold to the image
    //..
    QImage *imgThre = new QImage(auxQstring);
    funcImgThreshold( value, imgThre );
    imgThre->save(_PATH_DISPLAY_IMAGE);
    //Rotate image if requested
    //..
    if(globaIsRotated){
        float rotAngle = getLastAngle();
        QImage imgRot = funcRotateImage(_PATH_DISPLAY_IMAGE, rotAngle);
        imgRot.save(_PATH_DISPLAY_IMAGE);
        qDebug() << "Rotate: " << rotAngle;
    }
    //Update canvas
    //..
    //reloadImage2Display();

    /*
    if( imgThre->save(_PATH_DISPLAY_IMAGE) ){
        QtDelay(100);
        QPixmap pix(_PATH_DISPLAY_IMAGE);
        pix = pix.scaledToHeight(_GRAPH_CALIB_HEIGHT);
        canvasCalib->setBackgroundBrush(pix);
        ui->slide2AxCalThre->setValue(value);
        ui->slide2AxCalThre->setToolTip(QString::number(value));
        ui->slide2AxCalThre->update();
        //QtDelay(20);
    }
    */
    QApplication::restoreOverrideCursor();
}

float MainWindow::getLastAngle(){
    return readAllFile( _PATH_LAST_ROTATION ).trimmed().toFloat(0);
}


void MainWindow::mouseCursorWait(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void MainWindow::mouseCursorReset(){
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_actionLoadSquareRectangle_triggered()
{
    //Obtaining square aperture params
    squareAperture *tmpSqAperture = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SQUARE_APERTURE2, tmpSqAperture ) ){
        funcShowMsg("ERROR","Loading _PATH_REGION_OF_INTERES");
        return (void)false;
    }

    int x, y, w, h;
    x = round(((float)tmpSqAperture->rectX / (float)tmpSqAperture->canvasW) * (float)canvasCalib->scene()->width());
    y = round(((float)tmpSqAperture->rectY / (float)tmpSqAperture->canvasH) * (float)canvasCalib->scene()->height());
    w = round(((float)tmpSqAperture->rectW / (float)tmpSqAperture->canvasW) * (float)canvasCalib->scene()->width());
    h = round(((float)tmpSqAperture->rectH / (float)tmpSqAperture->canvasH) * (float)canvasCalib->scene()->height());

    //Draw a rectangle of the square aperture
    QPoint p1(x,y);
    QPoint p2(w,h);
    customRect *tmpRect = new customRect(p1,p2,globalEditImg);
    tmpRect->setPen(QPen(Qt::red));
    tmpRect->parameters.W = canvasCalib->width();
    tmpRect->parameters.H = canvasCalib->height();
    canvasCalib->scene()->clear();
    canvasCalib->scene()->addItem(tmpRect);
    canvasCalib->update();

}

void MainWindow::on_actionLoadRegOfInteres_triggered()
{
    //Obtaining square aperture params
    squareAperture* squareDiff = (squareAperture*)malloc(sizeof(squareAperture));
    //if( !funGetSquareXML( _PATH_SQUARE_USABLE, squareDiff ) ){
    if( !funGetSquareXML( _PATH_REGION_OF_INTERES2, squareDiff ) ){
        funcShowMsg("ERROR","Loading _PATH_REGION_OF_INTERES");
        return (void)false;
    }

    int x, y, w, h;
    x = round(((float)squareDiff->rectX / (float)squareDiff->canvasW) * (float)canvasCalib->scene()->width());
    y = round(((float)squareDiff->rectY / (float)squareDiff->canvasH) * (float)canvasCalib->scene()->height());
    w = round(((float)squareDiff->rectW / (float)squareDiff->canvasW) * (float)canvasCalib->scene()->width());
    h = round(((float)squareDiff->rectH / (float)squareDiff->canvasH) * (float)canvasCalib->scene()->height());

    //Draw a rectangle of the square aperture
    QPoint p1(x,y);
    QPoint p2(w,h);
    customRect *tmpRect = new customRect(p1,p2,globalEditImg);
    tmpRect->setPen(QPen(Qt::red));
    tmpRect->parameters.W = canvasCalib->width();
    tmpRect->parameters.H = canvasCalib->height();
    canvasCalib->scene()->clear();
    canvasCalib->scene()->addItem(tmpRect);
    canvasCalib->update();
}

int MainWindow::funcDrawRectangleFromXML(QString fileName)
{
    //Obtain square coordinates from XML file
    squareAperture *tmpSqAperture = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( fileName, tmpSqAperture ) ){
        funcShowMsg("ERROR","Loading " + fileName);
        return -1;
    }

    //Draw a rectangle of the square aperture
    QPoint p1(tmpSqAperture->rectX,tmpSqAperture->rectY);
    QPoint p2(tmpSqAperture->rectW,tmpSqAperture->rectH);
    customRect *tmpRect = new customRect(p1,p2,globalEditImg);
    tmpRect->setPen(QPen(Qt::red));
    tmpRect->parameters.W = canvasCalib->width();
    tmpRect->parameters.H = canvasCalib->height();

    canvasCalib->scene()->addItem(tmpRect);
    canvasCalib->update();

    return 1;
}

/*
void MainWindow::on_slideShuterSpeedSmall_valueChanged(int value)
{
    QString qstrVal = QString::number(value + ui->slideShuterSpeed->value());
    ui->labelShuterSpeed->setText( "Diffraction Shuter Speed: " + qstrVal );
}*/

void MainWindow::on_actionToolPenArea_triggered()
{
    clearRectangle();
    mouseCursorCross();
    canvasAux = canvasCalib;
    connect(
                canvasCalib,
                SIGNAL( signalMousePressed(QMouseEvent*) ),
                this,
                SLOT( funcAddPoint(QMouseEvent*) )
           );
}

void MainWindow::mouseCursorHand(){
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
}

void MainWindow::mouseCursorCross(){
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_actionSend_to_XYZ_triggered()
{
    //Validate that exist pixel selected
    //..
    if( lstSelPix->count()<1){
        funcShowMsg("Lack","Not pixels selected");
        return (void)false;
    }

    //Create xycolor space
    //..
    GraphicsView *xySpace = new GraphicsView(this);
    funcPutImageIntoGV(xySpace, "./imgResources/CIEManual.png");
    xySpace->setWindowTitle( "XY color space" );
    xySpace->show();

    //Transform each pixel from RGB to xy and plot it
    //..
    QImage tmpImg(_PATH_DISPLAY_IMAGE);
    int W = tmpImg.width();
    int H = tmpImg.height();
    int pixX, pixY;
    QRgb tmpPix;
    colSpaceXYZ *tmpXYZ = (colSpaceXYZ*)malloc(sizeof(colSpaceXYZ));
    int tmpOffsetX = -13;
    int tmpOffsetY = -55;
    int tmpX, tmpY;
    int i;
    qDebug() << "<lstSelPix->count(): " << lstSelPix->count();
    for( i=0; i<lstSelPix->count(); i++ ){
        //Get pixel position in real image
        pixX = (float)(lstSelPix->at(i).first * W) / (float)canvasAux->width();
        pixY = (float)(lstSelPix->at(i).second * H) / (float)canvasAux->height();
        tmpPix = tmpImg.pixel( pixX, pixY );
        funcRGB2XYZ( tmpXYZ, (float)qRed(tmpPix), (float)qGreen(tmpPix), (float)qBlue(tmpPix) );
        //funcRGB2XYZ( tmpXYZ, 255.0, 0, 0  );
        tmpX = floor( (tmpXYZ->x * 441.0) / 0.75 ) + tmpOffsetX;
        tmpY = 522 - floor( (tmpXYZ->y * 481.0) / 0.85 ) + tmpOffsetY;
        funcAddPoit2Graph( xySpace, tmpX, tmpY, 1, 1,
                           QColor(qRed(tmpPix),qGreen(tmpPix),qBlue(tmpPix)),
                           QColor(qRed(tmpPix),qGreen(tmpPix),qBlue(tmpPix))
                         );
    }

    //Save image plotted
    //..
    QPixmap pixMap = QPixmap::grabWidget(xySpace);
    pixMap.save("./Results/Miscelaneas/RGBPloted.png");

}

void MainWindow::on_actionSaveCanvas_triggered()
{
    recParamFrm *recParam = new recParamFrm(this);
    recParam->setModal(false);
    connect(recParam,SIGNAL(paramGenerated(QString)),this,SLOT(saveCalib(QString)));
    recParam->setWindowTitle("Type the filename...");
    recParam->show();
    recParam->raise();
    recParam->update();
}

void MainWindow::saveCalib(QString fileName){
    bool result = saveCanvas(canvasCalib,fileName);
    if( result ){
        funcShowMsg("Success","Canvas saved");
    }else{
        funcShowMsg("ERROR","Saving Canvas");
    }
}

bool MainWindow::saveCanvas(GraphicsView *tmpCanvas, QString fileName){
    //Save
    //..
    //Remove if exists
    QFile prevImg(fileName);
    if(prevImg.exists()){
        prevImg.remove();
    }
    prevImg.close();
    QPixmap pixMap = QPixmap::grabWidget(tmpCanvas);
    if(pixMap.save(fileName)){
        return true;
    }else{
        return false;
    }
    return true;
}

void MainWindow::on_actionExportPixelsSelected_triggered()
{
    if( lstSelPix->isEmpty() ){
        funcShowMsg("LACK","Not pixels to export");
    }else{
        int i;
        QString filePath = "./Results/lstPixels.txt";
        QFile fileLstPixels(filePath);
        if (!fileLstPixels.open(QIODevice::WriteOnly | QIODevice::Text)){
            funcShowMsg("ERROR","Creating file fileLstPixels");
        }else{
            QTextStream streamLstPixels(&fileLstPixels);
            for( i=0; i<lstSelPix->count(); i++ ){
                streamLstPixels << QString::number(lstSelPix->at(i).first) << " "<< QString::number(lstSelPix->at(i).second) << "\n";
            }
        }
        fileLstPixels.close();
        funcShowMsg("Success","List of pixels exported into: "+filePath);
    }
}

void MainWindow::on_pbLANConnect_clicked()
{
    ui->txtCommand->setText("sudo iwconfig wlan0 essid ESSID-NAME key s:PASS");
    ui->checkBlind->setChecked(true);
    funcShowMsg("Alert","Execute IWCONFIG setting BLIND mode");
}

void MainWindow::on_pbLANTestConn_clicked()
{
    ui->txtCommand->setText("iwconfig");
    ui->checkBlind->setChecked(false);
    ui->pbSendComm->click();
}

void MainWindow::on_actionGenHypercube_triggered()
{

    //----------------------------------------
    // Validate lst of wavelengths selected
    //----------------------------------------
    QList<double> lstChoises;
    lstChoises  = getWavesChoised();
    if( lstChoises.size() <= 0 )
    {
        funcShowMsg("ERROR","Please, select wavelengths to extract");
        return (void)false;
    }

    //----------------------------------------
    // Select hypercube destination
    //----------------------------------------
    QString fileName;
    fileName = QFileDialog::getSaveFileName(
                                                this,
                                                tr("Save Hypercube as..."),
                                                "./Hypercubes/",
                                                tr("Documents (*.hypercube)")
                                            );
    if( fileName.isEmpty() )
    {
        return (void)NULL;
    }


    QTime timeStamp;
    timeStamp.start();
    int tmpEMNumIte;
    tmpEMNumIte = readFileParam(_PATH_SETTINGS_EM_ITERATIONS).toInt(0);
    qDebug() << "tmpEMNumIte: " << tmpEMNumIte;

    if( generatesHypcube(tmpEMNumIte, fileName) )
    {
        //Extracts hypercube
        extractsHyperCube(fileName);
        //Show time to extract files
        QString time;
        time = timeToQString( timeStamp.elapsed() );
        qDebug() << time;
        //Inform to the user
        funcShowMsg(" ", "Hypercube exported successfully\n\n"+time);
    }
    //exit(2);


}

bool MainWindow::generatesHypcube(int numIterations, QString fileName){

    mouseCursorWait();

    int i, l, aux, N;
    double *F, *fRed, *fGreen, *fBlue;
    QList<double> lstChoises;
    int hypW, hypH, hypL;
    lstDoubleAxisCalibration daCalib;    
    lstChoises  = getWavesChoised();
    funcGetCalibration(&daCalib);

    hypW        = daCalib.squareUsableW;
    hypH        = daCalib.squareUsableH;
    hypL        = lstChoises.count();
    N           = hypW * hypH * hypL;//Voxels

    F           = (double*)malloc(N*sizeof(double));
    fRed        = calculatesF(numIterations,_RED,&daCalib);
    fGreen      = calculatesF(numIterations,_GREEN,&daCalib);
    fBlue       = calculatesF(numIterations,_BLUE,&daCalib);

    //---------------------------------------------
    //Demosaicing hypercube
    //---------------------------------------------
    if(true)
    {
        if(false)
        {
            fRed    = demosaiseF2D(fRed,hypL,hypH,hypW);
            fGreen  = demosaiseF2D(fGreen,hypL,hypH,hypW);
            fBlue   = demosaiseF2D(fBlue,hypL,hypH,hypW);
        }
        else
        {
            if( SQUARE_BICUBIC_ITERATIONS > 0 )
            {
                //Da otra pasada
                for( i=0; i<SQUARE_BICUBIC_ITERATIONS; i++ )
                {
                    fRed    = demosaiseF3D(fRed,hypL,hypH,hypW);
                    fGreen  = demosaiseF3D(fGreen,hypL,hypH,hypW);
                    fBlue   = demosaiseF3D(fBlue,hypL,hypH,hypW);
                }
            }
        }
    }




    //---------------------------------------------
    //Extracting spectral measure
    //---------------------------------------------
    //Get hash to the corresponding sensitivity
    QList<double> Sr;
    QList<double> Sg;
    QList<double> Sb;
    for(l=0; l<hypL; l++)
    {        
        //qDebug() << "Hola1" << l << " of "<< hypL << " size: " << daCalib.Sr.size();
        aux = ((floor(lstChoises.at(l)) - floor(daCalib.minWavelength) )==0)?0:floor( (floor(lstChoises.at(l)) - floor(daCalib.minWavelength) ) / (double)daCalib.minSpecRes );
        Sr.append( daCalib.Sr.at(aux) );
        Sg.append( daCalib.Sg.at(aux) );
        Sb.append( daCalib.Sb.at(aux) );
    }
    //qDebug() << "Hola13";
    int j, pixByImage;
    double min, max;
    int minPos, maxPos;
    min = 9999;
    max = -1;
    pixByImage = daCalib.squareUsableW * daCalib.squareUsableH;    
    i=0;
    for(l=0; l<hypL;l++)
    {
        for(j=0; j<pixByImage; j++)
        {
            F[i]    = (fRed[i]+fGreen[i]+fBlue[i]) / (Sr.at(l)+Sg.at(l)+Sb.at(l));
            if(min>F[i])
            {
                min     = F[i];
                minPos  = i;
            }
            if(max<F[i])
            {
                max     = F[i];
                maxPos  = i;
            }
            i++;
        }
    }
    printf("min(%lf,%d) max(%lf,%d)\n",min,minPos,max,maxPos);
    fflush(stdout);

    //---------------------------------------------
    //Save hypercube
    //---------------------------------------------
    //Hypercube Format: Date,W,H,L,l1,...,lL,pix_1_l1,pix_2_l1,...pix_n_l1,pix_1_l2,pix_2_l2,...pix_n_l2,...,pix_1_L,pix_2_L,...pix_n_L
    QString hypercube;
    QDateTime dateTime = QDateTime::currentDateTime();
    hypercube.append(dateTime.toString("yyyy-MM-dd HH:mm:ss"));
    hypercube.append(","+QString::number(daCalib.squareUsableW));
    hypercube.append(","+QString::number(daCalib.squareUsableH));
    hypercube.append(","+QString::number(lstChoises.count()));
    for(l=0; l<lstChoises.count(); l++)
    {
        hypercube.append(","+QString::number(lstChoises.at(l)));
    }
    N = hypW * hypH * hypL;//Voxels
    for(i=0; i<N; i++)
    {
        hypercube.append(","+QString::number(F[i]));
    }
    fileName.replace(".hypercube","");
    saveFile(fileName+".hypercube",hypercube);






    mouseCursorReset();





    /*

    if(false)
    {
        double min, max;
        int minPos, maxPos;
        min = 9999;
        max = -1;

        for(int n=0; n<N; n++)
        {
            if(min > F[n])
            {
                min = F[n];
                minPos = n;
            }
            if(max < F[n])
            {
                max = F[n];
                maxPos = n;
            }
           //    printf("F[%d] | %lf\n",n,F[n]);
        }
        printf("min(%lf,%d) max(%lf,%d)\n",min,minPos,max,maxPos);
        fflush(stdout);
    }
    */







    return true;

}


double *MainWindow::demosaiseF2D(double *f, int L, int H, int W)
{
    //Variables
    int i, l, w, h;
    double ***aux;
    aux = (double***)malloc(L*sizeof(double**));
    for(l=0;l<L;l++)
    {
        aux[l] = (double**)malloc(H*sizeof(double*));
        for(h=0;h<H;h++)
        {
            aux[l][h] = (double*)malloc(W*sizeof(double));
        }
    }

    //Fill as 3D matrix
    i=0;
    for(l=0;l<L;l++)
    {
        for(h=0;h<H;h++)
        {
            for(w=0;w<W;w++)
            {
                aux[l][h][w] = f[i];
                i++;
            }
        }
    }

    //Demosaize
    i=0;
    for(l=0;l<L;l++)
    {
        for(h=0;h<H;h++)
        {
            for(w=0;w<W;w++)
            {
                if( h>0 && w>0 && h<H-1 && w<W-1 )
                {
                    f[i] = (    aux[l][h-1][w-1] +
                                aux[l][h+1][w-1] +
                                aux[l][h-1][w+1] +
                                aux[l][h+1][w+1]
                           ) / 4.0;
                }
                else
                {
                    //ROWS
                    if( h==0 || h==(H-1) )
                    {
                        if(h==0)//First row
                        {
                            if( w>0 && w<(W-1) )
                                f[i] = ( aux[l][h+1][w-1] + aux[l][h+1][w+1]) / 2.0;
                            else//Corners
                            {
                                if(w==0)//Up-Left
                                    f[i] = ( aux[l][h+1][w] + aux[l][h+1][w+1] + aux[l][h][w+1]) / 3.0;
                                if(w==(W-1))//Up-Right
                                    f[i] = ( aux[l][h][w-1] + aux[l][h+1][w-1] + aux[l][h+1][w]) / 3.0;
                            }
                        }
                        else
                        {
                            if(h==(H-1))//Last row
                            {
                                if( w>0 && w<(W-1) )
                                    f[i] = ( aux[l][h-1][w-1] + aux[l][h-1][w+1]) / 2.0;
                                else//Corners
                                {
                                    if(w==0)//Down-Left
                                        f[i] = ( aux[l][h-1][w] + aux[l][h-1][w+1] + aux[l][h][w+1]) / 3.0;
                                    if(w==(W-1))//Down-Right
                                        f[i] = ( aux[l][h][w-1] + aux[l][h-1][w-1] + aux[l][h-1][w]) / 3.0;
                                }
                            }
                            else
                            {
                                //COLS
                                if( w==0 || w==(W-1) )
                                {
                                    if(w==0)
                                        f[i] = ( aux[l][h-1][w+1] + aux[l][h+1][w+1]) / 2.0;
                                    else
                                        f[i] = ( aux[l][h-1][w-1] + aux[l][h+1][w-1]) / 2.0;
                                }
                            }
                        }
                    }
                }
                i++;
            }
        }
    }

    //Free memory
    for(l=0;l<L;l++)
    {
        for(h=0;h<H;h++)
        {
             delete[] aux[l][h];
        }
        delete[] aux[l];
    }
    delete[] aux;

    return f;
}


double *MainWindow::demosaiseF3D(double *f, int L, int H, int W)
{
    f = demosaiseF2D(f,L,H,W);

    //...............................................
    //Variables
    //...............................................
    int i, l, w, h;
    double ***aux;
    aux     = (double***)malloc(L*sizeof(double**));
    for(l=0;l<L;l++)
    {
        aux[l] = (double**)malloc(H*sizeof(double*));
        for(h=0;h<H;h++)
        {
            aux[l][h] = (double*)malloc(W*sizeof(double));
        }
    }

    //...............................................
    //Fill a 3D matrix
    //...............................................
    i=0;
    for(l=0;l<L;l++)
    {
        for(h=0;h<H;h++)
        {
            for(w=0;w<W;w++)
            {
                aux[l][h][w] = f[i];
                i++;
            }
        }
    }

    //...............................................
    //Demosaize 3D
    //...............................................
    trilinear tmpNode;
    tmpNode.L = L;
    tmpNode.W = W;
    tmpNode.H = H;
    i=0;
    for(tmpNode.l=0;tmpNode.l<L;tmpNode.l++)
    {
        for(tmpNode.h=0;tmpNode.h<H;tmpNode.h++)
        {
            for(tmpNode.w=0;tmpNode.w<W;tmpNode.w++)
            {
                //qDebug() << "i: " << i << "l: " << tmpNode.l << "w: " << tmpNode.w << "h: " << tmpNode.h << "W: " << tmpNode.W << "H: " << tmpNode.H;
                f[i] = calcTrilinearInterpolation(aux, &tmpNode);
                i++;                
            }
        }
    }

    //...............................................
    //Free memory
    //...............................................
    for(l=0;l<L;l++)
    {
        for(h=0;h<H;h++)
        {
             delete[] aux[l][h];
        }
        delete[] aux[l];
    }
    delete[] aux;



    //Finishes
    return f;
}


double MainWindow::calcTrilinearInterpolation(double ***cube, trilinear *node)
{

    double result = cube[node->l][node->h][node->w];
    if(
            node->l>0 && node->l<node->L-1 &&
            node->w>0 && node->w<node->W-1 &&
            node->h>0 && node->h<node->H-1
    )
    {
        result = (
                    cube[node->l-1][node->h-1][node->w-1]   +
                    cube[node->l-1][node->h-1][node->w]     +
                    cube[node->l-1][node->h-1][node->w+1]   +
                    cube[node->l-1][node->h][node->w-1]     +
                    cube[node->l-1][node->h][node->w]       +
                    cube[node->l-1][node->h][node->w+1]     +
                    cube[node->l-1][node->h+1][node->w-1]   +
                    cube[node->l-1][node->h+1][node->w]     +
                    cube[node->l-1][node->h+1][node->w+1]   +

                    cube[node->l][node->h-1][node->w-1]     +
                    cube[node->l][node->h-1][node->w]       +
                    cube[node->l][node->h-1][node->w+1]     +
                    cube[node->l][node->h][node->w-1]       +
                    cube[node->l][node->h][node->w]         +
                    cube[node->l][node->h][node->w+1]       +
                    cube[node->l][node->h+1][node->w-1]     +
                    cube[node->l][node->h+1][node->w]       +
                    cube[node->l][node->h+1][node->w+1]     +

                    cube[node->l+1][node->h-1][node->w-1]   +
                    cube[node->l+1][node->h-1][node->w]     +
                    cube[node->l+1][node->h-1][node->w+1]   +
                    cube[node->l+1][node->h][node->w-1]     +
                    cube[node->l+1][node->h][node->w]       +
                    cube[node->l+1][node->h][node->w+1]     +
                    cube[node->l+1][node->h+1][node->w-1]   +
                    cube[node->l+1][node->h+1][node->w]     +
                    cube[node->l+1][node->h+1][node->w+1]

                 ) / 27.0;
    }
    else
    {
        if(
            (node->l==0 || node->w==0 || node->h==0) &&
            ( node->l<node->L-1 && node->w<node->W-1 && node->h<node->H-1 )
        )
        {
            result = (
                        cube[node->l][node->h][node->w]         +
                        cube[node->l][node->h][node->w+1]       +
                        cube[node->l][node->h+1][node->w]       +
                        cube[node->l][node->h+1][node->w+1]     +

                        cube[node->l+1][node->h][node->w]       +
                        cube[node->l+1][node->h][node->w+1]     +
                        cube[node->l+1][node->h+1][node->w]     +
                        cube[node->l+1][node->h+1][node->w+1]

                     ) / 8.0;
        }
        else
        {
            if(
                    (node->l==node->L-1 || node->w==node->W-1 || node->h==node->H-1 ) &&
                    (node->l>0 && node->w>0 && node->h>0)
            )
            {
                result = (
                            cube[node->l][node->h][node->w]         +
                            cube[node->l][node->h][node->w-1]       +
                            cube[node->l][node->h-1][node->w]       +
                            cube[node->l][node->h-1][node->w-1]     +

                            cube[node->l-1][node->h][node->w]       +
                            cube[node->l-1][node->h][node->w-1]     +
                            cube[node->l-1][node->h-1][node->w]     +
                            cube[node->l-1][node->h-1][node->w-1]

                         ) / 8.0;
            }
        }

    }



    /*
    double result;
    if(node->l>0 && node->l<node->L-1)
    {
        //qDebug() << "l: " << node->l << " h: " << node->h << " w: " << node->w;
        result = (
                    cube[node->l-1][node->h][node->w] +
                    cube[node->l+1][node->h][node->w]
                 ) / 2.0;
    }
    else
    {
        if(node->l==0)
        {
            result = (
                        cube[node->l][node->h][node->w] +
                        cube[node->l+1][node->h][node->w]
                     ) / 2.0;
        }
        else
        {
            result = (
                        cube[node->l][node->h][node->w] +
                        cube[node->l-1][node->h][node->w]
                     ) / 2.0;
        }
    }
    */

    /*
    if(
            node->l > 0 && node->l < node->L-1 &&
            node->w > 0 && node->w < node->W-1 &&
            node->h > 0 && node->h < node->H-1
    )
    {//BOUNDED BY THE MARGIN
        if(true)
        {
            result = (  cube[node->l][node->h-1][node->w-1]     + cube[node->l][node->h-1][node->w+1]     +
                        cube[node->l][node->h+1][node->w-1]     + cube[node->l][node->h+1][node->w+1]     +
                        cube[node->l-1][node->h-1][node->w-1]   + cube[node->l-1][node->h-1][node->w+1]   +
                        cube[node->l-1][node->h+1][node->w-1]   + cube[node->l-1][node->h+1][node->w+1]   +
                        cube[node->l+1][node->h-1][node->w-1]   + cube[node->l+1][node->h-1][node->w+1]   +
                        cube[node->l+1][node->h+1][node->w-1]   + cube[node->l+1][node->h+1][node->w+1]
                     ) / 12.0;
        }
        else
        {
            result = (
                        cube[node->l-1][node->h-1][node->w-1]   + cube[node->l-1][node->h-1][node->w+1]   +
                        cube[node->l-1][node->h+1][node->w-1]   + cube[node->l-1][node->h+1][node->w+1]   +
                        cube[node->l+1][node->h-1][node->w-1]   + cube[node->l+1][node->h-1][node->w+1]   +
                        cube[node->l+1][node->h+1][node->w-1]   + cube[node->l+1][node->h+1][node->w+1]
                     ) / 8.0;
        }
    }
    else
    {
        result = cube[node->l][node->h][node->w];
    }
    */


    return result;
}


double *MainWindow::calculatesF(int numIterations, int sensor, lstDoubleAxisCalibration *daCalib)
{
    //Get original image
    //..
    int i, N, M;
    QImage img( _PATH_DISPLAY_IMAGE );
    M = img.width() * img.height();

    //Creates and fills H
    // ..
    //Creates containers
    int hypW, hypH, hypL;
    QList<double> lstChoises;
    pixel **Hcol;
    int **Hrow;

    lstChoises  = getWavesChoised();
    hypW        = daCalib->squareUsableW;
    hypH        = daCalib->squareUsableH;
    hypL        = lstChoises.count();
    N           = hypW * hypH * hypL;//Voxels

    //Reserves Memory for H
    //..
    Hcol        = (pixel**)malloc(N*sizeof(pixel*));
    for(int n=0; n<N; n++)
    {
        Hcol[n] = (pixel*)malloc(5*sizeof(pixel));
    }

    Hrow        = (int**)malloc(M*sizeof(int*));
    for(int m=0; m<M; m++)
    {
        Hrow[m]     = (int*)malloc(sizeof(int));
        Hrow[m][0]  = 0;
    }

    //It creates H
    //..
    createsHColAndHrow( Hcol, Hrow, &img, daCalib );

    //It creates image to proccess
    //..
    double *g, *gTmp, *f, *fKPlusOne;
    gTmp        = (double*)malloc(M*sizeof(double));
    fKPlusOne   = (double*)malloc(N*sizeof(double));
    g           = serializeImageToProccess( img, sensor );//g
    f           = createsF0(Hcol, g, N);//f0
    for( i=0; i<numIterations; i++ )
    {
        createsGTmp( gTmp, g, Hrow, f, M );//(Hf)m
        improveF( fKPlusOne, Hcol, f, gTmp, N );
        memcpy(f,fKPlusOne,(N*sizeof(double)));
    }

    //It finishes
    return f;
}




void MainWindow::improveF( double *fKPlusOne, pixel **Hcol, double *f, double *gTmp, int N )
{
    int n;
    double avgMeasure;//average measure
    double relevance;//How relevant it is respect to all voxels overlaped
                     //Error between g and g^ (g = original image)
    double numProj;//It is integer but is used double to evit many cast operations
    numProj = 5.0;
    for( n=0; n<N; n++ )
    {
        //fKPlusOne[n]    = 0.0;
        avgMeasure      = f[n] / numProj;
        //qDebug() << "n: " << n << " | avgMeasure: " << avgMeasure;

        relevance       = gTmp[Hcol[n][0].index] +
                          gTmp[Hcol[n][1].index] +
                          gTmp[Hcol[n][2].index] +
                          gTmp[Hcol[n][3].index] +
                          gTmp[Hcol[n][4].index];
        //qDebug() << "relevance: " << relevance;

        fKPlusOne[n]    = avgMeasure * relevance;
        //qDebug() << "fKPlusOne[" << n << "]: " << fKPlusOne[n];

    }

}

void MainWindow::createsGTmp(double *gTmp, double *g, int **Hrow, double *f, int M)
{
    int m, n;
    for( m=0; m<M; m++ )
    {
        gTmp[m] = 0.0;
        //if( Hrow[m][0] > 0 )
        if( Hrow[m][0] != 0 )
        {
            for( n=1; n<=Hrow[m][0]; n++ )
            {
                gTmp[m] += f[Hrow[m][n]];
            }
            //gTmp[m] = ( g[m] > 0 && gTmp[m] > 0 )?(g[m]/gTmp[m]):0;
            gTmp[m] = ( g[m] != 0 && gTmp[m] != 0 )?(g[m]/gTmp[m]):0;
        }
    }
}


double *MainWindow::createsF0(pixel **Hcol, double *g, int N)
{
    double *f;
    f = (double*)malloc(N*sizeof(double));
    for( int n=0; n<N; n++ )
    {
        f[n] = g[Hcol[n][0].index] +//Zero
               g[Hcol[n][1].index] +//Right
               g[Hcol[n][2].index] +//Up
               g[Hcol[n][3].index] +//Left
               g[Hcol[n][4].index]; //Down
        //qDebug() << "f[" << n << "]: " << f[n];
    }
    return f;
}

double *MainWindow::serializeImageToProccess(QImage img, int sensor)
{
    int M, m;
    double *g;
    M = img.width() * img.height();
    g = (double*)malloc( M * sizeof(double) );

    QRgb rgb;
    m=0;
    for( int r=0; r<img.height(); r++ )
    {
        for( int c=0; c<img.width(); c++ )
        {
            if( sensor == _RED )
            {
                rgb     = img.pixel(QPoint(c,r));
                g[m]    = (double)qRed(rgb);
            }
            else
            {
                if( sensor == _RGB )
                {
                    rgb     = img.pixel(QPoint(c,r));
                    g[m]    = (double)(qRed(rgb) + qGreen(rgb) + qBlue(rgb));
                }
                else
                {
                    if( sensor == _GREEN )
                    {
                        rgb     = img.pixel(QPoint(c,r));
                        g[m]    = (double)qGreen(rgb);
                    }
                    else
                    {   //_BLUE
                        rgb     = img.pixel(QPoint(c,r));
                        g[m]    = (double)qBlue(rgb);
                    }
                }
            }
            m++;
        }
    }
    return g;
}

void MainWindow::createsHColAndHrow(pixel **Hcol, int **Hrow, QImage *img, lstDoubleAxisCalibration *daCalib )
{
    //Prepares variables and constants
    //..
    int hypW, hypH, hypL, idVoxel;
    QList<double> lstChoises;
    strDiffProj Pj;
    lstChoises  = getWavesChoised();
    hypW        = daCalib->squareUsableW;
    hypH        = daCalib->squareUsableH;
    hypL        = lstChoises.count();

    //Fill Hcol
    //..
    idVoxel = 0;
    for(int len=1; len<=hypL; len++)
    {
        Pj.wavelength = lstChoises.at(len-1);
        for(int row=1; row<=hypH; row++)
        {
            for(int col=1; col<=hypW; col++)
            {
                //Obtain diffraction projection for the acutual wavelength
                Pj.x = col;
                Pj.y = row;
                calcDiffProj(&Pj,daCalib);
                //Creates a new item in the c-th Hcol
                Hcol[idVoxel][0].x      = Pj.x;//Zero
                Hcol[idVoxel][0].y      = Pj.y;
                Hcol[idVoxel][0].index  = xyToIndex( Hcol[idVoxel][0].x, Hcol[idVoxel][0].y, img->width() );

                Hcol[idVoxel][1].x      = Pj.rx;//Right
                Hcol[idVoxel][1].y      = Pj.ry;
                Hcol[idVoxel][1].index  = xyToIndex( Hcol[idVoxel][1].x, Hcol[idVoxel][1].y, img->width() );

                Hcol[idVoxel][2].x      = Pj.ux;//Up
                Hcol[idVoxel][2].y      = Pj.uy;
                Hcol[idVoxel][2].index  = xyToIndex( Hcol[idVoxel][2].x, Hcol[idVoxel][2].y, img->width() );

                Hcol[idVoxel][3].x      = Pj.lx;//Left
                Hcol[idVoxel][3].y      = Pj.ly;
                Hcol[idVoxel][3].index  = xyToIndex( Hcol[idVoxel][3].x, Hcol[idVoxel][3].y, img->width() );

                Hcol[idVoxel][4].x      = Pj.dx;//Down
                Hcol[idVoxel][4].y      = Pj.dy;
                Hcol[idVoxel][4].index  = xyToIndex( Hcol[idVoxel][4].x, Hcol[idVoxel][4].y, img->width() );

                //Creates new item in Hrow
                insertItemIntoRow(Hrow,Hcol[idVoxel][0].index,idVoxel);
                insertItemIntoRow(Hrow,Hcol[idVoxel][1].index,idVoxel);
                insertItemIntoRow(Hrow,Hcol[idVoxel][2].index,idVoxel);
                insertItemIntoRow(Hrow,Hcol[idVoxel][3].index,idVoxel);
                insertItemIntoRow(Hrow,Hcol[idVoxel][4].index,idVoxel);

                idVoxel++;
            }
        }
    }
}

void MainWindow::insertItemIntoRow(int **Hrow, int row, int col)
{
    int actualPos;
    actualPos = Hrow[row][0]+1;
    Hrow[row] = (int*)realloc(Hrow[row],((actualPos+1)*sizeof(int)));
    Hrow[row][actualPos] = col;
    Hrow[row][0]++;
}

QList<double> MainWindow::getWavesChoised()
{
    QList<double> wavelengths;
    QString waves;
    waves = readFileParam(_PATH_WAVE_CHOISES);
    QList<QString> choises;
    choises = waves.split(",");
    Q_FOREACH(const QString choise, choises)
    {
        if( !choise.isEmpty() && choise != " " && choise!="\n" )
        {
            wavelengths.append(choise.toDouble(0));
        }
    }
    return wavelengths;
}

void MainWindow::on_actionValidCal_triggered()
{
    //validateCalibration *frmValCal = new validateCalibration(this);
    //frmValCal->setModal(false);
    //frmValCal->show();
}

void MainWindow::on_actionValCal_triggered()
{
    selWathToCheck *tmpFrm = new selWathToCheck(globalEditImg,this);
    tmpFrm->setModal(false);
    tmpFrm->show();
}

void MainWindow::on_actionSquareUsable_triggered()
{
    //Read Calibration
    //Region of interes
    //..
    squareAperture *rectSquare = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SQUARE_USABLE, rectSquare ) )
    {
        funcShowMsg("ERROR","Loading _PATH_SQUARE_USABLE");
        return (void)false;
    }

    selWathToCheck *checkCalib = new selWathToCheck(globalEditImg,this);
    checkCalib->showSqUsable(
                                rectSquare->rectX,
                                rectSquare->rectY,
                                rectSquare->rectW,
                                rectSquare->rectH,
                                Qt::magenta
                            );

}

void MainWindow::on_actionChoseWavelength_triggered()
{
    choseWaveToExtract *form = new choseWaveToExtract(this);
    form->show();
}

void MainWindow::on_actionFittFunction_triggered()
{
    //Select images
    //..
    QString originFileName;
    originFileName = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select image origin..."),
                                                        "./tmpImages/",
                                                        "(*.ppm *.RGB888 *.tif *.png *.jpg, *.jpeg *.gif);;"
                                                     );
    if( originFileName.isEmpty() )
    {
        return (void)NULL;
    }

    //Obtains dots
    //..
    int row, col, min, minPos, val, i;
    QRgb pixel;
    QImage img(originFileName);
    QImage tmpImg(originFileName);
    QString function;
    function = QString::number(img.height());
    i = 0;
    for( col=0; col<img.width(); col++ )
    {
        min     = 900;
        minPos  = 0;
        for( row=0; row<img.height(); row++ )
        {
            pixel   = img.pixel(col,row);
            val     = qRed(pixel)+qGreen(pixel)+qBlue(pixel);
            if( val < min )
            {
                min     = val;
                minPos  = row;
            }
            tmpImg.setPixelColor(QPoint(col,minPos),Qt::white);
        }
        function.append(","+QString::number(img.height()-minPos));
        tmpImg.setPixelColor(QPoint(col,minPos),Qt::magenta);
        i++;
    }
    //Save
    saveFile(_PATH_HALOGEN_FUNCTION,function);
    tmpImg.save(_PATH_AUX_IMG);

    funcShowMsg("Function saved successfully",_PATH_HALOGEN_FUNCTION);

}

void MainWindow::on_actionShow_hypercube_triggered()
{
    //Select images
    //..
    QString originFileName;
    originFileName = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select hypercube..."),
                                                        "./Hypercubes/",
                                                        "(*.hypercube);;"
                                                     );
    qDebug() << originFileName;
    if( originFileName.isEmpty() )
    {
        return (void)NULL;
    }

    //Generates hypercube
    //..
    extractsHyperCube(originFileName);

    funcShowMsg("Hypercube extracted successfully",_PATH_TMP_HYPCUBES);


}

void MainWindow::extractsHyperCube(QString originFileName)
{
    //Extracts information about the hypercube
    //..
    QString qstringHypercube;
    QList<QString> hypItems;
    QList<double> waves;
    qstringHypercube = readFileParam( originFileName );
    hypItems = qstringHypercube.split(",");
    QString dateTime;
    int W, H, L, l;
    dateTime = hypItems.at(0);  hypItems.removeAt(0);
    W = hypItems.at(0).toInt(0);         hypItems.removeAt(0);
    H = hypItems.at(0).toInt(0);         hypItems.removeAt(0);
    L = hypItems.at(0).toInt(0);         hypItems.removeAt(0);
    for(l=0;l<L;l++)
    {
        waves.append( hypItems.at(0).toDouble(0) );
        hypItems.removeAt(0);
    }

    //Generates the images into a tmpImage
    //..
    funcClearDirFolder(_PATH_TMP_HYPCUBES);
    QString tmpFileName;
    //QList<QImage> hypercube;
    QImage tmpImg(W,H,QImage::Format_RGB32);
    int tmpVal;
    int col, row;
    double tmp;
    //max = vectorMaxQListQString(hypItems);
    //qDebug() << "To norm max: " << max;

    //Calculate the max cal for each wavelength
    double max[L+1];
    int i;
    i=0;
    max[L] = 0;
    for( l=0; l<L; l++ )
    {
        max[l] = 0;
        for( row=0; row<H; row++ )
        {
            for( col=0; col<W; col++ )
            {
                if(hypItems.at(i).toDouble(0)>max[l])
                {
                    max[l] = hypItems.at(i).toDouble(0);
                }
                if(hypItems.at(i).toDouble(0)>max[L])
                {
                    max[L] = hypItems.at(i).toDouble(0);
                }
                i++;
            }
        }
    }

    //Normalize and export image
    //int tmpMax;
    i=0;
    for( l=0; l<L; l++ )
    {
        //tmpMax = 0;
        //qDebug() << "max[l]: " << max[l];

        for( row=0; row<H; row++ )
        {
            for( col=0; col<W; col++ )
            {
                tmp = hypItems.at(i).toDouble(0);
                if(true)//Normalize
                {
                    //tmpVal  = (tmp<=0.0)?0:round( (tmp/max[l]) * 255.0 );
                    tmpVal  = (tmp<=0.0)?0:round( (tmp/max[L]) * 255.0 );
                }
                else
                {
                    tmpVal = tmp;
                }
                //tmpImg.setPixelColor(QPoint(col,row),qGray(tmpVal,tmpVal,tmpVal));
                tmpImg.setPixelColor(QPoint(col,row),qRgb(tmpVal,tmpVal,tmpVal));

                //if(tmpVal>tmpMax)
                //{
                //    tmpMax=tmpVal;
                //}

                i++;
            }
        }

        //qDebug() << "tmpMax: " << tmpMax;

        if( SQUARE_BICUBIC_ITERATIONS > 1 )
        {
            tmpFileName =   _PATH_TMP_HYPCUBES +
                            QString::number(waves.at(l)) +
                            "."+
                            QString::number(SQUARE_BICUBIC_ITERATIONS) +
                            "Pasadas" +
                            ".png";
        }
        else
        {
            tmpFileName = _PATH_TMP_HYPCUBES +
                          QString::number(waves.at(l)) +
                          ".png";
        }
        tmpImg.save(tmpFileName);
        tmpImg.fill(Qt::black);
        //hypercube.append(tmpImg);
    }

}

void MainWindow::on_actionBilinear_interpolation_triggered()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(
                                                this,
                                                tr("Save Hypercube as..."),
                                                _PATH_IMG_FILTERED,
                                                tr("Images (*.png *.jpg *.jpeg *.gif *.BMP)")
                                            );
    if( fileName.isEmpty() )
    {
        return (void)NULL;
    }

    mouseCursorWait();
    QImage tmpImgOrig(_PATH_DISPLAY_IMAGE);
    QImage imgBilInt;
    imgBilInt = bilinearInterpolationQImage(tmpImgOrig);
    fileName.replace(".png","");
    imgBilInt.save(fileName+".png");
    tmpImgOrig.save(fileName+"LastOrig.png");
    mouseCursorReset();

    funcShowMsg("Success","Bilinear interpolation applied");



}

/*
void MainWindow::on_slideSquareShuterSpeedSmall_valueChanged(int value)
{
    value = value;
    refreshSquareShootSpeed();
}

void MainWindow::on_slideSquareShuterSpeed_valueChanged(int value)
{
    value = value;
    refreshSquareShootSpeed();
}

void MainWindow::refreshSquareShootSpeed()
{
    QString qstrVal = QString::number(
                                        ui->slideSquareShuterSpeedSmall->value() +
                                        ui->slideSquareShuterSpeed->value()
                                     );
    ui->labelSquareShuterSpeed->setText( "Square Shuter Speed: " + qstrVal );
}*/



void MainWindow::on_pbCopyShutter_clicked()
{
    QString tmpFileName = "./XML/camPerfils/";
    tmpFileName.append(_PATH_LAST_SNAPPATH);
    funcGetRaspParamFromXML( raspcamSettings, tmpFileName );
    funcIniCamParam( raspcamSettings );
}

structCamSelected* MainWindow::getCameraSelected()
{
    return camSelected;
}

void MainWindow::on_actionslideHypCam_triggered()
{
    slideHypCam* frmSlide = new slideHypCam(this);
    frmSlide->setWindowModality(Qt::ApplicationModal);
    frmSlide->setWindowTitle("Slide HypCam");
    frmSlide->showMaximized();
    frmSlide->show();
}

int MainWindow::obtainFile( std::string fileToObtain, std::string fileNameDestine, QString txtBar )
{
    if( funcRaspFileExists( fileToObtain ) == 0 )
    {
        debugMsg("Remote-File does not exists");
        return 0;
    }
    else
    {
        QString tmpPath = QString::fromStdString(fileNameDestine);

        //Remove local file
        funcDeleteFile( tmpPath );

        //Get remote file
        int fileLen;
        u_int8_t* fileReceived = funcQtReceiveFile( fileToObtain, &fileLen, txtBar );
        saveBinFile_From_u_int8_T( fileNameDestine, fileReceived, fileLen );
        delete[] fileReceived;

        //Check if remote file was transmitted
        if( !fileExists( tmpPath ) )
        {
            debugMsg("Error acquiring Remote File: " + fileToObtain);
            return 0;
        }

    }

    return 1;
}


QImage MainWindow::obtainImageFile( std::string fileToObtain, QString txtBar )
{
    QImage img;
    if( funcRaspFileExists( fileToObtain ) == 0 )
    {
        debugMsg("File does not exists");
        return img;
    }
    else
    {

        int fileLen;
        u_int8_t* fileReceived = funcQtReceiveFile( fileToObtain, &fileLen, txtBar );

        ui->progBar->setVisible(true);
        ui->progBar->setValue(0);
        ui->progBar->update();
        progBarUpdateLabel("Saving image locally",0);

        if( saveBinFile_From_u_int8_T(_PATH_IMAGE_RECEIVED,fileReceived,fileLen) )
            img = QImage(_PATH_IMAGE_RECEIVED);

        progBarUpdateLabel("",0);

        /*
        const unsigned char* fileReceivedQt = reinterpret_cast<const unsigned char*>(&fileReceived[0]);
        std::string strTmp;
        strTmp.append(fileReceivedQt);
         */
        /*
        const char *fileReceivedQt = reinterpret_cast<char*>(fileReceived);
        QBuffer buffer;
        buffer.open(QBuffer::ReadWrite);
        buffer.write(fileReceivedQt);
        buffer.seek(0);
        qDebug() << "buffer.size(): " << buffer.size() << " of " << fileLen;
        img = QImage::fromData( buffer.buffer() );*/


        /*
        QBuffer buffer;
        buffer.setData(reinterpret_cast<const char*>(fileReceived), fileLen);
        QByteArray ba = QByteArray::fromRawData(buffer.data(),fileLen);
        QDataStream s1(ba);
        s1.setByteOrder(QDataStream::LittleEndian);
        s1 >> img;*/

        //qDebug() << "imgW: " << img.width();
        //QByteArray ba = QByteArray(reinterpret_cast<const char*>(fileReceived), fileLen);
        //QBuffer buffer(ba);
        //camRes = getCamRes();
        //img = QImage::fromData(camRes->width, camRes->height, QImage::Format_RGB888);
        //img.save(_PATH_IMAGE_RECEIVED);
        debugMsg("File received complete");

        /*
        ui->progBar->setValue(100.0);
        ui->progBar->update();
        ui->progBar->setVisible(false);
        progBarUpdateLabel("",0);*/

        return img;

    }

    return img;
}

u_int8_t* MainWindow::funcQtReceiveFile( std::string fileNameRequested, int* fileLen, QString txtBar )
{

    ui->progBar->setVisible(true);
    ui->progBar->setValue( 0.0 );
    ui->progBar->update();
    progBarUpdateLabel(txtBar,0);
    //It assumes that file exists and this was verified by command 10
    //
    //Receive the path of a file into raspCamera and
    //return the file if exists or empty arry otherwise

    //Connect to socket
    int socketID = funcRaspSocketConnect();
    if( socketID == -1 )
    {
        debugMsg("ERROR connecting to socket");
        return (u_int8_t*)NULL;
    }

    //Request file
    int n;
    strReqFileInfo *reqFileInfo = (strReqFileInfo*)malloc(sizeof(strReqFileInfo));
    memset( reqFileInfo, '\0', sizeof(strReqFileInfo) );
    reqFileInfo->idMsg          = 11;
    reqFileInfo->fileNameLen    = fileNameRequested.size();
    memcpy( &reqFileInfo->fileName[0], fileNameRequested.c_str(), fileNameRequested.size() );
    int tmpFrameLen = sizeof(strReqFileInfo);//IdMsg + lenInfo + fileLen + padding
    n = ::write( socketID, reqFileInfo, tmpFrameLen+1 );
    if (n < 0){
        debugMsg("ERROR writing to socket");
        return (u_int8_t*)NULL;
    }

    //Receive file's size
    *fileLen = funcReceiveOnePositiveInteger( socketID );
    std::cout << "fileLen: " << *fileLen << std::endl;
    fflush(stdout);

    //Prepare container
    u_int8_t* theFILE = (u_int8_t*)malloc(*fileLen);
    bzero( &theFILE[0], *fileLen );

    //Read file from system buffer
    int filePos     = 0;
    int remainder   = *fileLen;

    //Read image from buffer
    while( remainder > 0 )
    {
        n = read(socketID,(void*)&theFILE[filePos],remainder);
        remainder   -= n;
        filePos     += n;

        ui->progBar->setValue( ( ( 1.0 - ( (float)remainder/(float)*fileLen ) ) * 100.0) );
        ui->progBar->update();

        //std::cout << ( ( 1.0 - ( (float)remainder/(float)*fileLen ) ) * 100.0) << "%" << std::endl;

    }
    //std::cout << "(Last info) filePos: " << filePos << " remainder: " << remainder << " n: " << n << std::endl;
    //fflush(stdout);

    //Close connection
    ::close(socketID);

    ui->progBar->setValue(0);
    ui->progBar->setVisible(false);
    ui->progBar->update();
    progBarUpdateLabel("",0);

    //Return file
    return theFILE;


}

/*
void MainWindow::on_pbGetSlideCube_clicked()
{

    //obtainFile( "./timeLapse/duck.png", "./tmpImages/patito.png" );



    ui->pbStartVideo->setEnabled(false);


    mouseCursorWait();

    camRes = getCamRes();

    int numImages = 0;
    u_int8_t** lstImgs = funcGetSLIDESnapshot( &numImages, true );

    delete[] lstImgs;

    mouseCursorReset();

    ui->pbStartVideo->setEnabled(true);


}
*/

structCamSelected* MainWindow::funcGetCamSelected()
{
    return camSelected;
}

u_int8_t** MainWindow::funcGetSLIDESnapshot( int* numImages, bool saveFiles )
{
    int i, n;

    u_int8_t** lstImgs;

    //
    //Getting calibration
    //..
    lstDoubleAxisCalibration daCalib;
    funcGetCalibration(&daCalib);

    //
    //Save path
    //..
    //saveFile(_PATH_LAST_SNAPPATH,ui->txtSnapPath->text());

    //Save lastest settings
    if( saveRaspCamSettings( _PATH_LAST_SNAPPATH ) == false ){
        funcShowMsg("ERROR","Saving last snap-settings");
    }

    //
    //Set required image's settings
    //..
    strReqImg *reqImg       = (strReqImg*)malloc(sizeof(strReqImg));
    memset(reqImg,'\0',sizeof(strReqImg));
    reqImg->idMsg           = (unsigned char)9;
    reqImg->raspSett        = funcFillSnapshotSettings( reqImg->raspSett );

    //
    //Define slide' region
    //..
    reqImg->needCut     = false;
    reqImg->fullFrame   = false;
    reqImg->isSlide     = true;
    reqImg->imgCols     = camRes->width;//2592 | 640
    reqImg->imgRows     = camRes->height;//1944 | 480
    reqImg              = funcFillSLIDESettings(reqImg);

    //
    //Send Slide-cube Request-parameters and recibe ACK if all parameters
    //has been received are correct

    //Open socket
    int sockfd = connectSocket( camSelected );
    unsigned char bufferRead[frameBodyLen];
    qDebug() << "Socket opened";

    //Require Slide-cube size..
    n = ::write(sockfd,reqImg,sizeof(strReqImg));
    qDebug() << "Slide-cube requested";

    //Receive ACK with the camera status
    n = read(sockfd,bufferRead,frameBodyLen);
    if( bufferRead[1] == 1 ){//Beginning the image adquisition routine
        qDebug() << "Slide: Parameters received correctly";
    }else{//Do nothing becouse camera is not accessible
        qDebug() << "Slide: Parameter received with ERROR";
        u_int8_t** tmp  = (u_int8_t**)malloc(sizeof(u_int8_t*));
        tmp[0]          = (u_int8_t*)malloc(sizeof(u_int8_t));
        tmp[0][0]       = '\0';
        return tmp;
    }

    //
    //Send request to start time lapse and wait for ACK if time lapse
    //was saved correctly
    //
    n = ::write(sockfd,"beginTimelapse",14);
    if( n != 14 )
        qDebug() << "ERROR starting timelapse";
    else
        qDebug() << "Timelapse started";

    //Move motor
    arduinoMotor motor;
    motor.setAWalk(
                    reqImg->slide.degreeIni,
                    reqImg->slide.degreeEnd,
                    reqImg->slide.degreeJump,
                    reqImg->slide.speed
                  );
    //motor.doAWalk();
    motor.start();

    //Show prog bar
    int expecNumImgs;
    expecNumImgs =  ceil(
                            (float)(reqImg->slide.degreeEnd - reqImg->slide.degreeIni) /
                            (float)reqImg->slide.degreeJump
                         );
    progBarTimer( (expecNumImgs+1) * reqImg->slide.speed );
    QtDelay(10);//Forces to hide prog-bar

    //Receive ACK with the time lapse status
    memset(bufferRead,0,frameBodyLen);
    n = read(sockfd,bufferRead,frameBodyLen);
    strNumSlideImgs numImgs;
    memcpy( &numImgs, bufferRead, sizeof(strNumSlideImgs) );

    //Validate image generated    
    //expecNumImgs = ceil( (float)duration / (float)reqImg->slide.speed );
    qDebug() << "Imagery generated: " << numImgs.numImgs << " of " << expecNumImgs;




    //
    //Request to compute slide-diffractio (it is received each image in the cube)
    //
    float timeLapseRatio;
    timeLapseRatio = 100.0 * ( (float)numImgs.numImgs / (float)expecNumImgs );
    int getImagery = 1;
    if( timeLapseRatio < 90.0)
    {
        QString tmpError;
        tmpError.append("Insufficient time lapse imagery generated, timeLapseRatio: ");
        tmpError.append(QString::number(timeLapseRatio));
        tmpError.append("% of 90.0% required.\n\nDo you want to obtain only aquired imagery?");
        getImagery = funcShowMsgYesNo("ALERT", tmpError);

        if( !getImagery )
        {
            n = ::write(sockfd,"0",2);
            ::close(sockfd);
            lstImgs = (u_int8_t**)malloc(sizeof(u_int8_t*));
            delete[] lstImgs;
            *numImages = 0;
            return (u_int8_t**)NULL;
        }
    }


    if( getImagery )
    {

        QTime myTimer;



        //Start slide-cube generation process
        qDebug() << "Slide-cube generation process starting request";
        n = ::write(sockfd,"1",2);

        //Recuest all expected images
        numImgs.idMsg   = 1;
        int imgLen      = (reqImg->slide.rows1 * (reqImg->slide.cols1 +reqImg->slide.cols2) * 3);
        //expecNumImgs    = 1;
        funcClearDirFolder( _PATH_SLIDE_TMP_FOLDER );
        int nextImg;
        lstImgs         = (u_int8_t**)malloc(expecNumImgs*sizeof(u_int8_t*));
        *numImages      = 0;
        int framePos    = 0;
        int remainder   = imgLen+1;
        myTimer.start();
        int firstImageFlag = 0;
        std::string firstImageName;

        //Show Prog bar
        ui->progBar->setVisible(true);
        ui->progBar->setValue(0);
        ui->progBar->update();
        QtDelay(5);

        for( i=1; i<=expecNumImgs; i++ )
        {
            //Read next image status (Id or -1 if finishes)
            memset(bufferRead,'\0',sizeof(int)+1);
            n = read(sockfd,bufferRead,sizeof(int)+1);
            memcpy(&nextImg,bufferRead,sizeof(int));

            if( nextImg == -1 )
            {
                funcDebug("Camera does not have more images");
                break;
            }
            else
            {

                qDebug() << "Receiving image" << nextImg;

                //Frame memory allocation
                lstImgs[*numImages]  = (u_int8_t*)malloc((imgLen*sizeof(u_int8_t)));//Frame container
                bzero( lstImgs[*numImages], imgLen );
                framePos            = 0;
                remainder           = imgLen+1;

                //Read image from buffer
                while( remainder > 0 )
                {
                    n = read(sockfd,(void*)&lstImgs[*numImages][framePos],remainder);
                    remainder   -= n;
                    framePos    += n;
                    fflush(stdout);
                }
                *numImages = *numImages+1;




                if(saveFiles==true)
                {
                    //Variables
                    int numRows, numCols, r, c;
                    numRows = reqImg->slide.rows1;
                    numCols = reqImg->slide.cols1 + reqImg->slide.cols2;
                    QImage tmpImg(numCols, numRows, QImage::Format_RGB888);
                    //Slide part
                    int j = 0;
                    for( r=0; r<numRows; r++ )
                    {
                        for( c=0; c<reqImg->slide.cols1; c++ )
                        {
                            tmpImg.setPixel( c, r, qRgb( lstImgs[i-1][j], lstImgs[i-1][j+1], lstImgs[i-1][j+2] ) );
                            j += 3;
                        }
                    }
                    //Diffraction part
                    for( r=0; r<numRows; r++ )
                    {
                        for( c=reqImg->slide.cols1; c<numCols; c++ )
                        {
                            tmpImg.setPixel( c, r, qRgb( lstImgs[i-1][j], lstImgs[i-1][j+1], lstImgs[i-1][j+2] ) );
                            j += 3;
                        }
                    }
                    //Save
                    std::string tmpOutFileName(_PATH_SLIDE_TMP_FOLDER);
                    tmpOutFileName.append(QString::number(nextImg).toStdString());
                    tmpOutFileName.append(".png");//Slide Cube Part
                    tmpImg.save( tmpOutFileName.c_str() );

                    if( firstImageFlag==0 )
                    {
                        firstImageName.assign( tmpOutFileName.c_str() );
                        firstImageFlag = 1;
                    }

                }


                if(
                        ( numImgs.numImgs == *numImages ) &&
                        ( numImgs.numImgs < expecNumImgs )
                )
                {
                    qDebug() << "All possible images received";
                    break;
                }




            }



            //ui->labelProgBar->setText( QString::number(i) + " of " + QString::number(expecNumImgs) );
            ui->progBar->setValue( round( 100.0*((float)i/(float)expecNumImgs ) ) );
            ui->progBar->update();
            QtDelay(5);



        }

        ui->labelProgBar->setText("");
        ui->progBar->setValue(0);
        ui->progBar->setVisible(false);
        ui->progBar->update();
        QtDelay(5);

        qDebug() << "Slide-Cube received. Time(" << myTimer.elapsed()/1000 << "seconds)" ;


        if( firstImageFlag == 1 )
        {
            QImage tmpImg( firstImageName.c_str() );
            tmpImg.save( _PATH_DISPLAY_IMAGE );
            updateDisplayImage(_PATH_DISPLAY_IMAGE);
            //reloadImage2Display();
        }

        //
        //Save parameters into slide-imagery's folder
        //

        //Number of imagery
        QString tmpContain;
        QString tmpFileName;
        tmpContain = QString::number(*numImages);
        tmpFileName = _PATH_SLIDE_TMP_FOLDER;
        tmpFileName.append(_FILENAME_SLIDE_PARAM_NUM_IMGS);
        saveFile( tmpFileName, tmpContain );

        //Slide width
        tmpContain = QString::number( reqImg->slide.cols1 );
        tmpFileName = _PATH_SLIDE_TMP_FOLDER;
        tmpFileName.append(_FILENAME_SLIDE_PARAM_W);
        saveFile( tmpFileName, tmpContain );

        //Slide height
        tmpContain = QString::number( reqImg->slide.rows1 );
        tmpFileName = _PATH_SLIDE_TMP_FOLDER;
        tmpFileName.append(_FILENAME_SLIDE_PARAM_H);
        saveFile( tmpFileName, tmpContain );

    }

    //
    // Clear all, close all, and return
    //
    ::close(sockfd);


    return lstImgs;
}

int MainWindow::funcReceiveFrame( int sockfd, int idImg, int* frameLen, strReqImg *reqImg )
{
    //clock_t start, end;
    //start = clock();


    //--
    //It assumes that frame was not allocated
    //--

    int i, n, numMsgs;

    QTime myTimer;
    myTimer.start();
    //Get frame len
    *frameLen = funcReceiveOnePositiveInteger( sockfd );
    if( *frameLen < 0 )
    {
        qDebug() << "ERROR receiving frameLen";
        return -1;
    }
    qDebug() << "Frame len: " << *frameLen << "--" << myTimer.elapsed() << "ms";

    myTimer.start();
    //Get number of messages
    numMsgs = funcReceiveOnePositiveInteger( sockfd );
    if( numMsgs < 1 )
    {
        qDebug() << "ERROR receiving number of messages";
        return -1;
    }
    qDebug() << "Number of messages: " << numMsgs<< "--" << myTimer.elapsed() << "ms";


    myTimer.start();
    //Frame memory allocation
    u_int8_t* frame = (u_int8_t*)malloc((*frameLen*sizeof(u_int8_t))+(frameBodyLen*2));//Frame container
    bzero( frame, *frameLen );

    int framePos = 0;
    int remainder = *frameLen;

    while( framePos < *frameLen )
    {
        n = read(sockfd,(void*)&frame[framePos],remainder+1);
        //qDebug() << "ALERT: Received: " << n << " of " << remainder;
        remainder   -= n;
        framePos    += n;
    }
    qDebug() << "All copied"<< "--" << myTimer.elapsed() << "ms";

    /*
    for( i=0; i<numMsgs; i++ )
    {
        QtDelay(1);
        n = funcReceiveOneMessage( sockfd, (void*)&frame[i*frameBodyLen] );
        if( n != frameBodyLen )
        {
            qDebug() << "ALERT: Message: " << i << " Received: " << n << " of " << frameBodyLen;
            //return -1;
        }
        QtDelay(2);
    }
    //Receive last message if necessary
    int remainder = *frameLen%frameBodyLen;
    if( remainder > 0 )
    {
        n = funcReceiveOneMessage( sockfd, (void*)&frame[i*frameBodyLen] );
        printf("remainder: %d pos: %d n: %d\n",remainder,(i*frameBodyLen),n);
        fflush(stdout);
        if( n != remainder )
        {
            qDebug() << "ERROR: In remainder message: " << i << " Received: " << n << " of " << remainder;
            //return -1;
        }
        i++;
        n = funcReceiveOneMessage( sockfd, (void*)&frame[i*frameBodyLen] );
        printf("remainder2: %d pos: %d n: %d\n",remainder,(i*frameBodyLen),n);
        fflush(stdout);
        if( n != remainder )
        {
            qDebug() << "ERROR: In remainder2 message: " << i << " Received: " << n << " of " << remainder;
            //return -1;
        }

    }

    debugMsg("Antes clock");
    */





    //end = clock();
    //qDebug() << "Spend: " << (end-start)/1000 << " seconds";
    //fflush(stdout);
    //
    //Save frame as image
    //

    /*
    printf("Uno(%d,%d,%d,%d) Dos(%d,%d,%d,%d)",
                                                    reqImg->slide.x1,
                                                    reqImg->slide.y1,
                                                    reqImg->slide.rows1,
                                                    reqImg->slide.cols1,
                                                    reqImg->slide.x2,
                                                    reqImg->slide.y2,
                                                    reqImg->slide.rows2,
                                                    reqImg->slide.cols2
           );
    fflush(stdout);
    */


    //debugMsg("Antes QImage");

    int numRows, numCols, r, c;
    numRows = reqImg->slide.rows1;
    numCols = reqImg->slide.cols1 + reqImg->slide.cols2;
    //printf("Img(%d,%d)\n",numCols,numRows);
    //fflush(stdout);
    QImage tmpImg(numCols, numRows, QImage::Format_RGB888);

    //debugMsg("Antes for");

    //Slide part
    i = 0;
    for( r=0; r<numRows; r++ )
    {
        for( c=0; c<reqImg->slide.cols1; c++ )
        {
            tmpImg.setPixel( c, r, qRgb( frame[i], frame[i+1], frame[i+2] ) );
            i += 3;
        }
    }
    //debugMsg("antes diff");

    //Diffraction part
    for( r=0; r<numRows; r++ )
    {
        for( c=reqImg->slide.cols1; c<numCols; c++ )
        {
            tmpImg.setPixel( c, r, qRgb( frame[i], frame[i+1], frame[i+2] ) );
            i += 3;
        }
    }

    //debugMsg("antes save");
    //exit(0);


    //Save result
    if(0)
    {
        std::string tmpOutFileName(_PATH_SLIDE_TMP_FOLDER);
        tmpOutFileName.append(QString::number(idImg).toStdString());
        tmpOutFileName.append(".png");//Slide Cube Part
        tmpImg.save( tmpOutFileName.c_str() );
    }


    //debugMsg("Despues save");


    //
    //Save raw frame
    //
    //std::string tmpOutFileName(_PATH_SLIDE_TMP_FOLDER);
    //tmpOutFileName.append(QString::number(idImg).toStdString());
    //tmpOutFileName.append(".scp");//Slide Cube Part
    //saveBinFile_From_u_int8_T( tmpOutFileName, frame, *frameLen);


    //exit(0);
    return 1;
}

void MainWindow::funcDebug(QString msg)
{
    qDebug() << msg;
}

int MainWindow::funcRequestSubframe( int sockfd, strReqSubframe* subFrameParams )
{
    int n;

    //Validate input parameters
    if(
            (subFrameParams->posIni < 0 || subFrameParams->len < 1 ) &&
            ( subFrameParams->len != -6543 )
    )
    {
        //funcDebug("Parameter not setted");
        return -1;
    }

    //Evaluate that message was correctly sent
    //funcDebug("subFrameParams sent");
    //qDebug() << "Sending, pos: " << subFrameParams->posIni << " len: " << subFrameParams->len;

    n = ::write(sockfd,subFrameParams,sizeof(strReqSubframe)+1);
    if( n != sizeof(strReqSubframe)+1 )
    {
        //funcDebug("Write empty");
        return -1;
    }

    //funcDebug("Receiving ACK");
    n = funcReceiveACK( sockfd );

    //qDebug() << "ACK code received: " << n;

    return n;

}

int MainWindow::funcReceiveACK( int sockfd )
{

    //qDebug() << "funcReceiveACK";

    int n;
    u_int8_t buffer[3];
    n = read(sockfd,buffer,3);
    if( n < 0 )
        return -1;//Error receiving message

    //qDebug() << "n: " << n << " buffer[0]: " << buffer[0] << " buffer[1]: " << buffer[1];

    if( buffer[0] == 1 )
        return 1;
    if( buffer[0] == 0 )
        return 0;

    return -1;
}

int MainWindow::funcReceiveOneMessage( int sockfd, void* frame )
{
    //qDebug() << "funcReceiveOneMessage";

    //It assumess that frame has memory prviously allocated
    //Return the number n of bytes received

    int n;
    n = read(sockfd,frame,frameBodyLen);
    if( n < 0 )
        return 0;//Error receiving message


    return n;
}








strReqImg* MainWindow::funcFillSLIDESettings(strReqImg* reqImg)
{

    //
    //Obtain square coordinates from XML file
    //
    squareAperture *tmpSlide = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SLIDE_APERTURE, tmpSlide ) ){
        funcShowMsg("ERROR","Loading Slide Area");
        return reqImg;
    }
    squareAperture *tmpSlideDiff = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SLIDE_DIFFRACTION, tmpSlideDiff ) ){
        funcShowMsg("ERROR","Loading Slide Diffraction Area");
        return reqImg;
    }

    //Fit speed, duration and other parameters
    if( !funGetSlideSettingsXML( _PATH_SLIDE_SETTINGS, reqImg ) ){
        funcShowMsg("ERROR","Loading Slide Settings");
        return reqImg;
    }

    //Fit slide area
    reqImg->slide.x1        = round( ((float)tmpSlide->rectX / (float)tmpSlide->canvasW) * (float)reqImg->imgCols );
    reqImg->slide.y1        = round( ((float)tmpSlide->rectY / (float)tmpSlide->canvasH) * (float)reqImg->imgRows );
    reqImg->slide.cols1     = round( ((float)tmpSlide->rectW / (float)tmpSlide->canvasW) * (float)reqImg->imgCols );
    reqImg->slide.rows1     = round( ((float)tmpSlide->rectH / (float)tmpSlide->canvasH) * (float)reqImg->imgRows );

    //Fit diffraction area
    reqImg->slide.x2        = round( ((float)tmpSlideDiff->rectX / (float)tmpSlideDiff->canvasW) * (float)reqImg->imgCols );
    reqImg->slide.y2        = round( ((float)tmpSlideDiff->rectY / (float)tmpSlideDiff->canvasH) * (float)reqImg->imgRows );
    reqImg->slide.cols2     = round( ((float)tmpSlideDiff->rectW / (float)tmpSlideDiff->canvasW) * (float)reqImg->imgCols );
    reqImg->slide.rows2     = round( ((float)tmpSlideDiff->rectH / (float)tmpSlideDiff->canvasH) * (float)reqImg->imgRows );

    //slideSetting.speed      = 800;
    //slideSetting.duration   = 1700;//8000

    return reqImg;
}


void MainWindow::on_pbDrawSlide_triggered()
{
    if( funcShowMsgYesNo("I have to options...","Video Diffraction Area?") )
        funcDrawRectangleFromXML( _PATH_VIDEO_SLIDE_DIFFRACTION );
    else
        funcDrawRectangleFromXML( _PATH_SLIDE_DIFFRACTION );

    /*
    //
    //Obtain square coordinates from XML file
    //
    squareAperture *tmpSlide = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SLIDE_APERTURE, tmpSlide ) ){
        funcShowMsg("ERROR","Loading Slide Area");
        return (void)NULL;
    }
    squareAperture *tmpSlideDiff = (squareAperture*)malloc(sizeof(squareAperture));
    if( !funGetSquareXML( _PATH_SLIDE_DIFFRACTION, tmpSlideDiff ) ){
        funcShowMsg("ERROR","Loading Slide Diffraction Area");
        return (void)NULL;
    }

    //
    // Fix coordinates
    //
    int minY;
    minY = (tmpSlide->rectY < tmpSlideDiff->rectY)?tmpSlide->rectY:tmpSlideDiff->rectY;
    tmpSlide->rectY     = minY;
    tmpSlideDiff->rectY = minY;

    int auxH1, auxH2, maxH;
    auxH1               = (tmpSlide->rectY + tmpSlide->rectH)-tmpSlide->rectY;
    auxH2               = (tmpSlideDiff->rectY + tmpSlideDiff->rectH)-tmpSlideDiff->rectY;
    maxH                = (auxH1 > auxH2)?auxH1:auxH2;
    tmpSlide->rectH     = maxH;
    tmpSlideDiff->rectH = maxH;

    //Save new coordinates
    saveRectangleAs( tmpSlide, _PATH_SLIDE_APERTURE );
    saveRectangleAs( tmpSlideDiff, _PATH_SLIDE_DIFFRACTION );

    //Draw into scene
    canvasCalib->scene()->clear();
    funcDrawRectangleFromXML( _PATH_SLIDE_APERTURE );
    funcDrawRectangleFromXML( _PATH_SLIDE_DIFFRACTION );*/
}

int MainWindow::saveRectangleAs( squareAperture *square, QString fileName )
{
    QString tmpContain;
    tmpContain.append( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" );
    tmpContain.append("<Variables>\n");
    tmpContain.append("\t<W>"+ QString::number( square->canvasW  ) +"</W>\n");
    tmpContain.append("\t<H>"+ QString::number( square->canvasH ) +"</H>\n");
    tmpContain.append("\t<x>"+ QString::number( square->rectX ) +"</x>\n");
    tmpContain.append("\t<y>"+ QString::number( square->rectY ) +"</y>\n");
    tmpContain.append("\t<w>"+ QString::number( square->rectW ) +"</w>\n");
    tmpContain.append("\t<h>"+ QString::number( square->rectH ) +"</h>\n");
    tmpContain.append("</Variables>");
    if( !saveFile( fileName, tmpContain ) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}

void MainWindow::on_actionSlide_settings_triggered()
{
    formSlideSettings* tmpForm = new formSlideSettings(this);
    tmpForm->setModal(true);
    tmpForm->show();
}

void MainWindow::on_pbShutdown_clicked()
{
    std::string cmdShutdown = "sudo shutdown -h now";

    if( !camSelected->isConnected){
        funcShowMsg("Alert","Camera not connected");
        return (void)NULL;
    }
    if( ui->tableLstCams->rowCount() == 0 ){
        funcShowMsg("Alert","Not camera detected");
        return (void)NULL;
    }
    ui->tableLstCams->setFocus();

    //Prepare message to send
    frameStruct *frame2send = (frameStruct*)malloc(sizeof(frameStruct));
    memset(frame2send,'\0',sizeof(frameStruct));
    frame2send->header.idMsg = (unsigned char)2;
    frame2send->header.numTotMsg = 1;
    frame2send->header.consecutive = 1;
    frame2send->header.trigeredTime = 0;
    frame2send->header.bodyLen   = cmdShutdown.length();
    bzero(frame2send->msg,cmdShutdown.length()+1);
    memcpy(
                frame2send->msg,
                cmdShutdown.c_str(),
                cmdShutdown.length()
          );

    //Prepare command message
    int sockfd, n, tmpFrameLen;
    //unsigned char bufferRead[sizeof(frameStruct)];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    qDebug() << "Comm IP: " << QString((char*)camSelected->IP);
    if (sockfd < 0){
        qDebug() << "opening socket";
        return (void)NULL;
    }
    //server = gethostbyname( ui->tableLstCams->item(tmpRow,1)->text().toStdString().c_str() );
    server = gethostbyname( (char*)camSelected->IP );
    if (server == NULL) {
        qDebug() << "Not such host";
        return (void)NULL;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(camSelected->tcpPort);
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        qDebug() << "ERROR: connecting to socket";
        return (void)NULL;
    }


    //Request command result
    qDebug() << "idMsg: " << (int)frame2send->header.idMsg;
    qDebug() << "command: " << frame2send->msg;
    qDebug() << "tmpFrameLen: " << sizeof(frameHeader) + cmdShutdown.length();
    tmpFrameLen = sizeof(frameHeader) + cmdShutdown.length();
    n = ::write(sockfd,frame2send,tmpFrameLen+1);
    if(n<0){
        qDebug() << "ERROR: Sending command";
        return (void)NULL;
    }
    ::close(sockfd);

    ui->pbConnect->setEnabled(false);
    ui->tableLstCams->removeRow( ui->tableLstCams->currentRow() );
    ui->pbShutdown->setEnabled(false);



}

void MainWindow::on_pbSnapVid_clicked()
{

    bool ok;

    //-----------------------------------------------------
    // Record Remote Video
    //-----------------------------------------------------
    QString videoID;
    funcMainCall_RecordVideo(&videoID,true);

    //-----------------------------------------------------
    // Obtain Remote Video
    //-----------------------------------------------------
    if( obtainFile( _PATH_VIDEO_REMOTE_H264, _PATH_VIDEO_RECEIVED_H264, "Transmitting Remote Video" ) )
    {
        QString rmRemVidCommand;
        rmRemVidCommand.append("rm ");
        rmRemVidCommand.append(_PATH_VIDEO_REMOTE_H264);
        funcRemoteTerminalCommand( rmRemVidCommand.toStdString(), camSelected, 0, false, &ok );
    }
    else
    {
        return (void)false;
    }

    //-----------------------------------------------------
    // Extract Frames from Video
    //-----------------------------------------------------

    //Clear local folder
    QString tmpFramesPath;
    tmpFramesPath.append(_PATH_VIDEO_FRAMES);
    tmpFramesPath.append("tmp/");
    funcClearDirFolder( tmpFramesPath );

    //Extract Frames from Local Video
    QString locFrameExtrComm;
    locFrameExtrComm.append("ffmpeg -framerate ");
    locFrameExtrComm.append(QString::number(_VIDEO_FRAME_RATE));
    locFrameExtrComm.append(" -r ");
    locFrameExtrComm.append(QString::number(_VIDEO_FRAME_RATE));
    locFrameExtrComm.append(" -i ");
    locFrameExtrComm.append(_PATH_VIDEO_RECEIVED_H264);
    locFrameExtrComm.append(" ");
    locFrameExtrComm.append(tmpFramesPath);
    locFrameExtrComm.append("%d");
    locFrameExtrComm.append(_FRAME_EXTENSION);
    qDebug() << locFrameExtrComm;
    progBarUpdateLabel("Extracting Frames from Video",0);
    if( !funcExecuteCommand(locFrameExtrComm) )
    {
        funcShowMsg("ERROR","Extracting Frames from Video");
    }
    progBarUpdateLabel("",0);

    //-----------------------------------------------------
    // Update mainImage from Frames
    //-----------------------------------------------------
    funcUpdateImageFromFolder(tmpFramesPath,_FRAME_EXTENSION);

    funcResetStatusBar();

}

int MainWindow::funcUpdateImageFromFolder( QString folder, QString fileExtension )
{
    numFrames = funcAccountFilesInFolder( folder );
    if( numFrames <= 0 )
    {
        funcShowMsgERROR("Empty Folder: " + folder);
        return 0;
    }
    else
    {
        //Image Received Path
        int idFrame;
        idFrame = ceil( numFrames * 0.6 );
        QString tmpImgRec;
        tmpImgRec.append( folder );
        tmpImgRec.append( QString::number(idFrame) );
        tmpImgRec.append( fileExtension );

        //Update Received Image
        free(globalEditImg);
        globalEditImg = new QImage(tmpImgRec);
        updateDisplayImage(globalEditImg);
        qDebug() << tmpImgRec;

    }
    return 1;
}


class MyVideoSurface : public QAbstractVideoSurface
{
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const
    {
        Q_UNUSED(handleType);

        // Return the formats you will support
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB565;
    }



    bool present(const QVideoFrame &frame)
    {
        Q_UNUSED(frame);
        // Handle the frame and do your processing

        return true;
    }
};

int MainWindow::funcVideoToFrames(QString videoSource)//_PATH_VIDEO_FRAMES, _PATH_VIDEO_RECEIVED_MP4
{

    ui->progBar->setValue(0);
    ui->progBar->setVisible(true);
    ui->progBar->update();

    numFrames = 0;
    //lstFrames.clear();
    funcClearDirFolder( _PATH_VIDEO_FRAMES );

    QMediaPlayer *player = new QMediaPlayer();
    QVideoProbe *probe = new QVideoProbe;
    MyVideoSurface* myVideoSurface = new MyVideoSurface;

    connect(probe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));

    probe->setSource(player); // Returns true, hopefully.

    player->setVideoOutput(myVideoSurface);
    player->setMedia( QUrl::fromUserInput(videoSource, qApp->applicationDirPath()) );

    player->setPlaybackRate(0.1);

    player->play(); // Start receiving frames as they get presented to myVideoSurface

    //while(player->state() != QMediaPlayer::EndOfMedia ){QtDelay(20);}
    //
    // STATUS BAR
    //
    progBarUpdateLabel(" .mp4 to frames",0);

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(processEndOfPlayer(QMediaPlayer::MediaStatus)));


    return 1;
}

void MainWindow::processEndOfPlayer(QMediaPlayer::MediaStatus status)
{
    if( status == QMediaPlayer::EndOfMedia )
    {
        funcHideBarAndLabel();

        //
        //Show 25th frame (stabilized frame)
        int numFiles = funcAccountFilesInDir( _PATH_VIDEO_FRAMES );
        if( numFiles > 0 )
        {
            int idFrameToShow = ( numFiles > 25 )?25:1;
            QString imgFilename = _PATH_VIDEO_FRAMES + QString::number(idFrameToShow) + _FRAME_EXTENSION;
            QImage tmpImg(imgFilename);
            tmpImg.save( _PATH_DISPLAY_IMAGE );
            updateDisplayImage(&tmpImg);
        }
    }
}


void MainWindow::funcHideBarAndLabel()
{
    funcLabelProgBarHide();
    ui->progBar->setValue(0);
    ui->progBar->setVisible(false);
    ui->progBar->update();
}

void MainWindow::processFrame(QVideoFrame actualFrame)
{
    QImage img;
    QVideoFrame frame(actualFrame);  // make a copy we can call map (non-const) on
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
    // BUT the frame.pixelFormat() is QVideoFrame::Format_Jpeg, and this is
    // mapped to QImage::Format_Invalid by
    // QVideoFrame::imageFormatFromPixelFormat
    if (imageFormat != QImage::Format_Invalid) {
        img = QImage(frame.bits(),
                     frame.width(),
                     frame.height(),
                     // frame.bytesPerLine(),
                     imageFormat);
    } else {
        // e.g. JPEG
        int nbytes = frame.mappedBytes();
        img = QImage::fromData(frame.bits(), nbytes);
    }

    frame.unmap();
    numFrames++;

    if( img.save( _PATH_VIDEO_FRAMES + QString::number(numFrames) + _FRAME_EXTENSION ) )
    {
        //qDebug() << "numFrames saved: " << numFrames;
        int videoDuration = ui->slideTriggerTime->value();//seconds
        qint64 expectedFrames = round(videoDuration * 10 * 1.25);
        ui->progBar->setValue( round( ((float)numFrames/(float)expectedFrames) * 100.0 ) );
        ui->progBar->update();
    }
    else
    {
        qDebug() << "Error saving frame " << numFrames;
    }
}

int MainWindow::rectangleInPixelsFromSquareXML( QString fileName, squareAperture *rectangle )
{
    //Get original rectangle properties
    if( !funGetSquareXML( fileName, rectangle ) )
    {
        funcShowMsg("ERROR","Loading in pixels: "+fileName);
        return 0;
    }

    //Translate coordinates into pixels    
    rectangle->rectX    = round( ((float)rectangle->rectX / (float)rectangle->canvasW) * (float)camRes->width );
    rectangle->rectW    = round( ((float)rectangle->rectW / (float)rectangle->canvasW) * (float)camRes->width );
    rectangle->rectY    = round( ((float)rectangle->rectY / (float)rectangle->canvasH) * (float)camRes->height );
    rectangle->rectH    = round( ((float)rectangle->rectH / (float)rectangle->canvasH) * (float)camRes->height );

    rectangle->canvasW  = camRes->width;
    rectangle->canvasH  = camRes->height;

    return 1;
}

int MainWindow::rectangleInPixelsFromSquareXML( QString fileName, QString respectTo, squareAperture *rectangle )
{
    squareAperture *rectangleRespecTo = (squareAperture*)malloc(sizeof(squareAperture));

    //Get original rectangle properties
    if( !funGetSquareXML( fileName, rectangle ) )
    {
        funcShowMsg("ERROR","Loading in pixels: "+fileName);
        return 0;
    }

    //Get reference rectangle properties
    if( !rectangleInPixelsFromSquareXML( respectTo, rectangleRespecTo ) )
    {
        funcShowMsg("ERROR","Loading in pixels: "+respectTo);
        return 0;
    }

    //funcPrintRectangle("Area de interes",rectangleRespecTo);

    //Translate coordinates into pixels
    rectangle->rectX    = round( ((float)rectangle->rectX / (float)rectangle->canvasW) * (float)rectangleRespecTo->rectW );
    rectangle->rectW    = round( ((float)rectangle->rectW / (float)rectangle->canvasW) * (float)rectangleRespecTo->rectW );
    rectangle->rectY    = round( ((float)rectangle->rectY / (float)rectangle->canvasH) * (float)rectangleRespecTo->rectH );
    rectangle->rectH    = round( ((float)rectangle->rectH / (float)rectangle->canvasH) * (float)rectangleRespecTo->rectH );

    rectangle->canvasW  = rectangleRespecTo->rectW;
    rectangle->canvasH  = rectangleRespecTo->rectH;

    return 1;

}

int MainWindow::createSubimageRemotelly(bool squareArea )
{
    int status = 0;

    //
    // Get cam resolution
    //
    int camMP = (ui->radioRes5Mp->isChecked())?5:8;
    camRes = getCamRes(camMP);

    //
    //Getting calibration
    //..
    lstDoubleAxisCalibration daCalib;
    funcGetCalibration(&daCalib);

    //
    //Generates Camera Command
    //..
    std::string tmp;
    structSubimage* subimage    = (structSubimage*)malloc(sizeof(structSubimage));
    memset(subimage,'\0',sizeof(structSubimage));
    subimage->idMsg             = (unsigned char)5;
    if( squareArea == true )
    {
        subimage->frame.canvasW     = daCalib.W;
        subimage->frame.canvasH     = daCalib.H;
        subimage->frame.rectX       = round( daCalib.squareX * (double)camRes->width );
        subimage->frame.rectY       = round( daCalib.squareY * (double)camRes->height );
        subimage->frame.rectW       = round( daCalib.squareW * (double)camRes->width );
        subimage->frame.rectH       = round( daCalib.squareH * (double)camRes->height );
    }
    else
    {
        subimage->frame.canvasW     = daCalib.W;
        subimage->frame.canvasH     = daCalib.H;
        subimage->frame.rectX       = round( daCalib.bigX * (double)camRes->width );
        subimage->frame.rectY       = round( daCalib.bigY * (double)camRes->height );
        subimage->frame.rectW       = round( daCalib.bigW * (double)camRes->width );
        subimage->frame.rectH       = round( daCalib.bigH * (double)camRes->height );
    }

    tmp.assign(_PATH_REMOTE_SNAPSHOT);
    memcpy( subimage->fileName, tmp.c_str(), tmp.length() );

    //
    // Send Command and Wait for ACK
    //

    //Open socket
    int n;
    int sockfd = connectSocket( camSelected );
    unsigned char bufferRead[frameBodyLen];
    qDebug() << "Socket opened";
    n = ::write(sockfd,subimage,sizeof(structSubimage));
    qDebug() << "Img request sent";

    //Receive ACK with the camera status
    memset(bufferRead,'\0',3);
    n = read(sockfd,bufferRead,2);

    if( bufferRead[1] == 1 )
    {
        qDebug() << "Image cropped remotelly";
        status = 1;
    }
    else
    {
        qDebug() << "ERROR cropping image";
        status = 0;
    }
    n=n;
    ::close(sockfd);
    delete[] subimage;

    return status;
}

int MainWindow::takeRemoteSnapshot( QString fileDestiny, bool squareArea )
{

    //
    // Get cam resolution
    //
    int camMP = (ui->radioRes5Mp->isChecked())?5:8;
    camRes = getCamRes(camMP);

    //
    //Getting calibration
    //..
    lstDoubleAxisCalibration daCalib;
    funcGetCalibration(&daCalib);

    //Save lastest settings
    if( saveRaspCamSettings( _PATH_LAST_SNAPPATH ) == false ){
        funcShowMsg("ERROR","Saving last snap-settings");
        return -1;
    }


    //
    //Generates Camera Command
    //..
    structRaspistillCommand* structRaspiCommand = (structRaspistillCommand*)malloc(sizeof(structRaspistillCommand));
    strReqImg *reqImg                           = (strReqImg*)malloc(sizeof(strReqImg));
    memset(reqImg,'\0',sizeof(strReqImg));
    memset(structRaspiCommand,'\0',sizeof(structRaspistillCommand));
    structRaspiCommand->idMsg                   = (unsigned char)4;
    reqImg->squApert                            = squareArea;
    reqImg->raspSett                            = funcFillSnapshotSettings( reqImg->raspSett );
    reqImg->imgCols                             = camRes->width;//2592 | 640
    reqImg->imgRows                             = camRes->height;//1944 | 480
    reqImg->raspSett.Flipped                    = (ui->cbFlipped->isChecked())?1:0;

    //--------------------------------------
    //Create Command
    //--------------------------------------
    QString tmpCommand;
    tmpCommand = genCommand(reqImg, fileDestiny.toStdString())->c_str();

    //--------------------------------------
    //Take Remote Photo
    //--------------------------------------
    bool executedCommand;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&executedCommand);
    if( !executedCommand )
    {
        funcShowMsgERROR_Timeout("Applying Remote Snapshot Command",this);
        return -1;
    }

    /*
    progBarUpdateLabel("Stabilizing Remote Camera...",0);
    progBarTimer((ui->slideTriggerTime->value()+1)*1000);
    progBarUpdateLabel("",0);*/

    //Get Remote File
    //obtainFile( _PATH_REMOTE_SNAPSHOT, _PATH_IMAGE_RECEIVED );

    /*
    //
    //Generates Camera Command
    //..
    std::string tmp;
    structRaspistillCommand* structRaspiCommand = (structRaspistillCommand*)malloc(sizeof(structRaspistillCommand));
    strReqImg *reqImg                           = (strReqImg*)malloc(sizeof(strReqImg));
    memset(reqImg,'\0',sizeof(strReqImg));
    memset(structRaspiCommand,'\0',sizeof(structRaspistillCommand));
    structRaspiCommand->idMsg                   = (unsigned char)4;
    reqImg->squApert                            = squareArea;
    reqImg->raspSett                            = funcFillSnapshotSettings( reqImg->raspSett );
    reqImg->imgCols                             = camRes->width;//2592 | 640
    reqImg->imgRows                             = camRes->height;//1944 | 480

    tmp.assign(_PATH_REMOTE_SNAPSHOT);
    memcpy( structRaspiCommand->fileName, tmp.c_str(), tmp.length() );
    tmp = genCommand(reqImg, _PATH_REMOTE_SNAPSHOT)->c_str();
    memcpy( structRaspiCommand->raspiCommand, tmp.c_str(), tmp.length() );
    qDebug() << structRaspiCommand->raspiCommand;    



    //
    // Send Command and Wait for ACK
    //

    //Open socket
    int n;
    int sockfd = connectSocket( camSelected );
    unsigned char bufferRead[frameBodyLen];
    qDebug() << "Socket opened";
    n = ::write(sockfd,structRaspiCommand,sizeof(structRaspistillCommand));
    qDebug() << "Img request sent";

    //Show progbar
    progBarUpdateLabel("Stabilizing camera",0);
    progBarTimer( (ui->slideTriggerTime->value() + 1) * 1000 );
    ui->progBar->setValue(0);
    ui->progBar->setVisible(true);
    ui->progBar->update();
    progBarUpdateLabel("Transferring image",0);


    //Receive ACK with the camera status
    memset(bufferRead,'\0',3);
    n = read(sockfd,bufferRead,2);

    if( bufferRead[1] == 1 )
    {
        qDebug() << "Image taked remotelly";
        status = 1;
    }
    else
    {
        qDebug() << "ERROR taking image";
        status = 0;
    }
    n=n;

    ::close(sockfd);
    delete[] structRaspiCommand;
    delete[] reqImg;
    */

    return 1;
}

void MainWindow::on_pbSnapshotSquare_clicked()
{

    mouseCursorWait();

    if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,false) )
    {
        qDebug() << "ERROR: Taking Diffration Area";
        return (void)NULL;
    }
    else
    {
        QImage diffImage = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
        if( diffImage.isNull() )
        {
            qDebug() << "ERROR: Obtaining Diffration Area";
            return (void)NULL;
        }
        else
        {
            if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,true) )
            {
                qDebug() << "ERROR: Taking Aperture Area";
                return (void)NULL;
            }
            else
            {
                QImage apertureImage = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
                if( apertureImage.isNull() )
                {
                    qDebug() << "ERROR: Obtaining Aperture Area";
                    return (void)NULL;
                }
                else
                {
                    //-------------------------------------------
                    //Merge image
                    //-------------------------------------------

                    squareAperture *aperture = (squareAperture*)malloc(sizeof(squareAperture));

                    //
                    //Crop original image to release the usable area
                    //
                    //Get usable area coordinates
                    memset(aperture,'\0',sizeof(squareAperture));
                    if( !rectangleInPixelsFromSquareXML( _PATH_REGION_OF_INTERES2, aperture ) )
                    {
                        funcShowMsg("ERROR","Loading Usable Area in Pixels: _PATH_SQUARE_USABLE");
                        return (void)false;
                    }
                    diffImage       = diffImage.copy(QRect( aperture->rectX, aperture->rectY, aperture->rectW, aperture->rectH ));
                    apertureImage   = apertureImage.copy(QRect( aperture->rectX, aperture->rectY, aperture->rectW, aperture->rectH ));

                    //
                    //Get square aperture coordinates
                    //
                    memset(aperture,'\0',sizeof(squareAperture));
                    if( !rectangleInPixelsFromSquareXML( _PATH_SQUARE_APERTURE2, _PATH_REGION_OF_INTERES2, aperture ) )
                    {
                        funcShowMsg("ERROR","Loading Rectangle in Pixels: _PATH_SQUARE_APERTURE");
                        return (void)false;
                    }
                    funcPrintRectangle( "Square Aperture", aperture );

                    //
                    //Copy square aperture into diffraction image
                    //
                    for( int y=aperture->rectY; y<=(aperture->rectY+aperture->rectH); y++ )
                    {
                        for( int x=aperture->rectX; x<=(aperture->rectX+aperture->rectW); x++ )
                        {
                            diffImage.setPixel( x, y, apertureImage.pixel( x, y ) );
                        }
                    }



                    //
                    //Save cropped image
                    //
                    diffImage.save(_PATH_DISPLAY_IMAGE);
                    updateDisplayImage(&diffImage);
                }
            }
        }
    }

    funcResetStatusBar();
    mouseCursorReset();
}

void MainWindow::on_pbSaveImage_clicked()
{
    //
    //Read the filename
    //
    QString fileName;
    QString lastPath = readFileParam(_PATH_LAST_IMG_SAVED);
    if( lastPath.isEmpty() )//First time using this parameter
    {
        lastPath = "./snapshots/";
    }
    fileName = QFileDialog::getSaveFileName(
                                                this,
                                                tr("Save Snapshot as..."),
                                                lastPath,
                                                tr("Documents (*.png)")
                                            );
    if( fileName.isEmpty() )
    {
        qDebug() << "Filename not typed";
        return (void)false;
    }
    else
    {
        lastPath = funcRemoveFileNameFromPath(fileName);
        saveFile(_PATH_LAST_IMG_SAVED,lastPath);
    }

    //
    //Validate filename
    //
    fileName = funcRemoveImageExtension(fileName);

    //
    //Save image
    //
    mouseCursorWait();
    globalEditImg->save(fileName);
    mouseCursorReset();

}

void MainWindow::on_pbOneShotSnapshot_clicked()
{
    mouseCursorWait();    

    if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,false) )
    {
        qDebug() << "ERROR: Taking Diffration Area";
        return (void)NULL;
    }
    else
    {
        QImage diffImage = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
        if( diffImage.isNull() )
        {
            qDebug() << "ERROR: Obtaining Diffration Area";
            return (void)NULL;
        }
        else
        {
            //
            //Crop original image to release the usable area
            //
            //Get usable area coordinates
            squareAperture *aperture = (squareAperture*)malloc(sizeof(squareAperture));
            memset(aperture,'\0',sizeof(squareAperture));
            if( !rectangleInPixelsFromSquareXML( _PATH_REGION_OF_INTERES2, aperture ) )
            {
                funcShowMsg("ERROR","Loading Usable Area in Pixels: _PATH_REGION_OF_INTERES");
                return (void)false;
            }
            //Crop and save
            diffImage = diffImage.copy(QRect( aperture->rectX, aperture->rectY, aperture->rectW, aperture->rectH ));
            diffImage.save(_PATH_DISPLAY_IMAGE);
            qDebug() << "Images saved";

            updateDisplayImage(&diffImage);
        }
    }

    funcResetStatusBar();

    mouseCursorReset();
}

void MainWindow::funcResetStatusBar()
{
    ui->progBar->setValue(0);
    ui->progBar->setVisible(false);
    ui->labelProgBar->setText("");
    ui->progBar->update();
    ui->labelProgBar->update();
}

void MainWindow::on_actionsquareSettings_triggered()
{
    formSquareApertureSettings* squareSettings = new formSquareApertureSettings(this);
    squareSettings->setModal(true);
    squareSettings->show();
}

void MainWindow::on_pbSelectFolder_clicked()
{
    int l, r, c;
    //---------------------------------------
    //Get directory
    //---------------------------------------
    QString pathSource = funcShowSelDir(_PATH_TMP_HYPCUBES);
    if( pathSource.isNull() )
    {
        qDebug() << "Dir not selected";
        return (void)false;
    }

    //---------------------------------------
    //List files in directory
    //---------------------------------------
    lstImages = funcListFilesInDir(pathSource);
    if( lstImages.size() == 0 )
    {
        funcShowMsg("ERROR","Invalid directory");
        return (void)false;
    }

    //---------------------------------------
    //Validate imagery in Dir
    //---------------------------------------
    QImage tmpImg(lstImages.at(0).absoluteFilePath());
    int W, H, L;
    W = tmpImg.width();
    H = tmpImg.height();
    L = lstImages.size();
    for( l=1; l<L; l++ )
    {
        tmpImg = QImage(lstImages.at(l).absoluteFilePath());
        if( tmpImg.width() != W || tmpImg.height() != H )
        {
            funcShowMsg("ERROR","Dir selected contains image with different size");
            return (void)false;
        }
    }

    //---------------------------------------
    //Create Cube from HDD
    //---------------------------------------
    tmpHypercube = (int***)funcAllocInteger3DMatrixMemo( H, W, L, tmpHypercube );
    for( l=0; l<L; l++ )
    {
        tmpImg = QImage(lstImages.at(l).absoluteFilePath());
        for( r=0; r<H; r++ )
        {
            for( c=0; c<W; c++ )
            {
                tmpHypercube[r][c][l]    = qRed(tmpImg.pixel(c,r));
            }
        }
    }

    //---------------------------------------
    //Display first photo
    //---------------------------------------
    if( graphViewSmall == NULL )
    {
        graphViewSmall = new GraphicsView(this);
        connect( graphViewSmall, SIGNAL( signalMouseMove(QMouseEvent*) ),this, SLOT( funcMouseMoveReaction(QMouseEvent*) ));
        graphViewSmall->resize(tmpImg.width(),tmpImg.height());
        QLayout *layout = new QVBoxLayout();
        layout->addWidget(graphViewSmall);
        layout->setEnabled(false);
        ui->tabShowPixels->setLayout(layout);
        graphViewSmall->move(20,50);
    }
    else
    {
        graphViewSmall->scene()->clear();
        qDebug() << "Removiendo";
    }
    funcLoadImageIntoGaphView(graphViewSmall,lstImages.at(0).absoluteFilePath());


    //---------------------------------------
    //Enable slide bar
    //---------------------------------------
    //funcLoadImageIntoGaphView(ui->gaphViewImg,lstImages.at(0).absoluteFilePath());
    ui->slideChangeImage->setMinimum(1);
    ui->slideChangeImage->setToolTip(QString::number(1));
    ui->slideChangeImage->setMaximum(lstImages.size());
    ui->slideChangeImage->setMaximumWidth( tmpImg.width() );    
    ui->labelCubeImageName->setText( "(1/" + QString::number(lstImages.size()) + ") " + lstImages.at(0).fileName() );
    ui->slideChangeImage->setEnabled(true);
    ui->graphViewPlot->move(20,tmpImg.height()+115);
    ui->labelCubeImageName->move(20,tmpImg.height()+55);
    ui->slideChangeImage->move(20,tmpImg.height()+85);

    //---------------------------------------
    //Draw plot limits
    //---------------------------------------
    funcDrawPlotLimits();

}

void MainWindow::funcDrawSpectralPixelIntoSmall(int x, int y)
{
    /*
    const int frameX    = 40;       //left and right
    const int frameY    = 30;       //up and down
    const int rangeInit = 300;      //Wavelength(nm) at the origin
    const int rangeEnd  = 1100;     //Wavelength(nm) max limit
    const int rangeStep = 50;       //Wavelength(nm) between slides*/

    int l;
    qreal W, H, x1, y1, x2, y2, wavelength1, wavelength2, energy1, energy2;

    //------------------------------------------
    //Initialize canvas
    //------------------------------------------
    graphViewSmall->scene()->clear();
    funcDrawPlotLimits();

    //------------------------------------------
    //Prepare variables
    //------------------------------------------
    W = ui->graphViewPlot->width();
    H = ui->graphViewPlot->height();
    const float pixelsByNm      = (float)(W - (2*frameX)) /
                                  (float)(rangeEnd-rangeInit);
    const float energyRange     = (float)(H - (2*frameY));



    //------------------------------------------
    //Plot Spectral Pixel Componnets
    //------------------------------------------
    QPen penLine(Qt::magenta,1);
    QPen penEllipse(Qt::magenta,1);
    QBrush brushEllipse(Qt::magenta);
    for( l=1; l<lstImages.size(); l++ )
    {
        //Calculate coordinates
        wavelength1 = lstImages.at(l-1).completeBaseName().toFloat(0) - (float)rangeInit;
        wavelength2 = lstImages.at(l).completeBaseName().toFloat(0) - (float)rangeInit;
        energy1     = ((float)tmpHypercube[y][x][l-1] / 255.0) * energyRange;
        energy2     = ((float)tmpHypercube[y][x][l] / 255.0) * energyRange;
        x1          = (float)frameX + ( pixelsByNm * wavelength1 );
        y1          = H - (float)frameY - energy1;
        x2          = (float)frameX + ( pixelsByNm * wavelength2 );
        y2          = H - (float)frameY - energy2;
        //Plot Line
        QLineF line(x1,y1,x2,y2);
        ui->graphViewPlot->scene()->addLine(line,penLine);
        //Plot Ellipse
        ui->graphViewPlot->scene()->addEllipse(x1,y1-3,5,5,penEllipse,brushEllipse);
        ui->graphViewPlot->scene()->addEllipse(x2,y2-3,5,5,penEllipse,brushEllipse);
    }
}

void MainWindow::funcMouseMoveReaction(QMouseEvent* e)
{
    int x, y, W, H;
    W = graphViewSmall->width();
    H = graphViewSmall->height();
    x = (e->x()>=0)?e->x():0;
    y = (e->y()>=0)?e->y():0;
    x = (x<W)?x:W;
    y = (y<H)?y:H;
    funcDrawSpectralPixelIntoSmall(x,y);
}

void MainWindow::funcLoadImageIntoGaphView( QGraphicsView* canvas, QString filePath )
{
    QPixmap pixMap(filePath);
    QGraphicsScene *scene = new QGraphicsScene(0,0,pixMap.width(),pixMap.height());
    scene->setBackgroundBrush(QBrush(Qt::black));
    canvas->setBackgroundBrush(pixMap);
    canvas->setScene(scene);
    canvas->resize(pixMap.width(),pixMap.height());
    canvas->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvas->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    canvas->update();
}

void MainWindow::on_slideChangeImage_valueChanged(int value)
{
    ui->slideChangeImage->setToolTip( QString::number(value) );    
    ui->labelCubeImageName->setText( "("+ QString::number(value) +"/" + QString::number(lstImages.size()) + ") " + lstImages.at(value-1).fileName() );
    funcLoadImageIntoGaphView(graphViewSmall,lstImages.at(value-1).absoluteFilePath());
}

void MainWindow::funcDrawPlotLimits()
{
    //Set scene
    int i, W, H;
    W = ui->graphViewPlot->width();
    H = ui->graphViewPlot->height();
    QGraphicsScene *scene = new QGraphicsScene(0,0,W,H);
    scene->setBackgroundBrush(QBrush(Qt::black));
    ui->graphViewPlot->setScene(scene);
    ui->graphViewPlot->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->graphViewPlot->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    //Prepare font
    QFont font;
    font.setPixelSize(12);
    font.setBold(false);
    font.setFamily("Calibri");

    //Set axis
    //int range, rangeStep, rangeInit, rangeEnd; //wavelength range
    //int frameX, frameY;
    int range = rangeEnd - rangeInit;


    QPen pen(Qt::white,1.5);
    QPen penTxt(Qt::white,1);
    QLineF lineX(0,H-frameY,W,H-frameY);
    scene->addLine(lineX,pen);
    QLineF lineY(frameX,0,frameX,H);
    scene->addLine(lineY,pen);

    //Set slides X
    qreal frameW, frameH, x1, y1, x2, y2, numLines, spaceBetwLines;
    frameW = W - (frameX*2);
    frameH = H - (frameY*2);
    int lineLen = 6;
    numLines = floor((float)range / (float)rangeStep);
    spaceBetwLines = (float)frameW / (float)numLines;
    for( i=0; i<=numLines; i++ )
    {
        //Line
        x1 = frameX + (i*spaceBetwLines);
        y1 = frameH + frameY - lineLen;
        x2 = x1;
        y2 = y1 + (2*lineLen);
        QLineF line(x1,y1,x2,y2);
        scene->addLine(line,pen);
        //Text
        QGraphicsSimpleTextItem* tmpTxt = new QGraphicsSimpleTextItem(QString::number(rangeInit+(i*rangeStep)) + QString::fromLatin1(" nm"));
        tmpTxt->setPos(x2-15,y2+5);
        tmpTxt->setFont(font);
        tmpTxt->setPen(penTxt);
        scene->addItem(tmpTxt);
    }

    //Set slides Y
    qreal rangeStepY;
    rangeStepY = 0.2;
    numLines = floor(1/rangeStepY);
    spaceBetwLines = (float)frameH / (float)numLines;
    for( i=1; i<=numLines; i++ )
    {
        //Line
        x1 = frameX - lineLen;
        y1 = H - frameY - (i*spaceBetwLines);
        x2 = x1 + (2*lineLen);
        y2 = y1;
        QLineF line(x1,y1,x2,y2);
        scene->addLine(line,pen);
        //Text
        QGraphicsSimpleTextItem* tmpTxt = new QGraphicsSimpleTextItem(QString::number(i*rangeStepY));
        tmpTxt->setPos(x1-25,y1-8);
        tmpTxt->setFont(font);
        tmpTxt->setPen(penTxt);
        scene->addItem(tmpTxt);
    }



}

void MainWindow::on_actionvideoToFrames_triggered()
{
    //-------------------------------------------------
    //Select video
    //-------------------------------------------------
    auxQstring = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select video..."),
                                                        _PATH_VIDEO_FRAMES,
                                                        "(*.mp4);;"
                                                     );
    if( auxQstring.isEmpty() )
    {
        return (void)NULL;
    }

    //-------------------------------------------------
    //Esport Video
    //-------------------------------------------------
    funcVideoToFrames(auxQstring);

}

void MainWindow::on_actionframesToCube_triggered()
{
    int sourceW         = 15;   //Diffraction width
    int waveResolution  = 50.0; //Spectral Resolution
    double minWave      = 430.0;//Spectral sensitivity
    double maxWave      = 750.0;//Spectral sensitivity

    int row, l;

    mouseCursorWait();

    //---------------------------------------
    //Get directory
    //---------------------------------------
    QString pathSource = funcShowSelDir(_PATH_VIDEO_FRAMES);
    if( pathSource.isNull() )
    {
        qDebug() << "Dir not selected";
        mouseCursorReset();
        return (void)false;
    }

    //---------------------------------------
    //List files in directory
    //---------------------------------------
    lstImages = funcListFilesInDir(pathSource);
    if( lstImages.size() == 0 )
    {
        funcShowMsg("ERROR","Invalid directory");
        mouseCursorReset();
        return (void)false;
    }

    //---------------------------------------
    //Validate imagery in Dir
    //---------------------------------------
    QImage tmpImg(lstImages.at(0).absoluteFilePath());
    int W, H, numSlides;
    W           = tmpImg.width();
    H           = tmpImg.height();
    numSlides   = lstImages.size();
    for( l=1; l<numSlides; l++ )
    {
        tmpImg = QImage(lstImages.at(l).absoluteFilePath());
        if( tmpImg.width() != W || tmpImg.height() != H )
        {
            qDebug() << "W: " << W << " H: " << H << " image: " << lstImages.at(l).absoluteFilePath();
            funcShowMsg("ERROR","Dir selected contains image with different size");
            mouseCursorReset();
            return (void)false;
        }
    }

    //---------------------------------------
    //1) Calculates aberration (main curve)
    //2) Define wavelength linear regression
    //3) Define Source Area
    //4) Define Rotation
    //---------------------------------------
    QList<int> mainCurveLeft, mainCurveRight;
    double aux;
    double fitLeft2, fitLeft1, fitLeft0;
    double fitRight2, fitRight1, fitRight0;
    fitLeft2 = -0.000086713; fitLeft1  = 0.099027;  fitLeft0  = 950.78;
    fitRight2 = 0.00014989;  fitRight1 = -0.16814; fitRight0 = 989.78;
    for( row=1; row<=tmpImg.height(); row++ )
    {
        //Left curve
        aux = ((double)row*(double)row*fitLeft2) + ((double)row*fitLeft1) + fitLeft0;
        mainCurveLeft.append( aux );
        //Right curve
        aux = ((double)row*(double)row*fitRight2) + ((double)row*fitRight1) + fitRight0;
        mainCurveRight.append( aux );
    }
    //Wavelegth linear regression
    double fitWave1, fitWave0;
    fitWave1 = 0.85972; fitWave0 = -23.49514;
    //Define source area
    int sourceH;
    int sourceY1, sourceY2;
    //sourceX1 = 966;
    sourceY1 = 200;
    sourceY2 = 600;//988
    //sourceX1 = 966;         sourceY1 = 400;
    //sourceX2 = 966+sourceW; sourceY2 = 700;//988
    sourceH  = sourceY2 -sourceY1;
    //Define Rotation
    double imgRotation;
    imgRotation = 0.8;

    //---------------------------------------
    //Creates Final Image Container
    //---------------------------------------
    int frankiImgW, frankiImgH;
    int L;
    frankiImgW = sourceW * lstImages.size();
    frankiImgH = sourceY2 - sourceY1;
    //QImage frankyImage(frankiImgW,frankiImgH,QImage::Format_RGB32);

    //---------------------------------------
    //Test: Draw RGB Lines Into Image
    //---------------------------------------
    if( 0 )
    {
        int testX, testY, testWaveDistance, offsetLeft, offsetRight;
        double waveR, waveG, waveB;
        waveR = 618.0;
        waveG = 548.0;
        waveB = 438.0;
        offsetLeft = 0;
        offsetRight = 0;
        QImage testImg = funcRotateImage( _PATH_DISPLAY_IMAGE, imgRotation );
        //Red
        testWaveDistance = (waveR*fitWave1)+fitWave0;
        for( testY=sourceY1; testY<sourceY2; testY++ )
        {
            //Right
            testX = mainCurveRight.at(testY) + testWaveDistance + offsetRight;
            testImg.setPixel(testX,testY,qRgb(255,0,0));
            //Left
            testX = mainCurveLeft.at(testY) - testWaveDistance - offsetLeft;
            testImg.setPixel(testX,testY,qRgb(255,0,0));
        }
        //Green
        testWaveDistance = (waveG*fitWave1)+fitWave0;
        for( testY=sourceY1; testY<sourceY2; testY++ )
        {
            //Right
            testX = mainCurveRight.at(testY) + testWaveDistance + offsetRight;
            testImg.setPixel(testX,testY,qRgb(0,255,0));
            //Left
            testX = mainCurveLeft.at(testY) - testWaveDistance - offsetLeft;
            testImg.setPixel(testX,testY,qRgb(0,255,0));
        }
        //Blue
        testWaveDistance = (waveB*fitWave1)+fitWave0;
        for( testY=sourceY1; testY<sourceY2; testY++ )
        {
            //Right
            testX = mainCurveRight.at(testY) + testWaveDistance + offsetRight;
            testImg.setPixel(testX,testY,qRgb(0,0,255));
            //Left
            testX = mainCurveLeft.at(testY) - testWaveDistance - offsetLeft;
            testImg.setPixel(testX,testY,qRgb(0,0,255));
        }
        testImg.save(_PATH_AUX_IMG);
    }

    //---------------------------------------
    //Define Spectral Resolution
    //---------------------------------------
    double tmpWave;

    //---------------------------------------
    //Fill each wavelength into frankiImage
    //---------------------------------------

    //.......................................
    //Creates one frankiImage for each
    //wavelength
    //.......................................
    funcClearDirFolder(_PATH_TMP_HYPCUBES);
    L = round((maxWave-minWave)/waveResolution);
    //qDebug() << "frankiImgH: " << frankiImgH << " frankiImgW: " << frankiImgW << " frankiImgL: " << frankiImgL;
    //int hypFranki[frankiImgH][frankiImgW][frankiImgL];
    //qDebug() << "frankiImgH: " << frankiImgH << " frankiImgW: " << frankiImgW << " frankiImgL: " << frankiImgL;
    int*** hypFranki;
    hypFranki = (int***)funcAllocInteger3DMatrixMemo( frankiImgH/*Rows*/, frankiImgW/*Cols*/, L, hypFranki );
    //qDebug() << "Después... frankiImgH: " << frankiImgH << " frankiImgW: " << frankiImgW << " L: " << L;

    //.......................................
    //List frankiImagery
    //.......................................
    QList<int> colorSensed;
    QRgb tmpPixelLelft,tmpPixelRight;
    int waveDistance, frankiID, frankiX, frankiY, frankiSlideXInit;
    int diffFrameY, tmpDiffFrameXL, tmpDiffFrameXR;
    int sourceX, sourceY;
    QList<QFileInfo> lstFrankiFrames = funcListFilesInDir(_PATH_VIDEO_FRAMES);
    if( lstFrankiFrames.size() == 0 )
    {
        funcShowMsg("ERROR","Invalid frankiDirectory");
        return (void)false;
    }

    for( frankiID=0; frankiID<lstFrankiFrames.size(); frankiID++ )
    {
        //Obtain frame
        QImage tmpImg = funcRotateImage( lstFrankiFrames.at(frankiID).filePath(), imgRotation );
        //Calculates initial position for this slide
        for(l=0; l<L; l++)
        {
            //Define x position in frankiImage
            frankiSlideXInit = frankiID*sourceW;
            //Define wavelength
            tmpWave      = minWave + (l*waveResolution);
            //Define distance respect to the source for that wavelength
            waveDistance = (tmpWave*fitWave1)+fitWave0;
            //qDebug() << "l: " << l << "waveDistance: " << waveDistance;
            for(sourceX=0; sourceX<sourceW; sourceX++)
            {
                frankiX = frankiSlideXInit + sourceX;
                for(sourceY=0; sourceY<sourceH; sourceY++)
                {
                    //Diffraction Y position
                    diffFrameY      = sourceY + sourceY1;
                    //Calculate position for diffraction wavelength in original image
                    tmpDiffFrameXL  = mainCurveLeft.at(diffFrameY) - waveDistance;//Left
                    tmpDiffFrameXR  = mainCurveRight.at(diffFrameY) + waveDistance;//Right
                    if(tmpDiffFrameXL<0 || tmpDiffFrameXR>=tmpImg.width() )
                    {
                        qDebug() << "Wrong coordinates: tmpDiffFrameXL: " << tmpDiffFrameXL << " tmpDiffFrameXR: " << tmpDiffFrameXR;
                        mouseCursorReset();
                        exit(0);
                    }


                    //Obtain the maximum value for diffraction on left and
                    //right in order to avoid emission light angle
                    if( tmpImg.pixel(tmpDiffFrameXL,diffFrameY) )
                    tmpPixelLelft = tmpImg.pixel(tmpDiffFrameXL,diffFrameY);
                    tmpPixelRight = tmpImg.pixel(tmpDiffFrameXR,diffFrameY);
                    colorSensed.clear();
                    colorSensed.append(qRed(tmpPixelLelft));
                    colorSensed.append(qGreen(tmpPixelLelft));
                    colorSensed.append(qBlue(tmpPixelLelft));
                    //colorSensed.append(qRed(tmpPixelRight));
                    //colorSensed.append(qGreen(tmpPixelRight));
                    //colorSensed.append(qBlue(tmpPixelRight));
                    //Save value
                    frankiY = sourceY;
                    //qDebug() << "(" << frankiX << "," << frankiY << "): " << *std::max_element(colorSensed.begin(),colorSensed.end());
                    hypFranki[frankiY][frankiX][l] = *std::max_element(colorSensed.begin(),colorSensed.end());
                }
            }
        }
    }
    tmpPixelRight = tmpPixelRight;

    //.......................................
    //Exports layers in the cube created
    //Each layer is a gray-scale image
    //.......................................
    //qDebug() << "Exportando... L: " << L;
    for(l=0; l<L; l++)
    {
        //Define wavelength
        tmpWave      = minWave + (l*waveResolution);
        //Copy values into a new gray-scale image
        QImage imgFromCube(frankiImgW,frankiImgH,QImage::Format_Grayscale8);
        for(sourceX=0; sourceX<sourceW; sourceX++)
        {
            for(sourceY=0; sourceY<sourceH; sourceY++)
            {
                imgFromCube.setPixel(sourceX,sourceY,hypFranki[sourceY][sourceX][l]);
            }
        }
        //qDebug() << _PATH_TMP_HYPCUBES + QString::number(tmpWave) + ".png";
        imgFromCube.save( _PATH_TMP_HYPCUBES + QString::number(tmpWave) + ".png" );
    }

    mouseCursorReset();

}

void MainWindow::on_pbTimeLapse_clicked()
{

    bool ok;
    QString tmpCommand;

    //Get Local Sync Folder
    QString syncFolder;
    syncFolder = funcGetSyncFolder();

    //--------------------------------------
    //Clear Temporal Remote Directory
    //--------------------------------------

    //CREATE Temporal Remote Directory
    tmpCommand.clear();
    tmpCommand.append("mkdir ");
    tmpCommand.append(_PATH_REMOTE_FOLDER_TEST_SLIDE);
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&ok);
    qDebug() << "tmpCommand: " << tmpCommand;
    if( !ok )
    {
        funcShowMsgERROR_Timeout("Clearing Remote Folder",this);
        return (void)false;
    }

    //Clear Folder
    tmpCommand.clear();
    tmpCommand.append("sudo rm ");
    tmpCommand.append(_PATH_REMOTE_FOLDER_TEST_SLIDE);
    tmpCommand.append("*");    
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&ok);
    if( !ok )
    {
        funcShowMsgERROR_Timeout("Clearing Remote Folder",this);
        return (void)false;
    }

    //--------------------------------------
    //Clear Temporal Local Folder
    //--------------------------------------
    QString tmpLocalFolder;
    tmpLocalFolder.clear();
    tmpLocalFolder.append(syncFolder);
    tmpLocalFolder.append(_PATH_LOCAL_FOLDER_SLIDELAPSE);
    tmpLocalFolder.append(_PATH_LOCAL_FOLDER_TEST_SLIDE);
    if( fileExists(tmpLocalFolder) )
    {
        tmpCommand.clear();
        tmpCommand.append("rm -R ");
        tmpCommand.append(tmpLocalFolder);
        tmpCommand.append("*");
        funcExecuteCommand(tmpCommand.toStdString().data());
        qDebug() << "tmpCommand: " << tmpCommand;
    }
    else
    {
        //Clear Local Folder's Parent
        tmpLocalFolder.clear();
        tmpLocalFolder.append(syncFolder);
        tmpLocalFolder.append(_PATH_LOCAL_FOLDER_SLIDELAPSE);
        qDebug() << "Checking if exists: " << tmpCommand;
        if( !fileExists(tmpLocalFolder) )
        {
            //Create Folder's Parent
            tmpCommand.clear();
            tmpCommand.append("mkdir ");
            tmpCommand.append(tmpLocalFolder);
            funcExecuteCommand(tmpCommand.toStdString().data());
            qDebug() << "tmpCommand: " << tmpCommand;

            //Create Local Folder
            tmpCommand.clear();
            tmpCommand.append("mkdir ");
            tmpCommand.append(syncFolder);
            tmpCommand.append(_PATH_LOCAL_FOLDER_SLIDELAPSE);
            tmpCommand.append(_PATH_LOCAL_FOLDER_TEST_SLIDE);
            funcExecuteCommand(tmpCommand.toStdString().data());
            qDebug() << "tmpCommand: " << tmpCommand;

        }
        else
        {
            //Create Local Folder
            tmpCommand.clear();
            tmpCommand.append("mkdir ");
            tmpCommand.append(syncFolder);
            tmpCommand.append(_PATH_LOCAL_FOLDER_SLIDELAPSE);
            tmpCommand.append(_PATH_LOCAL_FOLDER_TEST_SLIDE);
            funcExecuteCommand(tmpCommand.toStdString().data());
            qDebug() << "tmpCommand: " << tmpCommand;
        }
    }

    //--------------------------------------
    //Take Timelapse
    //--------------------------------------
    tmpCommand = genTimelapseCommand(_PATH_REMOTE_FOLDER_TEST_SLIDE);
    qDebug() << "tmpCommand: " << tmpCommand;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&ok);
    progBarUpdateLabel("Timelapsing...",0);
    progBarTimer((ui->slideTriggerTime->value()+1)*1000);    

    //--------------------------------------
    //Obtain Remote Folder
    //--------------------------------------

    //Local Folder Path
    tmpLocalFolder.clear();
    tmpLocalFolder.append(syncFolder);
    tmpLocalFolder.append(_PATH_LOCAL_FOLDER_SLIDELAPSE);
    tmpLocalFolder.append(_PATH_LOCAL_FOLDER_TEST_SLIDE);

    //Tmp
    QString errorTrans, errorDel;
    int status;
    status = obtainRemoteFolder( _PATH_REMOTE_FOLDER_TEST_SLIDE, tmpLocalFolder, &errorTrans, &errorDel );
    if( status == _ERROR )
    {
        funcShowMsgERROR_Timeout("Obtaining Remote Slides",this);
        return (void)false;
    }

    //--------------------------------------
    //Update Image Displayed
    //--------------------------------------
    funcUpdateImageFromFolder(tmpLocalFolder,_SNAPSHOT_REMOTE_EXTENSION);
    funcResetStatusBar();

}

QString MainWindow::genRemoteVideoCommand(QString remoteVideo,bool ROI)
{
    QString tmpCommand;
    tmpCommand.append("raspivid -n -t ");
    tmpCommand.append( QString::number( ui->spinBoxVideoDuration->value() * 1000 ) );
    //tmpCommand.append( " -vf -b 50000000 -fps " );
    tmpCommand.append( " -b 50000000 -fps " );
    tmpCommand.append( QString::number(_VIDEO_FRAME_RATE) );
    tmpCommand.append( " -o " );
    tmpCommand.append( remoteVideo );

    //.................................
    //Diffraction Area ROI
    //.................................
    if( ROI == true )
    {
        double W, H;
        squareAperture *aperture = (squareAperture*)malloc(sizeof(squareAperture));
        memset(aperture,'\0',sizeof(squareAperture));
        if( !funGetSquareXML( _PATH_VIDEO_SLIDE_DIFFRACTION, aperture ) )
        {
            funcShowMsg("ERROR","Loading Usable Area in Pixels: _PATH_SLIDE_DIFFRACTION");
            tmpCommand.clear();
            return tmpCommand;
        }
        W = (double)aperture->rectW/(double)aperture->canvasW;
        H = (double)aperture->rectH/(double)aperture->canvasH;
        qDebug() << "W: " << W << " H: " << H;

        tmpCommand.append(" -roi ");
        tmpCommand.append(QString::number((double)aperture->rectX/(double)aperture->canvasW)+",");
        tmpCommand.append(QString::number((double)aperture->rectY/(double)aperture->canvasH)+",");
        tmpCommand.append(QString::number(W)+",");
        tmpCommand.append(QString::number(H));

        //.................................
        //Image Size
        //.................................
        int camMP = (ui->radioRes5Mp->isChecked())?5:8;
        camRes          = getCamRes(camMP);
        //Width
        tmpCommand.append(" -w ");
        tmpCommand.append(QString::number( round((double)camRes->videoW*W) ));
        //Height
        tmpCommand.append(" -h ");
        tmpCommand.append(QString::number( round((double)camRes->videoH*H) ));
    }
    else
    {
        //.................................
        //Image Size: Full Resolution
        //.................................
        int camMP       = (ui->radioRes5Mp->isChecked())?5:8;
        camRes          = getCamRes(camMP);
        //Width
        tmpCommand.append(" -w ");
        tmpCommand.append(QString::number( camRes->videoW ));
        //Height
        tmpCommand.append(" -h ");
        tmpCommand.append(QString::number( camRes->videoH ));
    }



    //.................................
    //Colour balance?
    //.................................
    if(ui->cbColorBalance->isChecked() ){
        tmpCommand.append(" -ifx colourbalance");
    }

    //.................................
    //Denoise?
    //.................................
    if( ui->cbDenoise->isChecked() ){
        tmpCommand.append(" -ifx denoise");
    }

    //.................................
    //Diffraction Shuter speed
    //.................................
    int shutSpeed = ui->spinBoxShuterSpeed->value();
    if( shutSpeed > 0 ){
        tmpCommand.append(" -ss " + QString::number(shutSpeed));
    }

    //.................................
    //AWB
    //.................................
    if( ui->cbAWB->currentText() != "none" ){
        tmpCommand.append(" -awb " + ui->cbAWB->currentText());
    }

    //.................................
    //Exposure
    //.................................
    if( ui->cbExposure->currentText() != "none" ){
        tmpCommand.append(" -ex " + ui->cbExposure->currentText());
    }

    //.................................
    //ISO
    //.................................
    if( ui->slideISO->value() > 0 ){
        tmpCommand.append(" -ISO " + QString::number(ui->slideISO->value()) );
    }

    //.................................
    //Flipped
    //.................................
    if( ui->cbFlipped->isChecked() ){
        tmpCommand.append(" -vf " );
    }


    return tmpCommand;
}



QString MainWindow::genTimelapseCommand(QString folder,bool setROI)
{
    QString tmpCommand;
    tmpCommand.clear();
    tmpCommand.append("raspistill -t");
    tmpCommand.append(" " + QString::number(ui->spinBoxTimelapseDuration->value()*1000));
    tmpCommand.append(" -tl ");
    tmpCommand.append(QString::number( ui->spinBoxTimelapse->value() ));
    tmpCommand.append(" -o "+ folder +"%d.RGB888");
    tmpCommand.append(" -n -q 100 -gc");

    //tmpCommand.append(" -tl 1000 -n -roi 0.221649485,0.313559322,0.416237113,0.372881356 -o ./tmpSnapVideos/%d.RGB888");
    //tmpCommand.append(" -tl 1000 -n -roi 0.2200,0.3136,0.4162,0.3729 -o ./tmpSnapVideos/%d.RGB888");
    //progBarTimer((ui->slideTriggerTime->value()+1)*1000);
    //funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected);

    if( setROI == true )
    {
        //.................................
        //Diffraction Area ROI
        //.................................
        double W, H;
        squareAperture *aperture = (squareAperture*)malloc(sizeof(squareAperture));
        memset(aperture,'\0',sizeof(squareAperture));
        if( !funGetSquareXML( _PATH_SLIDE_DIFFRACTION, aperture ) )
        {
            funcShowMsg("ERROR","Loading Usable Area in Pixels: _PATH_SLIDE_DIFFRACTION");
            tmpCommand.clear();
            return tmpCommand;
        }
        W = (double)aperture->rectW/(double)aperture->canvasW;
        H = (double)aperture->rectH/(double)aperture->canvasH;

        tmpCommand.append(" -roi ");
        tmpCommand.append(QString::number((double)aperture->rectX/(double)aperture->canvasW)+",");
        tmpCommand.append(QString::number((double)aperture->rectY/(double)aperture->canvasH)+",");
        tmpCommand.append(QString::number(W)+",");
        tmpCommand.append(QString::number(H));

        //.................................
        //Image Size ROI
        //.................................
        int camMP       = (ui->radioRes5Mp->isChecked())?5:8;
        camRes          = getCamRes(camMP);
        //Width
        tmpCommand.append(" -w ");
        tmpCommand.append(QString::number( round( W * (double)camRes->width ) ));
        //Height
        tmpCommand.append(" -h ");
        tmpCommand.append(QString::number( round( H * (double)camRes->height ) ));
    }
    else
    {
        //.................................
        //Image Size WITHOUT ROI
        //.................................
        int camMP       = (ui->radioRes5Mp->isChecked())?5:8;
        camRes          = getCamRes(camMP);
        //Width
        tmpCommand.append(" -w ");
        tmpCommand.append(QString::number( camRes->width ));
        //Height
        tmpCommand.append(" -h ");
        tmpCommand.append(QString::number( camRes->height ));
    }

    //.................................
    //Colour balance?
    //.................................
    if(ui->cbColorBalance->isChecked() ){
        tmpCommand.append(" -ifx colourbalance");
    }

    //.................................
    //Denoise?
    //.................................
    if( ui->cbDenoise->isChecked() ){
        tmpCommand.append(" -ifx denoise");
    }

    //.................................
    //Diffraction Shuter speed
    //.................................
    int shutSpeed = ui->spinBoxShuterSpeed->value();
    if( shutSpeed > 0 ){
        tmpCommand.append(" -ss " + QString::number(shutSpeed));
    }



    /*
    //Width
    ss.str("");
    ss<<reqImg->imgCols;
    tmpCommand->append(" -w " + ss.str());

    //Height
    ss.str("");
    ss<<reqImg->imgRows;
    tmpCommand->append(" -h " + ss.str());*/

    //.................................
    //AWB
    //.................................
    if( ui->cbAWB->currentText() != "none" ){
        tmpCommand.append(" -awb " + ui->cbAWB->currentText());
    }

    //.................................
    //Exposure
    //.................................
    if( ui->cbExposure->currentText() != "none" ){
        tmpCommand.append(" -ex " + ui->cbExposure->currentText());
    }

    //.................................
    //ISO
    //.................................
    if( ui->slideISO->value() > 0 ){
        tmpCommand.append(" -ISO " + QString::number(ui->slideISO->value()) );
    }

    //.................................
    //Flipped
    //.................................
    if( ui->cbFlipped->isChecked() ){
        tmpCommand.append(" -vf " );
    }


    return tmpCommand;
}


QString MainWindow::genSubareaRaspistillCommand( QString remoteFilename, QString subareaRectangle )
{
    //.................................
    //Basics Settings
    //.................................
    QString tmpCommand;
    tmpCommand.clear();
    tmpCommand.append("raspistill -o ");
    tmpCommand.append(remoteFilename);
    tmpCommand.append(" -t " + QString::number(ui->slideTriggerTime->value()*1000));
    tmpCommand.append(" -n -q 100 -gc");

    //.................................
    //Diffraction Area ROI
    //.................................
    double W, H;
    squareAperture *aperture = (squareAperture*)malloc(sizeof(squareAperture));
    memset(aperture,'\0',sizeof(squareAperture));
    if( !funGetSquareXML( subareaRectangle, aperture ) )
    {
        funcShowMsg("ERROR","Loading Usable Area in Pixels: _PATH_SLIDE_DIFFRACTION");
        tmpCommand.clear();
        return tmpCommand;
    }
    W = (double)aperture->rectW/(double)aperture->canvasW;
    H = (double)aperture->rectH/(double)aperture->canvasH;

    tmpCommand.append(" -roi ");
    tmpCommand.append(QString::number((double)aperture->rectX/(double)aperture->canvasW)+",");
    tmpCommand.append(QString::number((double)aperture->rectY/(double)aperture->canvasH)+",");
    tmpCommand.append(QString::number(W)+",");
    tmpCommand.append(QString::number(H));

    //.................................
    //Image Size ROI
    //.................................
    int camMP       = (ui->radioRes5Mp->isChecked())?5:8;
    camRes          = getCamRes(camMP);
    //Width
    tmpCommand.append(" -w ");
    tmpCommand.append(QString::number( round( W * (double)camRes->width ) ));
    //Height
    tmpCommand.append(" -h ");
    tmpCommand.append(QString::number( round( H * (double)camRes->height ) ));

    //.................................
    //Colour balance?
    //.................................
    if(ui->cbColorBalance->isChecked() ){
        tmpCommand.append(" -ifx colourbalance");
    }

    //.................................
    //Denoise?
    //.................................
    if( ui->cbDenoise->isChecked() ){
        tmpCommand.append(" -ifx denoise");
    }

    //.................................
    //Diffraction Shuter speed
    //.................................
    int shutSpeed = ui->spinBoxShuterSpeed->value();
    if( shutSpeed > 0 ){
        tmpCommand.append(" -ss " + QString::number(shutSpeed));
    }



    /*
    //Width
    ss.str("");
    ss<<reqImg->imgCols;
    tmpCommand->append(" -w " + ss.str());

    //Height
    ss.str("");
    ss<<reqImg->imgRows;
    tmpCommand->append(" -h " + ss.str());*/

    //.................................
    //AWB
    //.................................
    if( ui->cbAWB->currentText() != "none" ){
        tmpCommand.append(" -awb " + ui->cbAWB->currentText());
    }

    //.................................
    //Exposure
    //.................................
    if( ui->cbExposure->currentText() != "none" ){
        tmpCommand.append(" -ex " + ui->cbExposure->currentText());
    }

    //.................................
    //ISO
    //.................................
    if( ui->slideISO->value() > 0 ){
        tmpCommand.append(" -ISO " + QString::number(ui->slideISO->value()) );
    }

    //.................................
    //Flipped
    //.................................
    if( ui->cbFlipped->isChecked() ){
        tmpCommand.append(" -vf " );
    }


    return tmpCommand;
}


std::string MainWindow::funcRemoteTerminalCommand(
                                                    std::string command,
                                                    structCamSelected *camSelected,
                                                    int trigeredTime,
                                                    bool waitForAnswer,
                                                    bool* ok
){
    //* It is used when waitForAnswer==true
    //* Wait for answer when you need to know a parameter or the
    //  command result
    *ok = true;
    std::string tmpTxt;

    //--------------------------------------
    //Open socket
    //--------------------------------------
    int n;
    int sockfd = connectSocket( camSelected );
    qDebug() << "Socket opened";

    //--------------------------------------
    //Excecute command
    //--------------------------------------
    frameStruct *frame2send = (frameStruct*)malloc(sizeof(frameStruct));
    memset(frame2send,'\0',sizeof(frameStruct));    
    frame2send->header.idMsg        = (waitForAnswer==true)?(unsigned char)2:(unsigned char)5;    
    frame2send->header.numTotMsg    = 1;
    frame2send->header.consecutive  = 1;
    frame2send->header.trigeredTime = trigeredTime;
    frame2send->header.bodyLen      = command.length();
    bzero(frame2send->msg,command.length()+1);
    memcpy( frame2send->msg, command.c_str(), command.length() );

    //Request command result
    n = ::write(sockfd,frame2send,sizeof(frameStruct)+1);
    if(n<0){
        qDebug() << "ERROR: Excecuting Remote Command";
        *ok = false;
        return "";
    }

    //Receibing ack with file len    
    unsigned char bufferRead[frameBodyLen];
    n = read(sockfd,bufferRead,frameBodyLen);    

    if( waitForAnswer == true )
    {
        unsigned int fileLen;
        memcpy(&fileLen,&bufferRead,sizeof(unsigned int));
        fileLen = (fileLen<frameBodyLen)?frameBodyLen:fileLen;
        qDebug() << "fileLen: " << fileLen;
        //funcShowMsg("FileLen n("+QString::number(n)+")",QString::number(fileLen));

        //Receive File
        unsigned char tmpFile[fileLen];
        if( funcReceiveFile( sockfd, fileLen, bufferRead, tmpFile ) == false )
        {
            *ok = false;
        }
        tmpTxt.clear();
        tmpTxt.assign((char*)tmpFile);
        qDebug() <<tmpFile;
    }
    ::close(sockfd);

    //Return Command Result    
    return tmpTxt;
}

void MainWindow::on_actionRGB_to_XY_triggered()
{
    //Validate that exist pixel selected
    //..
    if( lstSelPix->count()<1){
        funcShowMsg("Lack","Not pixels selected");
        return (void)false;
    }

    //Create xycolor space
    //..
    GraphicsView *xySpace = new GraphicsView(this);
    funcPutImageIntoGV(xySpace, "./imgResources/CIEManual.png");
    xySpace->setWindowTitle( "XY color space" );
    xySpace->show();

    //Transform each pixel from RGB to xy and plot it
    //..
    QImage tmpImg(_PATH_DISPLAY_IMAGE);
    int W = tmpImg.width();
    int H = tmpImg.height();
    int pixX, pixY;
    QRgb tmpPix;
    colSpaceXYZ *tmpXYZ = (colSpaceXYZ*)malloc(sizeof(colSpaceXYZ));
    int tmpOffsetX = -13;
    int tmpOffsetY = -55;
    int tmpX, tmpY;
    int i;
    qDebug() << "<lstSelPix->count(): " << lstSelPix->count();
    for( i=0; i<lstSelPix->count(); i++ ){
        //Get pixel position in real image
        pixX = (float)(lstSelPix->at(i).first * W) / (float)canvasAux->width();
        pixY = (float)(lstSelPix->at(i).second * H) / (float)canvasAux->height();
        tmpPix = tmpImg.pixel( pixX, pixY );
        funcRGB2XYZ( tmpXYZ, (float)qRed(tmpPix), (float)qGreen(tmpPix), (float)qBlue(tmpPix) );
        //funcRGB2XYZ( tmpXYZ, 255.0, 0, 0  );
        tmpX = floor( (tmpXYZ->x * 441.0) / 0.75 ) + tmpOffsetX;
        tmpY = 522 - floor( (tmpXYZ->y * 481.0) / 0.85 ) + tmpOffsetY;
        funcAddPoit2Graph( xySpace, tmpX, tmpY, 1, 1,
                           QColor(qRed(tmpPix),qGreen(tmpPix),qBlue(tmpPix)),
                           QColor(qRed(tmpPix),qGreen(tmpPix),qBlue(tmpPix))
                         );
    }

    //Save image plotted
    //..
    QPixmap pixMap = QPixmap::grabWidget(xySpace);
    pixMap.save("./Results/Miscelaneas/RGBPloted.png");
}

void MainWindow::on_actionNDVI_triggered()
{
    mouseCursorWait();

    QString stringThreshold;
    QString stringBrilliant;
    QString infraredChanel;
    QString redChanel;
    stringThreshold = readFileParam(_PATH_NDVI_THRESHOLD);
    stringBrilliant = readFileParam(_PATH_NDVI_BRILLIANT);
    infraredChanel  = readFileParam(_PATH_NDVI_IR_CHANEL);
    redChanel       = readFileParam(_PATH_NDVI_RED_CHANEL);

    int makeBrilliant = (stringBrilliant.toInt(0)==1)?1:0;
    funcNDVI( globalEditImg, stringThreshold.toDouble(0), makeBrilliant, infraredChanel, redChanel );
    updateDisplayImage(globalEditImg);

    mouseCursorReset();
}

void MainWindow::on_actionNDVI_Algorithm_triggered()
{
    formNDVISettings* ndviSettings = new formNDVISettings(this);
    ndviSettings->setModal(true);
    ndviSettings->show();
}

void MainWindow::on_actionFull_Screen_triggered()
{
    QImage tmpImg(auxQstring);
    displayImageFullScreen(&tmpImg);
}

void MainWindow::on_actionDisplay_Original_triggered()
{
    updateDisplayImage(_PATH_DISPLAY_IMAGE);
}

void MainWindow::on_actionFull_photo_triggered()
{
    mouseCursorWait();

    if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,false) )
    {
        qDebug() << "ERROR: Taking Full Area";
        return (void)NULL;
    }

    QImage snapShot = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
    updateDisplayImage(&snapShot);
    snapShot.save(_PATH_DISPLAY_IMAGE);

    mouseCursorReset();
}

void MainWindow::on_actionDiffraction_triggered()
{
    mouseCursorWait();

    if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,false) )
    {
        qDebug() << "ERROR: Taking Diffration Area";
        return (void)NULL;
    }
    else
    {
        QImage diffImage = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
        if( diffImage.isNull() )
        {
            qDebug() << "ERROR: Obtaining Diffration Area";
            return (void)NULL;
        }
        else
        {
            //
            //Crop original image to release the usable area
            //
            //Get usable area coordinates
            squareAperture *aperture = (squareAperture*)malloc(sizeof(squareAperture));
            memset(aperture,'\0',sizeof(squareAperture));
            if( !rectangleInPixelsFromSquareXML( _PATH_REGION_OF_INTERES2, aperture ) )
            {
                funcShowMsg("ERROR","Loading Usable Area in Pixels: _PATH_REGION_OF_INTERES");
                return (void)false;
            }
            //Crop and save
            diffImage = diffImage.copy(QRect( aperture->rectX, aperture->rectY, aperture->rectW, aperture->rectH ));
            diffImage.save(_PATH_DISPLAY_IMAGE);
            qDebug() << "Images saved";

            updateDisplayImage(&diffImage);
        }
    }

    mouseCursorReset();
}

void MainWindow::on_actionComposed_triggered()
{
    mouseCursorWait();

    if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,false) )
    {
        qDebug() << "ERROR: Taking Diffration Area";
        return (void)NULL;
    }
    else
    {
        QImage diffImage = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
        if( diffImage.isNull() )
        {
            qDebug() << "ERROR: Obtaining Diffration Area";
            return (void)NULL;
        }
        else
        {
            if( !takeRemoteSnapshot(_PATH_REMOTE_SNAPSHOT,true) )
            {
                qDebug() << "ERROR: Taking Aperture Area";
                return (void)NULL;
            }
            else
            {
                QImage apertureImage = obtainImageFile( _PATH_REMOTE_SNAPSHOT, "" );
                if( apertureImage.isNull() )
                {
                    qDebug() << "ERROR: Obtaining Aperture Area";
                    return (void)NULL;
                }
                else
                {
                    //-------------------------------------------
                    //Merge image
                    //-------------------------------------------

                    squareAperture *aperture = (squareAperture*)malloc(sizeof(squareAperture));

                    //
                    //Crop original image to release the usable area
                    //
                    //Get usable area coordinates
                    memset(aperture,'\0',sizeof(squareAperture));
                    if( !rectangleInPixelsFromSquareXML( _PATH_REGION_OF_INTERES2, aperture ) )
                    {
                        funcShowMsg("ERROR","Loading Usable Area in Pixels: _PATH_SQUARE_USABLE");
                        return (void)false;
                    }
                    diffImage       = diffImage.copy(QRect( aperture->rectX, aperture->rectY, aperture->rectW, aperture->rectH ));
                    apertureImage   = apertureImage.copy(QRect( aperture->rectX, aperture->rectY, aperture->rectW, aperture->rectH ));

                    //
                    //Get square aperture coordinates
                    //
                    memset(aperture,'\0',sizeof(squareAperture));
                    if( !rectangleInPixelsFromSquareXML( _PATH_SQUARE_APERTURE2, _PATH_REGION_OF_INTERES2, aperture ) )
                    {
                        funcShowMsg("ERROR","Loading Rectangle in Pixels: _PATH_SQUARE_APERTURE");
                        return (void)false;
                    }
                    funcPrintRectangle( "Square Aperture", aperture );

                    //
                    //Copy square aperture into diffraction image
                    //
                    for( int y=aperture->rectY; y<=(aperture->rectY+aperture->rectH); y++ )
                    {
                        for( int x=aperture->rectX; x<=(aperture->rectX+aperture->rectW); x++ )
                        {
                            diffImage.setPixel( x, y, apertureImage.pixel( x, y ) );
                        }
                    }



                    //
                    //Save cropped image
                    //
                    diffImage.save(_PATH_DISPLAY_IMAGE);
                    updateDisplayImage(&diffImage);
                }
            }
        }
    }

    mouseCursorReset();
}

void MainWindow::on_actionVideo_triggered()
{
    //Update camera resolution
    //..
    int camMP       = (ui->radioRes5Mp->isChecked())?5:8;
    camRes          = getCamRes(camMP);

    //Set required image's settings
    //..
    strReqImg *reqImg       = (strReqImg*)malloc(sizeof(strReqImg));
    memset(reqImg,'\0',sizeof(strReqImg));

    //Codec H264 or MJPEG
    QString videoLocalFilename;
    QString videoRemoteFilename;
    //const u_int8_t videoCodec = _MJPEG;
    const u_int8_t videoCodec = _H264;
    if( videoCodec == _H264 )
    {
        videoRemoteFilename.append(_PATH_VIDEO_REMOTE_H264);
        videoLocalFilename.append(_PATH_VIDEO_RECEIVED_H264);
    }
    else
    {
        memcpy( reqImg->video.cd, "MJPEG", 5 );
        qDebug() << "MJPEG";

        if( 1 )
        {
            videoRemoteFilename.append(_PATH_VIDEO_REMOTE_MJPEG);
            videoLocalFilename.append(_PATH_VIDEO_RECEIVED_MJPEG);
        }
        else
        {
            videoRemoteFilename.append(_PATH_VIDEO_REMOTE_H264);
            videoLocalFilename.append(_PATH_VIDEO_RECEIVED_H264);
        }
    }
    qDebug() << "videoRemoteFilename: " << videoRemoteFilename;
    memcpy( reqImg->video.o, videoRemoteFilename.toStdString().c_str(), videoRemoteFilename.size() );

    //Others
    reqImg->idMsg           = (unsigned char)12;
    reqImg->video.t         = ui->slideTriggerTime->value();
    reqImg->video.ss        = (int16_t)round(ui->spinBoxShuterSpeed->value());
    reqImg->video.awb       = (int16_t)ui->cbAWB->currentIndex();
    reqImg->video.ex        = (int16_t)ui->cbExposure->currentIndex();
    reqImg->video.md        = 2;//2
    reqImg->video.w         = 0;//1640
    reqImg->video.h         = 0;//1232
    reqImg->video.fps       = 5;//1-15

    //
    //Motor walk
    //
    // HELP
    //
    // (ERROR on binding: Address already in use) -> netstat -tulpn -> kill process
    // Para saber el USB -> ls -l /dev/tty (tab)
    reqImg->motorWalk.degreeIni     = 0;
    reqImg->motorWalk.degreeEnd     = 180;
    reqImg->motorWalk.durationMs    = 3000;
    reqImg->motorWalk.stabilizingMs = 1000;

    //Open socket
    int n;
    int sockfd = connectSocket( camSelected );
    unsigned char bufferRead[frameBodyLen];
    qDebug() << "Socket opened";
    //Require photo size
    //QtDelay(5);
    n = ::write(sockfd,reqImg,sizeof(strReqImg));
    qDebug() << "Video request sent";

    //Receive ACK with the camera status
    memset(bufferRead,'\0',3);
    n = read(sockfd,bufferRead,2);
    if( bufferRead[1] == 1 )
    {//Begin the video adquisition routine

        //
        // STATUS BAR
        //
        progBarUpdateLabel("Recording video",0);
        progBarTimer( (ui->slideTriggerTime->value() + 1) * 1000 );

        //Delete if file exists
        funcDeleteFile( _PATH_VIDEO_RECEIVED_H264 );
        funcDeleteFile( _PATH_VIDEO_RECEIVED_MJPEG );
        funcDeleteFile( _PATH_VIDEO_RECEIVED_MP4 );

        //Download new
        qDebug() << "Video recorded";
        progBarUpdateLabel("Transferring video",0);
        obtainFile( videoRemoteFilename.toStdString(), videoLocalFilename.toStdString(), "" );

        //Convert into MP4
        if( videoCodec == _H264 )
        {
            //Convert into .MP4
            QString converToMP4;
            converToMP4 = "";
            converToMP4.append( "MP4Box -add ");
            converToMP4.append(_PATH_VIDEO_RECEIVED_H264);
            converToMP4.append(" ");
            converToMP4.append(_PATH_VIDEO_RECEIVED_MP4);
            funcExecuteCommand( converToMP4 );
            //Update permissions
            //converToMP4 = "chmod +777 ";
            //converToMP4.append(_PATH_VIDEO_RECEIVED_MP4);
            //funcExecuteCommand( converToMP4 );
        }

        //
        //Send .mp4 to frames
        //
        //funcVideoToFrames(_PATH_VIDEO_FRAMES, _PATH_VIDEO_RECEIVED_MP4);




    }
    else
    {//Video does not generated by raspicam
        funcShowMsg("ERROR on Camera","Recording video");
    }

    n = n;
    ::close(sockfd);
}

void MainWindow::on_actionTimelapse_triggered()
{
    mouseCursorWait();
    QString tmpCommand;
    //--------------------------------------
    //Clear Remote Directory
    //--------------------------------------
    tmpCommand.clear();
    tmpCommand.append("sudo rm tmpSnapVideos/*");
    bool ok;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&ok);

    //--------------------------------------
    //Take Timelapse
    //--------------------------------------
    tmpCommand = genTimelapseCommand(_PATH_REMOTE_FOLDER_SLIDELAPSE);
    qDebug() << "tmpCommand: " << tmpCommand;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&ok);
    progBarUpdateLabel("Timelapsing...",0);
    progBarTimer((ui->slideTriggerTime->value()+1)*1000);

    //--------------------------------------
    //Get Number of Frames
    //--------------------------------------
    std::string stringNumOfFrames = funcRemoteTerminalCommand(
                                                                "ls ./tmpSnapVideos/ -a | wc -l",
                                                                camSelected,
                                                                0,
                                                                true,
                                                                &ok
                                                            );
    int numOfFrames = atoi(stringNumOfFrames.c_str()) - 2;
    //qDebug() << "numOfFrames: " << numOfFrames;

    //--------------------------------------
    //Get Frames
    //--------------------------------------
    if( numOfFrames > 0 )
    {
        funcClearDirFolder( _PATH_VIDEO_FRAMES );
        QString tmpRemoteFrame, tmpLocalFrame;
        int framesTransmited = 0;
        int i = 0;
        int filesNotFound = 0;
        int maxFilesError = 5;
        while( framesTransmited < numOfFrames )
        {
            tmpRemoteFrame = "./tmpSnapVideos/" + QString::number(i) + ".RGB888";
            tmpLocalFrame  = _PATH_VIDEO_FRAMES + QString::number(i) + ".RGB888";

            //Get Remote File
            obtainFile( tmpRemoteFrame.toStdString(), tmpLocalFrame.toStdString(), "" );

            //Check if was transmited
            if( fileExists(tmpLocalFrame) )
            {
                framesTransmited++;
                filesNotFound = 0;
            }
            else
            {
                filesNotFound++;
            }

            //Next image to check
            i++;

            //Break if error
            if( filesNotFound >= maxFilesError )
            {
                funcShowMsg("ERROR","maxFilesError Achieved");
                break;
            }
        }
    }

    progBarUpdateLabel("",0);
    mouseCursorReset();
}

void MainWindow::on_actionSave_triggered()
{
    //
    //Read the filename
    //
    QString fileName;
    QString lastPath = readFileParam(_PATH_LAST_IMG_SAVED);
    if( lastPath.isEmpty() )//First time using this parameter
    {
        lastPath = "./snapshots/";
    }
    fileName = QFileDialog::getSaveFileName(
                                                this,
                                                tr("Save Snapshot as..."),
                                                lastPath,
                                                tr("Documents (*.png)")
                                            );
    if( fileName.isEmpty() )
    {
        qDebug() << "Filename not typed";
        return (void)false;
    }
    else
    {
        lastPath = funcRemoveFileNameFromPath(fileName);
        saveFile(_PATH_LAST_IMG_SAVED,lastPath);
    }

    //
    //Validate filename
    //
    fileName = funcRemoveImageExtension(fileName);

    //
    //Save image
    //
    QImage tmpImg( _PATH_DISPLAY_IMAGE );
    tmpImg.save(fileName);
}

void MainWindow::on_actionSlideDiffraction_triggered()
{
    //----------------------------------------
    // Validate Camera Connection
    //----------------------------------------
    if( camSelected->isConnected == false )
    {
        funcShowMsg("ERROR","Not Camera Connection Found");
        return (void)false;
    }


    mouseCursorWait();
    QString tmpCommand;


    //......................................
    //Clear Remote Directory
    //......................................
    //tmpCommand.clear();
    //tmpCommand.append("sudo rm tmpSnapshots/*");
    //funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,false);

    //......................................
    //Generate Remote Command
    //......................................
    tmpCommand.clear();
    tmpCommand = genSubareaRaspistillCommand( _PATH_REMOTE_SNAPSHOT, _PATH_SLIDE_DIFFRACTION );
    qDebug() << "tmpCommand: " << tmpCommand;

    //......................................
    //Execute Remote Command
    //......................................
    bool ok;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&ok);
    progBarUpdateLabel(_MSG_PROGBAR_STABILIZING,0);
    progBarTimer((ui->slideTriggerTime->value()+1)*1000);    

    //......................................
    //Get Remote File
    //......................................
    obtainImageFile( _PATH_REMOTE_SNAPSHOT, _PATH_IMAGE_RECEIVED );

    //......................................
    //Check if the image was transmited
    //......................................
    if( !fileExists(_PATH_IMAGE_RECEIVED) )
    {
        funcShowMsg("ERROR","Image was not received");
    }
    else
    {
        QImage tmpImg(_PATH_IMAGE_RECEIVED);
        updateDisplayImage(&tmpImg);
    }

    //Reset Mouse and Progress-Bar
    progBarUpdateLabel(_MSG_PROGBAR_RESET,0);
    mouseCursorReset();
}

void MainWindow::on_actionObtain_Folder_triggered()
{
    formObtainFolder* formObtRemoteFolder = new formObtainFolder(this);
    formObtRemoteFolder->setModal(true);    
    connect(
                formObtRemoteFolder,
                SIGNAL( signalObtainRemoteFolder(QString,QString) ),
                this,
                SLOT( obtainRemoteFolder(QString,QString) )
           );
    formObtRemoteFolder->show();
}

int MainWindow::obtainRemoteFolder(
                                        QString remoteFolder,
                                        QString localFolder,
                                        QString* errTrans,
                                        QString* errDel,
                                        bool delFolder
){

    bool delDir = true;

    //------------------------------------------------------
    // Obtain list of files in Remote Folder
    //------------------------------------------------------
    QString tmpCommand;
    tmpCommand.append("ls ");
    tmpCommand.append(remoteFolder);
    tmpCommand.append("*.*");
    bool executedCommand;
    std::string stringLstFiles = funcRemoteTerminalCommand(
                                                                tmpCommand.toStdString(),
                                                                camSelected,
                                                                0,
                                                                true,
                                                                &executedCommand
                                                           );
    if( !executedCommand )return _ERROR;

    //------------------------------------------------------
    // Create local folder if not exists
    //------------------------------------------------------
    QString remoteFile, localFile, barTxt, tmpFileName;
    QString tmpAnswer(stringLstFiles.data());
    QList<QString> tmpFileParts;
    QList<QString> lstFiles = tmpAnswer.split('\n');
    if( lstFiles.size() > 0 )
    {
        //qDebug() << "localFolder: " << localFolder;
        if( !fileExists(localFolder) )
        {
            if( !funcExecuteCommand( "mkdir " + localFolder ) )
            {
                funcShowMsgERROR_Timeout("Creating Local Folder: " + localFolder, this, 1800 );
                return _ERROR;
            }
        }
    }

    //------------------------------------------------------
    // Get each element into the folder
    //------------------------------------------------------
    int i;
    //qDebug() << "lstFiles.size(): " << lstFiles.size() << " remoteFolder: " << remoteFolder;
    for( i=0; i<lstFiles.size(); i++ )
    {
        tmpFileName = lstFiles.at(i).trimmed();
        if(
            !tmpFileName.isEmpty()  &&
            tmpFileName != "./"     &&
            tmpFileName != "../"    &&
            tmpFileName != "."      &&
            tmpFileName != ".."
        ){
            //Define File Names
            tmpFileParts.clear();
            tmpFileParts    = lstFiles.at(i).trimmed().split("/");
            localFile       = localFolder  + tmpFileParts.at(tmpFileParts.size()-1);
            remoteFile      = remoteFolder + tmpFileParts.at(tmpFileParts.size()-1);

            //qDebug() << "remoteFile: " << remoteFile;
            //qDebug() << "localFile: " << localFile;
            //exit(0);

            //Transmit Remote File
            barTxt.clear();
            barTxt.append("Transmitting ");
            barTxt.append(lstFiles.at(i));
            barTxt.append("...");
            obtainFile( remoteFile.toStdString(), localFile.toStdString(), barTxt );

            //Check if file was transmited successfully
            if( !fileExists(localFile) )
            {
                errTrans->append(remoteFile+"\n");
                delDir = false;
            }
            else
            {
                //Remove Remote File
                if( !funcRemoveRemoteFile(remoteFile) )
                {
                    errDel->append(remoteFile+"\n");
                    delDir = false;
                }
            }
        }
    }

    //------------------------------------------------------
    // Copy Folders Recursivelly
    //------------------------------------------------------

    //Get List of Folders
    tmpCommand.clear();
    tmpCommand.append("ls -d ");
    tmpCommand.append(remoteFolder);
    tmpCommand.append("*/");
    std::string stringLstFolders = funcRemoteTerminalCommand(
                                                                tmpCommand.toStdString(),
                                                                camSelected,
                                                                0,
                                                                true,
                                                                &executedCommand
                                                            );
    if( !executedCommand )return _ERROR;

    //......................................................
    // Get each folder into the main folder
    //......................................................
    //QString remoteFile, localFile, barTxt, tmpFileName;
    QString tmpRemoteSubfolder, tmpLocalSubfolder;
    QString tmpAnswerFolder(stringLstFolders.data());
    QList<QString> lstFolders = tmpAnswerFolder.split('\n');
    QList<QString> tmpFolderParts;
    //qDebug() << "lstFolders.size(): " << lstFolders.size() << " remoteFolder: " << remoteFolder;
    for( i=0; i<lstFolders.size(); i++ )
    {
        tmpRemoteSubfolder = lstFolders.at(i).trimmed();
        if(
            !tmpRemoteSubfolder.isEmpty()  &&
             tmpRemoteSubfolder != "./"    &&
             tmpRemoteSubfolder != "../"   &&
             tmpRemoteSubfolder != "."     &&
             tmpRemoteSubfolder != ".."
        ){
            //Extract folder name
            tmpFolderParts.clear();
            tmpFolderParts = tmpRemoteSubfolder.split('/');

            //Obtain Subfolder
            tmpLocalSubfolder  = localFolder  + tmpFolderParts.at(tmpFolderParts.size()-2)+"/";
            //qDebug() << "tmpRemoteSubfolder2: " << tmpRemoteSubfolder;
            //qDebug() << "tmpFolderParts.at(tmpFolderParts.size()): " << tmpFolderParts.at(tmpFolderParts.size()-2);
            //qDebug() << "tmpLocalSubfolder: " << tmpLocalSubfolder;
            //qDebug() << "tmpRemoteSubfolder: " << tmpRemoteSubfolder;
            //fflush(stdout);
            //exit(0);
            obtainRemoteFolder(tmpRemoteSubfolder,tmpLocalSubfolder,errTrans,errDel,true);
        }
    }

    if( delFolder && delDir )
    {
        tmpCommand.clear();
        tmpCommand.append("sudo rm -R " + remoteFolder );
        funcRemoteTerminalCommand(
                                        tmpCommand.toStdString(),
                                        camSelected,
                                        0,
                                        false,
                                        &executedCommand
                                 );
        if( !executedCommand )
        {
            errDel->append(remoteFolder+"\n");
        }
    }

    //Return
    return _OK;
}

bool MainWindow::funcRemoveRemoteFile( QString fileName )
{
    QString tmpCommand;
    tmpCommand.append("sudo rm " + fileName);
    bool executedCommand;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&executedCommand);
    if( !executedCommand )
        return _ERROR;
    return _OK;
}

void MainWindow::on_actionSlide_Build_Hypercube_triggered()
{


    //------------------------------------------------------
    //Show Form
    //------------------------------------------------------
    formBuildSlideHypeCubePreview* slidePreview = new formBuildSlideHypeCubePreview(this);
    slidePreview->setModal(true);
    slidePreview->show();


    /*
    //------------------------------------------------------
    //Select Directory
    //------------------------------------------------------
    QString workDir, lastSlideFrames;
    if( !readFileParam(_PATH_LAST_SLIDE_FRAMES_4CUBE,&lastSlideFrames) )
    {
        lastSlideFrames.clear();
        lastSlideFrames.append(_PATH_VIDEO_FRAMES);
    }
    if( !funcShowSelDir(lastSlideFrames,&workDir) )
    {
        return (void)false;
    }
    saveFile(_PATH_LAST_SLIDE_FRAMES_4CUBE,workDir);

    //------------------------------------------------------
    //Define List With Imagery to Be Considered
    //------------------------------------------------------

    //......................................................
    //Gel List of Files in Directory Selected
    //......................................................
    QList<QFileInfo> lstFiles = funcListFilesInDir( workDir, _FRAME_RECEIVED_EXTENSION );
    qDebug() << "lstFiles: " << lstFiles.length();
    if(lstFiles.size()==0)
    {
        funcShowMsgERROR("Empty Directory");
        return (void)false;
    }

    //......................................................
    //Read Hyperimage's Parameters
    //......................................................
    QString timelapseFile(workDir + _FILENAME_SLIDE_DIR_TIME);
    std::string tmpParam;
    if( !readParamFromFile( timelapseFile.toStdString(), &tmpParam) )
    {
        funcShowMsgERROR("Timelapse Parameter not found at: " + workDir);
        return (void)false;
    }
    int timelapse           = atoi(tmpParam.c_str());
    qDebug() << "numFrames: " << lstFiles.size() << " time: " << timelapse;

    //......................................................
    //Discard Stabilization Frames
    //......................................................
    int numFrame2Discard    = ceil( (float)_RASPBERRY_STABILIZATION_TIME / (float)timelapse );
    int i;
    for( i=0; i<numFrame2Discard; i++ )
    {
        //qDebug() << "file discarded: " << lstFiles.at(i).completeBaseName();
        lstFiles.removeAt(0);
    }*/


    //structSlideHypCube slideCubeSettings;
    //slideCubeSettings.rotateLeft  = true;
    //slideCubeSettings.width = 70;
    //buildHypercubeFromFilelist( lstFiles, slideCubeSettings );

}

void MainWindow::buildHypercubeFromFilelist(
                                                QList<QFileInfo> lstFrames,
                                                structSlideHypCube slideCubeSettings
){
    //[COMMENT]
    //It assumes that lstFrames contains only usable frames

    //----------------------------------------------------
    // Create Image Container
    //----------------------------------------------------
    int resultImgW;
    resultImgW = lstFrames.size() * slideCubeSettings.width;
    QImage tmpImg( lstFrames.at(0).absoluteFilePath() );
    QImage resultImg( resultImgW, tmpImg.height(), QImage::Format_RGB16 );

    //----------------------------------------------------
    // Read RGB Position
    //----------------------------------------------------
    QString tmpParam;
    if( !readFileParam(_PATH_SLIDE_HALOGEN_SENSITIVITIES,&tmpParam) )
    {
        if( !readFileParam(_PATH_SLIDE_FLUORESCENT,&tmpParam) )
        {
            funcShowMsgERROR("RGB Positions Not Found");
            return (void)false;
        }
    }
    int rPosX, gPosX, bPosX;
    //rPosX = tmpParam.split(",").at(0).toInt(0) - round((float)slideCubeSettings.width/2.0);
    //gPosX = tmpParam.split(",").at(1).toInt(0) - round((float)slideCubeSettings.width/2.0);
    //bPosX = tmpParam.split(",").at(2).toInt(0) - round((float)slideCubeSettings.width/2.0);
    rPosX = 500;
    gPosX = 500;
    bPosX = 500;

    //----------------------------------------------------
    // Create Image To Display
    //----------------------------------------------------
    int i, w, h, r, c, slideX, red, green, blue;
    w = slideCubeSettings.width;
    h = tmpImg.height()-1;
    for( i=0; i<lstFrames.size(); i++ )
    {
        tmpImg      = QImage( lstFrames.at(i).absoluteFilePath() );
        slideX      = i*w;
        for( r=0; r<h; r++ )
        {
            for(c=0; c<w; c++ )
            {
                red     = qRed(   tmpImg.pixel( rPosX+c, r ) );
                green   = qGreen( tmpImg.pixel( gPosX+c, r ) );
                blue    = qBlue(  tmpImg.pixel( bPosX+c, r ) );
                resultImg.setPixel( QPoint( slideX+c, r ), qRgb( red, green, blue ) );
            }
        }
    }
    if( slideCubeSettings.rotateLeft == true )
    {
        rotateQImage(&resultImg,90);
    }
    resultImg.save( _PATH_DISPLAY_IMAGE );
    updateDisplayImage(&resultImg);
}


int MainWindow::funcAccountFilesInFolder( QString folder )
{
    //Generate the command
    QString tmpCommand;
    tmpCommand.append("ls ");
    tmpCommand.append(folder);
    tmpCommand.append(" -a | wc -l");
    //qDebug() << "tmpCommand: " << tmpCommand;

    //Execute command
    QString numFrames;
    numFrames = funcExecuteCommandAnswer( (char*)tmpCommand.toStdString().data() );
    return numFrames.toInt(0);

}

void MainWindow::on_actionVideo_2_triggered()
{
    QString remoteVideoID;
    funcMainCall_RecordVideo(&remoteVideoID,false,true);
}

void MainWindow::on_actionTimelapse_2_triggered()
{

    funcStartRemoteTimelapse( false );

}

/*
void MainWindow::funcShowMsgERROR_Timeout(QString msg, int ms)
{
    QMessageBox *msgBox         = new QMessageBox(QMessageBox::Warning,"ERROR",msg,NULL);
    QTimer *msgBoxCloseTimer    = new QTimer(this);
    msgBoxCloseTimer->setInterval(ms);
    msgBoxCloseTimer->setSingleShot(true);
    connect(msgBoxCloseTimer, SIGNAL(timeout()), msgBox, SLOT(reject()));
    msgBoxCloseTimer->start();
    msgBox->exec();
}


void MainWindow::funcShowMsgSUCCESS_Timeout(QString msg, int ms)
{
    QMessageBox *msgBox         = new QMessageBox(QMessageBox::Information,"SUCCESS",msg,NULL);
    QTimer *msgBoxCloseTimer    = new QTimer(this);
    msgBoxCloseTimer->setInterval(ms);
    msgBoxCloseTimer->setSingleShot(true);
    connect(msgBoxCloseTimer, SIGNAL(timeout()), msgBox, SLOT(reject()));
    msgBoxCloseTimer->start();
    msgBox->exec();
}*/

void MainWindow::funcStartRemoteTimelapse( bool setROI )
{
    //---------------------------------------------------
    //Get Folder Destine
    //---------------------------------------------------
    QString folderID;
    folderID = funcGetParam("Folder-Name");
    if(folderID.isEmpty())
    {
        funcShowMsgERROR_Timeout("Invalid Folder",this);
        return (void)false;
    }

    //---------------------------------------------------
    //Validate File/Dir Name
    //---------------------------------------------------
    QString remoteFile, localFile;
    if( setROI == true )
    {
        remoteFile = _PATH_REMOTE_FOLDER_SLIDELAPSE + folderID + "/";

        //Local File
        QString syncFolder;
        syncFolder = funcGetSyncFolder();
        localFile.clear();
        localFile.append(syncFolder);
        localFile.append(_PATH_LOCAL_FOLDER_SLIDELAPSE);
        localFile.append(folderID);
        localFile.append("/");
    }
    else
    {
        remoteFile = _PATH_REMOTE_FOLDER_TIMELAPSE + folderID + "/";

        //Local File
        QString syncFolder;
        syncFolder = funcGetSyncFolder();
        localFile.clear();
        localFile.append(syncFolder);
        localFile.append(_PATH_LOCAL_FOLDER_TIMELAPSE);
        localFile.append(folderID);
        localFile.append("/");
    }
    int whereExists;
    whereExists = funcValidateFileDirNameDuplicated( remoteFile, localFile );
    if( whereExists == -1 )
    {
        funcShowMsgERROR_Timeout("ID Exists Locally: Please, use another",this);
        return (void)false;
    }
    if( whereExists == -2 )
    {
        funcShowMsgERROR_Timeout("ID Exists Remotelly: Please, use another",this);
        return (void)false;
    }

    //--------------------------------------
    //Create an Empty Remote Directory
    //--------------------------------------
    QString tmpCommand;
    bool commandExecuted;
    tmpCommand.clear();
    tmpCommand.append("mkdir "+remoteFile);
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&commandExecuted);
    if( !commandExecuted )
    {
        funcShowMsgERROR_Timeout("Creating Remote Folder",this);
        return (void)false;
    }

    //--------------------------------------
    //Take Timelapse
    //--------------------------------------
    int triggeringTime;
    triggeringTime = ui->slideTriggerTime->value();
    tmpCommand = genTimelapseCommand(remoteFile,setROI);
    qDebug() << "tmpCommand: " << tmpCommand;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,triggeringTime,false,&commandExecuted);
    if( !commandExecuted )
    {
        funcShowMsgERROR_Timeout("Starting Remote Timelapse",this);
        return (void)false;
    }

    //-----------------------------------------------------
    //Start Timer
    //-----------------------------------------------------
    funcDisplayTimer("Countdown to Recording...",triggeringTime,Qt::black);

}

void MainWindow::on_actionSnapvideos_triggered()
{
    funcStartRemoteTimelapse( true );
}

void MainWindow::on_actionSnapshot_triggered()
{
    funcMainCall_GetSnapshot();
}

void MainWindow::on_actionSynchronize_triggered()
{

    //-----------------------------------------------------
    // Get All New Files
    //-----------------------------------------------------
    int status;
    QString errorTrans, errorDel, syncLocalFolder;

    //Read Sync Destiny Folder
    syncLocalFolder = funcGetSyncFolder();

    //Videos
    status = obtainRemoteFolder( _PATH_REMOTE_FOLDER_VIDEOS, syncLocalFolder+_PATH_LOCAL_FOLDER_VIDEOS, &errorTrans, &errorDel );
    if( status == _ERROR )
    {
        funcShowMsgERROR_Timeout("Obtaining Videos",this);
        return (void)false;
    }

    //Timelapse
    status = obtainRemoteFolder( _PATH_REMOTE_FOLDER_TIMELAPSE, syncLocalFolder+_PATH_LOCAL_FOLDER_TIMELAPSE, &errorTrans, &errorDel );
    if( status == _ERROR )
    {
        funcShowMsgERROR_Timeout("Obtaining Timelapse",this);
        return (void)false;
    }

    //Snapshots
    status = obtainRemoteFolder( _PATH_REMOTE_FOLDER_SNAPSHOTS, syncLocalFolder+_PATH_LOCAL_FOLDER_SNAPSHOTS, &errorTrans, &errorDel );
    if( status == _ERROR )
    {
        funcShowMsgERROR_Timeout("Obtaining Snapshots",this);
        return (void)false;
    }

    //Slidelapses
    status = obtainRemoteFolder( _PATH_REMOTE_FOLDER_SLIDELAPSE, syncLocalFolder+_PATH_LOCAL_FOLDER_SLIDELAPSE, &errorTrans, &errorDel );
    if( status == _ERROR )
    {
        funcShowMsgERROR_Timeout("Obtaining Slidelapses",this);
        return (void)false;
    }

    //-----------------------------------------------------
    // Display Result
    //-----------------------------------------------------
    if( !errorTrans.isEmpty() || !errorDel.isEmpty() )
    {
        QString strDisplayStatus;
        strDisplayStatus.append("Some Files/Dirs Produced Errors...\n\n\n");
        strDisplayStatus.append("Transmission: \n\n" + errorTrans);
        strDisplayStatus.append("Delations: \n\n" + errorDel);
        funcShowMsgERROR_Timeout(strDisplayStatus,this);
    }
    else
    {
        funcShowMsg("SUCCESS","Remote Camera Syncronized Successfully");
    }

}


int MainWindow::funcValidateFileDirNameDuplicated(QString remoteFile, QString localFile)
{
    qDebug() << "remoteFile: " << remoteFile;
    qDebug() << "localFile: " << localFile;

    //Check if exists locally
    if( fileExists(localFile) )
    {
        qDebug() << "Exists Locally";
        return -1;
    }

    //Check if exists remotelly
    if( funcRaspFileExists( remoteFile.toStdString() ) == 1 )
    {
        qDebug() << "Exists Remotelly";
        return -2;
    }

    //File does not exists locally or remotelly
    return _OK;
}

void MainWindow::on_actionSync_Folder_triggered()
{
    //--------------------------------------------------
    // Read Parameter
    //--------------------------------------------------
    QString syncFolder;
    syncFolder = funcGetSyncFolder();

    //--------------------------------------------------
    // Require Folder Destiny
    //--------------------------------------------------
    QString newDir;
    newDir = funcShowSelDir(syncFolder);
    if( !newDir.isEmpty() )
    {
        saveFile(_PATH_LAST_SYNC_FOLDER,newDir+"/");
    }

}

QString MainWindow::funcGetSyncFolder()
{
    QString tmpParam;
    if( !readFileParam( _PATH_LAST_SYNC_FOLDER, &tmpParam) )
    {
        saveFile(_PATH_LAST_SYNC_FOLDER,_PATH_LOCAL_SYNC_FOLDERS);
        tmpParam.clear();
        tmpParam.append(_PATH_LOCAL_SYNC_FOLDERS);
    }
    return tmpParam;
}


void MainWindow::funcMainCall_RecordVideo(QString* videoID, bool defaultPath, bool ROI)
{
    bool commandExecuted;

    videoID->clear();

    //---------------------------------------------------
    //Get Video-ID Destine
    //---------------------------------------------------
    if( defaultPath )
    {
        videoID->append(_PATH_VIDEO_REMOTE_H264);
    }
    else
    {
        *videoID = funcGetParam("Video-ID");
        if(videoID->isEmpty())
        {
            funcShowMsgERROR_Timeout("Invalid Video-ID",this);
            return (void)false;
        }

        //---------------------------------------------------
        //Validate File/Dir Name
        //---------------------------------------------------
        QString remoteFile, localFile;
        remoteFile.append(_PATH_REMOTE_FOLDER_VIDEOS);
        remoteFile.append(*videoID);
        remoteFile.append(_VIDEO_EXTENSION);

        //Local File
        QString syncLocalFolder = funcGetSyncFolder();
        localFile.clear();
        localFile.append(syncLocalFolder);
        localFile.append(_PATH_REMOTE_FOLDER_VIDEOS);
        localFile.append(videoID);
        localFile.append(_VIDEO_EXTENSION);

        //Check if exists
        int itExists = funcValidateFileDirNameDuplicated( remoteFile, localFile );
        if( itExists != _OK )
        {
            if( itExists == -1 )
                funcShowMsgERROR_Timeout("Video-ID Exists Locally: Please, use another",this);
            else
                funcShowMsgERROR_Timeout("Video-ID Exists Remotelly: Please, use another",this);
            return (void)false;
        }

        //---------------------------------------------------
        //Prepare Remote Scenary
        //---------------------------------------------------

        //Delete Remote File if Exists
        *videoID = _PATH_REMOTE_FOLDER_VIDEOS + *videoID + _VIDEO_EXTENSION;
        QString tmpCommand;
        tmpCommand.clear();
        tmpCommand.append("sudo rm "+ *videoID);
        funcRemoteTerminalCommand(
                                    tmpCommand.toStdString(),
                                    camSelected,
                                    0,
                                    false,
                                    &commandExecuted
                                );
        if( !commandExecuted )
        {
            funcShowMsgERROR_Timeout("Deleting Remote videoID",this);
            return (void)false;
        }
    }

    //-----------------------------------------------------
    // Save snapshots settings
    //-----------------------------------------------------
    // Get camera resolution
    int camMP       = (ui->radioRes5Mp->isChecked())?5:8;
    camRes          = getCamRes(camMP);

    //Getting calibration
    lstDoubleAxisCalibration daCalib;
    funcGetCalibration(&daCalib);

    //Save lastest settings
    if( saveRaspCamSettings( _PATH_LAST_SNAPPATH ) == false ){
        funcShowMsg("ERROR","Saving last snap-settings");
        return (void)false;
    }

    //-----------------------------------------------------
    //Start to Record Remote Video
    //-----------------------------------------------------

    // Generate Video Command
    QString getRemVidCommand = genRemoteVideoCommand(*videoID,ROI);

    // Execute Remote Command
    funcRemoteTerminalCommand(
                                getRemVidCommand.toStdString(),
                                camSelected,
                                ui->slideTriggerTime->value(),
                                false,
                                &commandExecuted
                            );
    if( !commandExecuted )
    {
        funcShowMsgERROR_Timeout("Starting Remote Recording",this);
        return (void)false;
    }

    //-----------------------------------------------------
    //Display Timer
    //-----------------------------------------------------
    //Before to Start Recording
    funcDisplayTimer("Countdown to Recording...",ui->slideTriggerTime->value(),Qt::black);

    //During Recording
    funcDisplayTimer("Recording...",ui->spinBoxVideoDuration->value(),Qt::red);
}

void MainWindow::funcMainCall_GetSnapshot()
{
    //---------------------------------------------------
    //Get Snapshot-ID Destine
    //---------------------------------------------------
    QString fileName;
    fileName = funcGetParam("Snapshot-ID");
    if(fileName.isEmpty())
    {
        funcShowMsgERROR_Timeout("Invalid Snapshot-ID",this);
        return (void)false;
    }

    //---------------------------------------------------
    //Validate File/Dir Name
    //---------------------------------------------------
    QString remoteFile, localFile;
    remoteFile = _PATH_REMOTE_FOLDER_SNAPSHOTS + fileName + _SNAPSHOT_REMOTE_EXTENSION;

    //Local File
    QString syncFolder;
    syncFolder = funcGetSyncFolder();
    localFile.clear();
    localFile.append(syncFolder);
    localFile.append(_PATH_LOCAL_FOLDER_SNAPSHOTS);
    localFile.append(fileName);
    localFile.append(_SNAPSHOT_REMOTE_EXTENSION);

    if( !funcValidateFileDirNameDuplicated( remoteFile, localFile ) )
    {
        funcShowMsgERROR_Timeout("Snapshot-ID Exists: Please, use another",this);
        return (void)false;
    }

    //--------------------------------------
    //Prepare Remote Scenary
    //--------------------------------------

    //Delete Remote File if Exists
    fileName = _PATH_REMOTE_FOLDER_SNAPSHOTS + fileName + _SNAPSHOT_REMOTE_EXTENSION;
    QString tmpCommand;
    tmpCommand.clear();
    tmpCommand.append("sudo rm "+ fileName);
    bool commandExecuted;
    funcRemoteTerminalCommand(tmpCommand.toStdString(),camSelected,0,false,&commandExecuted);
    if( !commandExecuted )
    {
        funcShowMsgERROR_Timeout("Deleting Remote Snapshot-ID",this);
        return (void)false;
    }

    //-----------------------------------------------------
    // Save snapshots settings
    //-----------------------------------------------------

    // Get camera resolution
    int camMP       = (ui->radioRes5Mp->isChecked())?5:8;
    camRes          = getCamRes(camMP);

    //Getting calibration
    lstDoubleAxisCalibration daCalib;
    funcGetCalibration(&daCalib);

    //Save lastest settings
    if( saveRaspCamSettings( _PATH_LAST_SNAPPATH ) == false )
    {
        funcShowMsgERROR_Timeout("Saving last Snap-settings",this);
        return (void)false;
    }

    //-----------------------------------------------------
    // Take Remote Snapshot
    //-----------------------------------------------------
    if( !takeRemoteSnapshot(fileName,false) )
    {
        funcShowMsgERROR_Timeout("Taking Full Area Snapshot",this);
        return (void)false;
    }

    //-----------------------------------------------------
    //Start Timer
    //-----------------------------------------------------
    int triggeringTime;
    triggeringTime = ui->slideTriggerTime->value();
    if( triggeringTime > 0 )
    {
        formTimerTxt* timerTxt = new formTimerTxt(this,"Remainning Time to Shoot...",triggeringTime);
        timerTxt->setModal(true);
        timerTxt->show();
        QtDelay(200);
        timerTxt->startMyTimer(triggeringTime);
    }
}

void MainWindow::on_actionFull_Video_triggered()
{
    bool ok;

    //-----------------------------------------------------
    // Record Remote Video
    //-----------------------------------------------------
    QString videoID;
    funcMainCall_RecordVideo(&videoID,true,false);

    //-----------------------------------------------------
    // Obtain Remote Video
    //-----------------------------------------------------
    if( obtainFile( _PATH_VIDEO_REMOTE_H264, _PATH_VIDEO_RECEIVED_H264, "Transmitting Remote Video" ) )
    {
        QString rmRemVidCommand;
        rmRemVidCommand.append("rm ");
        rmRemVidCommand.append(_PATH_VIDEO_REMOTE_H264);
        funcRemoteTerminalCommand( rmRemVidCommand.toStdString(), camSelected, 0, false, &ok );
    }
    else
    {
        return (void)false;
    }

    //-----------------------------------------------------
    // Extract Frames from Video
    //-----------------------------------------------------

    //Clear local folder
    QString tmpFramesPath;
    tmpFramesPath.append(_PATH_VIDEO_FRAMES);
    tmpFramesPath.append("tmp/");
    funcClearDirFolder( tmpFramesPath );

    //Extract Frames from Local Video
    QString locFrameExtrComm;
    locFrameExtrComm.append("ffmpeg -framerate ");
    locFrameExtrComm.append(QString::number(_VIDEO_FRAME_RATE));
    locFrameExtrComm.append(" -r ");
    locFrameExtrComm.append(QString::number(_VIDEO_FRAME_RATE));
    locFrameExtrComm.append(" -i ");
    locFrameExtrComm.append(_PATH_VIDEO_RECEIVED_H264);
    locFrameExtrComm.append(" ");
    locFrameExtrComm.append(tmpFramesPath);
    locFrameExtrComm.append("%d");
    locFrameExtrComm.append(_FRAME_EXTENSION);
    qDebug() << locFrameExtrComm;
    progBarUpdateLabel("Extracting Frames from Video",0);
    if( !funcExecuteCommand(locFrameExtrComm) )
    {
        funcShowMsg("ERROR","Extracting Frames from Video");
    }
    progBarUpdateLabel("",0);


    //-----------------------------------------------------
    // Update mainImage from Frames
    //-----------------------------------------------------
    funcUpdateImageFromFolder(tmpFramesPath,_FRAME_EXTENSION);



    funcResetStatusBar();
}


int MainWindow::funcDisplayTimer(QString title, int timeMs, QColor color)
{
    if( timeMs > 0 )
    {
        formTimerTxt* timerTxt = new formTimerTxt(this,title,timeMs,color);
        timerTxt->setModal(true);
        timerTxt->show();
        QtDelay(200);
        timerTxt->startMyTimer(timeMs);
    }
    return 1;
}

int MainWindow::func_MultiImageMerge( QString type )
{
    mouseCursorWait();

    QStringList lstImages;
    if( func_getMultiImages(&lstImages, this) == _OK && lstImages.size() < 2 )
    {
        funcShowMsgERROR_Timeout("Insufficient Selected Images",this,2000);
        mouseCursorReset();
        return _ERROR;
    }
    else
    {
        QImage imgDestine(lstImages.first());
        lstImages.removeFirst();

        //Merge Images
        while( !lstImages.isEmpty() )
        {
            QImage imgSource(lstImages.first());
            lstImages.removeFirst();
            if( func_MergeImages(&imgSource,&imgDestine,type) == _ERROR )
            {
                mouseCursorReset();
                return _ERROR;
            }
        }
        //Get Filename
        mouseCursorReset();
        QString fileName;
        if(
                func_getFilenameFromUser(
                                            &fileName,
                                            _PATH_LAST_LINE_SAVED,
                                            "./snapshots/",
                                            this
                                        ) == _OK
        ){
            mouseCursorWait();
            imgDestine.save(fileName);
            mouseCursorReset();
        }
        return _OK;
    }
}

void MainWindow::on_actionMultiImageAverage_triggered()
{
    if( func_MultiImageMerge("Average") == _ERROR )
    {
        funcShowMsgERROR("Merging Images");
    }
}

void MainWindow::on_actionMultiImageMinimum_triggered()
{
    if( func_MultiImageMerge("Minimum") == _ERROR )
    {
        funcShowMsgERROR("Merging Images");
    }
}

void MainWindow::on_actionMultiImageMaximum_triggered()
{
    if( func_MultiImageMerge("Maximum") == _ERROR )
    {
        funcShowMsgERROR("Merging Images");
    }
}

void MainWindow::on_actionSlide_Settings_triggered()
{
    //Get Slide Area
    QString slideArea;
    slideArea = readAllFile(_PATH_SLIDE_DIFFRACTION);
    if( slideArea == _ERROR_FILE_NOTEXISTS || slideArea == _ERROR_FILE )
    {
        funcShowMsgERROR_Timeout(slideArea,this);
        return (void)false;
    }


    //Create File-Contain
    QString fileContain;
    fileContain.append("echo '");
    fileContain.append(slideArea);
    fileContain.append("' > ");
    fileContain.append(_PATH_REMOTE_TMPSETTINGS);

    //Excecute Remote Command
    bool ok;
    funcRemoteTerminalCommand( fileContain.toStdString(), camSelected, 0, false, &ok );
    if(ok==true)
        funcShowMsgSUCCESS_Timeout("Settings Send",this);
    else
        funcShowMsgERROR_Timeout("Sending Settings",this);





}

void MainWindow::on_actionLinear_Regression_triggered()
{
    formGenLinearRegression* tmpForm = new formGenLinearRegression(this);
    tmpForm->setModal(true);
    tmpForm->show();
}

void MainWindow::on_actionDiffraction_Origin_triggered()
{
    //Get Filename
    QString filePath = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select File..."),
                                                        _PATH_CALIB,
                                                        "(*.hypcam);;"
                                                     );
    //Add Row
    if( !filePath.isEmpty() )
    {
        if( saveFile(_PATH_CALIB_LR_TMP_ORIGIN,filePath) == false )
        {
            funcShowMsgERROR("Saving: " + QString(_PATH_CALIB_LR_TMP_ORIGIN) );
        }
        else
            funcShowMsgSUCCESS_Timeout("Diffraction Origin Set",this);
    }
    else
    {
        funcShowMsgERROR("Origin was not Set");
    }
}

void MainWindow::on_openLine_triggered()
{
    //---------------------------------------
    //Let the user select the file
    //---------------------------------------
    QString fileOrigin;
    if( funcLetUserSelectFile(&fileOrigin) != _OK )
    {
        return (void)false;
    }

    //---------------------------------------
    //Draw Line
    //---------------------------------------
    funcDrawLineIntoCanvasEdit(fileOrigin);
}

void MainWindow::on_actionSlide_Linear_Regression_triggered()
{
    formSlideLinearRegression* tmpForm = new formSlideLinearRegression(this);
    tmpForm->setModal(true);
    tmpForm->show();
}

void MainWindow::on_actionarcLine_triggered()
{
    funcShowMsg_Timeout("Ups!!","Command not implemented yet.");
}

bool MainWindow::funcDrawLineIntoCanvasEdit(const QString &filePath)
{
    //---------------------------------------
    //Read the line from XML
    //---------------------------------------
    structLine tmpLineData;
    if( funcReadLineFromXML((QString*)&filePath, &tmpLineData) != _OK )
    {
        funcShowMsgERROR_Timeout("Opening: " + filePath, this);
        return _ERROR;
    }

    //---------------------------------------
    //Draw Line into canvasEdit
    //---------------------------------------    
    tmpLineData.x1 = round(
                            (
                                (float)tmpLineData.x1 /
                                (float)tmpLineData.originalW
                            ) * (float)canvasCalib->width()
                          );
    tmpLineData.y1 = round(
                            (
                                (float)tmpLineData.y1 /
                                (float)tmpLineData.originalH
                            ) * (float)canvasCalib->height()
                          );
    tmpLineData.x2 = round(
                            (
                                (float)tmpLineData.x2 /
                                (float)tmpLineData.originalW
                            ) * (float)canvasCalib->width()
                          );
    tmpLineData.y2 = round(
                            (
                                (float)tmpLineData.y2 /
                                (float)tmpLineData.originalH
                            ) * (float)canvasCalib->height()
                          );
    QPoint p1(tmpLineData.x1,tmpLineData.y1);
    QPoint p2(tmpLineData.x2,tmpLineData.y2);
    customLine* tmpLine = new customLine(
                                            p1,
                                            p2,
                                            QPen(QColor(qRgb(
                                                                tmpLineData.colorR,
                                                                tmpLineData.colorG,
                                                                tmpLineData.colorB
                                            )))
                                        );
    tmpLine->parameters.orientation = tmpLineData.oritation;
    tmpLine->parameters.originalW   = tmpLineData.originalW;
    tmpLine->parameters.originalH   = tmpLineData.originalH;
    tmpLine->parameters.wavelength  = tmpLineData.wavelength;
    tmpLine->parentSize.setWidth(canvasCalib->scene()->width());
    tmpLine->parentSize.setHeight(canvasCalib->scene()->height());
    canvasCalib->scene()->addItem( tmpLine );
    canvasCalib->update();
    return _OK;

}

void MainWindow::on_actionPlot_Calculated_Line_triggered()
{
    //---------------------------------------
    //Get Line from User
    //---------------------------------------
    //Get filename
    QString fileName;
    if( funcLetUserSelectFile(&fileName,"Select Horizontal Calibration...") != _OK )
    {
        funcShowMsgERROR_Timeout("Getting File from User");
        return (void)false;
    }
    //Get Line parameters from .XML
    structHorizontalCalibration horizCalib;
    funcReadHorizCalibration(&fileName,&horizCalib);

    //---------------------------------------
    //Plot Calculated Line
    //---------------------------------------
    int newX, newY;
    for(newX=1; newX<canvasCalib->width(); newX+=5)
    {
        newY    = round( (horizCalib.b*newX) + horizCalib.a );
        //std::cout << "(" << horizCalib.b << "*" << newX << ") + " << horizCalib.a << " = " << newY << std::endl;
        newY    = round( ((float)newY/(float)horizCalib.imgH) * (float)canvasCalib->height());
        double rad = 1.0;
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(newX, newY, rad, rad );
        item->setPen( QPen(Qt::white) );
        item->setBrush( QBrush(Qt::white) );
        canvasCalib->scene()->addItem(item);
    }

}

void MainWindow::on_actionPlot_Upper_Line_Rotation_triggered()
{
    //---------------------------------------
    //Get Line from User
    //---------------------------------------
    //Get filename
    QString fileName;
    if( funcLetUserSelectFile(&fileName) != _OK )
    {
        funcShowMsgERROR_Timeout("Getting File from User");
        return (void)false;
    }
    //Get Calibration Parameters from .XML
    structVerticalCalibration tmpVertCal;
    funcReadVerticalCalibration(&fileName,&tmpVertCal);

    //---------------------------------------
    //Plot Calculated Line
    //---------------------------------------
    int newX, newY;
    for(newY=1; newY<canvasCalib->height(); newY+=5)
    {
        newX    = (tmpVertCal.vertLR.b*newY) + tmpVertCal.vertLR.a;
        newX    = ((float)newX/(float)tmpVertCal.imgW) * canvasCalib->width();//Fit to Canvas size
        double rad = 1.0;
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(newX, newY, rad, rad );
        item->setPen( QPen(Qt::white) );
        item->setBrush( QBrush(Qt::white) );
        canvasCalib->scene()->addItem(item);
    }

}

int MainWindow::funcReadVerticalCalibration(
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
            if( xmlReader->name()=="wavelengthA" )
                vertCal->wavelengthLR.a = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="wavelengthB" )
                vertCal->wavelengthLR.b = xmlReader->readElementText().trimmed().toFloat(0);
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

int MainWindow::funcReadHorizCalibration(QString* filePath, structHorizontalCalibration* horizCalib)
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
            if( xmlReader->name()=="imgW" )
                horizCalib->imgW = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="imgH" )
                horizCalib->imgH = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="H" )
                horizCalib->H = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="a" )
                horizCalib->a = xmlReader->readElementText().trimmed().toFloat(0);
            if( xmlReader->name()=="b" )
                horizCalib->b = xmlReader->readElementText().trimmed().toFloat(0);
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

void MainWindow::on_actionOrigin_triggered()
{
    //---------------------------------------
    //Get Horizontal Calibration
    //---------------------------------------
    //Get filename
    QString fileName;
    if( funcLetUserSelectFile(&fileName, "Select Horizontal Calibration") != _OK )
    {
        funcShowMsgERROR_Timeout("Getting Horizontal Calibration");
        return (void)false;
    }
    //Get Line parameters from .XML
    structHorizontalCalibration tmpHorizontalCal;
    funcReadHorizCalibration(&fileName,&tmpHorizontalCal);

    //---------------------------------------
    //Get Vertical Calibration
    //---------------------------------------
    //Get filename
    if( funcLetUserSelectFile(&fileName,"Select Vertical Calibration") != _OK )
    {
        funcShowMsgERROR_Timeout("Getting Vertical Calibration");
        return (void)false;
    }
    //Get Calibration Parameters from .XML
    structVerticalCalibration tmpVertCal;
    funcReadVerticalCalibration(&fileName,&tmpVertCal);

    //---------------------------------------
    //Calc Origin
    //---------------------------------------
    float horA, horB, horY;
    float verA, verB, verX;
    //Prefill Coordinates
    horA  = tmpHorizontalCal.a;
    horB  = tmpHorizontalCal.b;
    verA  = tmpVertCal.vertLR.a;
    verB  = tmpVertCal.vertLR.b;
    horY  = round( (tmpVertCal.x2 * horB) + horA );
    verX  = round( (verB * horY) + verA );

    //---------------------------------------
    //Add Big Point
    //---------------------------------------
    float x, y;
    x   = verX;
    y   = horY;
    x   = round( ((float)x/(float)tmpHorizontalCal.imgW) * (float)canvasCalib->width() );
    y   = round( ((float)y/(float)tmpHorizontalCal.imgH) * (float)canvasCalib->height() );
    //std::cout << x << "," << y << std::endl;
    double rad = 10.0;
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(x-(rad/2.0), y-(rad/2.0), rad, rad );
    item->setPen( QPen(Qt::magenta) );
    item->setBrush( QBrush(Qt::white) );
    canvasCalib->scene()->addItem(item);

}

void MainWindow::on_actionBuld_HypImg_triggered()
{
    //--------------------------------
    //Read Calibrations
    //--------------------------------
    QString calFilename;
    calFilename = readAllFile(_PATH_SLIDE_ACTUAL_CALIB_PATH).trimmed();
    std::cout << "fileContain: " << calFilename.toStdString() << std::endl;
    structSlideCalibration slideCalibration;
    if( funcReadSlideCalib(calFilename,&slideCalibration) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Slide Calibration");
        return (void)false;
    }

    //--------------------------------
    //Define Video Origin
    //--------------------------------
    QString videoPath;
    videoPath.append(_PATH_LOCAL_SYNC_FOLDERS);
    videoPath.append(_PATH_LOCAL_FOLDER_VIDEOS);
    if(
            funcLetUserSelectFile(
                                    &videoPath,
                                    "Select Hyperspectral Image Source..."
                                 ) != _OK
    ){
        return (void)false;
    }

    //-----------------------------------------------------
    // Extract Frames from Video
    //-----------------------------------------------------
    //Clear local folder
    QString tmpFramesPath;
    tmpFramesPath.append(_PATH_VIDEO_FRAMES);
    tmpFramesPath.append("tmp/");
    funcClearDirFolder( tmpFramesPath );
    //Extract Frames from Local Video
    QString locFrameExtrComm;
    locFrameExtrComm.append("ffmpeg -framerate ");
    locFrameExtrComm.append(QString::number(_VIDEO_FRAME_RATE));
    locFrameExtrComm.append(" -r ");
    locFrameExtrComm.append(QString::number(_VIDEO_FRAME_RATE));
    locFrameExtrComm.append(" -i ");
    locFrameExtrComm.append(videoPath);
    locFrameExtrComm.append(" ");
    locFrameExtrComm.append(tmpFramesPath);
    locFrameExtrComm.append("%d");
    locFrameExtrComm.append(_FRAME_EXTENSION);
    qDebug() << locFrameExtrComm;
    progBarUpdateLabel("Extracting Frames from Video",0);
    if( funcExecuteCommand(locFrameExtrComm) != _OK )
    {
        funcShowMsg("ERROR","Extracting Frames from Video");
        progBarUpdateLabel("",0);
        return (void)false;
    }
    progBarUpdateLabel("",0);

    //--------------------------------
    //List Files in Folder
    //--------------------------------
    QList<QFileInfo> lstFrames = funcListFilesInDir(tmpFramesPath);
    std::cout << "numFrames: " << lstFrames.size() << std::endl;

    //--------------------------------
    //Update Progress Bar
    //--------------------------------
    progBarUpdateLabel("Building Slide Hyperspectral Image",0);
    ui->progBar->setVisible(true);
    ui->progBar->setValue(0);
    ui->progBar->update();
    QtDelay(10);

    //********************************
    //Build Slide Hyperspectral Image
    //********************************
    int x, y, z;
    int hypX    = lstFrames.size();
    int hypY    = slideCalibration.originH;
    int hypZ    = slideCalibration.maxNumCols;

    //--------------------------------
    //Reserve HypImg Dynamic Memory
    //--------------------------------
    int*** HypImg;//[hypX][hypY][hypZ];
    HypImg = (int***)malloc(hypX*sizeof(int**));
    for(x=0; x<hypX; x++)
    {
        HypImg[x] = (int**)malloc( hypY*sizeof(int*) );
        for(y=0; y<hypY; y++)
        {
            HypImg[x][y] = (int*)malloc( hypZ*sizeof(int) );
        }
    }

    //--------------------------------
    //Copy values int HypImg
    //--------------------------------
    QImage tmpActImg;    
    float pixQE;
    for(x=0; x<hypX; x++)
    {
        //Load Image (Column in the HypImg)
        tmpActImg = QImage(lstFrames.at(x).absoluteFilePath().trimmed());

        //Update Progress Bar
        ui->progBar->setValue(round( ((float)x/(float)hypX)*100.0 ));
        ui->progBar->update();

        //Copy Diffraction Into Slide Hyperspectral Image
        for(y=0; y<hypY; y++)
        {
            for(z=0; z<hypZ; z++)
            {
                pixQE = 0.0;
                if(
                        funcGetPixQE(
                                        &z,
                                        &y,
                                        &pixQE,
                                        tmpActImg,
                                        &slideCalibration
                                    ) != _OK
                ){
                    funcShowMsgERROR_Timeout("Pixel Coordinates Out of Range");
                    //Free Dynamic Memory
                    for(x=0; x<hypX; x++)
                    {
                        for(y=0; y<hypY; y++)
                        {
                            free( HypImg[x][y] );
                        }
                        free( HypImg[x] );
                    }
                    free(HypImg);
                    //Reset Progress Bar
                    funcResetStatusBar();
                    //Finish
                    return (void)false;
                }
                HypImg[x][y][z] = (int)pixQE;
            }
        }
    }

    //--------------------------------
    //Save Slide HypImg Layers
    //--------------------------------
    //Update Progress Bar
    progBarUpdateLabel("Exporting Hyperspectral Image",0);
    ui->progBar->setVisible(true);
    ui->progBar->setValue(0);
    ui->progBar->update();
    QtDelay(10);
    //Clear folder destine
    funcClearDirFolder( _PATH_LOCAL_SLIDE_HYPIMG );
    //Copy Layer into Image and Save Later
    QString imgOutname;
    QImage tmpLayer(QSize(hypX,hypY),QImage::Format_RGB32);
    for(z=0; z<hypZ; z++)
    {
        //Update Progbar
        ui->progBar->setValue(round( ((float)z/(float)hypZ)*100.0 ));
        ui->progBar->update();
        //Fill Image Pixels
        for(x=0; x<hypX; x++)
        {
            for(y=0; y<hypY; y++)
            {
                tmpLayer.setPixelColor(x,y,QColor(HypImg[x][y][z],HypImg[x][y][z],HypImg[x][y][z]));
            }
        }
        //Save image
        imgOutname.clear();
        imgOutname.append(_PATH_LOCAL_SLIDE_HYPIMG);
        imgOutname.append(QString::number(z+1));
        imgOutname.append(_FRAME_EXTENSION);
        tmpLayer.save(imgOutname);
    }

    //--------------------------------
    //Free Dynamic Memory
    //--------------------------------
    for(x=0; x<hypX; x++)
    {
        for(y=0; y<hypY; y++)
        {
            free( HypImg[x][y] );
        }
        free( HypImg[x] );
    }
    free(HypImg);

    //Reset Progress Bar
    funcResetStatusBar();

    //Finish
    funcShowMsgSUCCESS_Timeout("Hyperspectral Image Exported Successfully");

}

void MainWindow::on_actionMerge_Calibration_triggered()
{
    formMergeSlideCalibrations* tmpForm = new formMergeSlideCalibrations(this);
    tmpForm->setModal(true);
    tmpForm->show();
}


int MainWindow::getCamMP()
{
    if( ui->radioRes5Mp->isChecked() )
        return 5;
    if( ui->radioRes8Mp->isChecked() )
        return 8;
    return 8;
}

void MainWindow::on_actionPlot_over_Real_triggered()
{
    //----------------------------------------------
    //Let the user select Slide Calibration File
    //----------------------------------------------
    QString calibPath;
    if( funcLetUserSelectFile(&calibPath,"Select Slide Calibration File...") != _OK )
    {
        return (void)false;
    }

    //----------------------------------------------
    //Read Slide Calibration File
    //----------------------------------------------
    structSlideCalibration slideCalibration;
    if( funcReadSlideCalib( calibPath, &slideCalibration ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Slide Calibration File");
        return (void)false;
    }

    //**********************************************
    //Draw Over Real Image
    //**********************************************

    //----------------------------------------------
    //Validate Calibration-Size and Image-Size
    //----------------------------------------------
    if(
            slideCalibration.imgW != globalEditImg->width()     ||
            slideCalibration.imgH != globalEditImg->height()
    ){
        funcShowMsgERROR_Timeout("Image Size and Clibration Size are Different");
        return (void)false;
    }

    //----------------------------------------------
    //Draw Vertical Lower Bound Line
    //----------------------------------------------
    QPoint newPoint;
    int x, y;
    x=0;
    for( y=0; y<=slideCalibration.originH; y++ )
    {
        newPoint = funcGetCoor(x,y,&slideCalibration,false);
        globalEditImg->setPixelColor(newPoint,QColor(255,255,255));
    }

    //----------------------------------------------
    //Draw Vertical Upper Bound Line
    //----------------------------------------------
    x=slideCalibration.maxNumCols;
    for( y=0; y<=slideCalibration.originH; y++ )
    {
        newPoint = funcGetCoor(x,y,&slideCalibration,false);
        globalEditImg->setPixelColor(newPoint,QColor(255,255,255));
    }

    //----------------------------------------------
    //Draw Horizontal Upper Line
    //----------------------------------------------
    int x1, y1, x2, y2, len;
    y=0;
    for( x=0; x<=slideCalibration.maxNumCols; x++ )
    {
        newPoint = funcGetCoor(x,y,&slideCalibration,false);
        globalEditImg->setPixelColor(newPoint,QColor(255,255,255));
    }

    //----------------------------------------------
    //Draw Horizontal Lower Line
    //----------------------------------------------
    y = slideCalibration.originH;
    for( x=0; x<=slideCalibration.maxNumCols; x++ )
    {
        QPoint newPoint = funcGetCoor(x,y,&slideCalibration,false);
        globalEditImg->setPixelColor(newPoint,QColor(255,255,255));
    }

    //----------------------------------------------
    //Draw Origen
    //----------------------------------------------
    len  = 2;
    x1   = slideCalibration.originX - len;
    y1   = slideCalibration.originY - len;
    x2   = slideCalibration.originX + len;
    y2   = slideCalibration.originY + len;
    for( x=x1; x<=x2; x++ )
    {
        for( y=y1; y<=y2; y++ )
        {
            globalEditImg->setPixelColor(x,y,QColor(255,255,255));
        }
    }

    //----------------------------------------------
    //Draw Internal in order to check
    //----------------------------------------------
    x=slideCalibration.maxNumCols;
    for( x=0; x<=slideCalibration.maxNumCols; x+=10 )
    {
        for( y=0; y<=slideCalibration.originH; y+=10 )
        {
            newPoint = funcGetCoor(x,y,&slideCalibration,false);
            globalEditImg->setPixelColor(newPoint,QColor(255,0,0));
        }
    }

    //----------------------------------------------
    //Update Image Displayed in Canvas
    //----------------------------------------------
    updateDisplayImage(globalEditImg);

    //----------------------------------------------
    //Save Image
    //----------------------------------------------
    if( funcShowMsgYesNo("Saving Image","Save image?") )
    {
        //filePath:         File output, filename selected by the user
        //title:            Showed to User, what kind of file is the user selecting
        //pathLocation:     Where is saved the last path location saved
        //pathOfInterest:   If it is the first time, what path will be saved as default
        //parent:           In order to use this Dialog
        QString fileName;
        if(
                funcLetUserDefineFile(
                                        &fileName,
                                        "Define Image Filename",
                                        ".png",
                                        new QString(_PATH_LAST_IMG_OPEN),
                                        QString(_PATH_LAST_IMG_OPEN),
                                        this
                                     ) != _OK
        ){
            return (void)false;
        }
        //Save Image
        globalEditImg->save(fileName);
        //Notify Success
        funcShowMsgSUCCESS_Timeout("Image Saved Successfully");
    }
}

void MainWindow::on_actionPlot_Line_at_Wavelength_triggered()
{
    //----------------------------------------------
    //Let the user define Wavelength to Show
    //----------------------------------------------
    float wavelength;
    wavelength = funcGetParam("Wavelength","450").trimmed().toFloat(0);
    if( wavelength < 300 || wavelength > 1200 )
    {
        funcShowMsgERROR_Timeout("Wavelength incorrect");
        return (void)false;
    }

    //**********************************************
    //Display the Wavalength Selected by User
    //**********************************************

    //----------------------------------------------
    //Let the user select Slide Calibration File
    //----------------------------------------------
    //QString calibPath("./XML/slideCalibration/slideCam_002.xml");
    QString calibPath;
    if( funcLetUserSelectFile(&calibPath,"Select Slide Calibration File...") != _OK )
    {
        return (void)false;
    }

    //----------------------------------------------
    //Read Slide Calibration File
    //----------------------------------------------
    structSlideCalibration slideCalibration;
    if( funcReadSlideCalib( calibPath, &slideCalibration ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Slide Calibration File");
        return (void)false;
    }
    //std::cout << "calibPath: " << calibPath.toStdString() << std::endl;
    //std::cout << "1) slideCalibration.originWave: " << slideCalibration.originWave << std::endl;

    //**********************************************
    //Draw Over Real Image
    //**********************************************

    //----------------------------------------------
    //Validate Calibration-Size and Image-Size
    //----------------------------------------------
    if(
            slideCalibration.imgW != globalEditImg->width()     ||
            slideCalibration.imgH != globalEditImg->height()
    ){
        funcShowMsgERROR_Timeout("Image Size and Clibration Size are Different");
        return (void)false;
    }

    //----------------------------------------------
    //Calculate Distance from Lower Bound
    //----------------------------------------------
    int distPixFromLower;
    wavelength = wavelength - slideCalibration.originWave;
    distPixFromLower = round( funcApplyLR(wavelength,&slideCalibration.wave2DistLR,true) );
    std::cout << "distPixFromLower: " << distPixFromLower << "px" << std::endl;

    //----------------------------------------------
    //Draw Vertical Line
    //----------------------------------------------
    QPoint newPoint;
    int x = distPixFromLower;
    int y;
    for( y=0; y<=slideCalibration.originH; y++ )
    {
        newPoint = funcGetCoor(x,y,&slideCalibration,false);
        globalEditImg->setPixelColor(newPoint,QColor(255,255,255));
    }

    //----------------------------------------------
    //Update Image Displayed in Canvas
    //----------------------------------------------
    updateDisplayImage(globalEditImg);

    //----------------------------------------------
    //Save Image
    //----------------------------------------------
    if( funcShowMsgYesNo("Saving Image","Save image?") )
    {
        //filePath:         File output, filename selected by the user
        //title:            Showed to User, what kind of file is the user selecting
        //pathLocation:     Where is saved the last path location saved
        //pathOfInterest:   If it is the first time, what path will be saved as default
        //parent:           In order to use this Dialog
        QString fileName;
        if(
                funcLetUserDefineFile(
                                        &fileName,
                                        "Define Image Filename",
                                        ".png",
                                        new QString(_PATH_LAST_IMG_OPEN),
                                        QString(_PATH_LAST_IMG_OPEN),
                                        this
                                     ) != _OK
        ){
            return (void)false;
        }
        //Save Image
        globalEditImg->save(fileName);
        //Notify Success
        funcShowMsgSUCCESS_Timeout("Image Saved Successfully");
    }

}

void MainWindow::on_actionSlide_Calibration_File_triggered()
{
    //---------------------------------------
    //Select Slide Calibration Origin
    //---------------------------------------
    QString slideFile;
    if( funcLetUserSelectFile(&slideFile,"Select Slide Calibration File...") != _OK )
    {
        return (void)false;
    }

    //---------------------------------------
    //Save Parameter
    //---------------------------------------
    if( saveFile(_PATH_SLIDE_ACTUAL_CALIB_PATH,slideFile) == true )
    {
        funcShowMsgSUCCESS_Timeout("File Set Successfully");
    }
}
