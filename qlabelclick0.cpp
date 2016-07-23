//#include <__commonQt.h>
//#include <__comun.h>

#include "qlabelclick.h"

#include <QMouseEvent>
//#include <createarea.h>

//createArea* mainParent;
qlabelclick::qlabelclick(QWidget* parent)
{
    //mainParent = qobject_cast<createArea*>(parent);
}


void qlabelclick::mousePressEvent(QMouseEvent *event){
    QLabel::mousePressEvent(event);
    emit clicked(event->pos());
}

void qlabelclick::clicked(const QPoint &pos){
    //mainParent->refreshArea(QString::number(pos.x()),QString::number(pos.y()));
}
