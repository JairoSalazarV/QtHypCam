#ifndef __COMMON_H
#define __COMMON_H

    //#define MAX(x, y) (((x) > (y)) ? (x) : (y))
    //#define MIN(x, y) (((x) < (y)) ? (x) : (y))

    #include "lstStructs.h"
    #include "lstfilenames.h"

    #include <QString>
    #include <QDebug>
    #include <QTime>
    #include <QEventLoop>
    #include <QCoreApplication>

    //#include <highgui.h>
    //#include <opencv2/highgui/highgui.hpp>

    #include <customQMatrix3x3.h>

    #include <QFileInfo>

    #include <QMessageBox>

    #include <iostream>

    #include <QProgressBar>


    QPoint *calibPoint( QPoint *point, lstDoubleAxisCalibration *calib );

    double funcDet2x2(double **M);

    double vectorMax(double *vector, int len);
    double vectorMaxQListQString(QList<QString> lst);

    QImage bilinearInterpolationQImage(QImage img);

    //void funcSourcePixToDiffPix(strDiffPix *diffPix, lstDoubleAxisCalibration *calSett );

    void funcPrintCalibration(lstDoubleAxisCalibration *calibSettings);

    bool funcGetCalibration(lstDoubleAxisCalibration* calibSettings);

    void funcObtSettings( structSettings * lstSettings);

    void funcShowMsg( QString title, QString msg, QWidget *parent=Q_NULLPTR );
    void funcShowMsgERROR(QString msg, QWidget *parent=Q_NULLPTR);

    int funcShowSelDir(const QString &title, QString startedPath, QString* dirSelected);
    int funcShowSelDir(QString startedPath, QString* dirSelected);
    QString funcShowSelDir(QString path);

    int func_getFilenameFromUser(QString* fileName, QString lastPathSaved, QString lastDefaultPath, QWidget *parent);

    QString funcRemoveImageExtension( QString imgName );

    int func_MergeImages(QImage* source, QImage* destine, QString type);

    int func_getMultiImages(QStringList *fileNames, QWidget* parent);

    void funcShowFileError(int error, QString fileName);

    void funcPrintFirst(int n, int max, char *buffer);

    void funcObtainIP(char* host);

    bool funcIsIP( std::string ipCandidate );

    int connectSocket( structCamSelected *camSelected );    

    void QtDelay( unsigned int ms );

    int xyToIndex(int x, int y, int w);

    int *indexToxy( int index, int w, int h );

    int funcShowMsgYesNo( QString title, QString msg, QWidget* parent=Q_NULLPTR );

    bool funcGetRaspParamFromXML(structRaspcamSettings *raspcamSettings , QString filePath);

    void funcRGB2XYZ(colSpaceXYZ *spaceXYZ, float Red, float Green, float Blue);

    bool saveFile(const QString &fileName, QString contain );

    //IplImage *funcGetImgFromCam(int usb, int stabMs );

    QString readAllFile( QString filePath );

    int fileIsValid(QString fileContain);

    QString readFileParam( const QString &filePath, const QString &pathDefault, int* OK );
    int readFileParam(QString fileName, QString *tmpFileContain);
    QString readFileParam(QString fileName);    

    bool funGetSquareXML( QString fileName, squareAperture *squareParam );

    //QImage* IplImage2QImage(QImage* iplImg);

    colorAnalyseResult *funcAnalizeImage(QImage *img );

    linearRegresion *funcCalcLinReg(float *X);

    linearRegresion funcLinearRegression(double *X, double *Y, int numItems );

    QImage funcRotateImage(QString filePath, float rotAngle);

    void funcQStringToSensitivities(QString txt, QList<double> *p);

    void funcTransPix(calcAndCropSnap *calStruct , int w, int h, int W, int H);

    bool saveBinFile(unsigned long datasize, unsigned char *dataPtr, QString directory);

    int funcDeleteFile( QString fileName );

    QString funcRemoveFileNameFromPath( QString Path );

    customQMatrix3x3 matMultiply(QMatrix3x4 *M1, QMatrix4x3 *M2);
    QMatrix3x4 matMultiply(customQMatrix3x3 *M1, QMatrix3x4 *M2);
    QVector3D matMultiply(QMatrix3x4 *M1, QVector4D *M2);


    void funcOpenFolder(QString path);

    int funcAccountFilesInDir(QString Dir);

    void funcClearDirFolder(QString path);

    int funcPrintRectangle(QString title, squareAperture *rectangle);

    QList<QFileInfo> funcListFilesInDir(QString Dir, QString Suffix);
    QList<QFileInfo> funcListFilesInDir(QString Dir);
    QList<QFileInfo> funcListFilesInDirSortByNumberName(QString Dir);

    QList<QFileInfo> funcFilterFilelist(QList<QFileInfo> lstFiles, QString suffix);

    void calcDiffProj(strDiffProj *diffProj, lstDoubleAxisCalibration *daCalib);

    QString timeToQString(unsigned int totMilli);

    bool funGetSlideSettingsXML(QString fileName, strReqImg *reqImg );

    int saveBinFile_From_u_int8_T( std::string fileName, uint8_t *data, size_t len);

    int fileExists(QString fileName );

    int funcExecuteCommand( QString command );

    QString funcExecuteCommandAnswer(char* command );

    extern void *funcAllocInteger3DMatrixMemo(int rows, int cols, int layers, int*** M );

    void displayImageFullScreen(const QImage &tmpImg );

    void funcNDVI(
                    QImage *imgToNDVI,
                    double lowerBound,
                    int brilliant,
                    QString Infrared,
                    QString Red
                 );

    int funcGetPixelColor(QRgb* Pixel, QString* Infrared );

    int funcReadAnalysePlot( structAnalysePlotSaved* structPlotSaved );

    void rotateQImage(QImage* tmpImg, int degree);

    QRect screenResolution(QWidget* reference);

    int* imageDecriptor(QImage *img, bool horizontal=true);

    int pixelMaxValue( QRgb pixel );
    int pixelMaxValue(const QColor &color );

    QPoint imageSimilarity2D(QImage* img1, QImage* img2, float maxShiftPix, bool horizontal=true);

    int squareImageDifferenece(QImage* img1, QImage* img2, int k, bool horizontal=true);

    int vectorSimilarity(int* v1, int* v2, int n, float lang);

    int* vectorConvolution(int* v1, int* v2, int n);

    float *vectorCrossCorrelation(int* v1, int* v2, int n, float lang);

    float vectorSimpleCorrelation(int* v1, int* v2, int n, int k);

    float vectorCorrelation(int* v1, int* v2, int n, int k);
    float vectorCorrelation(int* v1, int* v2, int n, int k, float zeroCorr);

    int calcSlideExtraW(structSlideHypCube* slideSett);

    //inline int align(int size, int align);
    //IplImage *QImageToIplImage(const QImage * qImage);
    //QImage *IplImageToQImage(const IplImage * iplImage, uchar **data,
    //                         double mini, double maxi);


    //
    //Variable area
    //
    static cameraResolution* camRes = (cameraResolution*)malloc(sizeof(cameraResolution));

    QString funcGetParam(QString label);
    QString funcGetParam(QString label, QString defaultValue);




    //
    int funcSaveXML(
                        const QString &fileName,
                        QList<QString>* lstFixtures,
                        QList<QString>* lstValues,
                        bool notify=true
                   );

    void funcGuaranteeExtension(QString *filename, QString extension);

    int funcReadLineFromXML(QString* filePath, structLine *tmpLine);

    int funcLetUserSelectFile(
                                QString* filePath,
                                const QString &title,
                                const QString &pathLocation,
                                const QString &defaultLastPath,
                                QWidget* parent=Q_NULLPTR
                             );
    int funcLetUserSelectFile(
                                QString* filePath,
                                const QString &title="Select file...",
                                QWidget* parent=Q_NULLPTR
                             );

    int funcLetUserDefineFile(
                                QString* filePath,
                                QString  title,
                                QString  extension,
                                QString* pathLocation,
                                QString* defaultLastPath,
                                QWidget* parent=Q_NULLPTR
                            );

    void funcShowMsg_Timeout(QString title, QString msg, QMessageBox::Icon=QMessageBox::NoIcon, QWidget *parent=Q_NULLPTR, int ms=1800);

    void funcShowMsgERROR_Timeout(QString msg, QWidget *parent=Q_NULLPTR, int ms=1800);

    void funcShowMsgSUCCESS_Timeout(QString msg, QWidget *parent=Q_NULLPTR, int ms=1800);

    void mouseCursorWait();

    void mouseCursorReset();

    void funcExportLineToXML(structLine* tmpLine, const QString filePath);

    //int funcCalc_X_SlopToPoint(int newY, structLine* internLine);

    //int funcCalc_Y_SlopToPoint(int newX, structLine* internLine);

    int funcReadSlideCalib( const QString &filePath, structSlideCalibration* slideCalibration );

    int funcReadHorHalfCalib(const QString &filePath,structSlideCalibration* slideCalibration);

    int funcReadAffineTransXML( const QString &filePath, QTransform* T );

    int funcReadVertHalfCalib(
                                const QString &filePath,
                                float *referenceX2,
                                structSlideCalibration* slideCalibration
                             );

    cameraResolution* getCamRes( int megapixels );

    int scaleLen( int len, int canvasLen, int originalLen );

    int funcCSVToFloat(const QString &CSV, QList<float>* lst );

    float funcApplyLR(
                        const float &coordinate,
                        const linearRegresion &LR,
                        bool print=false
                     );

    QPoint funcGetCoor(
                            int tmpX,
                            int tmpY,
                            const structSlideCalibration &slideCalibration
                      );
    int funcGetPixQE(
                        const int &x,
                        const int &y,
                        float *pixQE,
                        const QImage &tmpImg,
                        const structSlideCalibration &slideCalibration
                    );

    int funcGetQEArea(
                        const QPoint &p1,
                        const QPoint &p2,
                        int** tmpSubarea,
                        const QImage &img,
                        const structSlideCalibration &slideCalibration
                     );

    int funcLetUserSelectDirectory(const QString &pathToLastFilenameParam, QString *pathDirContainer);

    int funcGetTranslation( QTransform* tmpTrans, QWidget *parent=Q_NULLPTR );

    int funcLines2Translation(
                                QTransform* tmpTrans,
                                const structLine &lowerLine,
                                const structLine &upperLine
                             );

    int funcGetImagesFromFolder(
                                    const QString &Dir,
                                    QList<QImage>* lstTransImages,
                                    QList<QFileInfo>* lstImagePaths,
                                    QProgressBar* progBar = Q_NULLPTR
                               );

    float funcPixelToQE(
                            const QColor &originPixColor,
                            const int &wavePos,
                            const structSlideSensitivities &slideSens,
                            const float &wS
                       );


    QList<double> getNormedFunction( QString fileName );

    //int funcGetSpectrallyDenoisedPixel(
    //                                        const QColor &originColor,
    //                                        const int &wavePos,
    //                                        const float &wS,
    //                                        const structSlideSensitivities &slideSens
    //                                   );

    int funcSlideDenoiseDefineSensorToUse(
                                            strDenoisedColorSelected* denColSel,
                                            const int &wavePos,
                                            const structSlideSensitivities &slideSens
                                         );

    void funcGetMaximumSensitivities(
                                        structSlideCalibration* mainSlideCalibration,
                                        QWidget *parent=Q_NULLPTR
                                    );

    int func_DirExistOrCreateIt( const QList<QString> &lstFolders, QWidget* parent=Q_NULLPTR );

    int readHypCubeParameters( const QString &cubePath, strCubeParameters* cubeParam );

    int readXMLIntoList(
                            const QString &xmlPath,
                            const QList<QString> &lstParameters,
                            QList<QString>* lstValues
                       );

    void msg( std::string msg );

    QPoint originFromCalibration(
                                    const structHorizontalCalibration &tmpHorizlCal,
                                    const structVerticalCalibration &tmpVertCal
    );

    int funcReadVerticalCalibration(
                                            QString* filePath,
                                            structVerticalCalibration* vertCal
    );

#endif // __COMMON_H
