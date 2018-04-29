#ifndef FORMBUILDSLIDEHYPECUBEPREVIEW_H
#define FORMBUILDSLIDEHYPECUBEPREVIEW_H

#include <QDialog>
#include <__common.h>
#include <QProgressBar>


namespace Ui {
class formBuildSlideHypeCubePreview;
}

class formBuildSlideHypeCubePreview : public QDialog
{
    Q_OBJECT

    QImage layerBackup;
    QList<QFileInfo> lstImagePaths;
    structExportHypcubeSettings mainExportSettings;    

public:

    QList<QImage> lstImgs;
    explicit formBuildSlideHypeCubePreview(QWidget *parent = 0);
    ~formBuildSlideHypeCubePreview();

    QImage funcGetImageAtWavelength(
                                        const float &wavelength,
                                        const structSlideCalibration &mainSlideCalibration
                                   );
    //int funcCalcHypercubeSize(
    //                                const structSlideCalibration &mainSlideCalibration,
    //                                structSlideHypCubeSize* slidecHypCubeSize
    //                         );
    int funcCalcHypercubeSize( structSlideHypCubeSize* slidecHypCubeSize );

    int funcSlideSpectralImageUnzip(
                                        u_int8_t*** slideHypCube,
                                        int idImg,
                                        QImage* tmpImg,
                                        const structSlideHypCubeSize &slideHypcubeSize,
                                        const structSlideCalibration &mainSlideCalibration
                                   );

    void funcCalcLstWaveAndPositions(
                                            structSlideHypCubeSize* slideHypcubeSize,
                                            const structSlideCalibration &mainSlideCalibration
                                    );

    QImage funcGetLayerFromHypCube(
                                        u_int8_t*** slideHypCube,
                                        const structSlideHypCubeSize &slideHypcubeSize,
                                        const int idLayer
                                  );

    int funcCopyDiffToSlideHypCube(
                                        u_int8_t*** slideHypCube,
                                        QImage* tmpImg,
                                        const QRect &originRect,
                                        const int &destineX,
                                        const int &destineZ,
                                        const int &wavePos,
                                        const structSlideSensitivities &slideSens,
                                        const float &wS,
                                        const int &type=copyOverride
                                   );


    enum copyType{
                    copyOverride,
                    copyAverage,
                    copyMin,
                    copyMax,
                    copySpecDemoised,
    };

    void exportSlideHypCube(
                                QString* destineDir,
                                QProgressBar* progBar,
                                QLabel* labelProgBar,
                                QWidget* parent
                           );

private slots:
    void on_pbApply_clicked();

    //QImage buildSlideCubePreview(QList<QFileInfo> lstFrames, structSlideHypCube *slideCubeSettings);

    structSlideShifting calculateShifting(QList<QFileInfo> lstFrames, structSlideHypCube *slideCubeSettings, int x=0, int i=0, int j=0);

    void mergeSlidePreview(QImage* canvas, structSlideShifting* slideShift, int x1, int y1, bool inside=true, bool right=true);

    void refreshGVImage(QImage* imgPreview);

    int funcCopyImageSubareas(
                                const QRect &originRect,
                                const QPoint &destinePoint,
                                const QImage &origImg,
                                QImage* destineImg,
                                int type=copyOverride
                             );

    int funcDemoiseAndCopyImageSubareas(
                                            const QRect &originRect,
                                            const QPoint &destinePoint,
                                            const QImage &origImg,
                                            QImage* destineImg,
                                            const int &wavePos,
                                            const structSlideSensitivities &slideSens,
                                            int type=copyOverride
                                        );

    int funcReloadExportSettings();

    //int funcGetWavelengthImage( const float &wavelen, QImage* imageContainer );

    void on_pbFolder_clicked();

    QString concatenateParameters(int firstTime=0);

    int setLastExecution(QString parameters);

    void on_pbUploadImages_clicked();

    void on_pbSettings_clicked();

    void on_pbSave_clicked();

    void on_pbExportImages_clicked();

    void on_pbExportCube_clicked();



private:
    Ui::formBuildSlideHypeCubePreview *ui;
};

#endif // FORMBUILDSLIDEHYPECUBEPREVIEW_H
