#include "showancalchrres.h"
#include <ui_showancalchrres.h>
#include <customline.h>
#include <QPen>
#include <__common.h>
#include <QDir>


bool globalIsHoriz;
calcAndCropSnap globalCalStruct;
customRect *globalRect;

customLine *globalRedLine;
customLine *globalGreenLine;
customLine *globalBlueLine;
customLine *globalHLine;
customLine *globalVLine;
customLine *globalTmpLine;

showAnCalChrRes::showAnCalChrRes(customRect *rect, QImage* origEditImg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showAnCalChrRes)
{
    ui->setupUi(this);

    //internOrigEditImg = origEditImg;

    //Get rectangle
    //..
    globalRect = rect;
    qreal rx1,ry1,rx2,ry2;
    //rect->rect().getRect(&rx1,&ry1,&rw,&rh);
    rect->rect().getCoords(&rx1,&ry1,&rx2,&ry2);    
    globalCalStruct.x1 = (int)rx1;
    globalCalStruct.y1 = (int)ry1;
    globalCalStruct.x2 = (int)rx2;
    globalCalStruct.y2 = (int)ry2;

    globalCalStruct.canvasW = rect->parameters.W;
    globalCalStruct.canvasH = rect->parameters.H;


    //Prepare variables
    //..
    int w, h, W, H;
    //QPixmap tmpPix(_PATH_DISPLAY_IMAGE);
    QPixmap tmpPix = QPixmap::fromImage(*origEditImg);
    W = tmpPix.width();
    H = tmpPix.height();
    w = rect->parameters.canvas->width();
    h = rect->parameters.canvas->height();
    //qDebug() << "Canvas w: " << w;
    //qDebug() << "Canvas h: " << h;
    //qDebug() << "x1: " << globalCalStruct.x1;
    //qDebug() << "y1: " << globalCalStruct.y1;
    //qDebug() << "x2: " << globalCalStruct.x2;
    //qDebug() << "y2: " << globalCalStruct.y2;
    //Extrapolate dimensions
    globalCalStruct.X1 = round( ((float)W/(float)w)*(float)globalCalStruct.x1 );
    globalCalStruct.Y1 = round( ((float)H/(float)h)*(float)globalCalStruct.y1 );
    globalCalStruct.X2 = round( ((float)W/(float)w)*(float)globalCalStruct.x2 );
    globalCalStruct.Y2 = round( ((float)H/(float)h)*(float)globalCalStruct.y2 );
    globalCalStruct.lenW = abs(globalCalStruct.X2-globalCalStruct.X1);
    globalCalStruct.lenH = abs(globalCalStruct.Y2-globalCalStruct.Y1);
    globalCalStruct.origImgW = W;
    globalCalStruct.origImgH = H;

    //qDebug() << "p1: " << globalCalStruct.x1 << ", " << globalCalStruct.y1;
    //qDebug() << "p2: " << globalCalStruct.x2 << ", " << globalCalStruct.y2;
    //qDebug() << "P1: " << globalCalStruct.X1 << ", " << globalCalStruct.Y1;
    //qDebug() << "P2: " << globalCalStruct.X2 << ", " << globalCalStruct.Y2;
    //qDebug() << "lenW: " << globalCalStruct.lenW;
    //qDebug() << "lenH: " << globalCalStruct.lenH;

    //Crop & show image
    //..
    //qDeleteAll(canvasSpec->scene()->items());
    int tmpOffset = 10;
    QPixmap cropped = tmpPix.copy( QRect( globalCalStruct.X1, globalCalStruct.Y1, globalCalStruct.lenW, globalCalStruct.lenH ) );
    cropped.save("./tmpImages/tmpCropped.ppm");
    QGraphicsScene *scene = new QGraphicsScene(0,0,cropped.width(),cropped.height());
    ui->canvasCroped->setBackgroundBrush(cropped);
    ui->canvasCroped->setCacheMode(QGraphicsView::CacheNone);
    ui->canvasCroped->setScene( scene );
    ui->canvasCroped->resize(cropped.width(),cropped.height());
    float toolBarW = ui->frame->width();
    int newW = (cropped.width()>toolBarW)?cropped.width():toolBarW;
    this->resize(QSize(newW+(tmpOffset*2),cropped.height()+(tmpOffset*2)+ui->frame->height()));
    int framex = round((float)(this->width()-ui->frame->width())/2.0);
    int framey = ui->canvasCroped->height()+tmpOffset;
    ui->frame->move(QPoint(framex,framey));
    int canvasx = round((float)(this->width()-ui->canvasCroped->width())/2.0);
    ui->canvasCroped->move(QPoint(canvasx,0));
    ui->canvasCroped->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->canvasCroped->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    //Update lines
    //..
    globalRedLine   = new customLine(QPoint(0,0),QPoint(0,0),QPen(Qt::red));
    globalGreenLine = new customLine(QPoint(0,0),QPoint(0,0),QPen(Qt::green));
    globalBlueLine  = new customLine(QPoint(0,0),QPoint(0,0),QPen(Qt::blue));
    drawSensitivities();

}

showAnCalChrRes::~showAnCalChrRes()
{
    delete ui;
}

void showAnCalChrRes::on_pbCloseDialog_clicked()
{
    this->destroy(true);
}

void showAnCalChrRes::updColSenVert(){
    //Accumulate values in each color
    //..
    QGraphicsView *tmpCanvas = ui->canvasCroped;

    tmpCanvas->scene()->clear();
    QImage tmpImg( "./tmpImages/tmpCropped.ppm" );
    int Red[tmpImg.height()];memset(Red,'\0',tmpImg.height());
    int Green[tmpImg.height()];memset(Green,'\0',tmpImg.height());
    int Blue[tmpImg.height()];memset(Blue,'\0',tmpImg.height());
    int r, c, maxR, maxG, maxB, yR, yG, yB;
    maxR = 0;
    maxG = 0;
    maxB = 0;
    yR   = 0;
    yG   = 0;
    yB   = 0;
    QRgb pixel;
    for(r=0;r<tmpImg.height();r++){
        Red[r]   = 0;
        Green[r] = 0;
        Blue[r]  = 0;
        for(c=0;c<tmpImg.width();c++){
            if(!tmpImg.valid(QPoint(c,r))){
                qDebug() << "Invalid r: " << r << "c: "<<c;
                qDebug() << "tmpImg.width(): " << tmpImg.width();
                qDebug() << "tmpImg.height(): " << tmpImg.height();
                return (void)NULL;
                close();
            }
            pixel     = tmpImg.pixel(QPoint(c,r));
            Red[r]   += qRed(pixel);
            Green[r] += qGreen(pixel);
            Blue[r]  += qBlue(pixel);
        }
        Red[r]   = round((float)Red[r]/tmpImg.width());
        Green[r] = round((float)Green[r]/tmpImg.width());
        Blue[r]  = round((float)Blue[r]/tmpImg.width());
        if( Red[r] > maxR ){
            maxR = Red[r];
            yR = r;
        }
        if( Green[r] > maxG ){
            maxG = Green[r];
            yG = r;
        }
        if( Blue[r] > maxB ){
            maxB = Blue[r];
            yB = r;
        }
    }
    int maxRGB = (maxR>maxG)?maxR:maxG;
    maxRGB = (maxB>maxRGB)?maxB:maxRGB;
    float upLimit = (float)tmpImg.width() * 0.7;

    //Draw camera's sensitivities
    //..
    int tmpPoint1, tmpPoint2;
    for(r=1;r<tmpImg.height();r++){
        if( ui->chbRed->isChecked() ){
            tmpPoint1 = ((float)Red[r-1]/((float)maxRGB)) * upLimit;
            tmpPoint2 = ((float)Red[r]/((float)maxRGB)) * upLimit;
            // = tmpHeight - tmpPoint1;
            //tmpPoint2 = tmpHeight - tmpPoint2;
            tmpCanvas->scene()->addLine( tmpPoint1, r, tmpPoint2, r+1, QPen(QColor("#FF0000")) );
        }
        if( ui->chbGreen->isChecked() ){
            tmpPoint1 = ((float)Green[r-1]/((float)maxRGB)) * upLimit;
            tmpPoint2 = ((float)Green[r]/((float)maxRGB)) * upLimit;
            //tmpPoint1 = tmpHeight - tmpPoint1;
            //tmpPoint2 = tmpHeight - tmpPoint2;
            tmpCanvas->scene()->addLine( tmpPoint1, r, tmpPoint2, r+1, QPen(QColor("#00FF00")) );
        }
        if( ui->chbBlue->isChecked() ){
            tmpPoint1 = ((float)Blue[r-1]/((float)maxRGB)) * upLimit;
            tmpPoint2 = ((float)Blue[r]/((float)maxRGB)) * upLimit;
            //tmpPoint1 = tmpHeight - tmpPoint1;
            //tmpPoint2 = tmpHeight - tmpPoint2;
            tmpCanvas->scene()->addLine( tmpPoint1, r, tmpPoint2, r+1, QPen(QColor("#0000FF")) );
        }
    }

    //Draw RGB peaks
    //..
    //Draw RGB peaks
    //..
    addLine2CanvasInPos(false,yR,Qt::red);
    globalRedLine = globalTmpLine;
    addLine2CanvasInPos(false,yG,Qt::green);
    globalGreenLine = globalTmpLine;
    addLine2CanvasInPos(false,yB,Qt::blue);
    globalBlueLine = globalTmpLine;

    globalRedLine->parameters.orientation   = _HORIZONTAL;
    globalGreenLine->parameters.orientation = _HORIZONTAL;
    globalBlueLine->parameters.orientation  = _HORIZONTAL;
    /*
    QPoint p1,p2;
    p1.setY(0);
    p1.setX(0);
    p2.setY(0);
    p2.setX(tmpImg.width());    
    customLine *redPeak = new customLine(p1,p2,QPen(Qt::red));
    customLine *greenPeak = new customLine(p1,p2,QPen(Qt::green));
    customLine *bluePeak = new customLine(p1,p2,QPen(Qt::blue));

    redPeak->setY(yR);
    greenPeak->setY(yG);
    bluePeak->setY(yB);
    if(ui->chbRedLine->isChecked()){
        tmpCanvas->scene()->addItem(redPeak);
    }
    if(ui->chbGreenLine->isChecked()){
        tmpCanvas->scene()->addItem(greenPeak);
    }
    if(ui->chbBlueLine->isChecked()){
        tmpCanvas->scene()->addItem(bluePeak);
    }
    globalRedLine = redPeak;
    globalGreenLine = greenPeak;
    globalBlueLine = bluePeak;
    */


}



void showAnCalChrRes::updColSenHoriz(){
    //Accumulate values in each color
    //..
    QGraphicsView *tmpCanvas = ui->canvasCroped;

    tmpCanvas->scene()->clear();
    QImage tmpImg( "./tmpImages/tmpCropped.ppm" );
    int Red[tmpImg.width()];memset(Red,'\0',tmpImg.width());
    int Green[tmpImg.width()];memset(Green,'\0',tmpImg.width());
    int Blue[tmpImg.width()];memset(Blue,'\0',tmpImg.width());
    int r, c, maxR, maxG, maxB, xR, xG, xB;
    maxR = 0;
    maxG = 0;
    maxB = 0;
    xR   = 0;
    xG   = 0;
    xB   = 0;
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
    int maxRGB = (maxR>maxG)?maxR:maxG;
    maxRGB = (maxB>maxRGB)?maxB:maxRGB;
    float upLimit = (float)tmpImg.height() * 0.7;

    //qDebug() << "c" << c << "maxR:"<<maxR<<" maxG:"<<maxG<<" maxB:"<<maxB;
    //qDebug() << "c" << c << "xR:"<<xR<<" xG:"<<xG<<" xB:"<<xB;
    //qDebug() << "tmpImg.width(): " << tmpImg.width();
    //qDebug() << "tmpImg.height(): " << tmpImg.height();

    //Draw camera's sensitivities
    //..
    int tmpPoint1, tmpPoint2, tmpHeight;
    tmpHeight = tmpImg.height();
    for(c=1;c<tmpImg.width();c++){
        if( ui->chbRed->isChecked() ){
            tmpPoint1 = ((float)Red[c-1]/((float)maxRGB)) * upLimit;
            tmpPoint2 = ((float)Red[c]/((float)maxRGB)) * upLimit;
            tmpPoint1 = tmpHeight - tmpPoint1;
            tmpPoint2 = tmpHeight - tmpPoint2;
            tmpCanvas->scene()->addLine( c, tmpPoint1, c+1, tmpPoint2, QPen(QColor("#FF0000")) );
        }
        if( ui->chbGreen->isChecked() ){
            tmpPoint1 = ((float)Green[c-1]/((float)maxRGB)) * upLimit;
            tmpPoint2 = ((float)Green[c]/((float)maxRGB)) * upLimit;
            tmpPoint1 = tmpHeight - tmpPoint1;
            tmpPoint2 = tmpHeight - tmpPoint2;
            tmpCanvas->scene()->addLine( c, tmpPoint1, c+1, tmpPoint2, QPen(QColor("#00FF00")) );            
        }
        if( ui->chbBlue->isChecked() ){
            tmpPoint1 = ((float)Blue[c-1]/((float)maxRGB)) * upLimit;
            tmpPoint2 = ((float)Blue[c]/((float)maxRGB)) * upLimit;
            tmpPoint1 = tmpHeight - tmpPoint1;
            tmpPoint2 = tmpHeight - tmpPoint2;
            tmpCanvas->scene()->addLine( c, tmpPoint1, c+1, tmpPoint2, QPen(QColor("#0000FF")) );
        }
    }

    //Draw RGB peaks
    //..
    addLine2CanvasInPos(true,xR,Qt::red);
    globalRedLine = globalTmpLine;
    addLine2CanvasInPos(true,xG,Qt::green);
    globalGreenLine = globalTmpLine;
    addLine2CanvasInPos(true,xB,Qt::blue);
    globalBlueLine = globalTmpLine;

    globalRedLine->parameters.orientation   = _VERTICAL;
    globalGreenLine->parameters.orientation = _VERTICAL;
    globalBlueLine->parameters.orientation  = _VERTICAL;

    /*
    QPoint p1,p2;
    p1.setX(0);
    p1.setY(0);
    p2.setX(0);
    p2.setY(tmpImg.height());
    customLine *redPeak = new customLine(p1,p2,QPen(Qt::red));
    customLine *greenPeak = new customLine(p1,p2,QPen(Qt::green));
    customLine *bluePeak = new customLine(p1,p2,QPen(Qt::blue));

    redPeak->setX(xR);
    greenPeak->setX(xG);
    bluePeak->setX(xB);
    if(ui->chbRedLine->isChecked()){
        tmpCanvas->scene()->addItem(redPeak);
    }
    if(ui->chbGreenLine->isChecked()){
        tmpCanvas->scene()->addItem(greenPeak);
    }
    if(ui->chbBlueLine->isChecked()){
        tmpCanvas->scene()->addItem(bluePeak);
    }
    globalRedLine = redPeak;
    globalGreenLine = greenPeak;
    globalBlueLine = bluePeak;
    */
}

void showAnCalChrRes::on_chbBlue_clicked()
{
    drawSensitivities();
}

void showAnCalChrRes::on_chbGreen_clicked()
{
    drawSensitivities();
}

void showAnCalChrRes::on_chbRed_clicked()
{
    drawSensitivities();
}

void showAnCalChrRes::drawRGBPeakLines(){

    //Red    
    if(ui->chbRedLine->isChecked())
    {
        globalRedLine->setVisible(true);
    }
    else
    {
        globalRedLine->setVisible(false);
    }
    //Green
    if(ui->chbGreenLine->isChecked())
    {
        globalGreenLine->setVisible(true);
    }
    else
    {
        globalGreenLine->setVisible(false);
    }
    //Blue
    if(ui->chbBlueLine->isChecked())
    {
        globalBlueLine->setVisible(true);
    }
    else
    {
        globalBlueLine->setVisible(false);
    }


    /*
    ui->canvasCroped->scene()->clear();
    drawSensitivities();
    if(ui->chbRedLine->isChecked()){
        ui->canvasCroped->scene()->addItem(globalRedLine);
    }
    if(ui->chbGreenLine->isChecked()){
        ui->canvasCroped->scene()->addItem(globalGreenLine);
    }
    if(ui->chbBlueLine->isChecked()){
        ui->canvasCroped->scene()->addItem(globalBlueLine);
    }
    */

    /*
    addLine2CanvasInPos(true,xR,Qt::red);
    globalRedLine = globalTmpLine;
    addLine2CanvasInPos(true,xG,Qt::green);
    globalRedLine = globalTmpLine;
    addLine2CanvasInPos(true,xB,Qt::blue);
    globalRedLine = globalTmpLine;
    */

}

void showAnCalChrRes::drawSensitivities(){
    if(globalRect->parameters.analCentroid==0){//No
        globalIsHoriz = (ui->canvasCroped->width()>ui->canvasCroped->height())?true:false;
        if(globalIsHoriz){//Horizontal
            updColSenHoriz();
        }else{//Vertical
            updColSenVert();
        }
    }

    if(globalRect->parameters.analCentroid==1){//Red
        int x, y;
        updColSenHoriz();
        x = globalRedLine->x();        
        updColSenVert();
        y = globalRedLine->y();
        //qDebug() << "rx: " << x;
        //qDebug() << "ry: " << y;
        drawCenter(x,y,Qt::red);
    }

    if(globalRect->parameters.analCentroid==2){//Green
        int x, y;
        updColSenHoriz();
        x = globalGreenLine->x();
        updColSenVert();
        y = globalGreenLine->y();
        //qDebug() << "gx: " << x;
        //qDebug() << "gy: " << y;
        drawCenter(x,y,Qt::green);
    }

    if(globalRect->parameters.analCentroid==3){//Blue
        int x, y;
        updColSenHoriz();
        x = globalBlueLine->x();
        updColSenVert();
        y = globalBlueLine->y();
        //qDebug() << "bx: " << x;
        //qDebug() << "by: " << y;
        drawCenter(x,y,Qt::blue);
    }
    if(globalRect->parameters.analCentroid==4){//Source
        int x, y;
        updColSenHoriz();
        x = round((float)(globalRedLine->x()+globalGreenLine->x()+globalBlueLine->x())/3.0);
        updColSenVert();
        y = round((float)(globalRedLine->y()+globalGreenLine->y()+globalBlueLine->y())/3.0);       
        qDebug() << "sx: " << x;
        qDebug() << "sy: " << y;
        qDebug() << "sX: " << globalCalStruct.X1;
        qDebug() << "sY: " << globalCalStruct.Y1;
        drawCenter(x,y,Qt::magenta);
    }
}

void showAnCalChrRes::drawCenter(int x, int y, Qt::GlobalColor color){
    ui->canvasCroped->scene()->clear();
    ui->canvasCroped->update();
    QtDelay(20);
    addLine2CanvasInPos(true,x,color);
    globalVLine = globalTmpLine;
    //qDebug() << "globalVLine_x: " << globalVLine->x();
    //qDebug() << "globalVLine_y: " << globalVLine->y();
    addLine2CanvasInPos(false,y,color);
    globalHLine = globalTmpLine;
    //qDebug() << "globalHLine_x: " << globalHLine->x();
    //qDebug() << "globalHLine_y: " << globalHLine->y();
}

void showAnCalChrRes::addLine2CanvasInPos(bool vertical, int pos, Qt::GlobalColor color){
    if(vertical)
    {
        qDebug() << "vPos: " << pos;
        QPoint p1(0,0);
        QPoint p2(0,ui->canvasCroped->scene()->height());
        //p1.setX(pos);
        //p2.setX(pos);
        customLine *tmpLine = new customLine(p1,p2,QPen(color));
        ui->canvasCroped->scene()->addItem(tmpLine);
        tmpLine->setX(pos);
        //tmpLine->mapToParent(p1.x(),p1.y(),1,p2.y());
        tmpLine->mapToScene(p1.x(),p1.y(),1,p2.y());
        globalTmpLine = tmpLine;
        tmpLine->refreshTooltip();
    }
    else
    {//Horizontal
        qDebug() << "hPos: " << pos;
        QPoint p1(0,0);
        QPoint p2(ui->canvasCroped->scene()->width(),0);
        //p1.setY(pos);
        //p2.setY(pos);
        customLine *tmpLine = new customLine(p1,p2,QPen(color));        
        ui->canvasCroped->scene()->addItem(tmpLine);
        tmpLine->setY(pos);
        tmpLine->mapToScene(p1.x(),p1.y(),1,p2.y());
        //tmpLine->mapToParent(p1.x(),p1.y(),1,p2.y());
        globalTmpLine = tmpLine;
        tmpLine->refreshTooltip();
    }
    ui->canvasCroped->update();
}

void showAnCalChrRes::on_pbCloseThis_clicked()
{
    this->close();
}

void showAnCalChrRes::on_pbSaveAnalysis_clicked()
{
    //----------------------------------------------------
    //Identify file-name's base
    //----------------------------------------------------
    if(ui->txtQuadFilename->text().trimmed().isEmpty()){
        funcShowMsg("Lack","Type a file-name");
        ui->txtQuadFilename->setFocus();
        return (void)NULL;
    }

    //----------------------------------------------------
    //Save calibration file
    //----------------------------------------------------
    //FileName
    QString fileName;
    fileName.append("./settings/Calib/");
    fileName.append(ui->txtQuadFilename->text());
    fileName.append(".hypcam");
    QString coordinates;

    if(globalRect->parameters.analCentroid > 0)
    {
        //----------------------------------------------------
        //Centroid
        //----------------------------------------------------
        int xPos, yPos;
        xPos = globalCalStruct.X1 + globalVLine->x();
        yPos = globalCalStruct.Y1 + globalHLine->y();

        coordinates.append(QString::number(xPos));
        coordinates.append(",");
        coordinates.append(QString::number(yPos));
    }
    else
    {
        //----------------------------------------------------
        //Obtain line positions
        //----------------------------------------------------
        int rPos=0, gPos=0, bPos=0;
        if(globalIsHoriz)
        {
            rPos = globalCalStruct.X1 + globalRedLine->x()   + globalRedLine->line().x1();   //Rectangle Corner + Initial Position + Movement
            gPos = globalCalStruct.X1 + globalGreenLine->x() + globalGreenLine->line().x1(); //Rectangle Corner + Initial Position + Movement
            bPos = globalCalStruct.X1 + globalBlueLine->x()  + globalBlueLine->line().x1();  //Rectangle Corner + Initial Position + Movement
            //rPos = globalCalStruct.X1 + globalRedLine->x();
            //gPos = globalCalStruct.X1 + globalGreenLine->x();
            //bPos = globalCalStruct.X1 + globalBlueLine->x();
        }
        else
        {
            rPos += globalCalStruct.Y1 + globalRedLine->y() + globalRedLine->line().y1();     //Rectangle Corner + Initial Position + Movement
            gPos += globalCalStruct.Y1 + globalGreenLine->y() + globalGreenLine->line().y1(); //Rectangle Corner + Initial Position + Movement
            bPos += globalCalStruct.Y1 + globalBlueLine->y() + globalBlueLine->line().y1() ;  //Rectangle Corner + Initial Position + Movement

            /*
            qDebug() << "Vertical";

            qDebug() << "globalCalStruct.Y1: " << globalCalStruct.Y1;
            qDebug() << "globalRedLine->y(): " << globalRedLine->y();
            qDebug() << "globalGreenLine->y(): " << globalGreenLine->y();
            qDebug() << "globalBlueLine->y(): " << globalBlueLine->y();

            qDebug() << "globalRedLine->line().y1(): " << globalRedLine->line().y1();
            qDebug() << "globalGreenLine->line().y1(): " << globalGreenLine->line().y1();
            qDebug() << "globalBlueLine->line().y1(): " << globalBlueLine->line().y1();

            qDebug() << "rPos: " << rPos;
            qDebug() << "gPos: " << gPos;
            qDebug() << "bPos: " << bPos;*/

        }

        //----------------------------------------------------
        //File contain
        //----------------------------------------------------
        coordinates.append(QString::number(rPos));
        coordinates.append(",");
        coordinates.append(QString::number(gPos));
        coordinates.append(",");
        coordinates.append(QString::number(bPos));
        coordinates.append(",");
        coordinates.append(QString::number(globalCalStruct.canvasW));
        coordinates.append(",");
        coordinates.append(QString::number(globalCalStruct.canvasH));
        coordinates.append(",");
        coordinates.append(QString::number(globalCalStruct.origImgW));
        coordinates.append(",");
        coordinates.append(QString::number(globalCalStruct.origImgH));
        coordinates.append(", Red-Green-Blue-canvasW-canvasH-origImgW-origImgH");
    }

    //----------------------------------------------------
    //Save
    //----------------------------------------------------
    //Save coordinates
    if(saveFile(fileName,coordinates)){
        //Save canvas background path
        saveFile(_PATH_CALBKG,globalRect->parameters.backgroundPath);
        funcShowMsg(" ","Setting Saved Successfully");
    }else{
        funcShowMsg("ERROR","Saving Setting-file");
    }

}

void showAnCalChrRes::on_pbClearCalib_clicked()
{
    if(funcShowMsgYesNo("Alert","Delete all calibration measurements?")){
        QDir calibFolder("./settings/Calib/");
        calibFolder.removeRecursively();
        QDir().mkdir("./settings/Calib/");
        QDir().mkdir("./settings/Calib/images/");
    }
}

void showAnCalChrRes::on_pbSaveScene_clicked()
{
    //Identify filename
    //..
    if(ui->txtQuadFilename->text().trimmed().isEmpty()){
        funcShowMsg("Lack","Type a file-name");
        ui->txtQuadFilename->setFocus();
        return (void)NULL;
    }
    //FileName
    //..
    QString fileName;
    fileName.append("./settings/Calib/images/");
    fileName.append(ui->txtQuadFilename->text());
    fileName.append(".png");
    //Save
    //..
    //Remove if exists
    QFile prevImg(fileName);
    if(prevImg.exists()){
        prevImg.remove();
    }
    prevImg.close();
    //Save Graphicsview's scene
    QPixmap pixMap = QPixmap::grabWidget(ui->canvasCroped);
    if(pixMap.save(fileName)){
        funcShowMsg(" ","Image saved successfully");
    }else{
        funcShowMsg("ERROR","Saving image");
    }

}

void showAnCalChrRes::on_chbRedLine_clicked()
{
    drawRGBPeakLines();
}

void showAnCalChrRes::on_chbGreenLine_clicked()
{
    drawRGBPeakLines();
}

void showAnCalChrRes::on_chbBlueLine_clicked()
{
    drawRGBPeakLines();
}

void showAnCalChrRes::on_pbInFolder_clicked()
{
    funcOpenFolder("./settings/Calib/");
}
