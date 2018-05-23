#ifndef FORMHYPERCUBEANALYSIS_H
#define FORMHYPERCUBEANALYSIS_H

#include <QDialog>
#include <__common.h>
#include <slideHypcube.h>
#include <lstStructs.h>
#include <QGraphicsRectItem>

namespace Ui {
class formHypercubeAnalysis;
}

class formHypercubeAnalysis : public QDialog
{
    Q_OBJECT

    //QGraphicsRectItem* tmpRect = new QGraphicsRectItem();

    QList<strlstRemarkedPix> lstRemarkedPix;

    strSlidePlotSettings plotSettings;

    slideHypcube* slideHypCube = new slideHypcube(this);

    GraphicsView* gvImg     = new GraphicsView(this);
    QGraphicsView* gvPlot   = new QGraphicsView(this);
    QList<int> lstXAxis;


public:
    explicit formHypercubeAnalysis(QWidget *parent = 0);
    ~formHypercubeAnalysis();

private slots:
    void on_slideTmpImg_valueChanged(const int &value);

    void on_pbCubeToImg_clicked();

    void on_pbLoadCube_clicked();

    void updateProgressBar(int value, QString label);

    void updateSignature(QMouseEvent* e);

    void addRemarkedPixel(QMouseEvent*);

    void updateSlidePicture(const int &l);

    void updateLabel(const int &value);

    void plotPixel(const strlstRemarkedPix &remarkedPix, const int &boundArea);

    void on_pbReset_clicked();

protected:
  //void resizeEvent ( QResizeEvent * event );

private:
    Ui::formHypercubeAnalysis *ui;



};

#endif // FORMHYPERCUBEANALYSIS_H
