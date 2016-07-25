#include "customQMatrix4x3.h"
//#include "customQMatrix3x3.h"


/*
customQMatrix4x3::customQMatrix4x3(){

}
*/

qreal customQMatrix4x3::getCell(int r, int c){//1 index
    return this->operator ()(c-1,r-1);
}

void customQMatrix4x3::setCell(int r, int c, qreal val){//1 index
    this->operator ()(c-1,r-1) = val;
}

void customQMatrix4x3::setRow(int index, QVector3D values)
{
    //0-index
    this->operator ()(0,index) = values.x();
    this->operator ()(1,index) = values.y();
    this->operator ()(2,index) = values.z();
}

QVector3D customQMatrix4x3::getRow(int index)
{    
    QVector3D row;
    row.setX(this->operator ()(0,index));
    row.setY(this->operator ()(1,index));
    row.setZ(this->operator ()(2,index));
    return row;
}

QVector4D customQMatrix4x3::getCol1Index(int index)
{
    QVector4D row;
    row.setX( getCell(1,index) );
    row.setX( getCell(2,index) );
    row.setX( getCell(3,index) );
    row.setX( getCell(4,index) );
    return row;
}


customQMatrix4x3 customQMatrix4x3::copy()
{
    customQMatrix4x3 aux;
    aux.setRow(0,this->getRow(0));
    aux.setRow(1,this->getRow(1));
    aux.setRow(2,this->getRow(2));
    aux.setRow(3,this->getRow(3));
    return aux;
}

void customQMatrix4x3::print(QString title)
{
    qDebug() << title;
    QVector3D aux;
    aux = this->getRow(0);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z();
    aux = this->getRow(1);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z();
    aux = this->getRow(2);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z();
    aux = this->getRow(3);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z();
    qDebug() << " ";
}

void customQMatrix4x3::isMultLinReg()
{
    //X(X'X)-1X'
    //QMatrix3x4 auxT = this->transposed();//X'
    //customQMatrix3x3 auxP = this->multiply3x4x3( &auxT, this );//(X'X)
    //auxP = auxP.inverted();//(X'X)-1





    /*
    aux = aux.transposed().copy();//X'
    aux.operator *=(this);//(X'X)
    aux = aux.inverted();//(X'X)-1
    aux.operator *=(this->transposed());//(X'X)-1X'
    this->operator *=(aux);
    */
}
