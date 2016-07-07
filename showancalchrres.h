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
    explicit showAnCalChrRes(customRect *rect, QWidget *parent = 0);
    ~showAnCalChrRes();

private slots:
    void on_pbCloseDialog_clicked();

    void updColSenHoriz();

    void updColSenVert();

    void on_chbBlue_clicked();

    void on_chbGreen_clicked();

    void on_chbRed_clicked();

    void drawLines();

    void drawCenter(int x, int y, Qt::GlobalColor color);

    void addLine2CanvasInPos(bool vertical, int pos, Qt::GlobalColor color);

    void on_pbCloseThis_clicked();

    void on_pbSaveAnalysis_clicked();

    void on_pbClearCalib_clicked();

private:
    Ui::showAnCalChrRes *ui;
};

#endif // SHOWANCALCHRRES_H
