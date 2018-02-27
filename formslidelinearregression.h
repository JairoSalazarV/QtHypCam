#ifndef FORMSLIDELINEARREGRESSION_H
#define FORMSLIDELINEARREGRESSION_H

#include <QDialog>

namespace Ui {
class formSlideLinearRegression;
}

class formSlideLinearRegression : public QDialog
{
    Q_OBJECT

public:
    explicit formSlideLinearRegression(QWidget *parent = 0);
    ~formSlideLinearRegression();

private slots:
    void on_pbSelectFile_clicked();

    void on_pbGenRegression_clicked();

    void on_pbRemoveItem_clicked();

private:
    Ui::formSlideLinearRegression *ui;
};

#endif // FORMSLIDELINEARREGRESSION_H
