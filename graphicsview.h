#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:
        explicit GraphicsView(QObject *parent = 0);
        ~GraphicsView(){};

        QAction *showContextMenuLine(QPoint pos);

        //void funcShowWavelenLines(int type);

    private:
        //float funcCalcWavelen(int pixX , int type);

        //void funcShowWavelen(QPoint pos, int type);

        //void identifyMinimumWavelenght(QPoint pos, int type);

        void identifyMaximumWavelenght(QPoint pos, int type);

        void save(QString fileName);

        void disableScrolls();

        void enableScrolls();

        void funcDisplayPixelProperties(QMouseEvent *e);

        //void funcTestCalibration();

        void mousePressEvent(QMouseEvent *e);

        void mouseReleaseEvent(QMouseEvent *e);

    signals:
        void signalMouseReleased(QMouseEvent *e);

        void signalMousePressed(QMouseEvent *e);

};


#endif // GRAPHICSVIEW_H
