#ifndef FORMHYPERCUBEANALYSIS_H
#define FORMHYPERCUBEANALYSIS_H

#include <QDialog>
#include <__common.h>

namespace Ui {
class formHypercubeAnalysis;
}

class formHypercubeAnalysis : public QDialog
{
    Q_OBJECT

    GraphicsView* mainGV = new GraphicsView(this);

public:
    explicit formHypercubeAnalysis(QWidget *parent = 0);
    ~formHypercubeAnalysis();

private slots:
    void on_slideTmpImg_valueChanged(const int &value);

    void updateLabel(const int &value);

    void on_pbCubeToImg_clicked();

    void on_pbLoadCube_clicked();

    void updateProgressBar(int value, QString label);

    void updateSignature(QMouseEvent* e);

private:
    Ui::formHypercubeAnalysis *ui;
};

#endif // FORMHYPERCUBEANALYSIS_H
