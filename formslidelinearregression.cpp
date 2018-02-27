#include "formslidelinearregression.h"
#include "ui_formslidelinearregression.h"
#include <QFileDialog>
#include <__common.h>

formSlideLinearRegression::formSlideLinearRegression(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formSlideLinearRegression)
{
    ui->setupUi(this);

    ui->tableLstPoints->setColumnWidth(0,133);
    ui->tableLstPoints->setColumnWidth(1,105);
    ui->tableLstPoints->setColumnWidth(2,105);
    ui->tableLstPoints->setColumnWidth(3,115);
}

formSlideLinearRegression::~formSlideLinearRegression()
{
    delete ui;
}

void formSlideLinearRegression::on_pbSelectFile_clicked()
{
    //Get Line
    structLine tmpNewLine;
    if( funcReadLineFromXML(&tmpNewLine) == _OK )
    {
        //Add Row to Table
        ui->tableLstPoints->insertRow( ui->tableLstPoints->rowCount() );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        0,
                                        new QTableWidgetItem(QString::number(tmpNewLine.wavelength))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        1,
                                        new QTableWidgetItem(QString::number(tmpNewLine.x1)+", "+QString::number(tmpNewLine.y1))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        2,
                                        new QTableWidgetItem(QString::number(tmpNewLine.x2)+", "+QString::number(tmpNewLine.y2))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        3,
                                        new QTableWidgetItem(QString::number(tmpNewLine.canvasW)+", "+QString::number(tmpNewLine.canvasH))
                                  );
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableLstPoints->item(ui->tableLstPoints->rowCount()-1,3)->setTextAlignment(Qt::AlignCenter);
        //ui->tableLstPoints->rowAt(ui->tableLstPoints->rowCount()-1)->setTextAlignment(Qt::AlignCenter);
    }
}

void formSlideLinearRegression::on_pbGenRegression_clicked()
{
    mouseCursorWait();
    //Obtain Points and Distances
    int i, numPoints;
    numPoints = ui->tableLstPoints->rowCount();
    double distances[numPoints-1];
    double wavelengths[numPoints-1];
    double origin[2];
    double tmpPoint[2];
    origin[0] = ui->tableLstPoints->item(0,1)->text().trimmed().toDouble(0);
    origin[1] = ui->tableLstPoints->item(0,2)->text().trimmed().toDouble(0);
    for( i=1; i<numPoints; i++ )
    {
        tmpPoint[0]         = ui->tableLstPoints->item(i,1)->text().trimmed().toDouble(0);
        tmpPoint[1]         = ui->tableLstPoints->item(i,2)->text().trimmed().toDouble(0);
        wavelengths[i-1]    = ui->tableLstPoints->item(i,0)->text().trimmed().toDouble(0);
        distances[i-1]      = sqrt( pow( (origin[0]-tmpPoint[0])+(origin[1]-tmpPoint[1]), 2.0 ) );
        //funcShowMsgERROR( "Wave: " + QString::number(wavelengths[i-1]) + " Dist: " + QString::number(distances[i-1]) );
    }

    //Get Linear Regression
    linearRegresion LR_Dist2Wavelebgth, LR_Wavelebgth2Dist;
    LR_Dist2Wavelebgth = funcLinearRegression( distances,   wavelengths, numPoints-1 );
    LR_Wavelebgth2Dist = funcLinearRegression( wavelengths, distances,   numPoints-1 );

    //Save LR
    QString LR;
    LR = QString::number(LR_Dist2Wavelebgth.a)+","+QString::number(LR_Dist2Wavelebgth.b)+","+QString::number(LR_Wavelebgth2Dist.a)+","+QString::number(LR_Wavelebgth2Dist.b);
    mouseCursorReset();
    if( saveFile(_PATH_SLIDE_CALIB_LR,LR) == false )
        funcShowMsgERROR("Saving "+QString(_PATH_SLIDE_CALIB_LR));
    else
        funcShowMsg("Success","File Saved Sucessfully");
}

void formSlideLinearRegression::on_pbRemoveItem_clicked()
{
    ui->tableLstPoints->removeRow( ui->tableLstPoints->currentRow() );
}
