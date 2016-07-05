#include "customline.h"
#include <QtGui>
#include <QApplication>
#include "__common.h"
//#include <QGraphicsItem>
#include <QMenu>

customLine::customLine(QPoint p1, QPoint p2, QPen pen)
{
    setLine(p1.x(),p1.y(),p2.x(),p2.y());
    setPen(pen);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);    
}

void customLine::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //Remark line
    event->accept();
    QPen backPen = this->pen();
    this->setPen(QPen(Qt::white,4));
    QtDelay(250);
    this->setPen(backPen);
    this->parameters.movible = false;
    //Obtain user request
    QAction *a = showContMenuLine(event->screenPos());

    if( a!=0 ){
        if(a->text()=="Move"){
            this->parameters.movible = true;
        }
        if(a->text()=="Remove"){
            this->removeFromIndex();
        }
        if(a->text()=="Right"){
        }
        update();
    }
}

QAction *customLine::showContMenuLine(QPoint pos){
    QMenu *xmenu = new QMenu();
    xmenu->addAction( "Move" );
    xmenu->addSeparator();

    QMenu* submenu2 = xmenu->addMenu( "It is on the" );
    submenu2->addAction( "Right" );
    submenu2->addAction( "Leftt" );
    submenu2->addAction( "Up" );
    submenu2->addAction( "Down" );

    xmenu->addSeparator();
    xmenu->addAction( "Remove" );

    return xmenu->exec(pos);
}



void customLine::keyPressEvent(QKeyEvent *event){
    if(this->parameters.movible){
        switch( event->key() ){
            case Qt::Key_Right:{
                moveBy(1,0);
                break;
            }
            case Qt::Key_Left:{
                moveBy(-1,0);
                break;
            }
            case Qt::Key_Up:{
                moveBy(0,-1);
                break;
            }
            case Qt::Key_Down:{
                moveBy(0,1);
                break;
            }
        }
        update();
    }
}
