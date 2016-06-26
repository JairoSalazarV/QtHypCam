#include "graphicsview.h"



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
    emit signalMouseReleased(e);

    //qDebug() << "Signal mouse pressed emmited";
}


void GraphicsView::mousePressEvent(QMouseEvent *e)
{
    emit signalMousePressed(e);

    //qDebug() << "Signal mouse pressed emmited";
}
