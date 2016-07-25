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
    //setAcceptHoverEvents(true);
    //setAcceptTouchEvents(true);
    refreshTooltip();
}

void customLine::refreshTooltip(){
    QString tt;
    tt.append("("+ QString::number(this->x()) +","+ QString::number(this->y()) +")");
    this->setToolTip(tt);
}

void customLine::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //Remark line
    event->accept();
    QPen backPen = this->pen();
    this->setPen(QPen(Qt::white,4));
    QtDelay(250);
    this->setPen(backPen);
    this->parameters.movible = false;
    this->parameters.rotate = false;
    //Obtain user request
    QAction *tmpA = showContMenuLine(event->screenPos());

    if( tmpA!=0 ){
        if(tmpA->text()=="Move"){
            this->parameters.movible = true;
        }
        if(tmpA->text()=="Rotate"){
            this->parameters.rotate = true;
        }
        if(tmpA->text()=="Remove"){
            this->removeFromIndex();
        }
        if(tmpA->text()=="Right"){
        }
        update();
    }
}

QAction *customLine::showContMenuLine(QPoint pos){
    QMenu *xmenu = new QMenu();
    xmenu->addAction( "Move" );
    xmenu->addAction( "Rotate" );    

    xmenu->addSeparator();
    QMenu* submenu2 = xmenu->addMenu( "Save" );
    submenu2->addAction( "its rotation" );

    /*
    xmenu->addSeparator();
    QMenu* submenu3 = xmenu->addMenu( "It is on the" );
    submenu3->addAction( "Right" );
    submenu3->addAction( "Leftt" );
    submenu3->addAction( "Up" );
    submenu3->addAction( "Down" );
    */

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
    if(this->parameters.rotate){
        float inc = 0.1;
        switch( event->key() ){
            case Qt::Key_Up:{
                //this->parameters.rotation -= inc;
                setRotation(this->rotation()-inc);
                break;
            }
            case Qt::Key_Down:{
                //this->parameters.rotation += inc;
                setRotation(this->rotation()+inc);
                break;
            }
        }
        update();
    }
    refreshTooltip();
}
