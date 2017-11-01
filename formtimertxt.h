#ifndef FORMTIMERTXT_H
#define FORMTIMERTXT_H

#include <QDialog>
#include <__common.h>

namespace Ui {
class formTimerTxt;
}

class formTimerTxt : public QDialog
{
    Q_OBJECT

public:
    explicit formTimerTxt(QWidget *parent = 0, QString text="", int seconds=0, QColor color=Qt::black );
    ~formTimerTxt();
    void startMyTimer(int seconds=0);

signals:
    void signalTimerRefresh(int);

public slots:
    void updateTimer(int s);

private slots:    
    void on_pushButton_clicked();


private:
    Ui::formTimerTxt *ui;
};

#endif // FORMTIMERTXT_H
