#ifndef FORMMERGESLIDECALIBRATIONS_H
#define FORMMERGESLIDECALIBRATIONS_H

#include <QDialog>
#include <__common.h>

namespace Ui {
class formMergeSlideCalibrations;
}

class formMergeSlideCalibrations : public QDialog
{
    Q_OBJECT

public:
    explicit formMergeSlideCalibrations(QWidget *parent = 0);
    ~formMergeSlideCalibrations();

private slots:
    void on_pbHorizontal_clicked();

    void on_pbVertical_clicked();

    void on_pbMergeCalibration_clicked();

    int funcSaveSlideCalibration(
                                    QString* pathDestine,
                                    structSlideCalibration* slideCalibration
                                );



private:
    Ui::formMergeSlideCalibrations *ui;
};

#endif // FORMMERGESLIDECALIBRATIONS_H
