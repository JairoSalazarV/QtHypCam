#ifndef FORMGENLINEARREGRESSION_H
#define FORMGENLINEARREGRESSION_H

#include <QDialog>

namespace Ui {
class formGenLinearRegression;
}

class formGenLinearRegression : public QDialog
{
    Q_OBJECT

public:
    explicit formGenLinearRegression(QWidget *parent = 0);
    ~formGenLinearRegression();

private slots:

    //void mouseCursorWait();

    //void mouseCursorReset();

    void on_pbSelectFile_clicked();

    void on_pbRemoveItem_clicked();

    void on_pbGenRegression_clicked();

private:
    Ui::formGenLinearRegression *ui;
};

#endif // FORMGENLINEARREGRESSION_H
