#include "customrect.h"
#include <QtGui>
#include <QApplication>
#include "__common.h"
#include <QMenu>

#include <showancalchrres.h>

customRect::customRect(QPoint p1, QPoint p2)
{
    setRect(p1.x(),p1.y(),p2.x(),p2.y());
    //setPen(pen);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
}


QAction *customRect::showContMenuLine(QPoint pos){
    QMenu *xmenu = new QMenu();
    xmenu->addAction( "Move" );
    xmenu->addAction( "Scale" );

    xmenu->addSeparator();
    xmenu->addAction( "Analize" );
    xmenu->addSeparator();
    QMenu* submenu2 = xmenu->addMenu( "Calculate" );
    submenu2->addAction( "Red centroid" );
    submenu2->addAction( "Green centroid" );
    submenu2->addAction( "Blue centroid" );
    submenu2->addSeparator();
    submenu2->addAction( "Source centroid" );

    /*
    xmenu->addSeparator();
    QMenu* submenu2 = xmenu->addMenu( "It is on the" );
    submenu2->addAction( "Right" );
    submenu2->addAction( "Leftt" );
    submenu2->addAction( "Up" );
    submenu2->addAction( "Down" );
    */

    xmenu->addSeparator();
    xmenu->addAction( "Remove" );

    return xmenu->exec(pos);
}


void customRect::keyPressEvent(QKeyEvent *event){

    //Scale
    //..
    if(this->parameters.scalable){
        qreal x1,y1,w,h;
        this->rect().getRect(&x1,&y1,&w,&h);
        switch( event->key() ){
            case Qt::Key_Right:{
                this->setRect(x1+1,y1,w-1,h);
                break;
            }
            case Qt::Key_Left:{
                this->setRect(x1-1,y1,w+1,h);
                break;
            }
            case Qt::Key_Up:{
                this->setRect(x1,y1-1,w,h+1);
                break;
            }
            case Qt::Key_Down:{
                this->setRect(x1,y1+1,w,h-1);
                break;
            }
        }
        update();
    }

    //Move
    //..
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


void customRect::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //Remark line
    event->accept();
    QPen backPen = this->pen();
    this->setPen(QPen(Qt::white,4));
    QtDelay(250);
    this->setPen(backPen);
    //Set to default
    this->parameters.movible = false;
    this->parameters.scalable = false;
    //Obtain user request
    QAction *a = showContMenuLine(event->screenPos());
    if( a!=0 ){
        if(a->text()=="Move"){
            this->parameters.movible = true;
        }
        if(a->text()=="Remove"){
            this->removeFromIndex();
        }
        if(a->text()=="Scale"){
            this->parameters.scalable = true;
        }
        if(a->text()=="Analize"){
            //Call the results GUI
            //..
            showAnCalChrRes *anaRes = new showAnCalChrRes(this);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Red centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 1;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Green centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 2;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Blue centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 3;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Source centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 4;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this);
            anaRes->setModal(true);
            anaRes->exec();
        }
        update();
    }
}
