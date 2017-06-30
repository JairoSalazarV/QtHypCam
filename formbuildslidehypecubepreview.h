#ifndef FORMBUILDSLIDEHYPECUBEPREVIEW_H
#define FORMBUILDSLIDEHYPECUBEPREVIEW_H

#include <QDialog>
#include <__common.h>


namespace Ui {
class formBuildSlideHypeCubePreview;
}

class formBuildSlideHypeCubePreview : public QDialog
{
    Q_OBJECT

public:
    explicit formBuildSlideHypeCubePreview(QWidget *parent = 0);
    ~formBuildSlideHypeCubePreview();

private slots:
    void on_pbApply_clicked();

    QImage buildSlideCubePreview(QList<QFileInfo> lstFrames, structSlideHypCube *slideCubeSettings);

    QImage calculateSubimage(QList<QFileInfo> lstFrames, structSlideHypCube *slideCubeSettings, int x=0, int i=0, int j=0);

    void refreshGVImage(QImage* imgPreview);

    void on_pbAutoStepIncrement_clicked();

    void on_pbAutoStepDecrement_clicked();

    void on_pbFolder_clicked();

    QString concatenateParameters(int firstTime=0);

    int setLastExecution(QString parameters);

private:
    Ui::formBuildSlideHypeCubePreview *ui;
};

#endif // FORMBUILDSLIDEHYPECUBEPREVIEW_H
