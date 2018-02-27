#ifndef CUSTOMRECT_H
#define CUSTOMRECT_H

#include <QGraphicsRectItem>
#include <QAction>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <lstStructs.h>
#include <QPoint>

#include <QObject>
//#include <QtGui>
//#include <QtCore>

class customRect : public QGraphicsRectItem
{
    //Q_OBJECT    

public:
    customRectParameters parameters;

    customRect(QPoint p1, QPoint p2, QImage* originalImage);

    bool saveSquareAs(QString fileName);

    void refreshTooltip();

public slots:
    QAction *showContMenuLine(QPoint pos);

    //void emitSignal();

    /*
signals:
    virtual void saveSquareAperture(QRect* tmpRect);

    virtual void saveRegionOfInteres(customRect* tmpRect);
    */
private:
    QImage* internOrigEditImg;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // CUSTOMRECT_H
