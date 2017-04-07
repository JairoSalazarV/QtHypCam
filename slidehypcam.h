#ifndef SLIDEHYPCAM_H
#define SLIDEHYPCAM_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class slideHypCam;
}

class slideHypCam : public QMainWindow
{
    Q_OBJECT

public:
    explicit slideHypCam(QWidget *parent = 0);
    ~slideHypCam();
    //void readSerialPort();

private slots:
    //void on_pbSendSerial_clicked();

    //void on_pbSendCommand_clicked();

    //void serialPortConnect();

    void funcUpdateSlideToDisplay();

    void funcGenerateSlideImage();

    int funcGetNumImgs( QString path );

    void on_actionpbGetSlideCube_triggered();

    void resizeEvent(QResizeEvent* event);

    void on_actionSlide_settings_triggered();

private:
    Ui::slideHypCam *ui;
};

#endif // SLIDEHYPCAM_H
