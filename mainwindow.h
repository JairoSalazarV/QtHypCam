#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lstStructs.h"
#include <QGraphicsView>
#include "graphicsview.h"

#include <QtSerialPort/QSerialPort>
#include <QtCore/QtGlobal>
#include <QVideoFrame>
#include <QMediaPlayer>

#include <QFileInfo>

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

    int getCamMP();

    structCamSelected* getCameraSelected();

    u_int8_t** funcGetSLIDESnapshot(int* numImages , bool saveFiles);

    structCamSelected* funcGetCamSelected();

    int funcVideoToFrames(QString videoSource);

    void processEndOfPlayer(QMediaPlayer::MediaStatus status);

    void funcHideBarAndLabel();

    void processFrame(QVideoFrame actualFrame);

    void funcDrawSpectralPixelIntoSmall(int x, int y);

    void funcMouseMoveReaction(QMouseEvent* e);

    void funcLoadImageIntoGaphView( QGraphicsView* canvas, QString filePath );

    std::string funcRemoteTerminalCommand(
                                                    std::string command,
                                                    structCamSelected *camSelected,
                                                    int trigeredTime,
                                                    bool waitForAnswer,
                                                    bool* ok);

    int obtainFile(std::string fileToObtain, std::string fileNameDestine, QString txtBar);

    QImage obtainImageFile( std::string fileToObtain, QString txtBar );



private slots:

    int funcValidateMinimalStatus();

    void funcEndRect(QMouseEvent *e, GraphicsView *tmpCanvas);

    void funcCalibMouseRelease(QMouseEvent *e);

    void funcSpectMouseRelease(QMouseEvent *e);

    //void on_pbGetVideo_clicked();

    void on_actionExit_triggered();

    void on_pbAddIp_clicked();

    void on_pbSearchAll_clicked();

    void on_pbSendComm_clicked();

    bool funcReceiveFile(
                            int sockfd,
                            unsigned int Len,
                            unsigned char *bufferRead,
                            unsigned char *tmp
                        );

    void funcActivateProgBar();

    void on_pbConnect_clicked();

    //void on_pbCamTurnOn_clicked();

    //void on_pbStartVideo_clicked();

    void progBarTimer( int ms );

    bool funcGetRemoteImg(strReqImg *reqImg , bool saveImg=false);

    unsigned char *funcObtVideo(unsigned char saveLocally);

    bool funcUpdateVideo( unsigned int msSleep, int sec2Stab );

    //void on_pbSaveImage_clicked();

    void funcIniCamParam(structRaspcamSettings *raspcamSettings);

    //void on_slideBrightness_valueChanged(int value);

    //void on_slideSharpness_valueChanged(int value);

    //void on_slideContrast_valueChanged(int value);

    //void on_slideSaturation_valueChanged(int value);

    //void on_slideShuterSpeed_valueChanged(int value);

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
                            float wave,
                            strDiffProj *p11,
                            strDiffProj *p12,
                            strDiffProj *p21,
                            strDiffProj *p22,
                            lstDoubleAxisCalibration *daCalib
                       );

    void getMaxCalibRect(QRect *rect , lstDoubleAxisCalibration *calib);

    //QString getnameForRecImg();

    void updateDisplayImage(QImage* tmpImg);
    void updateDisplayImage(QString fileName);

    void updatePreviewImage(QString* fileName);
    void updatePreviewImage(QImage* tmpImg);

    void on_pbSnapshot_clicked();

    void funcGetSnapshot();


    //void on_pbExpIds_clicked();

    void funcPutImageIntoGV(QGraphicsView *gvContainer, QString impPath);

    //void on_pbPtsClearAll_clicked();

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

    //void on_pbSavePixs_clicked();

    //bool on_pb2XY_clicked();

    //void on_pbLoadImg_clicked();

    void funcLabelProgBarHide();

    void progBarUpdateLabel(QString txt , int color);

    structRaspcamSettings funcFillSnapshotSettings(structRaspcamSettings raspSett );

    //void on_pbUpdCut_clicked();

    //void on_slideCutPosX_valueChanged(int xpos);

    //void funcSetLines();

    //void on_slideCutWX_valueChanged(int value);

    //void on_slideCutWY_valueChanged(int value);

    //void on_slideCutPosY_valueChanged(int value);

    //void on_pbSaveSquare_clicked();

    //bool funcSaveRect( QString Name );

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

    //void on_pbObtPar_2_clicked();


    //void on_slide2AxCalPos_valueChanged(int value);

    void updateCalibLine();

    //void on_pbCalSaveRot_clicked();

    void refreshGvCalib(QString fileName);

    //void on_pbClearCalScene_clicked();

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

    //void reloadImage2Display();

    void on_actionLoadCanvas_triggered();

    void updateImageCanvasEdit(QString fileName);
    void updateImageCanvasEdit(QImage* origImg);

    void on_actionApplyThreshold_triggered();

    void applyThreshol2Scene(QString threshold);

    float getLastAngle();

    void on_actionLoadSquareRectangle_triggered();

    void on_actionLoadRegOfInteres_triggered();

    //void on_slideShuterSpeedSmall_valueChanged(int value);

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

    //void on_slideSquareShuterSpeedSmall_valueChanged(int value);

    //void on_slideSquareShuterSpeed_valueChanged(int value);

    //void refreshSquareShootSpeed();

    void on_pbCopyShutter_clicked();

    void on_actionslideHypCam_triggered();

    u_int8_t* funcQtReceiveFile(std::string fileNameRequested, int* fileLen , QString txtBar);

    //void on_pbGetSlideCube_clicked();



    int funcReceiveFrame(int sockfd, int idImg, int *frameLen, strReqImg *reqImg);



    void funcDebug(QString msg);

    int funcRequestSubframe( int sockfd, strReqSubframe* subFrameParams );

    int funcReceiveACK( int sockfd );

    int funcReceiveOneMessage( int sockfd, void* frame );

    strReqImg *funcFillSLIDESettings(strReqImg *reqImg);

    int funcDrawRectangleFromXML(QString fileName);

    void on_pbDrawSlide_triggered();

    int saveRectangleAs( squareAperture *square, QString fileName );

    void on_actionSlide_settings_triggered();

    void on_pbShutdown_clicked();

    void on_pbSnapVid_clicked();

    int funcUpdateImageFromFolder(const QString &folder);

    //void on_pbSnapshot_2_clicked();

    int rectangleInPixelsFromSquareXML( QString fileName, squareAperture *rectangle );
    int rectangleInPixelsFromSquareXML( QString fileName, QString respectTo, squareAperture *rectangle );

    int createSubimageRemotelly(bool squareArea);

    int takeRemoteSnapshot(QString fileDestiny, bool squareArea);

    void on_pbSnapshotSquare_clicked();

    void on_pbSaveImage_clicked();

    void funcResetStatusBar();

    void on_pbOneShotSnapshot_clicked();

    void on_actionsquareSettings_triggered();

    void on_pbSelectFolder_clicked();

    void on_slideChangeImage_valueChanged(int value);

    void funcDrawPlotLimits();

    void on_actionvideoToFrames_triggered();

    void on_actionframesToCube_triggered();

    void on_pbTimeLapse_clicked();

    QString genRemoteVideoCommand(QString remoteVideo, bool ROI=true);

    QString genTimelapseCommand(QString folder,bool setROI=true);

    QString genSubareaRaspistillCommand(QString remoteFilename, QString subareaRectangle);

    void on_actionRGB_to_XY_triggered();

    void on_actionNDVI_triggered();

    void on_actionNDVI_Algorithm_triggered();

    void on_actionFull_Screen_triggered();

    void on_actionDisplay_Original_triggered();

    void on_actionFull_photo_triggered();

    void on_actionDiffraction_triggered();

    void on_actionComposed_triggered();

    void on_actionVideo_triggered();

    void on_actionTimelapse_triggered();

    void on_actionSave_triggered();

    void on_actionSlideDiffraction_triggered();

    void on_actionObtain_Folder_triggered();

    int obtainRemoteFolder(
                                QString remoteFolder,
                                QString localFolder,
                                QString *errTrans,
                                QString *errDel,
                                bool delFolder=false
                           );

    bool funcRemoveRemoteFile( QString fileName );

    void on_actionSlide_Build_Hypercube_triggered();

    void buildHypercubeFromFilelist(QList<QFileInfo> lstFrames , structSlideHypCube slideCubeSettings);

    int funcAccountFilesInFolder( QString folder );

    //void funcShowMsgERROR_Timeout(QString msg, int ms=3000);

    //void funcShowMsgSUCCESS_Timeout(QString msg, int ms=3000);

    void funcStartRemoteTimelapse( bool setROI=false );

    int funcValidateFileDirNameDuplicated(QString remoteFile, QString localFile);

    QString funcGetSyncFolder();

    void on_actionVideo_2_triggered();

    void on_actionTimelapse_2_triggered();

    void on_actionSnapvideos_triggered();

    void on_actionSnapshot_triggered();

    void on_actionSynchronize_triggered();

    void on_actionSync_Folder_triggered();

    void funcMainCall_RecordVideo(QString* videoID, bool defaultPath=false, bool ROI=false);

    void funcMainCall_GetSnapshot();

    void on_actionFull_Video_triggered();

    int funcDisplayTimer(QString title, int timeMs, QColor color);

    void on_actionMultiImageAverage_triggered();

    int func_MultiImageMerge( QString type );


    void on_actionMultiImageMinimum_triggered();

    void on_actionMultiImageMaximum_triggered();


    void on_actionSlide_Settings_triggered();

    void on_actionLinear_Regression_triggered();

    void on_actionDiffraction_Origin_triggered();

    void on_openLine_triggered();

    void on_actionSlide_Linear_Regression_triggered();

    void on_actionarcLine_triggered();

    bool funcDrawLineIntoCanvasEdit(const QString &filePath);

    void on_actionPlot_Calculated_Line_triggered();

    void on_actionPlot_Upper_Line_Rotation_triggered();

    //int funcReadVerticalCalibration(
    //                                    QString* filePath,
    //                                    structVerticalCalibration* vertCal
    //                               );

    int funcReadHorizCalibration(QString *filePath, structHorizontalCalibration *horizCalib);

    void on_actionOrigin_triggered();

    void on_actionBuld_HypImg_triggered();

    void on_actionMerge_Calibration_triggered();

    void on_actionPlot_over_Real_triggered();

    void on_actionPlot_Line_at_Wavelength_triggered();

    void on_actionSlide_Calibration_File_triggered();

    void on_actionMerge_into_RGB_triggered();

    //void on_actionTesting_triggered();

    void on_actionApply_Rotation_triggered();

    void on_actionApply_Transformation_triggered();

    void on_actionRestore_Original_triggered();    

    void on_actionExtract_frames_2_triggered();

    int funcExtractFramesFromH264( bool updateImage=true );

    void on_actionSlide_Max_Wavelength_triggered();

    void on_actionBuild_HypCube_triggered();

    void on_actionBuild_HypCube_2_triggered();

    void funcOpticalCorrection(
                                    QList<QImage>* lstTransImages,
                                    structSlideCalibration *slideCalibration,
                                    bool tryToSave=true,
                                    bool refreshImage=true
                              );

    void on_actionSlide_HypCube_Building_triggered();

    void on_actionCamera_Sensitivities_triggered();

    void on_actionCalc_Sensitivities_triggered();

    void on_actionSlide_Min_Wavelength_triggered();

    void on_actionHypCube_From_H264_triggered();

    void on_actionCube_Analysis_triggered();

    void on_actionApply_Affine_Transformation_triggered();

    void on_actionApply_Optical_Correction_triggered();

    void on_actionExtract_ROI_triggered();

    void resizeEvent(QResizeEvent* event);

    void on_actionApply_Region_of_Interes_triggered();

    void functionTakeComposedSquarePicture();

private:
    Ui::MainWindow *ui;
    //QLabel *status;
    //Console *console;
    //SettingsDialog *settings;
    //QSerialPort *serial;
};















#endif // MAINWINDOW_H





