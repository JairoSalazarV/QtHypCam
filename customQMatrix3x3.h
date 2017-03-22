#ifndef CUSTOMQMATRIX3x3_H
#define CUSTOMQMATRIX3x3_H


//#include <QMatrix3x3>
//#include <QMatrix4x3>
#include <QVector3D>
//#include <QGenericMatrix>
#include <QMatrix2x2>
//#include <__common.h>
//#include <QMatrix3x4>
//#include "customQMatrix4x3.h"



class customQMatrix3x3 : public QMatrix3x3
{
    public:

        qreal getCell(int r, int c);

        void setCell(int r, int c, qreal val);

        void setRow(int index, QVector3D values);

        QVector3D getRow(int index);

        double detSub2x2(int col);

        double determinant();

        customQMatrix3x3 inverted();

        customQMatrix3x3 copy();

        void print(QString title);

};

#endif // CUSTOMQMATRIX3x3_H
