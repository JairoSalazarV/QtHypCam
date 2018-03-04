#ifndef CUSTOMLINE_H
#define CUSTOMLINE_H

#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <lstStructs.h>

class customLine : public QGraphicsLineItem
{
public:
    QRect parentSize;
    customLineParameters parameters;
    customLine(QPoint p1, QPoint p2, QPen pen);
    void signalColorSelected(QString newColor);
    void funcSaveLineParameters();
    QAction *showContMenuLine(QPoint pos);
    void refreshTooltip();

protected:    
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    //void drawTmpRemarkLines(int x, int y, int len);

};

#endif // CUSTOMLINE_H
