#ifndef __COMMON_H
#define __COMMON_H

    #include "lstStructs.h"
    #include <QString>
    #include <QDebug>
    #include <QTime>
    #include <QEventLoop>
    #include <QCoreApplication>

    #include <highgui.h>

    void funcObtSettings( structSettings * lstSettings);

    void funcShowMsg( QString title, QString msg );

    void funcPrintFirst(int n, int max, char *buffer);

    void funcObtainIP(char* host);

    bool funcIsIP( std::string ipCandidate );

    int connectSocket( structCamSelected *camSelected );

    void QtDelay( unsigned int ms );

    int funcShowMsgYesNo( QString title, QString msg );

    bool funcGetRaspParamFromXML(structRaspcamSettings *raspcamSettings , QString filePath);

    void funcRGB2XYZ(colSpaceXYZ *spaceXYZ, float Red, float Green, float Blue);

    bool saveFile( QString fileName, QString contain );

    IplImage *funcGetImgFromCam(int usb, int stabMs );

    QString readAllFile( QString filePath );

    bool funGetSquareXML( QString fileName, squareAperture *squareParam );

    QImage* IplImage2QImage(IplImage *iplImg);

    colorAnalyseResult *funcAnalizeImage(QImage *img );

    linearRegresion *funcCalcLinReg(float *X);

    QImage funcRotateImage(QString filePath, float rotAngle);

    //inline int align(int size, int align);
    //IplImage *QImageToIplImage(const QImage * qImage);
    //QImage *IplImageToQImage(const IplImage * iplImage, uchar **data,
    //                         double mini, double maxi);


#endif // __COMMON_H
