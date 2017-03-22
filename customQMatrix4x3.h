#ifndef CUSTOMQMATRIX4x3_H
#define CUSTOMQMATRIX4x3_H


#include <QMatrix4x3>
#include <QVector3D>
#include <QVector4D>
#include <QGenericMatrix>
#include <customQMatrix3x3.h>
//#include <customQMatrix4x3.h>

class customQMatrix4x3 : public QMatrix4x3
{
    public:
        //explicit customQMatrix4x3();

        qreal getCell(int r, int c);

        void setCell(int r, int c, qreal val);

        void setRow(int index, QVector3D values);

        QVector3D getRow(int index);

        QVector4D getCol1Index(int index);

        customQMatrix4x3 copy();

        void print(QString title);

        void isMultLinReg();

        //QVector3D calcBeta(QVector3D *y);

};

#endif // CUSTOMQMATRIX4x3_H
