#ifndef SLIDEHYPCUBE_H
#define SLIDEHYPCUBE_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QVBoxLayout>
#include <lstStructs.h>
#include <QDialog>


namespace Ui {
class slideHypcube;
}

class slideHypcube : public QDialog
{
    Q_OBJECT

    enum{
            cubeEmpty,
            cubeLoaded,
            cubeExported
    };

    public:
        explicit slideHypcube(QObject *parent = 0);
        ~slideHypcube(){};

        strCubeParameters cubeParam;
        u_int8_t*** HypCube;
        QList<QImage> lstCubeThumbs;
        QString dirPath;
        int cubeStatus = cubeEmpty;

        int loadHypercube(QWidget* parent);

        QImage slideImgFromCube( const int &l=0 );


    signals:      

        void signalProgBarValue( int value, QString label );

};


#endif // SLIDEHYPCUBE_H
