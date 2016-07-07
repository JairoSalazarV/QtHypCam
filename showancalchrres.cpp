#include "showancalchrres.h"
#include "ui_showancalchrres.h"
#include <customline.h>
#include <QPen>
#include <__common.h>
#include <QDir>

customLine *globalRedLine;
customLine *globalGreenLine;
customLine *globalBlueLine;
customLine *globalHLine;
customLine *globalVLine;
customLine *globalTmpLine;
bool globalIsHoriz;
calcAndCropSnap globalCalStruct;
customRect *globalRect;

showAnCalChrRes::showAnCalChrRes(customRect *rect, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showAnCalChrRes)
{
    ui->setupUi(this);

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


    //Prepare variables
    //..
    int w, h, W, H;
    QPixmap tmpPix(_DISPLAY_IMAGE);
    W = tmpPix.width();
    H = tmpPix.height();
    w = rect->parameters.canvas->width();
    h = rect->parameters.canvas->height();
    //Extrapolate dimensions
    qDebug() << globalCalStruct.x1 << ", " << globalCalStruct.y1;
    qDebug() << globalCalStruct.x2 << ", " << globalCalStruct.y2;
    globalCalStruct.X1 = round( ((float)W/(float)w)*(float)globalCalStruct.x1 );
    globalCalStruct.Y1 = round( ((float)H/(float)h)*(float)globalCalStruct.y1 );
    globalCalStruct.X2 = round( ((float)W/(float)w)*(float)globalCalStruct.x2 );
    globalCalStruct.Y2 = round( ((float)H/(float)h)*(float)globalCalStruct.y2 );
    globalCalStruct.lenW = abs(globalCalStruct.X2-globalCalStruct.X1);
    globalCalStruct.lenH = abs(globalCalStruct.Y2-globalCalStruct.Y1);
    //qDebug() << globalCalStruct.X1 << ", " << globalCalStruct.Y1;
    //qDebug() << globalCalStruct.X2 << ", " << globalCalStruct.Y2;
    //qDebug() << "lenW=" << globalCalStruct.lenW;
    //qDebug() << "lenH=" << globalCalStruct.lenH;

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
    ui->canvasCroped->resize(cropped.width()+tmpOffset,cropped.height()+tmpOffset);    
    float toolBarW = ui->frame->width();
    int newW = (cropped.width()>toolBarW)?cropped.width():toolBarW;
    this->resize(QSize(newW+tmpOffset,cropped.height()+tmpOffset+(tmpOffset*2)+ui->frame->height()));
    int framex = round((float)(this->width()-ui->frame->width())/2.0);
    int framey = ui->canvasCroped->height()+tmpOffset;
    ui->frame->move(QPoint(framex,framey));
    int canvasx = round((float)(this->width()-ui->canvasCroped->width())/2.0);
    ui->canvasCroped->move(QPoint(canvasx,0));

    //Update lines
    //..
    drawLines();

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

    tmpCanvas->scene()->addItem(redPeak);
    tmpCanvas->scene()->addItem(greenPeak);
    tmpCanvas->scene()->addItem(bluePeak);

    globalRedLine = redPeak;
    globalGreenLine = greenPeak;
    globalBlueLine = bluePeak;
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

    tmpCanvas->scene()->addItem(redPeak);
    tmpCanvas->scene()->addItem(greenPeak);
    tmpCanvas->scene()->addItem(bluePeak);

    globalRedLine = redPeak;
    globalGreenLine = greenPeak;
    globalBlueLine = bluePeak;
}

void showAnCalChrRes::on_chbBlue_clicked()
{
    drawLines();
}

void showAnCalChrRes::on_chbGreen_clicked()
{
    drawLines();
}

void showAnCalChrRes::on_chbRed_clicked()
{
    drawLines();
}

void showAnCalChrRes::drawLines(){
    if(globalRect->parameters.analCentroid==0){
        globalIsHoriz = (ui->canvasCroped->width()>ui->canvasCroped->height())?true:false;
        if(globalIsHoriz){//Horizontal
            updColSenHoriz();
        }else{//Vertical
            updColSenVert();
        }
    }

    if(globalRect->parameters.analCentroid==1){
        int x, y;
        updColSenHoriz();
        x = globalRedLine->x();        
        updColSenVert();
        y = globalRedLine->y();
        qDebug() << "rX: " << x;
        qDebug() << "rY: " << y;
        drawCenter(x,y,Qt::red);
    }

    if(globalRect->parameters.analCentroid==2){
        int x, y;
        updColSenHoriz();
        x = globalGreenLine->x();
        updColSenVert();
        y = globalGreenLine->y();
        qDebug() << "gX: " << x;
        qDebug() << "gY: " << y;
        drawCenter(x,y,Qt::green);
    }

    if(globalRect->parameters.analCentroid==3){
        int x, y;
        updColSenHoriz();
        x = globalBlueLine->x();
        updColSenVert();
        y = globalBlueLine->y();
        qDebug() << "bX: " << x;
        qDebug() << "bY: " << y;
        drawCenter(x,y,Qt::blue);
    }
    if(globalRect->parameters.analCentroid==4){
        int x, y;
        updColSenHoriz();
        x = round((float)(globalRedLine->x()+globalGreenLine->x()+globalBlueLine->x())/3.0);
        updColSenVert();
        y = round((float)(globalRedLine->y()+globalGreenLine->y()+globalBlueLine->y())/3.0);
        qDebug() << "sX: " << x;
        qDebug() << "sY: " << y;
        drawCenter(x,y,Qt::magenta);
    }
}

void showAnCalChrRes::drawCenter(int x, int y, Qt::GlobalColor color){
    ui->canvasCroped->scene()->clear();
    ui->canvasCroped->update();
    QtDelay(20);
    addLine2CanvasInPos(true,x,color);
    globalVLine = globalTmpLine;
    addLine2CanvasInPos(false,y,color);
    globalHLine = globalTmpLine;
}

void showAnCalChrRes::addLine2CanvasInPos(bool vertical, int pos, Qt::GlobalColor color){
    if(vertical){
        qDebug() << "vPos: " << pos;
        QPoint p1(0,0);
        QPoint p2(0,ui->canvasCroped->height());
        p1.setX(pos);
        p2.setX(pos);
        customLine *tmpLine = new customLine(p1,p2,QPen(color));
        globalTmpLine = tmpLine;
        ui->canvasCroped->scene()->addItem(tmpLine);
    }else{//Horizontal
        qDebug() << "hPos: " << pos;
        QPoint p1(0,0);
        QPoint p2(ui->canvasCroped->width(),0);
        p1.setY(pos);
        p2.setY(pos);
        customLine *tmpLine = new customLine(p1,p2,QPen(color));
        globalTmpLine = tmpLine;
        ui->canvasCroped->scene()->addItem(tmpLine);
    }
    ui->canvasCroped->update();
}

void showAnCalChrRes::on_pbCloseThis_clicked()
{
    this->close();
}

void showAnCalChrRes::on_pbSaveAnalysis_clicked()
{

    //Identify quadrant
    //..
    if(ui->txtQuadFilename->text().trimmed().isEmpty()){
        funcShowMsg("Lack","Type a file-name");
        ui->txtQuadFilename->setFocus();
        return (void)NULL;
    }
    //Save calibration file
    //..
    //FileName
    QString fileName;
    fileName.append("./settings/Calib/");
    fileName.append(ui->txtQuadFilename->text());
    fileName.append(".hypcam");
    QString coordinates;

    if(globalRect->parameters.analCentroid > 0){//Source
        int xPos, yPos;
        xPos  = globalCalStruct.X1 + globalVLine->x();
        yPos  = ui->canvasCroped->height() - globalHLine->y();
        yPos += globalCalStruct.Y1;
        coordinates.append(QString::number(xPos));
        coordinates.append(",");
        coordinates.append(QString::number(yPos));
    }else{
        //Obtain line positions
        //..
        int rPos,gPos,bPos;
        if(globalIsHoriz){
            rPos = globalCalStruct.X1 + globalRedLine->x();
            gPos = globalCalStruct.X1 + globalGreenLine->x();
            bPos = globalCalStruct.X1 + globalBlueLine->x();
        }else{
            rPos = ui->canvasCroped->height() - globalRedLine->y();
            gPos = ui->canvasCroped->height() - globalGreenLine->y();
            bPos = ui->canvasCroped->height() - globalBlueLine->y();
            rPos += globalCalStruct.Y1;
            gPos += globalCalStruct.Y1;
            bPos += globalCalStruct.Y1;
        }
        //File contain
        //..
        coordinates.append(QString::number(rPos));
        coordinates.append(",");
        coordinates.append(QString::number(gPos));
        coordinates.append(",");
        coordinates.append(QString::number(bPos));
    }

    //Save
    if(saveFile(fileName,coordinates)){
        funcShowMsg("Setting saver successfully","");
    }else{
        funcShowMsg("ERROR","Saving setting-file");
    }

}

void showAnCalChrRes::on_pbClearCalib_clicked()
{
    if(funcShowMsgYesNo("Alert","Delete all calibration measurements?")){
        QDir calibFolder("./settings/Calib/");
        calibFolder.removeRecursively();
        QDir().mkdir("./settings/Calib/");
    }
}
