#ifndef GENCALIBXML_H
#define GENCALIBXML_H

#include <QDialog>

namespace Ui {
class genCalibXML;
}

class genCalibXML : public QDialog
{
    Q_OBJECT

public:
    explicit genCalibXML(QWidget *parent = 0);
    ~genCalibXML();

private slots:
    void funcGetFilePath(QPushButton *tmpPb);

    void autoLoadCentroids();

    void setButton(QPushButton *tmpPb, QString tip, bool doAlert=true);

    void on_pbSource_clicked();

    void on_pbBlueRight_clicked();

    void on_pbGreenRight_clicked();

    void on_pbRedRight_clicked();

    void on_pbBlueRightUp_clicked();

    void on_pbGreenRightUp_clicked();

    void on_pbRedRightUp_clicked();

    void on_pbBlueUp_clicked();

    void on_pbGreenUp_clicked();

    void on_pbRedUp_clicked();

    void on_pbBlueLeftUp_clicked();

    void on_pbGreenLeftUp_clicked();

    void on_pbRedLeftUp_clicked();

    void on_pbBlueLeft_clicked();

    void on_pbGreenLeft_clicked();

    void on_pbRedLeft_clicked();

    void on_pbBlueLeftDown_clicked();

    void on_pbGreenLeftDown_clicked();

    void on_pbRedLeftDown_clicked();

    void on_pbBlueDown_clicked();

    void on_pbRedDown_clicked();

    void on_pbGreenDown_clicked();

    void on_pbBlueRightDown_clicked();

    void on_pbGreenRightDown_clicked();

    void on_pbRedRightDown_clicked();

private:
    Ui::genCalibXML *ui;
};

#endif // GENCALIBXML_H
