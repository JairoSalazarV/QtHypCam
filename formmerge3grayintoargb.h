#ifndef FORMMERGE3GRAYINTOARGB_H
#define FORMMERGE3GRAYINTOARGB_H

#include <QDialog>

namespace Ui {
class formMerge3GrayIntoARgb;
}

class formMerge3GrayIntoARgb : public QDialog
{
    Q_OBJECT

public:
    explicit formMerge3GrayIntoARgb(QWidget *parent = 0);
    ~formMerge3GrayIntoARgb();

private slots:
    void on_pbRed_clicked();

    void on_pbGreen_clicked();

    void on_pbBlue_clicked();

    void on_pbSave_clicked();

private:
    Ui::formMerge3GrayIntoARgb *ui;
};

#endif // FORMMERGE3GRAYINTOARGB_H
