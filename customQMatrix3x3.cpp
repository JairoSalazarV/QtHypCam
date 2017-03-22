#include <__common.h>
#include <customQMatrix3x3.h>




qreal customQMatrix3x3::getCell(int r, int c){//1 index
    return this->operator ()(c-1,r-1);
}

void customQMatrix3x3::setCell(int r, int c, qreal val){//1 index
    this->operator ()(c-1,r-1) = val;
}

void customQMatrix3x3::setRow(int index, QVector3D values)
{
    this->operator ()(0,index) = values.x();
    this->operator ()(1,index) = values.y();
    this->operator ()(2,index) = values.z();
}

QVector3D customQMatrix3x3::getRow(int index)
{    
    QVector3D row;
    row.setX(this->operator ()(0,index));
    row.setY(this->operator ()(1,index));
    row.setZ(this->operator ()(2,index));
    return row;
}

double customQMatrix3x3::detSub2x2(int col)
{
    double mat2x2[2][2];
    QVector3D row1, row2;
    row1 = this->getRow(1);
    row2 = this->getRow(2);
    switch(col)
    {
        case 0:
            mat2x2[0][0] = row1.y();
            mat2x2[0][1] = row1.z();
            mat2x2[1][0] = row2.y();
            mat2x2[1][1] = row2.z();
            break;
        case 1:
            mat2x2[0][0] = row1.x();
            mat2x2[0][1] = row1.z();
            mat2x2[1][0] = row2.x();
            mat2x2[1][1] = row2.z();
            break;
        case 2:
            mat2x2[0][0] = row1.x();
            mat2x2[0][1] = row1.y();
            mat2x2[1][0] = row2.x();
            mat2x2[1][1] = row2.y();
            break;
    }
    return (mat2x2[0][0] * mat2x2[1][1]) - (mat2x2[1][0] * mat2x2[0][1]);
}


double customQMatrix3x3::determinant()
{
    QVector3D row0;
    row0 = this->getRow(0);
    return  (row0.x() * detSub2x2(0) ) -
            (row0.y() * detSub2x2(1) ) +
            (row0.z() * detSub2x2(2) );
}

customQMatrix3x3 customQMatrix3x3::inverted()
{
    //Prepare variables and calculate inverse of determinant at same time
    //..
    customQMatrix3x3 inverted;
    float ratio = 1.0 / this->determinant();
    //qDebug() << "ratio: " << ratio;
    double **tmpM = (double**)malloc(2*sizeof(double*));
    tmpM[0] = (double*)malloc(sizeof(double));
    tmpM[1] = (double*)malloc(sizeof(double));

    //Fill cells and calculates inverse at same time
    //..
    //Row 1
    tmpM[0][0] = getCell(2,2); tmpM[0][1] = getCell(2,3);
    tmpM[1][0] = getCell(3,2); tmpM[1][1] = getCell(3,3);
    inverted.setCell(1,1,(ratio*funcDet2x2(tmpM)));

    tmpM[0][0] = getCell(1,3); tmpM[0][1] = getCell(1,2);
    tmpM[1][0] = getCell(3,3); tmpM[1][1] = getCell(3,2);
    inverted.setCell(1,2,(ratio*funcDet2x2(tmpM)));

    tmpM[0][0] = getCell(1,2); tmpM[0][1] = getCell(1,3);
    tmpM[1][0] = getCell(2,2); tmpM[1][1] = getCell(2,3);
    inverted.setCell(1,3,(ratio*funcDet2x2(tmpM)));

    //Row 2
    tmpM[0][0] = getCell(2,3); tmpM[0][1] = getCell(2,1);
    tmpM[1][0] = getCell(3,3); tmpM[1][1] = getCell(3,1);
    inverted.setCell(2,1,(ratio*funcDet2x2(tmpM)));

    tmpM[0][0] = getCell(1,1); tmpM[0][1] = getCell(1,3);
    tmpM[1][0] = getCell(3,1); tmpM[1][1] = getCell(3,3);
    inverted.setCell(2,2,(ratio*funcDet2x2(tmpM)));

    tmpM[0][0] = getCell(1,3); tmpM[0][1] = getCell(1,1);
    tmpM[1][0] = getCell(2,3); tmpM[1][1] = getCell(2,1);
    inverted.setCell(2,3,(ratio*funcDet2x2(tmpM)));

    //Row 3
    tmpM[0][0] = getCell(2,1); tmpM[0][1] = getCell(2,2);
    tmpM[1][0] = getCell(3,1); tmpM[1][1] = getCell(3,2);
    inverted.setCell(3,1,(ratio*funcDet2x2(tmpM)));

    tmpM[0][0] = getCell(1,2); tmpM[0][1] = getCell(1,1);
    tmpM[1][0] = getCell(3,2); tmpM[1][1] = getCell(3,1);
    inverted.setCell(3,2,(ratio*funcDet2x2(tmpM)));

    tmpM[0][0] = getCell(1,1); tmpM[0][1] = getCell(1,2);
    tmpM[1][0] = getCell(2,1); tmpM[1][1] = getCell(2,2);
    inverted.setCell(3,3,(ratio*funcDet2x2(tmpM)));

    //Free memory and return
    delete[] tmpM;
    return inverted;
}

customQMatrix3x3 customQMatrix3x3::copy()
{
    customQMatrix3x3 aux;
    aux.setRow(0,this->getRow(0));
    aux.setRow(1,this->getRow(1));
    aux.setRow(2,this->getRow(2));
    return aux;
}


void customQMatrix3x3::print(QString title)
{
    qDebug() << title;
    QVector3D aux;
    aux = this->getRow(0);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z();
    aux = this->getRow(1);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z();
    aux = this->getRow(2);
    qDebug() << aux.x() << ", " << aux.y() << ", " << aux.z();
    qDebug() << " ";
}


