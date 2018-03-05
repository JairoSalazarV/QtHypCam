#include "slidehypcam.h"
#include "ui_slidehypcam.h"

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <slidehypcam.h>
#include <__common.h>

#include <mainwindow.h>
#include <rasphypcam.h>

#include <arduinomotor.h>

#include <QThread>

#include <formslidesettings.h>
#include <QDir>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QGraphicsPixmapItem>
#include <QScreen>

#include <lstfilenames.h>

GraphicsView *slideCanvas;

slideHypCam::slideHypCam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::slideHypCam)
{
    ui->setupUi(this);

    slideCanvas = new GraphicsView(this);

    //
    //Create slide if does not exists previously
    //
    if( !fileExists(_PATH_SLIDE_TO_DISPLAY) )
    {
        funcGenerateSlideImage();
        qDebug() << "Slide created";
    }

    //
    //Display Slide
    //
    funcUpdateSlideToDisplay();

}

slideHypCam::~slideHypCam()
{
    delete ui;
}

void slideHypCam::funcUpdateSlideToDisplay()
{

    QDesktopWidget* desktop = new QDesktopWidget();
    QRect  screenGeometry = desktop->screenGeometry( desktop->screenNumber(this) );

    /*
    QString tmpMsg;
    tmpMsg = "W: " + QString::number(screenGeometry.width());
    tmpMsg.append( " H: " + QString::number(screenGeometry.height()) );
    funcShowMsg("Cambió",tmpMsg);*/

    QPixmap imgToDisp( _PATH_SLIDE_TO_DISPLAY );
    //QRect screen = QApplication::desktop()->screenGeometry();

    int canvasW, canvasH, screenMaxW, screenMaxH;
    screenMaxW = screenGeometry.width() - _PADDING_W;
    screenMaxH = screenGeometry.height() - _PADDING_H;
    canvasW = ( imgToDisp.width() < screenMaxW )?imgToDisp.width():screenMaxW;
    canvasH = ( imgToDisp.height() < screenMaxH )?imgToDisp.height():screenMaxH;
    canvasW += 25;
    canvasH += 25;
    //qDebug() << "canvasW: " << canvasW << "canvasH: " << canvasH;
    QGraphicsScene* tmpScene = new QGraphicsScene(0,0,imgToDisp.width(),imgToDisp.height());
    //tmpScene->setBackgroundBrush( imgToDisp );
    tmpScene->addPixmap(imgToDisp);
    //slideCanvas->resize(canvasW,canvasH);
    slideCanvas->setFixedSize(canvasW,canvasH);
    slideCanvas->setScene( tmpScene );
    //slideCanvas->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //slideCanvas->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    QLayout* layout = new QVBoxLayout();
    layout->addWidget(slideCanvas);
    ui->centralwidget->setLayout(layout);
    ui->centralwidget->update();


}

void slideHypCam::funcGenerateSlideImage()
{
    QString tmpParam;

    //
    //Obtain Slide image based on last slide getted
    //

    QString tmpFileName;

    //Read number of images
    tmpFileName = _PATH_SLIDE_TMP_FOLDER;
    tmpFileName.append(_FILENAME_SLIDE_PARAM_NUM_IMGS);
    tmpParam = readFileParam( tmpFileName );
    if( tmpParam.isEmpty() )
    {
        qDebug() << "ERROR reading slide number of images from " << _FILENAME_SLIDE_PARAM_NUM_IMGS;
        return (void)NULL;
    }
    int numSlides = tmpParam.toInt(0);
    if( numSlides <= 0 )
    {
        qDebug() << "ERROR, numSlides <= 0";
        return (void)NULL;
    }

    //Read slide's width
    tmpFileName = _PATH_SLIDE_TMP_FOLDER;
    tmpFileName.append(_FILENAME_SLIDE_PARAM_W);
    tmpParam = readFileParam( tmpFileName );
    if( tmpParam.isEmpty() )
    {
        qDebug() << "ERROR reading slide width from " << _FILENAME_SLIDE_PARAM_W;
        return (void)NULL;
    }
    int slideW = tmpParam.toInt(0);
    if( slideW <= 0 )
    {
        qDebug() << "ERROR, Slide width is corrupt";
        return (void)NULL;
    }

    //Read slide's height
    tmpFileName = _PATH_SLIDE_TMP_FOLDER;
    tmpFileName.append(_FILENAME_SLIDE_PARAM_H);
    tmpParam = readFileParam( tmpFileName );
    if( tmpParam.isEmpty() )
    {
        qDebug() << "ERROR reading slide height from " << _FILENAME_SLIDE_PARAM_H;
        return (void)NULL;
    }
    int slideH = tmpParam.toInt(0);
    if( slideH <= 0 )
    {
        qDebug() << "ERROR, Slide height is corrupt";
        return (void)NULL;
    }

    //
    //Create slide image and display
    //
    int row, col;
    int i=1;
    int maxSteps = 5;
    int actualStep = 1;
    int slidesCut = 0;
    QImage tmpImg((numSlides*slideW), slideH, QImage::Format_RGB888);
    QString tmpImgName;
    bool flagMaxSteps;
    int colIni;
    while( slidesCut < numSlides )
    {
        flagMaxSteps = true;
        for( i=actualStep; i<actualStep+maxSteps; i++ )
        {
            tmpImgName = _PATH_SLIDE_TMP_FOLDER;
            tmpImgName.append(QString::number(i));
            tmpImgName.append(".png");
            if( fileExists( tmpImgName ) )
            {
                flagMaxSteps = false;
                //qDebug() << "File foud: " << tmpImgName;
                break;
            }
        }
        if( flagMaxSteps == true )
        {
            QString tmpError = "ERROR finding next file, exceeds max space between filenames and Slide-Image will not be displayed.\n\n";
            tmpError.append("Last filename used: ");
            tmpError.append(tmpImgName);
            funcShowMsg("ALERT",tmpError);
            break;
        }
        else
        {
            colIni = slidesCut*slideW;
            QImage tmpImgOrig( tmpImgName );
            for( col=0; col<slideW; col++ )
            {
                for( row=0; row<slideH; row++ )
                {
                    tmpImg.setPixelColor(QPoint((colIni+col),row), tmpImgOrig.pixel(QPoint(col,row)) );
                }
            }
            actualStep = i+1;
            slidesCut++;
            //qDebug() << "i: " << i << " slidesCut: " << slidesCut << " actualStep: " << actualStep;
        }
    }

    //
    //Display image
    //
    if( flagMaxSteps == false )
    {
        tmpImg.save(_PATH_SLIDE_TO_DISPLAY);
    }


}

int slideHypCam::funcGetNumImgs( QString path )
{
    QDir recoredDir(path);
    QStringList allFiles = recoredDir.entryList(QDir::Files);//(QDir::Filter::Files,QDir::SortFlag::NoSort)
    return allFiles.size();
}

void slideHypCam::on_actionpbGetSlideCube_triggered()
{

    ui->actionpbGetSlideCube->setEnabled(false);

    MainWindow* internMainWin       = new MainWindow(this);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    camRes = getCamRes( internMainWin->getCamMP() );

    int numImages = 0;
    u_int8_t** lstImgs = internMainWin->funcGetSLIDESnapshot( &numImages, true );

    if( numImages == 0 )
    {
        return (void)NULL;
    }
    else
    {
        delete[] lstImgs;
    }

    QApplication::restoreOverrideCursor();

    ui->actionpbGetSlideCube->setEnabled(true);




    /*
    //Instances MainWindow
    MainWindow* internMainWin       = new MainWindow(this);
    structCamSelected* camSelected  = internMainWin->funcGetCamSelected();

    //Check if camara is selected
    if( camSelected->tcpPort == 0 )
    {
        funcShowMsg("ALERT","Connect to the camera");
        return (void)NULL;
    }

    //Change mouse pointer
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //Obtains camera resolution selected on MainWindow
    camRes = internMainWin->getCamRes();

    //
    //Start motor rotation in an independen thread
    //
    arduinoMotor* motor = new arduinoMotor();
    motor->setAWalk( 0, 180, 60, 800 );
    motor->start();

    //
    //Obtain imagery and put into RAM
    //
    int numImgs;
    u_int8_t** lstImgs = internMainWin->funcGetSLIDESnapshot( &numImgs, true );
    delete[] lstImgs;
    fflush(stdout);

    //Restores mouse pointer
    QApplication::restoreOverrideCursor();
    */

}



void slideHypCam::on_actionSlide_settings_triggered()
{
    formSlideSettings* tmpForm = new formSlideSettings(this);
    tmpForm->setModal(true);
    tmpForm->show();
}


void slideHypCam::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    // Your code here.

    funcUpdateSlideToDisplay();
}
