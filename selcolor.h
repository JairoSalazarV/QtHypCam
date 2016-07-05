#ifndef SELCOLOR_H
#define SELCOLOR_H

#include <QDialog>

namespace Ui {
class selColor;
}

class selColor : public QDialog
{
    Q_OBJECT

public:
    explicit selColor(QWidget *parent = 0);
    ~selColor();

private slots:

    void on_pushButton_clicked();

private:
    Ui::selColor *ui;

signals:
    void signalColorSelected(QString);

};

#endif // SELCOLOR_H
