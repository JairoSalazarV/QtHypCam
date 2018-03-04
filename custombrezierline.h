#ifndef CUSTOMBREZIERLINE_H
#define CUSTOMBREZIERLINE_H

#include <QGraphicsObject>

class customBrezierLine : public QGraphicsObject
{
public:
    customBrezierLine(
                        const QPointF &initPoint,
                        const QPointF &middlePoint,
                        const QPointF &endPoint,
                        QGraphicsItem* parent = nullptr
                    );
protected:
    void paint(
                QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget=nullptr) override;   
private:
    QPointF initPoint;
    QPointF middlePoint;
    QPointF endPoint;
    QPainterPath getPath() const;
};

#endif // CUSTOMBREZIERLINE_H
