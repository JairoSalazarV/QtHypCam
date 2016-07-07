#ifndef CUSTOMRECT_H
#define CUSTOMRECT_H

#include <QGraphicsRectItem>
#include <QAction>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <lstStructs.h>
#include <QPoint>

//#include <QObject>
//#include <QtGui>
//#include <QtCore>

class customRect : public QGraphicsRectItem
{
    //Q_OBJECT

public:
    customRectParameters parameters;
    customRect(QPoint p1, QPoint p2);
    QAction *showContMenuLine(QPoint pos);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    /*
signals:
    void signalCustRectAreaSelected(QPoint p1, QPoint p2);
*/

};

#endif // CUSTOMRECT_H
