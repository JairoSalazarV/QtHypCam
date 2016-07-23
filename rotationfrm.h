#ifndef ROTATIONFRM_H
#define ROTATIONFRM_H

#include <QDialog>

namespace Ui {
class rotationFrm;
}

class rotationFrm : public QDialog
{
    Q_OBJECT    

public:
    explicit rotationFrm(QWidget *parent = 0);
    ~rotationFrm();

private:
    Ui::rotationFrm *ui;

signals:
    void angleChanged(float angle);

private slots:
    void on_pbSaveRotation_clicked();
    void on_spinAngle_valueChanged(double arg1);
    void on_pbLoadLastRotation_clicked();
};

#endif // ROTATIONFRM_H
