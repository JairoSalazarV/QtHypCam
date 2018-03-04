#include "custombrezierline.h"
#include <QPainter>
#include <QPainterPath>

customBrezierLine::customBrezierLine(
                                        const QPointF &initPoint,
                                        const QPointF &middlePoint,
                                        const QPointF &endPoint,
                                        QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    this->initPoint     = initPoint;
    this->middlePoint   = middlePoint;
    this->endPoint      = endPoint;
}

void customBrezierLine::paint(
                                QPainter* painter,
                                const QStyleOptionGraphicsItem* option,
                                QWidget* widget
){
    painter->drawPath(this->getPath());
    widget = widget;
    option = option;
}

QPainterPath customBrezierLine::getPath() const
{
    QPainterPath path( this->initPoint );
    path.quadTo(this->middlePoint,this->endPoint);
    return path;
}
