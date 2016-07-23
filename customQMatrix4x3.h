#ifndef CUSTOMQMATRIX4x3_H
#define CUSTOMQMATRIX4x3_H


#include <QMatrix4x3>
#include <QVector3D>
#include <QGenericMatrix>
#include <customQMatrix3x3.h>

class customQMatrix4x3 : public QMatrix4x3
{
    public:
        //explicit customQGenericMatrix();

        void setRow(int index, QVector3D values);

        QVector3D getRow(int index);

        QMatrix3x3 det3x3();

        QMatrix3x3 inver3x3();

        customQMatrix3x3 multiply(QMatrix3x4 *M1, customQMatrix4x3 *M2);

        customQMatrix4x3 copy();

        void print(QString title);

        void isMultLinReg();

        //QVector3D calcBeta(QVector3D *y);

};

#endif // CUSTOMQMATRIX4x3_H
