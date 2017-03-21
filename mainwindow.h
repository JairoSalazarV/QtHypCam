#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lstStructs.h"
#include <QGraphicsView>
#include "graphicsview.h"

#include <QtSerialPort/QSerialPort>
#include <QtCore/QtGlobal>

//QT_BEGIN_NAMESPACE

class QLabel;
class SettingsDialog;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString auxQstring;

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void funcAddPoint(QMouseEvent *e);

    void addRect2Calib(QString colSeld);

    void addCircle2Calib(QString colSeld);

    void addHorLine2Calib(QString colSeld);

    void addVertLine2Calib(QString colSeld);

private slots:

    void funcEndRect(QMouseEvent *e, GraphicsView *tmpCanvas);

    void funcCalibMouseRelease(QMouseEvent *e);

    void funcSpectMouseRelease(QMouseEvent *e);

    void on_pbGetVideo_clicked();

    void on_actionExit_triggered();

    void on_pbAddIp_clicked();

    void on_pbSearchAll_clicked();

    void on_pbSendComm_clicked();

    bool funcReceiveFile(
                            int sockfd,
                            unsigned int fileLen,
                            unsigned char *bufferRead,
                            unsigned char *tmpFile
                        );

    void funcActivateProgBar();

    void on_pbConnect_clicked();

    void on_pbCamTurnOn_clicked();

    void on_pbStartVideo_clicked();

    bool funcGetRemoteImg(strReqImg *reqImg , bool saveImg=false);

    unsigned char *funcObtVideo(unsigned char saveLocally);

    bool funcUpdateVideo( unsigned int msSleep, int sec2Stab );

    void on_pbSaveImage_clicked();

    void funcIniCamParam(structRaspcamSettings *raspcamSettings);

    //void on_slideBrightness_valueChanged(int value);

    //void on_slideSharpness_valueChanged(int value);

    //void on_slideContrast_valueChanged(int value);

    //void on_slideSaturation_valueChanged(int value);

    void on_slideShuterSpeed_valueChanged(int value);

    void on_slideISO_valueChanged(int value);

    //void on_slideExpComp_valueChanged(int value);

    //void on_slideRed_valueChanged(int value);

    //void on_slideGreen_valueChanged(int value);

    void on_pbSaveRaspParam_clicked();

    bool saveRaspCamSettings( QString tmpName );

    void on_pbObtPar_clicked();

    bool funcSetCam( structRaspcamSettings *raspcamSettings );

    //void getRemoteImgByPartsAndSave(strReqImg *reqImg );

    /*
    void calcRectangles(
                            QList<QRect> *lstRect,
                            strDiffProj  *p11Min,
                            strDiffProj  *p12Min,
                            strDiffProj  *p21Min,
                            strDiffProj  *p22Min,
                            strDiffProj  *p11Max,
                            strDiffProj  *p12Max,
                            strDiffProj  *p21Max,
                            strDiffProj  *p22Max
                       );
                       */

    void mergeSnapshot(QImage *diff, QImage *aper, lstDoubleAxisCalibration *daCalib );

    void calcDiffPoints(
                            double wave,
                            strDiffProj *p11,
                            strDiffProj *p12,
                            strDiffProj *p21,
                            strDiffProj *p22,
                            lstDoubleAxisCalibration *daCalib
                       );

    void getMaxCalibRect(QRect *rect , lstDoubleAxisCalibration *calib);

    QString getFilenameForRecImg();

    void updateDisplayImageReceived();

    void on_pbSnapshot_clicked();

    void funcGetSnapshot();


    void on_pbExpIds_clicked();

    void funcPutImageIntoGV(QGraphicsView *gvContainer, QString impPath);

    void on_pbPtsClearAll_clicked();

    //void on_pbPtsDel_clicked();

    bool funcUpdatePolitope();

    void funcCreateLine(
                            bool drawVertex,
                            int x1,
                            int y1,
                            int x2,
                            int y2
                        );

    void funcFillFigure();

    void funcDrawPointIntoCanvas(
                                    int x,
                                    int y,
                                    int w,
                                    int h,
                                    QString color,
                                    QString lineColor
                                );
    void funcAddPoit2Graph(
                                GraphicsView *tmpCanvas,
                                int x,
                                int y,
                                int w,
                                int h,
                                QColor color,
                                QColor lineColor
    );

    void on_pbSavePixs_clicked();

    //bool on_pb2XY_clicked();

    //void on_pbLoadImg_clicked();

    structRaspcamSettings funcFillSnapshotSettings(structRaspcamSettings raspSett );

    //void on_pbUpdCut_clicked();

    //void on_slideCutPosX_valueChanged(int xpos);

    //void funcSetLines();

    //void on_slideCutWX_valueChanged(int value);

    //void on_slideCutWY_valueChanged(int value);

    //void on_slideCutPosY_valueChanged(int value);

    //void on_pbSaveSquare_clicked();

    //bool funcSaveRect( QString fileName );

    //void on_pbSaveBigSquare_clicked();

    void on_pbSpecSnapshot_clicked();

    void on_chbRed_clicked();

    void funcUpdateColorSensibilities();

    void funcBeginRect(QMouseEvent *e);

    void on_pbSpecCut_clicked();

    void on_chbGreen_clicked();

    void on_chbBlue_clicked();

    void funcDrawLines(int flag, int xR, int xG, int xB);

    void on_slideRedLen_sliderReleased();

    void on_slideBlueLen_sliderReleased();

    void on_slideGreenLen_sliderReleased();

    void on_pbViewBack_clicked();

    void on_pbSnapCal_clicked();

    void on_pbObtPar_2_clicked();


    void on_slide2AxCalPos_valueChanged(int value);

    void updateCalibLine();

    void on_pbCalSaveRot_clicked();

    void refreshGvCalib(QString fileName);

    void on_pbClearCalScene_clicked();

    //void on_slide2AxCalThre_valueChanged(int value);

    void funcImgThreshold( int threshold, QImage *tmpImage );

    //void on_pbCalSaveTop_clicked();

    //void funcUpdateImgView(QImage *tmpImg);

    void on_pbSpecLoadSnap_clicked();

    void on_actionRect_triggered();



    void on_actionHorizontalLine_triggered();

    void on_actionCircle_triggered();

    void on_actionVerticalLine_triggered();

    void on_actionClear_triggered();

    void clearFreeHandPoligon();

    void clearRectangle();

    void on_actionSelection_triggered();

    void ResetGraphToolBar( QString toEnable );

    void funcAnalizeAreaSelected(QPoint p1, QPoint p2);

    void disableAllToolBars();

    void on_actionDrawToolbar_triggered();

    void on_pbExpPixs_tabBarClicked(int index);

    //void on_slide2AxCalThre_sliderReleased();

    void on_actionDoubAxisDiff_triggered();


    void on_slideTriggerTime_valueChanged(int value);

    void on_actionRotateImg_triggered();

    void doImgRotation(float angle );

    void DrawVerAndHorLines(GraphicsView *tmpCanvas, Qt::GlobalColor color);

    void reloadImage2Display();

    void on_actionLoadCanvas_triggered();

    void loadImageIntoCanvasEdit(QString fileName, bool ask);

    void on_actionApplyThreshold_triggered();

    void applyThreshol2Scene(QString threshold);

    float getLastAngle();

    void on_actionLoadSquareRectangle_triggered();

    void on_actionLoadRegOfInteres_triggered();

    void on_slideShuterSpeedSmall_valueChanged(int value);

    void on_actionToolPenArea_triggered();

    void mouseCursorHand();

    void mouseCursorCross();

    void mouseCursorWait();

    void mouseCursorReset();

    void on_actionSend_to_XYZ_triggered();

    void on_actionSaveCanvas_triggered();

    void saveCalib(QString fileName);

    bool saveCanvas(GraphicsView *tmpCanvas, QString fileName);

    void on_actionExportPixelsSelected_triggered();

    void on_pbLANConnect_clicked();

    void on_pbLANTestConn_clicked();

    void on_actionGenHypercube_triggered();

    bool generatesHypcube(int numIterations, QString fileName);

    double *demosaiseF2D(double *f, int L, int H, int W);
    double *demosaiseF3D(double *f, int L, int H, int W);

    double calcTrilinearInterpolation(double ***cube, trilinear *node);

    double *calculatesF(int iterations, int sensor, lstDoubleAxisCalibration *daCalib);

    void improveF(double *fKPlusOne, pixel **Hcol, double *f, double *gTmp, int N );

    void createsGTmp(double *gTmp, double *g, int **Hrow, double *f, int M);

    double *createsF0(pixel **Hcol, double *g, int N);

    double *serializeImageToProccess(QImage img, int sensor);

    void createsHColAndHrow(pixel **Hcol, int **Hrow, QImage *img, lstDoubleAxisCalibration *daCalib );

    void insertItemIntoRow(int **Hrow, int row, int col);

    QList<double> getWavesChoised();

    void on_actionValidCal_triggered();

    void on_actionValCal_triggered();

    void on_actionSquareUsable_triggered();

    void on_actionChoseWavelength_triggered();

    void on_actionFittFunction_triggered();

    void on_actionShow_hypercube_triggered();

    void extractsHyperCube(QString originFileName);

    void on_actionBilinear_interpolation_triggered();

    void on_slideSquareShuterSpeedSmall_valueChanged(int value);    

    void on_slideSquareShuterSpeed_valueChanged(int value);

    void refreshSquareShootSpeed();

    void on_pbCopyShutter_clicked();

    cameraResolution* getCamRes();

    void on_actionslideHypCam_triggered();

    void on_pbGetSlideCube_clicked();

    bool funcGetSLIDESnapshot();

    strSlideSettings funcFillSLIDESettings(strSlideSettings slideSetting);

private:
    Ui::MainWindow *ui;
    //QLabel *status;
    //Console *console;
    //SettingsDialog *settings;
    //QSerialPort *serial;
};















#endif // MAINWINDOW_H





