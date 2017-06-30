#include "formbuildslidehypecubepreview.h"
#include "ui_formbuildslidehypecubepreview.h"

#include <lstpaths.h>
#include <QGraphicsPixmapItem>

formBuildSlideHypeCubePreview::formBuildSlideHypeCubePreview(QWidget *parent) :
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

}

formBuildSlideHypeCubePreview::~formBuildSlideHypeCubePreview()
{
    delete ui;
}

void formBuildSlideHypeCubePreview::on_pbApply_clicked()
{

    QApplication::setOverrideCursor(Qt::WaitCursor);

    //------------------------------------------------------
    //Define List With Imagery to Be Considered
    //------------------------------------------------------

    //......................................................
    //Save Excecution
    //......................................................
    QString lastSlideFrames;
    lastSlideFrames = concatenateParameters();
    saveFile(_PATH_LAST_SLIDE_FRAMES_4CUBE,lastSlideFrames);

    //......................................................
    //Gel List of Files in Directory Selected
    //......................................................
    QString workDir = ui->txtFolder->text();
    QList<QFileInfo> lstFiles = funcListFilesInDir( workDir, _FRAME_RECEIVED_EXTENSION );
    //qDebug() << "lstFiles: " << lstFiles.length();
    if(lstFiles.size()==0)
    {
        funcShowMsgERROR("Empty Directory");
        return (void)false;
    }

    //......................................................
    //Read Hyperimage's Parameters
    //......................................................
    QString timelapseFile(workDir + _FILENAME_SLIDE_DIR_TIME);
    QString tmpParam;
    if( !readFileParam( timelapseFile, &tmpParam) )
    {
        funcShowMsgERROR("Timelapse Parameter not found at: " + workDir);
        tmpParam = "800";
        saveFile(timelapseFile,tmpParam);
    }
    int timelapse           = tmpParam.toInt(0);
    //qDebug() << "numFrames: " << lstFiles.size() << " time: " << timelapse;

    //......................................................
    //Discard Stabilization Frames
    //......................................................
    int numFrame2Discard    = ceil( (float)_RASPBERRY_STABILIZATION_TIME / (float)timelapse );
    int i;
    for( i=0; i<numFrame2Discard; i++ )
    {
        //qDebug() << "file discarded: " << lstFiles.at(i).completeBaseName();
        lstFiles.removeAt(0);
    }

    //------------------------------------------------------
    //Create Slide Cube Preview
    //------------------------------------------------------
    structSlideHypCube slideCubeSett;
    slideCubeSett.rotateLeft    = (ui->cbFlip->isChecked())?true:false;
    slideCubeSett.width         = ui->spinSlideW->value();
    slideCubeSett.extraW        = 0.3;
    QImage imgPreview           = buildSlideCubePreview(lstFiles,&slideCubeSett);


    //------------------------------------------------------
    //Display Result
    //------------------------------------------------------
    imgPreview.save(_PATH_DISPLAY_IMAGE);
    refreshGVImage(&imgPreview);

    QApplication::restoreOverrideCursor();

}

void formBuildSlideHypeCubePreview::refreshGVImage(QImage* imgPreview)
{
    ui->gvSlideHypCubePreview->scene()->clear();
    ui->gvSlideHypCubePreview->scene()->addPixmap(QPixmap::fromImage(*imgPreview));
    ui->gvSlideHypCubePreview->scene()->setSceneRect(0,0,imgPreview->width(),imgPreview->height());
    ui->gvSlideHypCubePreview->update();

    //.......................................................
    //Fix GraphicsView Resolution
    //.......................................................
    QRect screenRes = screenResolution(this);
    int maxW, maxH, xMargin, yMargin;
    xMargin     = 20;
    yMargin     = 150;
    maxW        = screenRes.width() - xMargin;
    maxH        = screenRes.height() - yMargin;
    *imgPreview  = imgPreview->scaledToWidth(maxW);
    *imgPreview  = (imgPreview->height()<maxH)?*imgPreview:imgPreview->scaledToHeight(maxH);
    ui->gvSlideHypCubePreview->setGeometry(QRect(0,80,imgPreview->width(),imgPreview->height()));
    ui->gvSlideHypCubePreview->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->gvSlideHypCubePreview->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );


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
    int posX;
    posX = round( ((float)ui->spinSlideLocation->value()/100.0) * (float)tmpImg.width());

    //----------------------------------------------------
    // Create Image To Display
    //----------------------------------------------------
    int i, w, extraW, h, r, c, slideX, red, green, blue;
    w       = slideCubeSettings->width;
    h       = tmpImg.height()-1;
    for( i=0; i<lstFrames.size()-1; i++ )
    {
        tmpImg      = calculateSubimage(lstFrames,slideCubeSettings,posX,i,i+1);



    }
    if( slideCubeSettings->rotateLeft == true )
    {
        rotateQImage(&resultImg,90);
    }
    return resultImg;
}

QImage formBuildSlideHypeCubePreview::calculateSubimage(
                                                     QList<QFileInfo> lstFrames,
                                                     structSlideHypCube* slideCubeSettings,
                                                     int x, int i, int j
){
    //--------------------------------------------------------------
    //Prepare Imagery
    //--------------------------------------------------------------

    //..............................................................
    //Read from HDD
    //..............................................................
    QImage imgLeft( lstFrames.at(i).absoluteFilePath() );
    QImage imgRight( lstFrames.at(j).absoluteFilePath() );

    //..............................................................
    //Cut Subimages Including Merge Band (Extra Width)
    //..............................................................
    int leftX, rightX, extraW;
    int w, h;
    extraW      = round( (float)slideCubeSettings->width * slideCubeSettings->extraW );
    w           = slideCubeSettings->width + extraW;
    h           = imgLeft.height();
    leftX       = x;
    rightX      = x - extraW;
    imgLeft     = imgLeft.copy(leftX,0,w,h);
    imgRight    = imgRight.copy(rightX,0,w,h);

    //..............................................................
    //Get Imagery Shifting by 2D Similarity
    //..............................................................
    QPoint shift2D = imageSimilarity2D(&imgLeft, &imgLeft);
    qDebug() << "Shifting: " << shift2D.x() << ", " << shift2D.y();
    exit(0);


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


void formBuildSlideHypeCubePreview::on_pbAutoStepIncrement_clicked()
{
    int aux;
    aux = ui->spinSlideW->value()+ui->spinAutoStep->value();
    int slideW = (aux>ui->spinSlideW->maximum())?ui->spinSlideW->maximum():aux;
    ui->spinSlideW->setValue(slideW);
    ui->pbApply->click();
}

void formBuildSlideHypeCubePreview::on_pbAutoStepDecrement_clicked()
{
    int aux;
    aux = ui->spinSlideW->value()-ui->spinAutoStep->value();
    int slideW = (aux>=1)?aux:1;
    ui->spinSlideW->setValue(slideW);
    ui->pbApply->click();
}

void formBuildSlideHypeCubePreview::on_pbFolder_clicked()
{
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
    ui->txtFolder->setText(workDir);

    //......................................................
    //Refresh Scene
    //......................................................
    ui->pbApply->click();
}

QString formBuildSlideHypeCubePreview::concatenateParameters(int firstTime)
{
    //firstTime -> new file to be created
    QString lstParameters;

    lstParameters = (firstTime)?_PATH_VIDEO_FRAMES:ui->txtFolder->text();
    lstParameters.append(","+QString::number(ui->spinSlideW->value()));
    lstParameters.append(","+QString::number(ui->spinSlideLocation->value()));
    lstParameters.append(","+QString::number(ui->spinAutoStep->value()));
    if(ui->cbFlip->isChecked())
        lstParameters.append(",1");
    else
        lstParameters.append(",0");

    return lstParameters;
}

int formBuildSlideHypeCubePreview::setLastExecution(QString parameters)
{
    //qDebug() << "parameters: " << parameters;
    if( parameters.isEmpty() )
        return _ERROR;

    ui->txtFolder->setText(parameters.split(",").at(0));
    ui->spinSlideW->setValue(parameters.split(",").at(1).toInt(0));
    ui->spinSlideLocation->setValue(parameters.split(",").at(2).toInt(0));
    ui->spinAutoStep->setValue(parameters.split(",").at(3).toInt(0));
    if( parameters.split(",").at(3).toInt(0) )
        ui->cbFlip->setChecked(true);
    return _OK;
}
