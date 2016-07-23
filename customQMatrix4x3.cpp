#include "customQMatrix4x3.h"
#include "customQMatrix3x3.h"

void customQMatrix4x3::setRow(int index, QVector3D values)
{
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


QMatrix3x3 customQMatrix4x3::det3x3()
{

}

QMatrix3x3 customQMatrix4x3::inver3x3()
{

}

customQMatrix3x3 customQMatrix4x3::multiply(QMatrix3x4 *M1, customQMatrix4x3 *M2)
{
    customQMatrix3x3 auxP;
    int i, j, c;
    for(j=0;j<3;j++){
        for(i=0;i<3;i++){
            auxP.operator ()(j,i) = 0;
            for(c=0;c<4;c++){
                auxP.operator ()(j,i) += M1->operator ()(c,i) * M2->operator ()(j,c);
            }
        }
    }
    return auxP;
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
    this->print("Original");
    QMatrix3x4 auxT = this->transposed();//X'
    customQMatrix3x3 auxP = this->multiply( &auxT, this );
    auxP.print("Product");
    auxP = auxP.inverted();




    /*
    aux = aux.transposed().copy();//X'
    aux.operator *=(this);//(X'X)
    aux = aux.inverted();//(X'X)-1
    aux.operator *=(this->transposed());//(X'X)-1X'
    this->operator *=(aux);
    */
}
