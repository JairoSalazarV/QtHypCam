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
        if(tmpA->text()=="Save"){
            funcSaveLineParameters();
        }
        if(tmpA->text()=="Remove"){
            this->removeFromIndex();
        }
        if(tmpA->text()=="Right"){
        }
        if(tmpA->text()=="Set Wavelength"){
            this->parameters.wavelength = funcGetParam("Wavelegth").trimmed().toFloat(0);
            funcShowMsgSUCCESS_Timeout("Wavelength Updated",Q_NULLPTR);
        }
        if(tmpA->text()=="Set Color"){
            QString newColor = funcGetParam("Color","#FFF");
            this->setPen(QPen(QColor(newColor)));
        }
        update();
    }
}

void customLine::funcSaveLineParameters()
{
    //----------------------------------------
    //Get the filename
    //----------------------------------------
    QString tmpName;
    if(
            func_getFilenameFromUser(
                                        &tmpName,
                                        _PATH_LAST_LINE_SAVED,
                                        "./XML/lines/",
                                        Q_NULLPTR
                                     ) != _OK
    ){
        return (void)false;
    }
    else
    {
        //Do nothing
        //funcShowMsg("Obt",tmpName);
    }

    //****************************************
    //Save line parameters
    //****************************************

    //----------------------------------------
    //Translate Canvas-Coordinates
    //to Img-Coordinates
    //----------------------------------------
    int x1, y1, x2, y2, imgW, imgH;
    imgW    = this->parameters.originalW;
    imgH    = this->parameters.originalH;
    x1      = this->line().p1().x();
    y1      = this->line().p1().y();
    x2      = this->line().p2().x();
    y2      = this->line().p2().y();
    x1      = (x1==0)?0:round( ( (float)x1/(float)this->parentSize.width() )  * (float)imgW);
    y1      = (y1==0)?0:round( ( (float)y1/(float)this->parentSize.height() ) * (float)imgH);
    x2      = (x2==0)?0:round( ( (float)x2/(float)this->parentSize.width() )  * (float)imgW);
    y2      = (y2==0)?0:round( ( (float)y2/(float)this->parentSize.height() ) * (float)imgH);

    //----------------------------------------
    //Fill List of Fixtures and Values
    //----------------------------------------
    QList<QString> lstFixtures;
    QList<QString> lstValues;
    lstFixtures.clear();
    lstValues.clear();
    lstFixtures << "imgW" << "imgH"
                << "x1" << "y1" << "x2" << "y2"
                << "colorR" << "colorG" << "colorB"
                << "wavelength" << "orientation";
    lstValues.append(QString::number(imgW));
    lstValues.append(QString::number(imgH));
    lstValues.append(QString::number(x1));
    lstValues.append(QString::number(y1));
    lstValues.append(QString::number(x2));
    lstValues.append(QString::number(y2));
    lstValues.append(QString::number(this->pen().color().red()));
    lstValues.append(QString::number(this->pen().color().green()));
    lstValues.append(QString::number(this->pen().color().blue()));
    lstValues.append(QString::number(this->parameters.wavelength));
    lstValues.append(QString::number(this->parameters.orientation));

    //----------------------------------------
    //Save line
    //----------------------------------------
    if( funcSaveXML(tmpName,&lstFixtures,&lstValues) != _OK )
    {
        funcShowMsgERROR("Saving Line Parameters");
    }
}

QAction *customLine::showContMenuLine(QPoint pos){
    QMenu *xmenu = new QMenu();
    xmenu->addAction( "Move" );
    xmenu->addAction( "Rotate" );
    xmenu->addAction( "Set Wavelength" );
    xmenu->addAction( "Set Color" );
    xmenu->addSeparator();
    xmenu->addAction( "Save" );

    /*
    xmenu->addSeparator();
    QMenu* submenu2 = xmenu->addMenu( "Save" );
    submenu2->addAction( "its rotation" );
    */

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
                if( this->parameters.orientation == _VERTICAL )
                    this->setLine(
                                    this->line().x1()+1,
                                    this->line().y1(),
                                    this->line().x2()+1,
                                    this->line().y2()
                                 );
                break;
            }
            case Qt::Key_Left:{
                if( this->parameters.orientation == _VERTICAL )
                    this->setLine(
                                    this->line().x1()-1,
                                    this->line().y1(),
                                    this->line().x2()-1,
                                    this->line().y2()
                                 );
                break;
            }
            case Qt::Key_Up:{
                if( this->parameters.orientation == _HORIZONTAL )
                    this->setLine(
                                    this->line().x1(),
                                    this->line().y1()-1,
                                    this->line().x2(),
                                    this->line().y2()-1
                                 );
                break;
            }
            case Qt::Key_Down:{
                if( this->parameters.orientation == _HORIZONTAL )
                    this->setLine(
                                    this->line().x1(),
                                    this->line().y1()+1,
                                    this->line().x2(),
                                    this->line().y2()+1
                                 );
                break;
            }
        }
        update();
    }
    if(this->parameters.rotate){
        float inc = 1;
        switch( event->key() ){
            case Qt::Key_Right:{
                if( this->parameters.orientation == _VERTICAL )
                    this->setLine(
                                    this->line().x1()+1,
                                    this->line().y1(),
                                    this->line().x2()-1,
                                    this->line().y2()
                                 );
                break;
            }
            case Qt::Key_Left:{
                if( this->parameters.orientation == _VERTICAL )
                    this->setLine(
                                    this->line().x1()-1,
                                    this->line().y1(),
                                    this->line().x2()+1,
                                    this->line().y2()
                                 );
                break;
            }
            case Qt::Key_Up:{
                if( this->parameters.orientation == _HORIZONTAL )
                    this->setLine(
                                    this->line().x1(),
                                    this->line().y1()+inc,
                                    this->line().x2(),
                                    this->line().y2()-inc
                                 );
                break;
            }
            case Qt::Key_Down:{
                if( this->parameters.orientation == _HORIZONTAL )
                    this->setLine(
                                    this->line().x1(),
                                    this->line().y1()-inc,
                                    this->line().x2(),
                                    this->line().y2()+inc
                                 );
                break;
            }
        }
        update();
    }
    refreshTooltip();
}
