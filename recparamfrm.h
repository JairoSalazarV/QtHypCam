#ifndef RECPARAMFRM_H
#define RECPARAMFRM_H

#include <QDialog>

namespace Ui {
class recParamFrm;
}

class recParamFrm : public QDialog
{
    Q_OBJECT

public:
    explicit recParamFrm(QWidget *parent = 0);
    ~recParamFrm();

private slots:
    void on_pbSendParam_clicked();

private:
    Ui::recParamFrm *ui;

signals:
    void paramGenerated(QString param);

};

#endif // RECPARAMFRM_H
