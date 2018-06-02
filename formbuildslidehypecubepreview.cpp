#include "formbuildslidehypecubepreview.h"
#include "ui_formbuildslidehypecubepreview.h"

#include <lstpaths.h>
#include <QGraphicsPixmapItem>
#include <__common.h>
#include <formhypcubebuildsettings.h>
#include <formhypcubebuildsettings.h>

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
    ui->progBar->setVisible(false);


    //------------------------------------------------------
    //Load Exporting Slide Hypcube
    //------------------------------------------------------
    if( fileExists( _PATH_SLIDE_EXPORTING_HYPCUBE ) )
    {
        formHypcubeBuildSettings* tmpHypSettings = new formHypcubeBuildSettings(this);
        if( tmpHypSettings->funcReadSettings(&mainExportSettings) != _OK )
        {
            funcShowMsgERROR("Reading Exporting Settings");
        }
    }
    else
    {
        funcShowMsgERROR("Exporting Settings don't exists");
    }
    //Load last wavelength
    float lastWavelen = readAllFile(_PATH_LAST_WAVELEN_SIMULATOR).trimmed().toFloat(0);
    if( lastWavelen>0 )
    {
        ui->spinBoxWavelen->setValue(lastWavelen);
    }

}

formBuildSlideHypeCubePreview::~formBuildSlideHypeCubePreview()
{
    delete ui;
}

QImage formBuildSlideHypeCubePreview
       ::funcGetImageAtWavelength(
                                    const float &wavelength,
                                    const structSlideCalibration &mainSlideCalibration
){
    //------------------------------------------------------
    //Load Imagery
    //------------------------------------------------------
    if( lstImgs.size() == 0 )
    {
        //Get Lst Images From Folder
        ui->labelProgBar->setText("Putting Images into Memory...");
        funcGetImagesFromFolder(
                                    ui->txtFolder->text().trimmed(),
                                    &lstImgs,
                                    &lstImagePaths,
                                    ui->progBar
                                );
        ui->labelProgBar->setText("");
    }

    //------------------------------------------------------
    //Reserve Memory
    //------------------------------------------------------
    int layerW, layerH, layerTmpPos;
    int slideW, slideH, imgW, numFrames;
    int overlapW, notOverlapW;
    imgW        = lstImgs.at(0).width();
    numFrames   = lstImgs.size();
    slideW      = mainExportSettings.spatialResolution;
    slideH      = lstImgs.at(0).height();
    overlapW    = ceil( (float)slideW * (mainExportSettings.spatialOverlap/100.0));
    notOverlapW = slideW - overlapW;
    layerW      = imgW + ( notOverlapW * (numFrames-1) );
    layerH      = lstImgs.at(0).height();

    //------------------------------------------------------
    //Calculate Spectral Location
    //------------------------------------------------------
    int frameNotOverlapPos, frameOverlapPos, maxPos;
    int wavePos;
    float internWavelen;
    maxPos              = imgW-slideW-1;
    internWavelen       = wavelength - mainSlideCalibration.originWave;
    frameNotOverlapPos  = round( funcApplyLR(internWavelen,mainSlideCalibration.wave2DistLR,false) );
    wavePos             = round( funcApplyLR(internWavelen,mainSlideCalibration.wave2DistLR,false) );
    frameNotOverlapPos  = frameNotOverlapPos - round((float)slideW/2.0);
    frameNotOverlapPos  = (frameNotOverlapPos<0)?0:frameNotOverlapPos;//Minimum position
    frameNotOverlapPos  = (frameNotOverlapPos<maxPos)?frameNotOverlapPos:maxPos;
    frameNotOverlapPos += overlapW;
    frameOverlapPos     = frameNotOverlapPos - overlapW;
    layerTmpPos         = frameOverlapPos;

    //======================================================
    //Build Layer
    //======================================================
    QImage mainTmpLayerImg(layerW,layerH,QImage::Format_RGB32);
    mainTmpLayerImg.fill(Qt::GlobalColor::black);

    mouseCursorWait();
    //------------------------------------------------------
    //Copy Fist Slide
    //------------------------------------------------------
    QRect originRect;
    QPoint destinePoint;
    //Origin Rectangle [overlapPos is at the left and notOverlapPos on the Right]
    originRect.setX(frameOverlapPos);
    originRect.setY(0);
    originRect.setWidth(slideW);
    originRect.setHeight(slideH);
    //Destine Rectangle
    destinePoint.setX(layerTmpPos);
    destinePoint.setY(0);
    //Copy Slide
    //funcCopyImageSubareas( originRect, destinePoint, lstImgs.at(0), &mainTmpLayerImg );
    funcDemoiseAndCopyImageSubareas(
                                        originRect,
                                        destinePoint,
                                        lstImgs.at(0),
                                        &mainTmpLayerImg,
                                        wavePos,
                                        mainSlideCalibration.sensitivities,
                                        copyOverride
                                   );
    layerTmpPos += notOverlapW;

    //------------------------------------------------------
    //Copy Remainder Slides
    //------------------------------------------------------
    int idImg;
    for( idImg=1; idImg<lstImgs.size(); idImg++ )
    {
        //..................................................
        //Copy Overlap
        //..................................................
        //Origin Rectangle
        originRect.setX(frameOverlapPos);
        originRect.setY(0);
        originRect.setWidth(overlapW);
        originRect.setHeight(slideH);
        //Destine Init Point
        destinePoint.setX(layerTmpPos);
        destinePoint.setY(0);
        //Copy Slide
        funcDemoiseAndCopyImageSubareas(
                                            originRect,
                                            destinePoint,
                                            lstImgs.at(idImg),
                                            &mainTmpLayerImg,
                                            wavePos,
                                            mainSlideCalibration.sensitivities,
                                            copyAverage
                                       );
        layerTmpPos += overlapW;        

        //..................................................
        //Copy Non-Overlap
        //..................................................
        //Origin Rectangle
        originRect.setX(frameNotOverlapPos);
        originRect.setY(0);
        originRect.setWidth(notOverlapW);
        originRect.setHeight(slideH);
        //Destine Init Point
        destinePoint.setX(layerTmpPos);
        destinePoint.setY(0);
        //Copy Slide
        funcDemoiseAndCopyImageSubareas(
                                            originRect,
                                            destinePoint,
                                            lstImgs.at(idImg),
                                            &mainTmpLayerImg,
                                            wavePos,
                                            mainSlideCalibration.sensitivities,
                                            copyOverride
                                       );
        layerTmpPos += (notOverlapW - overlapW);
    }

    //------------------------------------------------------
    //Flip if Required
    //------------------------------------------------------
    if( mainExportSettings.flip )
    {
        mainTmpLayerImg = mainTmpLayerImg.mirrored(true,false);
    }
    mouseCursorReset();

    //------------------------------------------------------
    //Return Layer Image Created
    //------------------------------------------------------
    return mainTmpLayerImg;

}

int formBuildSlideHypeCubePreview
        ::funcCalcHypercubeSize(
                                    structSlideHypCubeSize* slidecHypCubeSize
){
    //Memset
    memset(slidecHypCubeSize,'\0',sizeof(structSlideHypCubeSize));

    //------------------------------------------------------
    //Load Imagery
    //------------------------------------------------------
    if( lstImgs.size() == 0 )
    {
        //Get Lst Images From Folder
        ui->labelProgBar->setText("Putting Images into Memory...");
        funcGetImagesFromFolder(
                                    ui->txtFolder->text().trimmed(),
                                    &lstImgs,
                                    &lstImagePaths,
                                    ui->progBar
                                );
        ui->labelProgBar->setText("");
    }

    //------------------------------------------------------
    //Reserve Memory
    //------------------------------------------------------
    int layerW, layerH;
    int slideW, slideH, imgW, imgH, numFrames;
    int overlapW, notOverlapW;
    float specW, specRes;
    imgW        = lstImgs.at(0).width();
    imgH        = lstImgs.at(0).height();
    numFrames   = lstImgs.size();
    slideW      = mainExportSettings.spatialResolution;
    specRes     = mainExportSettings.spectralResolution;
    slideH      = lstImgs.at(0).height();
    overlapW    = ceil( (float)slideW * (mainExportSettings.spatialOverlap/100.0));
    notOverlapW = slideW - overlapW;
    layerW      = imgW + ( notOverlapW * (numFrames-1) );
    layerH      = lstImgs.at(0).height();

    //------------------------------------------------------
    // Calculate Spectral Resolution
    // Ralf Pag. 6
    // Spectral Resolution = (x_ir - x_ib) / (lambda_r - lambda_b)
    //------------------------------------------------------
    int L;
    specW   = mainExportSettings.expMaxWave - mainExportSettings.expMinWave;
    L       = floor(specW / specRes);

    //------------------------------------------------------
    //Fill Parameters
    //------------------------------------------------------
    slidecHypCubeSize->hypcubeW         = layerW;
    slidecHypCubeSize->hypcubeH         = layerH;
    slidecHypCubeSize->hypcubeL         = L;
    slidecHypCubeSize->imgW             = imgW;
    slidecHypCubeSize->imgH             = imgH;
    slidecHypCubeSize->slideW           = slideW;
    slidecHypCubeSize->slideH           = slideH;
    slidecHypCubeSize->overlapW         = overlapW;
    slidecHypCubeSize->notOverlapW      = notOverlapW;
    slidecHypCubeSize->specW            = specW;
    slidecHypCubeSize->specResolution   = specRes;
    slidecHypCubeSize->minSpecRes       = mainExportSettings.expMinWave;

    //------------------------------------------------------
    //Return Layer Image Created
    //------------------------------------------------------
    return _OK;

}


void formBuildSlideHypeCubePreview::on_pbApply_clicked()
{
    //------------------------------------------------------
    //Reload Export Settings
    //------------------------------------------------------
    funcReloadExportSettings();

    //------------------------------------------------------
    //Calc Maximum Sensitivities
    //------------------------------------------------------
    structSlideCalibration mainSlideCalibration;
    funcGetMaximumSensitivities( &mainSlideCalibration, this );

    //------------------------------------------------------
    //Get Layer
    //------------------------------------------------------
    layerBackup = funcGetImageAtWavelength(
                                                ui->spinBoxWavelen->value(),
                                                mainSlideCalibration
                                           );
    displayImageFullScreen( layerBackup );

    //------------------------------------------------------
    //Save Last Wavelength
    //------------------------------------------------------
    saveFile(_PATH_LAST_WAVELEN_SIMULATOR,QString::number(ui->spinBoxWavelen->value()));

}

int formBuildSlideHypeCubePreview::funcReloadExportSettings()
{
    if( fileExists( _PATH_SLIDE_EXPORTING_HYPCUBE ) )
    {
        formHypcubeBuildSettings* formHypSettings = new formHypcubeBuildSettings(this);
        if( formHypSettings->funcReadSettings(&mainExportSettings) != _OK )
        {
            funcShowMsgERROR("Reading Exporting Settings");
            return _ERROR;
        }
    }
    else
    {
        funcShowMsgERROR("Exporting Settings don't exists");
        return _ERROR;
    }
    return _OK;
}

int formBuildSlideHypeCubePreview::funcCopyImageSubareas(
                                                            const QRect  &originRect,
                                                            const QPoint &destinePoint,
                                                            const QImage &origImg,
                                                            QImage* destineImg,
                                                            int type
){
    //type[ 0:override | 1:Average | 2: maxVal | 3:minVal ]

    int x, y;
    int originX, originY;
    int destineX, destineY;
    int aux;
    QColor originColor;
    QColor destineColor;
    QColor tmpColor;
    //std::cout << "Pos: " << destineRect.x() << std::endl;
    for(x=0; x<originRect.width(); x++)
    {
        for(y=0; y<originRect.height(); y++)
        {
            //Calc Coordinates
            originX         = originRect.x()   + x;
            originY         = originRect.y()   + y;
            destineX        = destinePoint.x() + x;
            destineY        = destinePoint.y() + y;
            //std::cout << "oX: "     << originX << " oY: "   << originY << " dX: "
            //          << destineX   << " dY: " << destineY  << std::endl;
            //..................
            //Set Pixel
            //..................
            //specDemoised
            if( type == copySpecDemoised )
            {
                originColor = origImg.pixelColor(originX,originY);
                destineImg->setPixelColor(QPoint(destineX,destineY),originColor);
            }
            //Override
            if( type == copyOverride )
            {
                originColor = origImg.pixelColor(originX,originY);
                destineImg->setPixelColor(QPoint(destineX,destineY),originColor);
            }
            //Average
            if( type == copyAverage )
            {
                originColor     = origImg.pixelColor(originX,originY);
                destineColor    = destineImg->pixelColor(destineX,destineY);                
                tmpColor.setRed( round( (float)(originColor.red() + destineColor.red())/2.0 ) );
                tmpColor.setGreen( round( (float)(originColor.green() + destineColor.green())/2.0 ) );
                tmpColor.setBlue( round( (float)(originColor.blue() + destineColor.blue())/2.0 ) );
                destineImg->setPixelColor(QPoint(destineX,destineY),tmpColor);
                //std::cout << "O -> R: " << originColor.red() << " G: " << originColor.green() << " B: " << originColor.blue() << std::endl;
                //std::cout << "D -> R: " << destineColor.red() << " G: " << destineColor.green() << " B: " << destineColor.blue() << std::endl;
                //std::cout << "A -> R: " << tmpColor.red() << " G: " << tmpColor.green() << " B: " << tmpColor.blue() << std::endl;
            }
            //Maximum
            if( type == copyMax )
            {
                originColor     = origImg.pixelColor(originX,originY);
                destineColor    = destineImg->pixelColor(destineX,destineY);
                aux = (originColor.red() > destineColor.red() )?originColor.red():destineColor.red();
                tmpColor.setRed( aux );
                aux = (originColor.green() > destineColor.green() )?originColor.green():destineColor.green();
                tmpColor.setGreen( aux );
                aux = (originColor.blue() > destineColor.blue() )?originColor.blue():destineColor.blue();
                tmpColor.setBlue( aux );
                destineImg->setPixelColor(QPoint(destineX,destineY),tmpColor);
                //std::cout << "O -> R: " << originColor.red() << " G: " << originColor.green() << " B: " << originColor.blue() << std::endl;
                //std::cout << "D -> R: " << destineColor.red() << " G: " << destineColor.green() << " B: " << destineColor.blue() << std::endl;
                //std::cout << "A -> R: " << tmpColor.red() << " G: " << tmpColor.green() << " B: " << tmpColor.blue() << std::endl;
            }
        }
    }


    return _OK;
}

int formBuildSlideHypeCubePreview
    ::funcDemoiseAndCopyImageSubareas(
                                            const QRect &originRect,
                                            const QPoint &destinePoint,
                                            const QImage &origImg,
                                            QImage* destineImg,
                                            const int &wavePos,
                                            const structSlideSensitivities &slideSens,
                                            int type
){
    //type[ 0:override | 1:Average | 2: maxVal | 3:minVal ]

    int x, y;
    int originX, originY;
    int destineX, destineY;
    //int maxColorID = 0;
    //float maxColorVal = 0.0;
    //float wR, wG, wB;
    //float wS = 1;
    //float tmpX;
    strDenoisedColorSelected denColSel;

    //--------------------------------------------------
    //Define the Sensor to Use
    //--------------------------------------------------    
    //wR  = slideSens.normedRalfR.at(wavePos);
    //wG  = slideSens.normedRalfG.at(wavePos);
    //wB  = slideSens.normedRalfB.at(wavePos);
    funcSlideDenoiseDefineSensorToUse( &denColSel, wavePos, slideSens );
    //std::cout << "denColSel.wS: " << denColSel.wS << std::endl;

    //wS = 1 + (wS * 1.2);//Arbitrariamente :D
    //std::cout << "wS6 " << wS << std::endl;
    //wS = 1 + wS + (1-slideSens.maximumColors.maxMaxS);//Arbitrariamente :D
    //std::cout << "wS7 " << wS
    //          << " maxMax: " << slideSens.maximumColors.maxMaxS
    //          << std::endl;
    //exit(0);
    //std::cout << "x: " << tmpX << " wS15: " << wS << std::endl;
    //exit(0);

    //--------------------------------------------------
    //Denoise Pixel Color
    //--------------------------------------------------
    int tmpColor;
    QColor originColor;
    QColor destineColor;
    for(x=0; x<originRect.width(); x++)
    {
        for(y=0; y<originRect.height(); y++)
        {
            //Calc Coordinates
            originX         = originRect.x()   + x;
            originY         = originRect.y()   + y;
            destineX        = destinePoint.x() + x;
            destineY        = destinePoint.y() + y;

            //Get Pixel
            originColor     = origImg.pixelColor(originX,originY);

            //Spectral Denoising
            tmpColor        = round(
                                        funcPixelToQE(
                                                        originColor,
                                                        wavePos,
                                                        slideSens,
                                                        denColSel.wS
                                                     )
                                   );

            //-----------------------------------------------
            //Set Color
            //-----------------------------------------------
            originColor.setRed(0);
            originColor.setGreen(0);
            originColor.setBlue(0);
            if( type == copyMax )
            {
                //Not Implemented
            }
            if( type == copyMin )
            {
                //Not Implemented
            }
            if( type == copyAverage )
            {
                destineColor = destineImg->pixelColor(QPoint(destineX,destineY));
                if( denColSel.colorID == _RED )
                {
                    tmpColor    = ( tmpColor + destineColor.red() ) * 0.5;
                    originColor.setRed( tmpColor );
                }
                if( denColSel.colorID == _GREEN )
                {
                    tmpColor    = ( tmpColor + destineColor.green() ) * 0.5;
                    originColor.setGreen( tmpColor );
                }
                if( denColSel.colorID == _BLUE )
                {
                    tmpColor    = ( tmpColor + destineColor.blue() ) * 0.5;
                    originColor.setBlue( tmpColor );
                }
            }

            if( type == copyOverride )
            {
                if( denColSel.colorID == _RED )    originColor.setRed( tmpColor );
                if( denColSel.colorID == _GREEN )  originColor.setGreen( tmpColor );
                if( denColSel.colorID == _BLUE )   originColor.setBlue( tmpColor );
            }


            //Save Color
            destineImg->setPixelColor(QPoint(destineX,destineY),originColor);

        }
    }


    return _OK;
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
    firstTime = firstTime;
    /*
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
    */

    return lstParameters;
}

int formBuildSlideHypeCubePreview::setLastExecution(QString parameters)
{
    parameters = parameters;
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

void formBuildSlideHypeCubePreview::on_pbUploadImages_clicked()
{

    if( !funcShowMsgYesNo("Alert!","Upload Images?",this) )
    {
        return (void)false;
    }

    //-----------------------------------------------
    //Get Lst Images From Folder
    //-----------------------------------------------
    ui->labelProgBar->setText("Putting Images into Memory...");
    funcGetImagesFromFolder(
                                ui->txtFolder->text().trimmed(),
                                &lstImgs,
                                &lstImagePaths,
                                ui->progBar
                            );
    ui->labelProgBar->setText("");
}

void formBuildSlideHypeCubePreview::on_pbSettings_clicked()
{
    formHypcubeBuildSettings* tmpForm = new formHypcubeBuildSettings(this);
    tmpForm->setModal(true);
    tmpForm->show();
}

void formBuildSlideHypeCubePreview::on_pbSave_clicked()
{
    //------------------------------------------
    //Check if Images is Valid
    //------------------------------------------
    if( layerBackup.isNull() )
    {
        funcShowMsgERROR("Not Image to Export",this);
        return (void)false;
    }

    //------------------------------------------
    //Get image filename
    //------------------------------------------
    QString fileName;
    if(
            funcLetUserDefineFile(
                                    &fileName,
                                    "Define Image Filename",
                                    ".png",
                                    new QString(_PATH_LAST_IMG_SAVED),
                                    new QString(_PATH_LAST_IMG_SAVED),
                                    this
                                 ) != _OK
    ){
        return (void)false;
    }

    //------------------------------------------------------
    //Save Image
    //------------------------------------------------------
    layerBackup.save(fileName);
    funcShowMsgSUCCESS_Timeout("Image Saved Successfully",this);

    //------------------------------------------------------
    //Save Last Wavelength
    //------------------------------------------------------
    saveFile(_PATH_LAST_WAVELEN_SIMULATOR,QString::number(ui->spinBoxWavelen->value()));



}

void formBuildSlideHypeCubePreview::on_pbExportImages_clicked()
{
    //------------------------------------------------------
    //Define Output Dir
    //------------------------------------------------------
    QString destineDir;
    if( funcLetUserSelectDirectory(_PATH_LAST_PATH_OPENED,&destineDir) != _OK )
    {
        funcShowMsgERROR("Saving Directory",this);
        return (void)false;
    }

    //------------------------------------------------------
    //Reload Export Settings
    //------------------------------------------------------
    funcReloadExportSettings();

    //------------------------------------------------------
    //Load Slide Calibration Settings Set as Default
    //------------------------------------------------------
    QString calibPath = readAllFile(_PATH_SLIDE_ACTUAL_CALIB_PATH).trimmed();
    structSlideCalibration mainSlideCalibration;
    if( funcReadSlideCalib( calibPath, &mainSlideCalibration ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Slide Calibration File: "+calibPath);
        return (void)false;
    }

    //------------------------------------------------------
    //Load Imagery
    //------------------------------------------------------
    if( lstImgs.size() == 0 )
    {
        //Get Lst Images From Folder
        ui->labelProgBar->setText("Putting Images into Memory...");
        funcGetImagesFromFolder(
                                    ui->txtFolder->text().trimmed(),
                                    &lstImgs,
                                    &lstImagePaths,
                                    ui->progBar
                                );
        ui->labelProgBar->setText("");
    }

    //------------------------------------------------------
    //Calculate Spectral Resolution
    // Ralf Pag. 6
    // Spectral Resolution = (x_ir - x_ib) / (lambda_r - lambda_b)
    //------------------------------------------------------
    float maxWavelen, minWavelen, specRes;//, specW;
    //int imgW;
    maxWavelen  = mainSlideCalibration.maxWave;
    minWavelen  = mainSlideCalibration.originWave;
    //imgW        = lstImgs.at(0).width();
    //specW       = maxWavelen - minWavelen;
    //specRes     = (float)imgW / specW;
    specRes     = mainExportSettings.spectralResolution;
    //std::cout << "maxWavelen: " << maxWavelen << std::endl;
    //std::cout << "minWavelen: " << minWavelen << std::endl;
    //std::cout << "specW: " << specW << std::endl;
    std::cout << "specRes: " << specRes << std::endl;

    //------------------------------------------------------
    //Get Layer
    //------------------------------------------------------
    float specI, percentage;
    QString tmpFileName;
    percentage = 0.0;
    //Update Progressbar
    ui->progBar->setValue(0);
    ui->progBar->setVisible(true);
    mouseCursorWait();
    for( specI=minWavelen; specI<=maxWavelen; specI+=specRes )
    {
        //Update Progress Bar
        percentage = round( ((specI-minWavelen) / (maxWavelen-minWavelen))*100.0 );
        ui->labelProgBar->setText("Exporting Layer " + QString::number(specI) + "nm...");
        ui->progBar->setValue(percentage);
        //Define Filename
        tmpFileName.clear();
        tmpFileName.append(destineDir);
        tmpFileName.append(QString::number(specI));
        tmpFileName.append(_FRAME_EXTENSION);
        //std::cout << "specI: " << specI << " tmpFileName: " << tmpFileName.toStdString() << std::endl;
        //Get Layer
        layerBackup = funcGetImageAtWavelength( specI, mainSlideCalibration );
        //Save Layer
        layerBackup.save(tmpFileName);
    }

    //Update Progressbar
    ui->progBar->setValue(0);
    ui->progBar->setVisible(false);
    ui->labelProgBar->setText("");
    mouseCursorReset();
}

void formBuildSlideHypeCubePreview::on_pbExportCube_clicked()
{
    //Select Dir
    QString destineDir;
    if( funcLetUserSelectDirectory(_PATH_LAST_SLIDE_HYPCUBE_EXPORTED,&destineDir) != _OK )
    {
        return (void)false;
    }
    //Export
    exportSlideHypCube(
                            &destineDir,
                            ui->progBar,
                            ui->labelProgBar,
                            this
                      );
}

void formBuildSlideHypeCubePreview
     ::exportSlideHypCube(
                            QString* destineDir,
                            QProgressBar* progBar,
                            QLabel* labelProgBar,
                            QWidget* parent
){
    //======================================================
    //PREPARE VARIABLES AND ENVIROMENT
    //======================================================

    //------------------------------------------------------
    //Reload Export Settings
    //------------------------------------------------------
    funcReloadExportSettings();

    //------------------------------------------------------
    //Load Default Slide Calibration Settings
    //------------------------------------------------------
    QString calibPath = readAllFile(_PATH_SLIDE_ACTUAL_CALIB_PATH).trimmed();
    structSlideCalibration mainSlideCalibration;
    if( funcReadSlideCalib( calibPath, &mainSlideCalibration ) != _OK )
    {
        funcShowMsgERROR_Timeout("Reading Slide Calibration File: "+calibPath,parent);
        return (void)false;
    }

    //------------------------------------------------------
    //Calc Maximum Sensitivities
    //------------------------------------------------------
    funcGetMaximumSensitivities( &mainSlideCalibration, parent );

    //------------------------------------------------------
    //Load Imagery
    //------------------------------------------------------
    if( lstImgs.size() == 0 )
    {
        //Get Lst Images From Folder
        labelProgBar->setText("Putting Images into Memory...");
        funcGetImagesFromFolder(
                                    ui->txtFolder->text().trimmed(),
                                    &lstImgs,
                                    &lstImagePaths,
                                    progBar
                                );
        labelProgBar->setText("");
    }

    //------------------------------------------------------
    //Get Hypercube Size
    //------------------------------------------------------    
    structSlideHypCubeSize slideHypcubeSize;
    funcCalcHypercubeSize( &slideHypcubeSize );
    //Reserve Memory
    slideHypcubeSize.lstWavelengths     = (float*)malloc(slideHypcubeSize.hypcubeL*sizeof(float));
    slideHypcubeSize.lstOverlapPos      = (int*)malloc(slideHypcubeSize.hypcubeL*sizeof(int));
    slideHypcubeSize.lstNotOverlapPos   = (int*)malloc(slideHypcubeSize.hypcubeL*sizeof(int));
    slideHypcubeSize.lstDestineInitX    = (int*)malloc(slideHypcubeSize.hypcubeL*sizeof(int));
    memset(slideHypcubeSize.lstWavelengths,'\0',slideHypcubeSize.hypcubeL*sizeof(float));
    memset(slideHypcubeSize.lstOverlapPos,'\0',slideHypcubeSize.hypcubeL*sizeof(int));
    memset(slideHypcubeSize.lstNotOverlapPos,'\0',slideHypcubeSize.hypcubeL*sizeof(int));
    if( 0 )
    {
        std::cout << "W: " << slideHypcubeSize.hypcubeW << std::endl;
        std::cout << "H: " << slideHypcubeSize.hypcubeH << std::endl;
        std::cout << "L: " << slideHypcubeSize.hypcubeL << std::endl;
        std::cout << "slideW: " << slideHypcubeSize.slideW << std::endl;
        std::cout << "slideH: " << slideHypcubeSize.slideH << std::endl;
        std::cout << "overlapW: " << slideHypcubeSize.overlapW << std::endl;
        std::cout << "notOverlapW: " << slideHypcubeSize.notOverlapW << std::endl;
        std::cout << "specW: " << slideHypcubeSize.specW << std::endl;
        std::cout << "specRes: " << slideHypcubeSize.specResolution << std::endl;
        std::cout << "minWave: " << slideHypcubeSize.minSpecRes << std::endl;
        std::cout << "maxWave: " << mainExportSettings.expMaxWave << std::endl;
    }

    //======================================================
    //BUILD THE SLIDE HYPERCUBE
    //======================================================

    //------------------------------------------------------
    //Calculate Frame's Position
    //------------------------------------------------------
    funcCalcLstWaveAndPositions( &slideHypcubeSize, mainSlideCalibration );
    if( 0 )
    {
        int l;
        for(l=0; l<slideHypcubeSize.hypcubeL; l++)
        {
            std::cout << "B) "
                      << " l: "  << l
                      << " wavelength: "  << slideHypcubeSize.lstWavelengths[l]
                      << " overlapPos: "    << slideHypcubeSize.lstOverlapPos[l]
                      << " nonOverlapPos: " << slideHypcubeSize.lstNotOverlapPos[l]
                      << std::endl;
        }
        exit(0);
    }


    //------------------------------------------------------
    //Reserve Memory
    //------------------------------------------------------
    int i, j;
    int W, H, L;
    W = slideHypcubeSize.hypcubeW;
    H = slideHypcubeSize.hypcubeH;
    L = slideHypcubeSize.hypcubeL;
    u_int8_t*** slideHypCube;
    slideHypCube = (u_int8_t***)malloc( W * sizeof(u_int8_t**) );
    for( i=0; i<W; i++ )
    {
        slideHypCube[i] = (u_int8_t**)malloc( H * sizeof(u_int8_t*) );
        for( j=0; j<H; j++ )
        {
            slideHypCube[i][j] = (u_int8_t*)malloc( L * sizeof(u_int8_t) );
            memset(slideHypCube[i][j],'\0',L * sizeof(u_int8_t));
        }
    }

    //------------------------------------------------------
    //For Each Image
    //------------------------------------------------------
    int idImg;

    //Update Progress Bar
    mouseCursorWait();
    progBar->setVisible(true);
    progBar->setValue(0);
    progBar->update();
    labelProgBar->setText("Building Slide Hypercube...");
    int tmpProgress = 0;
    for( idImg=0; idImg<lstImgs.size(); idImg++ )
    {
        funcSlideSpectralImageUnzip(
                                        slideHypCube,
                                        idImg,
                                        (QImage*)&lstImgs.at(idImg),
                                        slideHypcubeSize,
                                        mainSlideCalibration
                                   );
        //Update Progress Bar
        tmpProgress = round( ((float)idImg / (float)lstImgs.size()) * 100.0 );
        progBar->setValue(tmpProgress);
        progBar->update();
    }
    //Update Progress Bar
    progBar->setVisible(false);
    progBar->setValue(0);
    progBar->update();
    labelProgBar->setText("");
    mouseCursorReset();

    //------------------------------------------------------
    //Show the Result
    //------------------------------------------------------
    if( 0 )
    {
        for(i=0; i<slideHypcubeSize.hypcubeL; i+=15)
        {
            QImage layer = funcGetLayerFromHypCube( slideHypCube, slideHypcubeSize, i );
            displayImageFullScreen(layer);
        }
    }

    /*
    //------------------------------------------------------
    //Compare Layers
    //------------------------------------------------------
    if( 0 )
    {
        int numDifferent = 0;
        QImage layer = funcGetLayerFromHypCube( slideHypCube, slideHypcubeSize, i );
        int x, y;
        for(x=0; x<layer.width(); x++)
        {
            for(y=0; y<layer.height(); y++)
            {
                if( slideHypCube[x][y][0] != slideHypCube[x][y][slideHypcubeSize.hypcubeL-1] )
                {
                    std::cout << "x: " << x << " y: " << y << " " << slideHypCube[x][y][0] << " != " << slideHypCube[x][y][slideHypcubeSize.hypcubeL-1] << std::endl;
                    numDifferent++;
                }
            }
        }

        std::cout << "numDifferent: " << numDifferent << std::endl;

    }*/

    //------------------------------------------------------
    //Discard unsensed spaces
    //------------------------------------------------------
    int initX   = lstImgs.at(0).width() - slideHypcubeSize.slideW;
    int finalX  = slideHypcubeSize.hypcubeW - lstImgs.at(0).width() + slideHypcubeSize.slideW;
    int cubeW   = slideHypcubeSize.hypcubeW -
                  (2*lstImgs.at(0).width()) +
                  (2*slideHypcubeSize.slideW);

    //------------------------------------------------------
    //Save Slide Hypcube
    //------------------------------------------------------





    int x, y, l;
    int binLen, binPos;
    binLen  = slideHypcubeSize.hypcubeW *
              slideHypcubeSize.hypcubeH *
              slideHypcubeSize.hypcubeL;
    binPos  = 0;
    u_int8_t* serialHypCube = (u_int8_t*)malloc(binLen*sizeof(u_int8_t));
    //Serialize Slide Hypercube
    if( mainExportSettings.flip )
    {
        for(x=finalX-1; x>=initX; x--)
        {
            for(y=0; y<slideHypcubeSize.hypcubeH; y++)
            {
                for(l=0; l<slideHypcubeSize.hypcubeL; l++)
                {
                    serialHypCube[binPos] = slideHypCube[x][y][l];
                    binPos++;
                }
            }
        }
    }
    else
    {
        for(x=0; x<finalX; x++)
        {
            for(y=0; y<slideHypcubeSize.hypcubeH; y++)
            {
                for(l=0; l<slideHypcubeSize.hypcubeL; l++)
                {
                    serialHypCube[binPos] = slideHypCube[x][y][l];
                    binPos++;
                }
            }
        }
    }
    //Create Output Filename
    QString outFilename;
    outFilename.append(destineDir);
    outFilename.append("slideHypcube");
    outFilename.append(_EXT_HYPERCUBE);
    //Save File
    if( saveBinFile_From_u_int8_T(
                                    outFilename.toStdString(),
                                    serialHypCube,
                                    binLen
                                  ) != _OK
    ){
        funcShowMsgERROR_Timeout("Saving Hypercube into HDD",parent);
    }

    //------------------------------------------------------
    //Save Slide Hypcube's Data
    //------------------------------------------------------
    QList<QString> lstFixtures, lstValues;
    lstFixtures << "Autor"
                << "Web"
                << "Contact"
                << "About"
                << "license"
                << "Date"
                << "Type"
                << "hypcubeW"           << "hypcubeH"               << "hypcubeL"
                << "initWavelength"     << "spectralResolution"     << "wavelengthUnits"
                << "codification"       << "pixLen"
                << "x_0"                << "y_0";

    lstValues   << "Jairo Salazar-Vazquez"
                << "dotech.us"
                << "jairosalazarvazquez@gmail.com"
                << "Generated with HypCam " + QString(_VERSION_TXT)
                << "GPL-GNU No Warranty"
                << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
                << "Raw Data"
                //<< QString::number(slideHypcubeSize.hypcubeW)//cubeW
                << QString::number(cubeW)
                << QString::number(slideHypcubeSize.hypcubeH)
                << QString::number(slideHypcubeSize.hypcubeL)
                << QString::number(slideHypcubeSize.minSpecRes)
                << QString::number(slideHypcubeSize.specResolution)
                << "nanometers"
                << "DIM(bands 1,2,...,L)"
                << "u_int8_t"
                << "Left"
                << "Up";

    outFilename.clear();
    outFilename.append(destineDir);
    outFilename.append("info.xml");
    funcSaveXML(outFilename,&lstFixtures,&lstValues,false);
    //Notify Success
    funcShowMsgSUCCESS_Timeout("Slide Hypercube Saved Successfully",parent);

    //------------------------------------------------------
    //Free Memory
    //------------------------------------------------------
    for( i=0; i<W; i++ )
    {
        for( j=0; j<H; j++ )
        {
            delete[] slideHypCube[i][j];
        }
        delete[] slideHypCube[i];
    }
    delete[] slideHypCube;
    std::cout << "Released HypCube Memory" << std::endl;


    /*
    clear all; close all; clc;

    %========================================================
    %Prepare for XML reading
    %========================================================
    javaaddpath ("/usr/share/java/xercesImpl.jar");
    javaaddpath ("/usr/share/java/xml-apis.jar");
    pkg load io;

    %========================================================
    %Get Cube Data
    %========================================================
    path        = "/home/jairo/Documentos/DESARROLLOS/build-HypCam-Desktop_Qt_5_8_0_GCC_64bit-Release/tmpImages/frames/testHypCube/";
    fileName    = [path "slideHypcube.hypercube"]
    parser = javaObject('org.apache.xerces.parsers.DOMParser');
    parser.parse([path "info.xml"]); % it seems that cd in octave are taken into account
    xDoc        = parser.getDocument;
    elem        = xDoc.getElementsByTagName('hypcubeW').item(0);
    W           = str2num(elem.getFirstChild.getTextContent);
    elem        = xDoc.getElementsByTagName('hypcubeH').item(0);
    H           = str2num(elem.getFirstChild.getTextContent);
    elem        = xDoc.getElementsByTagName('hypcubeL').item(0);
    L           = str2num(elem.getFirstChild.getTextContent);
    elem        = xDoc.getElementsByTagName('initWavelength').item(0);
    cubeSpecIni = elem.getFirstChild.getTextContent;
    elem        = xDoc.getElementsByTagName('spectralResolution').item(0);
    cubeSpecRes = elem.getFirstChild.getTextContent;

    %========================================================
    %Get HypCube
    %========================================================
    N           = W*H*L;
    img         = fopen(fileName,"r","ieee-le");
    lineImg     = fread(fileName,N,"uint8");
    fclose(img);
    X           = reshape( lineImg, [L H W] );

    %========================================================
    %Display Imagery
    %========================================================
    if 1
      for l=1:1:L
        figure(l);
        imshow( squeeze( X(l,:,:) )/255.0 );
      end
    end
    */


}

QImage formBuildSlideHypeCubePreview
        ::funcGetLayerFromHypCube(
                                    u_int8_t*** slideHypCube,
                                    const structSlideHypCubeSize &slideHypcubeSize,
                                    const int idLayer
                                 )
{
    QImage tmpLayer(
                        slideHypcubeSize.hypcubeW,
                        slideHypcubeSize.hypcubeH,
                        QImage::Format_RGB32
                   );
    tmpLayer.fill(Qt::GlobalColor::black);

    int x,y;
    for(x=0; x<slideHypcubeSize.hypcubeW; x++)
    {
        for(y=0; y<slideHypcubeSize.hypcubeH; y++)
        {
            tmpLayer.setPixelColor(
                                        QPoint(x,y),
                                        QColor(
                                                    slideHypCube[x][y][idLayer],
                                                    slideHypCube[x][y][idLayer],
                                                    slideHypCube[x][y][idLayer]
                                               )
                                   );
        }
    }

    return tmpLayer;

}

void formBuildSlideHypeCubePreview
     ::funcCalcLstWaveAndPositions(
                                        structSlideHypCubeSize* slideHypcubeSize,
                                        const structSlideCalibration &mainSlideCalibration
){
    //------------------------------------------------------
    //Calculate Frame's Position
    //------------------------------------------------------    
    //Calc Wavelegths and Positions
    int i, maxOverlapImgPos;
    float internWavelen;
    maxOverlapImgPos    = slideHypcubeSize->imgW - slideHypcubeSize->slideW;
    for( i=0; i<slideHypcubeSize->hypcubeL; i++ ){
        //Save Wavelength
        slideHypcubeSize->lstWavelengths[i]  = slideHypcubeSize->minSpecRes +
                                               ( i * slideHypcubeSize->specResolution );
        //Overlap Initial Position
        internWavelen       = slideHypcubeSize->lstWavelengths[i] - mainSlideCalibration.originWave;
        slideHypcubeSize->lstOverlapPos[i]   = round( funcApplyLR(
                                                                    internWavelen,
                                                                    mainSlideCalibration.wave2DistLR,
                                                                    false
                                                                )
                                                   );
        //std::cout << "dentrOverlap: " << slideHypcubeSize->lstOverlapPos[i] << std::endl;
        slideHypcubeSize->lstOverlapPos[i]   = slideHypcubeSize->lstOverlapPos[i] - round((float)slideHypcubeSize->slideW/2.0);
        slideHypcubeSize->lstOverlapPos[i]   = (slideHypcubeSize->lstOverlapPos[i]<0)?0:slideHypcubeSize->lstOverlapPos[i];//Minimum position
        slideHypcubeSize->lstOverlapPos[i]   = (slideHypcubeSize->lstOverlapPos[i]<maxOverlapImgPos)?slideHypcubeSize->lstOverlapPos[i]:maxOverlapImgPos;

        //Non-Overlap Initial Position
        slideHypcubeSize->lstNotOverlapPos[i]= slideHypcubeSize->lstOverlapPos[i] + slideHypcubeSize->overlapW;

        //Destine Init Positions
        slideHypcubeSize->lstDestineInitX[i] = slideHypcubeSize->lstOverlapPos[i];
    }
}


int formBuildSlideHypeCubePreview
    ::funcSlideSpectralImageUnzip(
                                      u_int8_t*** slideHypCube,
                                      int idImg,
                                      QImage* tmpImg,
                                      const structSlideHypCubeSize &slideHypcubeSize,
                                      const structSlideCalibration &mainSlideCalibration
){

    //----------------------------------------------
    //Reload Exporting Settings
    //----------------------------------------------
    funcReloadExportSettings();

    //----------------------------------------------
    //For Each Layer
    //----------------------------------------------
    int l;
    int frameOverlapPos, frameNonoverlapPos;
    int overlapW, nonOverlapW;
    int tmpType;
    QRect tmpOriginRect;
    int tmpDestineX;
    int wavePos;
    float internWavelen;

    strDenoisedColorSelected denColSel;

    overlapW    = slideHypcubeSize.overlapW;
    nonOverlapW = slideHypcubeSize.notOverlapW;
    for( l=0; l<slideHypcubeSize.hypcubeL; l++ )
    {
        //.............................................
        // Calc Wavelength Position
        //.............................................
        internWavelen   = mainExportSettings.expMinWave +
                          ( l * mainExportSettings.spectralResolution );
        internWavelen   = internWavelen - mainSlideCalibration.originWave;
        wavePos         = round(
                                    funcApplyLR(
                                                    internWavelen,
                                                    mainSlideCalibration.wave2DistLR,
                                                    false
                                                )
                                );

        //.............................................
        // Calc wS used to offset the sensor
        // capabitities
        //.............................................
        funcSlideDenoiseDefineSensorToUse(
                                            &denColSel,
                                            wavePos,
                                            mainSlideCalibration.sensitivities
                                          );


        //.............................................
        //Calc Position
        //.............................................
        frameOverlapPos     = slideHypcubeSize.lstOverlapPos[l];
        frameNonoverlapPos  = slideHypcubeSize.lstNotOverlapPos[l];
        tmpType             = (idImg==0)?copyMax:copyAverage;

        //.............................................
        //Overlap
        //.............................................
        //Origin Rect
        tmpOriginRect.setX( frameOverlapPos );
        tmpOriginRect.setY( 0 );
        tmpOriginRect.setWidth( overlapW );
        tmpOriginRect.setHeight( tmpImg->height() );
        //Destine into the HypCube
        tmpDestineX = frameOverlapPos + (idImg * nonOverlapW);
        //Copy
        funcCopyDiffToSlideHypCube(
                                        slideHypCube,
                                        tmpImg,
                                        tmpOriginRect,
                                        tmpDestineX,
                                        l,
                                        wavePos,
                                        mainSlideCalibration.sensitivities,
                                        denColSel.wS,
                                        tmpType
                                  );

        //.............................................
        //NonOverlap
        //.............................................
        //Origin Rect
        tmpOriginRect.setX( frameNonoverlapPos );
        tmpOriginRect.setY( 0 );
        tmpOriginRect.setWidth( nonOverlapW );
        tmpOriginRect.setHeight( tmpImg->height() );
        //Destine into the HypCube
        tmpDestineX += overlapW;
        funcCopyDiffToSlideHypCube(
                                        slideHypCube,
                                        tmpImg,
                                        tmpOriginRect,
                                        tmpDestineX,
                                        l,
                                        wavePos,
                                        mainSlideCalibration.sensitivities,
                                        denColSel.wS,
                                        copyMax
                                  );
    }
    return _OK;
}

int formBuildSlideHypeCubePreview
    ::funcCopyDiffToSlideHypCube(
                                    u_int8_t*** slideHypCube,
                                    QImage* tmpImg,
                                    const QRect &originRect,
                                    const int &destineX,
                                    const int &destineZ,
                                    const int &wavePos,
                                    const structSlideSensitivities &slideSens,
                                    const float &wS,
                                    const int &type
){

    int x, y;
    float tmpQE;
    u_int8_t tmpLastQE;
    QColor tmpColor;
    for( x=0; x<originRect.width(); x++ )
    {
        for( y=0; y<originRect.height(); y++ )
        {
            //Get the color
            tmpColor    = tmpImg->pixelColor(originRect.x()+x,y);
            tmpLastQE   = slideHypCube[destineX+x][y][destineZ];
            tmpQE       = funcPixelToQE( tmpColor, wavePos, slideSens, wS );
            //Evaluate and apply copy type
            if( type == copyAverage )
            {
                slideHypCube[destineX+x][y][destineZ] = (u_int8_t)round(((float)tmpLastQE + tmpQE)/2.0);
            }
            else if( type == copyOverride )
            {
                slideHypCube[destineX+x][y][destineZ] = (u_int8_t)tmpQE;
            }
            else if( type == copyMax )
            {
                slideHypCube[destineX+x][y][destineZ] = (tmpLastQE>(u_int8_t)tmpQE)?tmpLastQE:(u_int8_t)tmpQE;
            }else if( type == copyMin )
            {
                slideHypCube[destineX+x][y][destineZ] = (tmpLastQE>(u_int8_t)tmpQE)?(u_int8_t)tmpQE:tmpLastQE;
            }
        }
    }

    return _OK;
}
