#ifndef SLIDEHYPCAM_H
#define SLIDEHYPCAM_H

#include <QMainWindow>

namespace Ui {
class slideHypCam;
}

class slideHypCam : public QMainWindow
{
    Q_OBJECT

public:
    explicit slideHypCam(QWidget *parent = 0);
    ~slideHypCam();
    void readSerialPort();

private slots:
    void on_pbSendSerial_clicked();

private:
    Ui::slideHypCam *ui;
};

#endif // SLIDEHYPCAM_H
