#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QVBoxLayout>

#include <math.h>


class GraphicsView : public QGraphicsView
{
    Q_OBJECT

    enum{
            cubeEmpty,
            cubeLoaded,
            cubeExported
    };

    public:
        explicit GraphicsView(QObject *parent = 0);
        ~GraphicsView(){};

        QAction *showContextMenuLine(QPoint pos);

        int originalW;

        int originalH;

        //void displayHypercubeAnalysisScenary();

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

        void funcDisplayWavelength(QMouseEvent *e);

        void funcDiffractionDisplayWavelength(QMouseEvent *e);

        //void funcTestCalibration();

        void mousePressEvent(QMouseEvent *e);

        void mouseReleaseEvent(QMouseEvent *e);

        void mouseMoveEvent(QMouseEvent *e);        


    signals:
        void signalMouseReleased(QMouseEvent *e);

        void signalMousePressed(QMouseEvent *e);

        void signalMouseMove(QMouseEvent *e);        

        void signalProgBarValue( int value, QString label );

        void signalPixeleSelected(QMouseEvent *e);

};


#endif // GRAPHICSVIEW_H
