#include "graphicsview.h"
#include <__common.h>

#include <QMenu>

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
            if(request->text()=="By column")
            {
                funcShowColumnSlide(true);
            }

        }
    }



}

void GraphicsView::funcShowColumnSlide( bool ColumnByColumn ){

    //Gets camera calibration from .XML
    //..
    lstCalibFileNames calibSettings;
    funcGetCalibration(&calibSettings);
    //funcPrintCalibration(&calibSettings);

    //Calculates the square aperture rectangle
    //..
    squareAperture *squareArea = (squareAperture*)malloc(sizeof(squareAperture));



    if( ColumnByColumn )
    {

    }
    else
    {

    }


}

QAction *GraphicsView::showContextMenuLine(QPoint pos){
    QMenu *xmenu = new QMenu();

    xmenu->addAction( "By column" );
    xmenu->addAction( "By row" );



    //xmenu->addSeparator();
    //QMenu* submenu2 = xmenu->addMenu( "Calculate" );
    //submenu2->addAction( "Red centroid" );
    //submenu2->addAction( "Green centroid" );
    //submenu2->addAction( "Blue centroid" );
    //submenu2->addSeparator();
    //submenu2->addAction( "Source centroid" );


    return xmenu->exec(pos);
}
