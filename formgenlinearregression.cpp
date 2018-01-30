#include "formgenlinearregression.h"
#include "ui_formgenlinearregression.h"

#include <__common.h>
#include <QFileDialog>

formGenLinearRegression::formGenLinearRegression(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formGenLinearRegression)
{
    ui->setupUi(this);
}

formGenLinearRegression::~formGenLinearRegression()
{
    delete ui;
}

void formGenLinearRegression::mouseCursorWait(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void formGenLinearRegression::mouseCursorReset(){
    QApplication::restoreOverrideCursor();
}

void formGenLinearRegression::on_pbSelectFile_clicked()
{
    //Get Filename
    QString filePath = QFileDialog::getOpenFileName(
                                                        this,
                                                        tr("Select XML..."),
                                                        _PATH_CALIB,
                                                        "(*.hypcam);;"
                                                     );
    //Add Row
    if( !filePath.isEmpty() )
    {
        //Read file
        QString tmpFileContain;
        tmpFileContain = readAllFile(filePath);
        QList<QString> tmpParamters = tmpFileContain.split(",");

        //Add Row to Table
        ui->tableLstPoints->insertRow( ui->tableLstPoints->rowCount() );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        0,
                                        new QTableWidgetItem(QString( QString::number(ui->spinWavelen->value()) ))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        1,
                                        new QTableWidgetItem(QString(tmpParamters.at(0)))
                                  );
        ui->tableLstPoints->setItem(
                                        ui->tableLstPoints->rowCount()-1,
                                        2,
                                        new QTableWidgetItem(QString(tmpParamters.at(1)))
                                  );
    }
}

void formGenLinearRegression::on_pbRemoveItem_clicked()
{
    ui->tableLstPoints->removeRow( ui->tableLstPoints->currentRow() );
}

void formGenLinearRegression::on_pbGenRegression_clicked()
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
    if( saveFile(_PATH_CALIB_LR,LR) == false )
        funcShowMsgERROR("Saving "+QString(_PATH_CALIB_LR));
    else
        funcShowMsg("Success","File Saved Sucessfully");

}



