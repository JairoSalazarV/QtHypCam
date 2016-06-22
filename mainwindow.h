#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lstStructs.h"
#include <QGraphicsView>
#include "graphicsview.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void funcAddPoint( int x, int y );

private slots:

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

    void on_pbConnect_clicked();

    void on_pbCamTurnOn_clicked();

    void on_pbStartVideo_clicked();

    unsigned char *funcGetRemoteImg(strReqImg *reqImg );

    unsigned char *funcObtVideo(unsigned char saveLocally);

    bool funcUpdateVideo( unsigned int msSleep, int sec2Stab );

    void on_pbSaveImage_clicked();

    void funcIniCamParam(structRaspcamSettings *raspcamSettings);

    void on_slideBrightness_valueChanged(int value);

    void on_slideSharpness_valueChanged(int value);

    void on_slideContrast_valueChanged(int value);

    void on_slideSaturation_valueChanged(int value);

    void on_slideShuterSpeed_valueChanged(int value);

    void on_slideISO_valueChanged(int value);

    void on_slideExpComp_valueChanged(int value);

    void on_slideRed_valueChanged(int value);

    void on_slideGreen_valueChanged(int value);

    void on_pbSaveRaspParam_clicked();

    void on_pbObtPar_clicked();

    bool funcSetCam( structRaspcamSettings *raspcamSettings );

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

    bool on_pb2XY_clicked();

    void on_pbLoadImg_clicked();

    structRaspcamSettings funcFillSnapshotSettings(structRaspcamSettings raspSett );

    void on_pbUpdCut_clicked();

    void on_slideCutPosX_valueChanged(int xpos);

    void funcSetLines();

    void on_slideCutWX_valueChanged(int value);

    void on_slideCutWY_valueChanged(int value);

    void on_slideCutPosY_valueChanged(int value);

    void on_pbSaveSquare_clicked();

    bool funcSaveRect( QString fileName );

    void on_pbSaveBigSquare_clicked();

    void on_pbSpecSnapshot_clicked();

    void on_chbRed_clicked();

    void funcUpdateColorSensibilities();

    void funcBeginRect(int x, int y);

    void funcEndRect(int x, int y);

    void on_pbSpecCut_clicked();

    void on_chbGreen_clicked();

    void on_chbBlue_clicked();

    void funcDrawLines(int flag, int xR, int xG, int xB);

    void on_slideRedLen_sliderReleased();

    void on_slideBlueLen_sliderReleased();

    void on_slideGreenLen_sliderReleased();

    void on_pbViewBack_clicked();

    void on_pbSnapCal_clicked();

private:
    Ui::MainWindow *ui;
};















#endif // MAINWINDOW_H





