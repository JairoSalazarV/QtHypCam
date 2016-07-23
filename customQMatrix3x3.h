#ifndef CUSTOMQMATRIX3x3_H
#define CUSTOMQMATRIX3x3_H


#include <QMatrix3x3>
#include <QVector3D>
#include <QGenericMatrix>

class customQMatrix3x3 : public QMatrix3x3
{
    public:
        //explicit customQGenericMatrix();

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
