#ifndef SELWATHTOCHECK_H
#define SELWATHTOCHECK_H

#include <QDialog>
#include <lstStructs.h>
namespace Ui {
class selWathToCheck;
}

class selWathToCheck : public QDialog
{
    Q_OBJECT

public:
    explicit selWathToCheck(QWidget *parent = 0);
    ~selWathToCheck();

private slots:

    void fillLabelImg(QString imgPath);

    void drawCentroid(QString file, Qt::GlobalColor color );

    void showLimitCalculated();

    void drawLinearRegression(bool horizontal);

    void drawAllLimits();

    void drawLimit(int side);

    void drawAllCentoides();

    void showGV();

    void on_pbCentroids_clicked();

    void on_pbLoadImage_clicked();

private:
    Ui::selWathToCheck *ui;
};

#endif // SELWATHTOCHECK_H
