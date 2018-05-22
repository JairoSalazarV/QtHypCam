#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

typedef struct strCubeParameters{
    int   W;
    int   H;
    int   L;
    float initWavelength;
    float spectralRes;
}strCubeParameters;

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

        strCubeParameters cubeParam;
        u_int8_t*** HypCube;
        QList<QImage> lstCubeThumbs;
        QString dirPath;
        int cubeStatus = cubeEmpty;


        QAction *showContextMenuLine(QPoint pos);

        int loadHypercube();

        int displayInternCubeThumb( int l=0 );

        QImage slideImgFromCube( const int &l );

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

};


#endif // GRAPHICSVIEW_H
