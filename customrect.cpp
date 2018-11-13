#include "customrect.h"
#include <QtGui>
#include <QApplication>
#include "__common.h"
#include <QMenu>

#include <showancalchrres.h>

customRect::customRect(QPoint p1, QPoint p2, QImage* originalImage)
{
    setRect(p1.x(),p1.y(),p2.x(),p2.y());
    //this->parameters.x = p1.x();
    //this->parameters.y = p1.y();
    //this->parameters.w = abs(p1.x()-p2.x());
    //this->parameters.h = abs(p1.y()-p2.y());
    //setPen(pen);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    this->refreshTooltip();

    internOrigEditImg = originalImage;

}


QAction *customRect::showContMenuLine(QPoint pos){
    QMenu *xmenu = new QMenu();
    xmenu->addAction( "Move" );
    xmenu->addAction( "Scale" );
    xmenu->addAction( "Analize" );
    xmenu->addAction( "Properties" );
    xmenu->addAction( "Save" );

    xmenu->addSeparator();
    QMenu* submenu2 = xmenu->addMenu( "Calculate" );
    submenu2->addAction( "Red centroid" );
    submenu2->addAction( "Green centroid" );
    submenu2->addAction( "Blue centroid" );
    submenu2->addSeparator();
    submenu2->addAction( "Source centroid" );

    xmenu->addSeparator();
    QMenu* submenu3 = xmenu->addMenu( "Save as" );
    submenu3->addAction( "Square aperture" );
    submenu3->addAction( "Square usable" );
    submenu3->addAction( "Region of interes" );
    submenu3->addAction( "Slide" );
    submenu3->addAction( "Slide diffraction" );
    submenu3->addAction( "Video Slide diffraction" );

    xmenu->addSeparator();
    QMenu* submenu4 = xmenu->addMenu( "Modify" );
    submenu4->addAction( "Auto-width" );
    submenu4->addAction( "Auto-height" );

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
                this->setRect(this->rect().x()+1,this->rect().y(),this->rect().width(),this->rect().height());
                break;
            }
            case Qt::Key_Left:{                
                this->setRect(this->rect().x()-1,this->rect().y(),this->rect().width(),this->rect().height());
                break;
            }
            case Qt::Key_Up:{
                this->setRect(this->rect().x(),this->rect().y()-1,this->rect().width(),this->rect().height());
                break;
            }
            case Qt::Key_Down:{
                this->setRect(this->rect().x(),this->rect().y()+1,this->rect().width(),this->rect().height());
                break;
            }
        }
        this->refreshTooltip();
        this->update();
    }
}

void customRect::refreshTooltip(){
    QString tt;
    qreal x,y,w,h;
    this->rect().getRect(&x,&y,&w,&h);
    tt.append("(" + QString::number(x) + ", " + QString::number(y) + "):");
    tt.append("(" + QString::number(w)+ ", " + QString::number(h) + ")");
    this->setToolTip(tt);
}

void customRect::mousePressEvent(QGraphicsSceneMouseEvent *event){

    //cameraResolution tmpCamRes;


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
            showAnCalChrRes *anaRes = new showAnCalChrRes(this,internOrigEditImg);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Save"){

            //Take fileName
            QString fileName;
            if(
                    funcLetUserDefineFile(
                                            &fileName,
                                            "Define output fileName",
                                            ".xml",
                                            new QString(_PATH_LAST_PATH_OPENED),
                                            new QString("./XML/")
                                         ) == _OK
            ){

                //Call the centroid
                saveSquareAs(fileName);
            }
        }
        if(a->text()=="Red centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 1;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this,internOrigEditImg);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Green centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 2;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this,internOrigEditImg);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Blue centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 3;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this,internOrigEditImg);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Source centroid"){
            //Call the centroid
            //..
            this->parameters.analCentroid = 4;
            showAnCalChrRes *anaRes = new showAnCalChrRes(this,internOrigEditImg);
            anaRes->setModal(true);
            anaRes->exec();
        }
        if(a->text()=="Square aperture"){
            //Call the centroid
            //..
            saveSquareAs(_PATH_SQUARE_APERTURE);
        }
        if(a->text()=="Square usable"){
            //Define usable area
            //..
            saveSquareAs(_PATH_SQUARE_USABLE);
        }

        if(a->text()=="Region of interes"){
            //Call the centroid
            //..
            saveSquareAs(_PATH_REGION_OF_INTERES);
        }

        if(a->text()=="Slide"){
            //Call the centroid
            //..
            saveSquareAs(_PATH_SLIDE_APERTURE);
        }
        if(a->text()=="Slide diffraction"){
            //Call the centroid
            //..
            saveSquareAs(_PATH_SLIDE_DIFFRACTION);
        }
        if(a->text()=="Video Slide diffraction"){
            //Call the centroid
            //..
            saveSquareAs(_PATH_VIDEO_SLIDE_DIFFRACTION);
        }
        if(a->text()=="Properties"){
            qreal x,y,w,h;
            this->rect().getRect(&x,&y,&w,&h);
            QString tmpPropMsg;
            tmpPropMsg.append("Canvas("+QString::number(this->parameters.W)+" x " + QString::number(this->parameters.H)  +")\n");
            tmpPropMsg.append("Pos("+QString::number(x)+", " + QString::number(y)  +")\n");
            tmpPropMsg.append(QString::number(w)+" x " + QString::number(h)  +"\n");
            funcShowMsg("Rectangle properties",tmpPropMsg);
        }

        if(a->text()=="Auto-width"){
            int tmpH = round( (float)this->rect().width() * ((float)camRes->height/(float)camRes->width) );
            this->setRect(this->rect().x(),this->rect().y(),this->rect().width(),tmpH);
        }

        if(a->text()=="Auto-height"){
            int tmpW = round( (float)this->rect().height() * ((float)camRes->width/(float)camRes->height) );
            this->setRect(this->rect().x(),this->rect().y(),tmpW,this->rect().height());
        }

        update();
    }
}


bool customRect::saveSquareAs(QString fileName){
    QString tmpContain;
    tmpContain.append( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" );
    tmpContain.append("<Variables>\n");
    tmpContain.append("\t<W>"+ QString::number( this->parameters.W  ) +"</W>\n");
    tmpContain.append("\t<H>"+ QString::number( this->parameters.H ) +"</H>\n");
    tmpContain.append("\t<x>"+ QString::number( this->rect().x() ) +"</x>\n");
    tmpContain.append("\t<y>"+ QString::number( this->rect().y() ) +"</y>\n");
    tmpContain.append("\t<w>"+ QString::number( this->rect().width() ) +"</w>\n");
    tmpContain.append("\t<h>"+ QString::number( this->rect().height() ) +"</h>\n");
    tmpContain.append("</Variables>");
    if( !saveFile( fileName, tmpContain ) )
    {
        funcShowMsg("ERROR","Saving rectangle");
        return false;
    }
    else
    {
        funcShowMsg("Success","Rectangle saved successfully");
    }
    return true;
}

