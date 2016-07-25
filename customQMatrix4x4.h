#ifndef CUSTOMQMATRIX4x4_H
#define CUSTOMQMATRIX4x4_H


#include <QMatrix4x4>
#include <QGenericMatrix>
#include <QVector3D>
#include <QVector4D>
#include <__common.h>

class customQMatrix4x4 : public QMatrix4x4
{
    public:

        QVector3D getRowCells(int r, int col1, int col2, int col3);

        QVector4D getCol1Index(int col);

        qreal getCell(int r, int c);

        void setCell(int r, int c, qreal val);

        void setRow(int index, QVector4D values);

        QVector4D getRow(int index/*0 index*/);

        void print(QString title);

};

#endif // CUSTOMQMATRIX4x4_H
