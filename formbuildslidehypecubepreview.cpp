#include "formbuildslidehypecubepreview.h"
#include "ui_formbuildslidehypecubepreview.h"

#include <lstpaths.h>
#include <QGraphicsPixmapItem>
#include <__common.h>

formBuildSlideHypeCubePreview
::formBuildSlideHypeCubePreview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formBuildSlideHypeCubePreview)
{
    ui->setupUi(this);

    this->showMaximized();

    //------------------------------------------------------
    //Load Last Sttings
    //------------------------------------------------------
    QString lastSlideFrames;
    if( !readFileParam(_PATH_LAST_SLIDE_FRAMES_4CUBE,&lastSlideFrames) )
    {
        lastSlideFrames = concatenateParameters(_OK);
        saveFile(_PATH_LAST_SLIDE_FRAMES_4CUBE,lastSlideFrames);
    }
    setLastExecution(lastSlideFrames);

    //------------------------------------------------------
    //Load Blank Scene into GraphicsView
    //------------------------------------------------------
    QGraphicsScene *scene = new QGraphicsScene(0,0,200,200);
    scene->setBackgroundBrush(QBrush(Qt::black));
    ui->gvSlideHypCubePreview->setScene( scene );
    ui->gvSlideHypCubePreview->update();
    //.......................................................
    //Fix GraphicsView Resolution
    //.......................................................
    QRect screenRes = screenResolution(this);
    int maxW, maxH, xMargin, yMargin;
    xMargin = 20;
    yMargin = 80;
    maxW    = screenRes.width() - xMargin;
    maxH    = screenRes.height() - yMargin;
    ui->gvSlideHypCubePreview->setGeometry(QRect(0,yMargin,maxW,maxH));

    QString framePath = readAllFile(_PATH_LAST_PATH_OPENED).trimmed();
    ui->txtFolder->setText(framePath);

}

formBuildSlideHypeCubePreview::~formBuildSlideHypeCubePreview()
{
    delete ui;
}

void formBuildSlideHypeCubePreview::on_pbApply_clicked()
{
    mouseCursorWait();

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
        mouseCursorReset();
        return (void)false;
    }

    //--------------------------------
    //List Files in Folder
    //--------------------------------
    //Validate Frames Path
    QString framePath;
    if( ui->txtFolder->text().trimmed().isEmpty() )
    {
        funcShowMsgERROR_Timeout("Invalid Folder");
        mouseCursorReset();
        return (void)false;
    }
    framePath = ui->txtFolder->text().trimmed();

    //Get files from Dir
    QList<QFileInfo> lstFrames = funcListFilesInDir(framePath);

    std::cout << "numFrames: " << lstFrames.size() << std::endl;
    if( lstFrames.size() < 2 )
    {
        funcShowMsgERROR_Timeout("The number of files are not enought");
        mouseCursorReset();
        return (void)false;
    }

    //********************************
    //Build Slide Hyperspectral Image
    //********************************
    int i;

    /*
    for(i=0; i<lstFrames.size(); i++)
    {
        std::cout << lstFrames.at(i).absoluteFilePath().toStdString() << std::endl;
    }*/




    int imgW, imgH, slideW;
    slideW  = ui->spinSlideW->value();
    imgW    = slideW * lstFrames.size();
    imgH    = slideCalibration.originH;
    //Reserve Subarea Memory
    int** tmpSubarea = (int**)malloc(imgH * sizeof(int*));
    for(i=0; i<imgH; i++)
    {
        tmpSubarea[i] = (int*)malloc(slideW * sizeof(int));
    }

    //----------------------------------------------
    //Calculate Distance from Lower Bound
    //----------------------------------------------
    float wavelength;
    int newX1, newX2;
    int distPixFromLower;
    wavelength = ui->spinBoxWavelen->value() - slideCalibration.originWave;
    if( wavelength < 0.0 )
    {
        funcShowMsgERROR("Image must to be bigger than "+QString::number(slideCalibration.originWave));
        mouseCursorReset();
        return (void)false;
    }
    distPixFromLower = round( funcApplyLR(wavelength,slideCalibration.wave2DistLR,true) );
    std::cout << "distPixFromLower: " << distPixFromLower << "px" << std::endl;
    //newX1 muns to be located in the middle
    newX1   = distPixFromLower - round((float)ui->spinSlideW->value() / 2.0);
    newX2   = newX1 + ui->spinSlideW->value();
    //Get the subarea
    QPoint p1, p2;
    p1.setX(newX1); p1.setY(0);
    p2.setX(newX2); p2.setY(slideCalibration.originH);
    newX1 = (newX1>0)?newX1:0;
    p1.setX(newX1);

    //----------------------------------------------
    //Get QE Area
    //----------------------------------------------
    int initPos = 0;
    int row, col;
    //Create Image
    //Create virtual Image
    QImage tmpImgWave(QSize(imgW,imgH),QImage::Format_RGB32);
    //Copy subarea
    QImage tmpImg;
    int j = 0;
    for(i=lstFrames.size()-1; i>=0; i--,j++)
    {
        //Read Image
        //std::cout << lstFrames.at(i).absoluteFilePath().trimmed().toStdString() << std::endl;
        tmpImg = QImage( lstFrames.at(i).absoluteFilePath().trimmed() );
        //Get subarea
        initPos = j * slideW;
        if(
                funcGetQEArea(
                                p1,
                                p2,
                                tmpSubarea,
                                tmpImg,
                                slideCalibration
                             ) != _OK
        ){
            //funcShowMsgERROR_Timeout("Reading QE Area");
            mouseCursorReset();
            return (void)false;
        }
        //std::cout << "i: " << i << std::endl;
        //Copy Subarea into image
        for(col=0; col<slideW; col++)
        {
            for(row=0; row<imgH; row++)
            {
                //std::cout << "tmpSubarea[col][row]: " << tmpSubarea[row][col] << std::endl;
                tmpImgWave.setPixelColor(
                                            QPoint(initPos+col,row),
                                            QColor(tmpSubarea[row][col],tmpSubarea[row][col],tmpSubarea[row][col])
                                        );
            }
        }
    }
    //funcShowMsgSUCCESS_Timeout("Creado");

    //----------------------------------------------
    //Display Result
    //----------------------------------------------
    refreshGVImage(&tmpImgWave);

    mouseCursorReset();

    //Reset Progress Bar
    //funcResetStatusBar();

}

void formBuildSlideHypeCubePreview::refreshGVImage(QImage* imgPreview)
{





    //.......................................................
    //Fix GraphicsView Resolution
    //.......................................................
    //Calc Image Size
    QRect screenRes = screenResolution(this);
    int maxW, maxH, xMargin, yMargin;
    xMargin     = 20;
    yMargin     = 150;
    maxW        = screenRes.width() - xMargin;
    maxH        = screenRes.height() - yMargin;
    //Resize Image
    QPixmap pix = QPixmap::fromImage(*imgPreview);
    pix = pix.scaledToWidth(maxW);
    if(pix.height() > maxH )
    {
            pix = pix.scaledToHeight(maxH);
    }
    //Prepare Scene
    *imgPreview  = imgPreview->scaledToWidth(maxW);
    *imgPreview  = (imgPreview->height()<maxH)?*imgPreview:imgPreview->scaledToHeight(maxH);
    ui->gvSlideHypCubePreview->setGeometry(QRect(0,80,imgPreview->width(),imgPreview->height()));
    ui->gvSlideHypCubePreview->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->gvSlideHypCubePreview->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //Display Image
    ui->gvSlideHypCubePreview->scene()->clear();
    ui->gvSlideHypCubePreview->scene()->addPixmap(pix);
    ui->gvSlideHypCubePreview->scene()->setSceneRect(0,0,pix.width(),pix.height());
    ui->gvSlideHypCubePreview->update();



    /*
    QRect calibArea = ui->pbExpPixs->geometry();
    int maxW, maxH;
    maxW = calibArea.width() - 3;
    maxH = calibArea.height() - 25;
    pix = pix.scaledToHeight(maxH);
    if( pix.width() > maxW )
        pix = pix.scaledToWidth(maxW);
    //It creates the scene to be loaded into Layout
    */
    //QGraphicsScene *scene = new QGraphicsScene(0,0,imgPreview->width(),imgPreview->height());
    //scene->addItem();
    //scene->setBackgroundBrush(QBrush(Qt::black));
    //scene->setBackgroundBrush(pix);
    //ui->gvSlideHypCubePreview->setScene( scene );
    //scene->resize(pix.width(),pix.height());
    //scene->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //scene->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //ui->gvSlideHypCubePreview->update();

}

/*
QImage formBuildSlideHypeCubePreview::buildSlideCubePreview(
                                                           QList<QFileInfo> lstFrames,
                                                           structSlideHypCube* slideCubeSettings
){

    //[COMMENT]
    //It assumes that lstFrames contains only usable frames

    //----------------------------------------------------
    // Create Image Container
    //----------------------------------------------------
    int resultImgW, resultImgH;
    QImage tmpImg( lstFrames.at(0).absoluteFilePath() );
    resultImgW  = (lstFrames.size() * slideCubeSettings->width);
    resultImgH  = (3 * tmpImg.height());
    QImage resultImg( resultImgW, resultImgH, QImage::Format_RGB16 );

    //----------------------------------------------------
    // Set Image Background
    //----------------------------------------------------
    int x, y;
    for( x=0; x<resultImgW; x++ )
    {
        for( y=0; y<resultImgH; y++ )
        {
            resultImg.setPixel(x,y,qRgb(255,255,255));
        }
    }

    //----------------------------------------------------
    // Read RGB Position
    //----------------------------------------------------
    QString tmpParam;
    if( !readFileParam(_PATH_SLIDE_HALOGEN_SENSITIVITIES,&tmpParam) )
    {
        if( !readFileParam(_PATH_SLIDE_FLUORESCENT,&tmpParam) )
        {
            funcShowMsgERROR("RGB Positions Not Found");
            return resultImg;
        }
    }
    int posX;
    posX = round( ((float)ui->spinSlideLocation->value()/100.0) * (float)tmpImg.width());

    //----------------------------------------------------
    // Create Image To Display
    //----------------------------------------------------
    structSlideShifting structShifting;
    structShifting    = calculateShifting(lstFrames,slideCubeSettings,posX,0,1);
    int i;
    x = 0;
    y = structShifting.imgRight.height();
    for( i=0; i<lstFrames.size()-1; i++ )
    {
        structShifting    = calculateShifting(lstFrames,slideCubeSettings,posX,i,i+1);        
        if( i==0 )
        {
            mergeSlidePreview(&resultImg,&structShifting,x,y,false);
        }
        y += structShifting.shifting.y();
        mergeSlidePreview(&resultImg,&structShifting,x,y);
        x += slideCubeSettings->width;
    }
    mergeSlidePreview(&resultImg,&structShifting,x-structShifting.extraWPix,y,false,true);
    if( slideCubeSettings->rotateLeft == true )
    {
        rotateQImage(&resultImg,90);
    }
    return resultImg;
}
*/

void formBuildSlideHypeCubePreview::mergeSlidePreview(
                                                        QImage* canvas,
                                                        structSlideShifting* slideShift,
                                                        int x1,
                                                        int y1,
                                                        bool inside,
                                                        bool right
){
    //-----------------------------------------------
    //Prepare Variables and Containers
    //-----------------------------------------------
    int x, y, w, h, red, green, blue;
    w   = slideShift->imgRight.width();
    h   = slideShift->imgRight.height();
    QRgb sourcePixel, targetPixel;

    //---------------------------------------------------
    //Inside Pixels
    //---------------------------------------------------
    if(inside==true)
    {
        //...............................................
        //Copy Overlapped Pixels as average
        //...............................................
        for( x=0; x<slideShift->extraWPix; x++ )
        {
            for( y=0; y<h; y++ )
            {
                sourcePixel = slideShift->imgRight.pixel(x,y);
                targetPixel = canvas->pixel(x1+x,y1+y);
                red         = (float)(qRed(targetPixel)+qRed(sourcePixel)) * 0.5;
                green       = (float)(qGreen(targetPixel)+qGreen(sourcePixel)) * 0.5;
                blue        = (float)(qBlue(targetPixel)+qBlue(sourcePixel)) * 0.5;
                canvas->setPixel(x1+x,y1+y,qRgb(red,green,blue));
            }
        }

        //...............................................
        //Copy New Pixels Implanted
        //...............................................
        for( x=slideShift->extraWPix; x<w; x++ )
        {
            for( y=0; y<h; y++ )
            {
                canvas->setPixel(x1+x,y1+y,slideShift->imgRight.pixel(x,y));
            }
        }
    }
    else
    {
        //...............................................
        //Outside Pixels
        //...............................................
        for( x=0; x<w; x++ )
        {
            for( y=0; y<h; y++ )
            {
                if(right==true)
                    canvas->setPixel(x1+x,y1+y,slideShift->imgRight.pixel(x,y));
                else
                    canvas->setPixel(x1+x,y1+y,slideShift->imgLeft.pixel(x,y));
            }
        }
    }

    //qDebug() << "acumX: " << slideShift->acumX;
    //slideShift->acumX += slideShift->shifting.x();
}

structSlideShifting formBuildSlideHypeCubePreview::calculateShifting(
                                                     QList<QFileInfo> lstFrames,
                                                     structSlideHypCube* slideCubeSettings,
                                                     int x, int i, int j
){
    //--------------------------------------------------------------
    //Prepare Imagery
    //--------------------------------------------------------------

    structSlideShifting slideShift;

    //..............................................................
    //Read from HDD
    //..............................................................
    slideShift.imgLeft.load( lstFrames.at(i).absoluteFilePath() );
    slideShift.imgRight.load( lstFrames.at(j).absoluteFilePath() );
    slideShift.extraWPix = calcSlideExtraW(slideCubeSettings);

    //..............................................................
    //Cut Subimages Including Merge Band (Extra Width)
    //..............................................................
    int leftX, rightX;
    int w, h;    
    w                           = slideCubeSettings->width + slideShift.extraWPix;
    h                           = slideShift.imgLeft.height();
    leftX                       = x;
    rightX                      = x - slideShift.extraWPix;
    slideShift.imgLeft          = slideShift.imgLeft.copy(leftX,0,w,h);
    slideShift.imgRight         = slideShift.imgRight.copy(rightX,0,w,h);

    //funcClearDirFolder("./tmpImages/frames/slidesForSimilarity/");
    //slideShift.imgLeft.save("./tmpImages/frames/slidesForSimilarity/"+QString::number(i)+".png");
    //imgRight.save("./tmpImages/frames/slidesForSimilarity/right.png");

    //..............................................................
    //Get Imagery Shifting by 2D Similarity
    //..............................................................
    QImage mergeAreaLeft, mergeAreaRight;
    int aux             = slideShift.imgLeft.width() - slideShift.extraWPix;
    mergeAreaLeft       = slideShift.imgLeft.copy(aux,0,slideShift.extraWPix,h);
    mergeAreaRight      = slideShift.imgRight.copy(0,0,slideShift.extraWPix,h);
    slideShift.shifting = imageSimilarity2D(&mergeAreaLeft, &mergeAreaRight, slideCubeSettings->shiftAllowed);
    //qDebug() << "Shifting: " << slideShift.shifting.x() << ", " << slideShift.shifting.y();
    //exit(0);

    return slideShift;
}

/*
QImage formBuildSlideHypeCubePreview::buildSlideCubePreview(QList<QFileInfo> lstFrames,structSlideHypCube* slideCubeSettings){
    //[COMMENT]
    //It assumes that lstFrames contains only usable frames

    //----------------------------------------------------
    // Create Image Container
    //----------------------------------------------------
    int resultImgW;
    resultImgW = lstFrames.size() * slideCubeSettings->width;
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
            return resultImg;
        }
    }
    int rPosX, gPosX, bPosX;
    //rPosX = tmpParam.split(",").at(0).toInt(0) - round((float)slideCubeSettings.width/2.0);
    //gPosX = tmpParam.split(",").at(1).toInt(0) - round((float)slideCubeSettings.width/2.0);
    //bPosX = tmpParam.split(",").at(2).toInt(0) - round((float)slideCubeSettings.width/2.0);
    rPosX = round( ((float)ui->spinSlideLocation->value()/100.0) * (float)tmpImg.width());
    gPosX = rPosX;
    bPosX = rPosX;

    //----------------------------------------------------
    // Create Image To Display
    //----------------------------------------------------
    int i, w, extraW, h, r, c, slideX, red, green, blue;
    w       = slideCubeSettings->width;
    extraW  = round((float)w*0.3);
    h       = tmpImg.height()-1;
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
    if( slideCubeSettings->rotateLeft == true )
    {
        rotateQImage(&resultImg,90);
    }
    return resultImg;
}
*/

/*
void formBuildSlideHypeCubePreview::on_pbAutoStepIncrement_clicked()
{
    int aux;
    aux = ui->spinSlideW->value()+ui->spinAutoStep->value();
    int slideW = (aux>ui->spinSlideW->maximum())?ui->spinSlideW->maximum():aux;
    ui->spinSlideW->setValue(slideW);
    ui->pbApply->click();
}*/

/*
void formBuildSlideHypeCubePreview::on_pbAutoStepDecrement_clicked()
{
    int aux;
    aux = ui->spinSlideW->value()-ui->spinAutoStep->value();
    int slideW = (aux>=1)?aux:1;
    ui->spinSlideW->setValue(slideW);
    ui->pbApply->click();
}*/

void formBuildSlideHypeCubePreview::on_pbFolder_clicked()
{
    //------------------------------------------------------
    //Select Directory
    //------------------------------------------------------
    QString workDir, lastSlideFrames;
    if( !readFileParam(_PATH_LAST_PATH_OPENED,&lastSlideFrames) )
    {
        lastSlideFrames.clear();
        lastSlideFrames.append(_PATH_VIDEO_FRAMES);
    }
    if( !funcShowSelDir(lastSlideFrames,&workDir) )
    {
        return (void)false;
    }
    saveFile(_PATH_LAST_PATH_OPENED,workDir);
    ui->txtFolder->setText(workDir);

    //......................................................
    //Refresh Scene
    //......................................................
    //ui->pbApply->click();
}

QString formBuildSlideHypeCubePreview::concatenateParameters(int firstTime)
{
    //firstTime -> new file to be created
    QString lstParameters;

    lstParameters = (firstTime)?_PATH_VIDEO_FRAMES:ui->txtFolder->text();
    lstParameters.append(","+QString::number(ui->spinSlideW->value()));
    //lstParameters.append(","+QString::number(ui->spinSlideLocation->value()));
    lstParameters.append(","+QString::number(ui->spinOverlap->value()));
    lstParameters.append(","+QString::number(ui->spinMaxShift->value()));
    //lstParameters.append(","+QString::number(ui->spinAutoStep->value()));
    if(ui->cbFlip->isChecked())
        lstParameters.append(",1");
    else
        lstParameters.append(",0");


    return lstParameters;
}

int formBuildSlideHypeCubePreview::setLastExecution(QString parameters)
{
    //std::cout << "parameters: " << parameters.toStdString() << std::endl;
    //if( parameters.isEmpty() )
    //    return _ERROR;

    //int intI = 0;
    //ui->txtFolder->setText(parameters.split(",").at(intI++));
    //ui->spinSlideW->setValue(parameters.split(",").at(intI++).toInt(0));
    //ui->spinSlideLocation->setValue(parameters.split(",").at(intI++).toInt(0));
    //ui->spinOverlap->setValue(parameters.split(",").at(intI++).toFloat(0));
    //ui->spinMaxShift->setValue(parameters.split(",").at(intI++).toFloat(0));
    //ui->spinAutoStep->setValue(parameters.split(",").at(intI++).toInt(0));
    //if( parameters.split(",").at(intI++).toInt(0) )
    //   ui->cbFlip->setChecked(true);

    return _OK;
}
