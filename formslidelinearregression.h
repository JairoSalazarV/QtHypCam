#ifndef FORMSLIDELINEARREGRESSION_H
#define FORMSLIDELINEARREGRESSION_H

#include <QDialog>
#include <lstStructs.h>

namespace Ui {
class formSlideLinearRegression;
}

class formSlideLinearRegression : public QDialog
{
    Q_OBJECT

public:
    explicit formSlideLinearRegression(QWidget *parent = 0);
    ~formSlideLinearRegression();

private slots:
    void on_pbSelectFile_clicked();

    void on_pbGenRegression_clicked();

    void on_pbRemoveItem_clicked();

    bool funcAddRowToTable(QString* filePath);

    int funcSaveVerticalCalibrationFile(
                                            structLine* lowerVerLine,
                                            linearRegresion* dist2WaveLR,
                                            linearRegresion* wave2DistLR,
                                            linearRegresion* vertLR,
                                            structVerticalCalibrationSettings* vertCalibSettings
                                       );

    void on_pbGenHorRegression_clicked();

    //int scaleLen( int len, int canvasLen, int originalLen );

    void funcTableToList(QList<structLine>* lstLines);

    int funcUpdateVerticalCalibrationFile(structVerticalCalibration* vertCalibSettings);

    void on_pbGenAffinTrans_clicked();

    //void on_pbUpdateVerticalLR_clicked();

private:
    Ui::formSlideLinearRegression *ui;
};

#endif // FORMSLIDELINEARREGRESSION_H
