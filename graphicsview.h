#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:
        explicit GraphicsView(QObject *parent = 0);
        ~GraphicsView(){};

        QAction *showContextMenuLine(QPoint pos);

    private:

        void funcShowColumnSlide( bool ColumnByColumn );

        void mousePressEvent(QMouseEvent *e);

        void mouseReleaseEvent(QMouseEvent *e);

    signals:
        void signalMouseReleased(QMouseEvent *e);

        void signalMousePressed(QMouseEvent *e);

};


#endif // GRAPHICSVIEW_H
