#ifndef QLABELCLICK_H
#define QLABELCLICK_H

#include <QLabel>

class qlabelclick : public QLabel
{
public:
    qlabelclick(QWidget *parent=0);
protected:
    void mousePressEvent(QMouseEvent* event);
signals:
    void clicked(const QPoint & pos);
};

#endif // QLABELCLICK_H
