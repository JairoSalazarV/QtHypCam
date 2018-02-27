#ifndef SHOWANCALCHRRES_H
#define SHOWANCALCHRRES_H

#include <QDialog>
#include <customrect.h>

namespace Ui {
    class showAnCalChrRes;
}

class showAnCalChrRes : public QDialog
{
    Q_OBJECT

public:
    explicit showAnCalChrRes(customRect *rect, QImage* origEditImg, QWidget *parent = 0);
    ~showAnCalChrRes();
    //QImage* internOrigEditImg;

private slots:
    void on_pbCloseDialog_clicked();

    void updColSenHoriz();

    void updColSenVert();

    void on_chbBlue_clicked();

    void on_chbGreen_clicked();

    void on_chbRed_clicked();

    void drawRGBPeakLines();

    void drawSensitivities();

    void drawCenter(int x, int y, Qt::GlobalColor color);

    void addLine2CanvasInPos(bool vertical, int pos, Qt::GlobalColor color);

    void on_pbCloseThis_clicked();

    void on_pbSaveAnalysis_clicked();

    void on_pbClearCalib_clicked();

    void on_pbSaveScene_clicked();

    void on_chbRedLine_clicked();

    void on_chbGreenLine_clicked();

    void on_chbBlueLine_clicked();

    void on_pbInFolder_clicked();

private:
    Ui::showAnCalChrRes *ui;
};

#endif // SHOWANCALCHRRES_H
