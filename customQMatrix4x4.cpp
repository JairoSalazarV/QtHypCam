#include "customQMatrix4x4.h"



QVector3D customQMatrix4x4::getRowCells(int r, int col1, int col2, int col3)
{
    //r,col1-3 as 1-index
    QVector3D tmpRes;
    tmpRes.setX(getCell(r,col1));
    tmpRes.setY(getCell(r,col2));
    tmpRes.setZ(getCell(r,col3));
    return tmpRes;
}

QVector4D customQMatrix4x4::getCol1Index(int col)
{
    //col in 1-index
    QVector4D tmpRes;
    tmpRes.setX(getCell(1,col));
    tmpRes.setY(getCell(2,col));
    tmpRes.setZ(getCell(3,col));
    tmpRes.setW(getCell(4,col));
    return tmpRes;
}

qreal customQMatrix4x4::getCell(int r, int c){//1 index
    return this->operator ()(c-1,r-1);
}

void customQMatrix4x4::setCell(int r, int c, qreal val){//1 index
    this->operator ()(c-1,r-1) = val;
}

void customQMatrix4x4::setRow(int index, QVector4D values)
{
    this->operator ()(0,index) = values.x();
    this->operator ()(1,index) = values.y();
    this->operator ()(2,index) = values.z();
    this->operator ()(3,index) = values.w();
}

QVector4D customQMatrix4x4::getRow(int index)
{    
    QVector4D row;
    row.setX(this->operator ()(0,index));
    row.setY(this->operator ()(1,index));
    row.setZ(this->operator ()(2,index));
    row.setW(this->operator ()(3,index));
    return row;
}


void customQMatrix4x4::print(QString title)
{
    qDebug() << title;
    QVector4D aux;
    aux = getRow(0);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z() << ", " << aux.w();
    aux = getRow(1);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z() << ", " << aux.w();
    aux = getRow(2);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z() << ", " << aux.w();
    aux = getRow(3);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z() << ", " << aux.w();
    qDebug() << " ";
}


