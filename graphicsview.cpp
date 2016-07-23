#include "graphicsview.h"
#include <__common.h>

#include <QMenu>
#include <QGraphicsLineItem>

GraphicsView::GraphicsView(QObject *parent) : QGraphicsView(){
    //QGraphicsScene *canvasScene = new QGraphicsScene;
    //canvasView = new QGraphicsView(canvasScene);
    //this->setScene(canvasScene); // here you associate the scene with the view
    //setSceneRect(0, 0, 200, 200); // override your sizeGS() call
    //mousePos.reserve(exMaxPolygonSize);
    //canvasView->setMouseTracking(true);
    //canvasView->setRenderHints( QPainter::Antialiasing );
    parent = parent;
    //qDebug() << "constructor is ok";
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    emit signalMouseReleased(e);

    //qDebug() << "Signal mouse pressed emmited";
}


void GraphicsView::mousePressEvent(QMouseEvent *e)
{

    emit signalMousePressed(e);


    //Click context
    //..
    if(e->button() == Qt::RightButton)
    {
        QAction *request = showContextMenuLine(e->screenPos().toPoint());
        if(request!=0)
        {
            if(request->text()=="Display information")
                funcDisplayPixelProperties(e);

            if(request->text()=="By wavelenght")
                funcTestCalibration();

            if(request->text()=="rightShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 1);
            if(request->text()=="rightDrawWavelenght")
                funcShowWavelenLines(1);
            if(request->text()=="rightMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 1);
            if(request->text()=="rightMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 1);

            if(request->text()=="upShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 2);
            if(request->text()=="upDrawWavelenght")
                funcShowWavelenLines(2);
            if(request->text()=="upMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 2);
            if(request->text()=="upMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 2);

            if(request->text()=="leftShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 3);
            if(request->text()=="leftDrawWavelenght")
                funcShowWavelenLines(3);
            if(request->text()=="leftMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 3);
            if(request->text()=="leftMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 3);

            if(request->text()=="downShowWavelenght")
                funcShowWavelen(e->screenPos().toPoint(), 4);
            if(request->text()=="downDrawWavelenght")
                funcShowWavelenLines(4);
            if(request->text()=="downMinWavelenght")
                identifyMinimumWavelenght(e->screenPos().toPoint(), 4);
            if(request->text()=="downMaxWavelenght")
                identifyMaximumWavelenght(e->screenPos().toPoint(), 4);

        }
    }



}

void GraphicsView::funcShowWavelenLines(int type){

    //Gets camera calibration from .XML
    //..
    lstDoubleAxisCalibration calSett;
    funcGetCalibration(&calSett);
    //funcPrintCalibration(&calibSettings);

    //Gets linear regression
    //..
    float a, b;
    switch(type){
    case 1:
        a = calSett.rightLinRegA;
        b = calSett.rightLinRegB;
        break;
    case 2:
        a = calSett.upLinRegA;
        b = calSett.upLinRegB;
        break;
    case 3:
        a = calSett.leftLinRegA;
        b = calSett.leftLinRegB;
        break;
    case 4:
        a = calSett.downLinRegA;
        b = calSett.downLinRegB;
        break;
    }

    //Calculates line positions
    //..
    int rS, rX, gX, bX, offset;
    offset = 140;
    rS = round( a + (b*0.0) );//it is a
    rX = round( a + (b*(float)_RED_WAVELENGHT) );
    gX = round( a + (b*(float)_GREEN_WAVELENGHT) );
    bX = round( a + (b*(float)_BLUE_WAVELENGHT) );

    if(offset!=0){
        rS += offset;
        rX += offset;
        gX += offset;
        bX += offset;
    }

    //bX = 927;
    //gX = 981;
    //rX = 1016;

    //qDebug() << "W: " << this->scene()->width();
    //qDebug() << "H: " << this->scene()->height();
    //qDebug() << "rX: " << rX;
    //qDebug() << "gX: " << gX;
    //qDebug() << "bX: " << bX;

    //Draw lines
    //..
    //red

    QGraphicsLineItem *sourceLine   = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );
    QGraphicsLineItem *redLine      = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );
    QGraphicsLineItem *greenLine    = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );
    QGraphicsLineItem *blueLine     = new QGraphicsLineItem( 0, 0, 0, (qreal)this->scene()->height() );

    sourceLine->setPen(QPen(Qt::magenta));
    redLine->setPen(QPen(Qt::red));
    greenLine->setPen(QPen(Qt::green));
    blueLine->setPen(QPen(Qt::blue));

    this->scene()->addItem(sourceLine);
    this->scene()->addItem(redLine);
    this->scene()->addItem(greenLine);
    this->scene()->addItem(blueLine);

    sourceLine->setX(rS);
    redLine->setX(rX);
    greenLine->setX(gX);
    blueLine->setX(bX);


    sourceLine->setToolTip("origin = 0nm");
    redLine->setToolTip(QString::number(_RED_WAVELENGHT) + "nm");
    greenLine->setToolTip(QString::number(_GREEN_WAVELENGHT) + "nm");
    blueLine->setToolTip(QString::number(_BLUE_WAVELENGHT) + "nm");

    this->scene()->update();
    this->update();

}

void GraphicsView::funcShowWavelen(QPoint pos, int type){
    float wavelenght;
    int val;
    if(type<1 && type>4)
    {
        funcShowMsg("ERROR -> funcShowWavelen", "Type wrong");
        return (void)NULL;
    }
    if( type==1 || type==3 ){
        val = pos.x();
    }
    else
    {
        val = pos.y();
    }
    wavelenght = funcCalcWavelen( val, type );
    funcShowMsg("max avelenght",QString::number(wavelenght) +" | x: "+QString::number(pos.x()));
}

void GraphicsView::identifyMinimumWavelenght(QPoint pos, int type){
    float wavelenght;
    wavelenght = funcCalcWavelen( pos.x(), type );
    funcShowMsg("min wavelenght",QString::number(wavelenght));
}

void GraphicsView::identifyMaximumWavelenght(QPoint pos, int type){
    float wavelenght;
    wavelenght = funcCalcWavelen( pos.x(), type );
    funcShowMsg("max avelenght",QString::number(wavelenght));
}

float GraphicsView::funcCalcWavelen( int pixX, int type ){
    //Gets camera calibration from .XML
    //..
    lstDoubleAxisCalibration calSett;
    funcGetCalibration(&calSett);
    //funcPrintCalibration(&calibSettings);

    //It calculates wavelenght
    //..
    float a, b;
    switch( type )
    {
        case 1:
            a = calSett.rightLinRegA;
            b = calSett.rightLinRegB;
            break;
        case 2:
            a = calSett.upLinRegA;
            b = calSett.upLinRegB;
            break;
        case 3:
            a = calSett.leftLinRegA;
            b = calSett.leftLinRegB;
            break;
        case 4:
            a = calSett.downLinRegA;
            b = calSett.downLinRegB;
            break;
    }
    return ((float)pixX - a) / b;
}

void GraphicsView::funcDisplayPixelProperties(QMouseEvent *e)
{
    //Get the position clicked into scene
    //..
    QString tmpProp;
    QPointF tmpPoint = this->mapToScene(e->pos().x(),e->pos().y());
    tmpProp.append("x: "+QString::number(tmpPoint.x())+"\n");
    tmpProp.append("y: "+QString::number(tmpPoint.y())+"\n");
    //Show point
    funcShowMsg("Pixel properties", tmpProp);
}


void GraphicsView::funcTestCalibration()
{

    //Gets camera calibration from .XML
    //..
    strDiffPix *diffPix = (strDiffPix *)malloc(sizeof(strDiffPix));
    lstDoubleAxisCalibration calSett;
    funcGetCalibration(&calSett);
    //funcPrintCalibration(&calibSettings);

    //Draw contour
    //..
    int col;

    //Horizontal lines
    for(col=calSett.squarePixX; col<calSett.squarePixX+calSett.squarePixW; col++){
        diffPix->x = col;
        diffPix->y = calSett.squarePixY;
        funcSourcePixToDiffPix( diffPix, &calSett );
        scene()->addEllipse(diffPix->x,diffPix->y,1,1,QPen(Qt::white));
        scene()->addEllipse(diffPix->rightX,diffPix->rightY,1,1,QPen(Qt::red));
        //scene()->addEllipse(diffPix->upX,diffPix->upY,1,1,QPen(Qt::green));
        //scene()->addEllipse(diffPix->leftX,diffPix->leftY,1,1,QPen(Qt::blue));
        //scene()->addEllipse(diffPix->downX,diffPix->downY,1,1,QPen(Qt::magenta));
        qDebug() << "x: " << diffPix->x;
        qDebug() << "y: " << diffPix->y;

        qDebug() << "rightX: " << diffPix->rightX;
        qDebug() << "rightY: " << diffPix->rightY;

        exit(0);

    }





}

QAction *GraphicsView::showContextMenuLine(QPoint pos){
    QMenu *xmenu = new QMenu();

    //xmenu->addAction( "By wavelenght" );
    xmenu->addAction( "Display information" );



    xmenu->addSeparator();
    QMenu* submenu2  = xmenu->addMenu( "Identify" );
    QMenu* submenu21 = submenu2->addMenu( "Right" );
    QMenu* submenu22 = submenu2->addMenu( "Up" );
    QMenu* submenu23 = submenu2->addMenu( "Left" );
    QMenu* submenu24 = submenu2->addMenu( "Down" );

    submenu21->addAction( "rightShowWavelenght" );
    submenu21->addAction( "rightDrawWavelenght" );
    submenu21->addAction( "rightMinWavelenght" );
    submenu21->addAction( "rightMaxWavelenght" );

    submenu22->addAction( "upDrawWavelenght" );
    submenu22->addAction( "upShowWavelenght" );
    submenu22->addAction( "upMinWavelenght" );
    submenu22->addAction( "upMaxWavelenght" );

    submenu23->addAction( "leftDrawWavelenght" );
    submenu23->addAction( "leftShowWavelenght" );
    submenu23->addAction( "leftMinWavelenght" );
    submenu23->addAction( "leftMaxWavelenght" );

    submenu24->addAction( "downDrawWavelenght" );
    submenu24->addAction( "downShowWavelenght" );
    submenu24->addAction( "downMinWavelenght" );
    submenu24->addAction( "downMaxWavelenght" );


    //submenu2->addAction( "Green centroid" );
    //submenu2->addAction( "Blue centroid" );
    //submenu2->addSeparator();
    //submenu2->addAction( "Source centroid" );


    return xmenu->exec(pos);
}
