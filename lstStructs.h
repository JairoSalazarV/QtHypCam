#ifndef LSTSTRUCTS_H
#define LSTSTRUCTS_H

    #include <string>
    #include <sys/types.h>
    #include <QString>    

    #include <graphicsview.h>

    #include <lstpaths.h>
    #include <lstcustoms.h>
    #include <lstraspividstruct.h>



    typedef struct strReqSubframe{
        int posIni;
        int len;
    }strReqSubframe;

    typedef struct strNumSlideImgs
    {
        unsigned char idMsg;        //Id message
        int numImgs;                //Number of images generated
    }strNumSlideImgs;

    typedef struct trilinear{
        int l;
        int L;
        int w;
        int W;
        int h;
        int H;
    }trilinear;

    typedef struct pixel{
        int x;
        int y;
        int index;
    }pixel;

    typedef struct voxel{
        int x;
        int y;
        int l;
    }voxel;

    typedef struct cameraResolution{
        int width;
        int height;
        int videoW;
        int videoH;
    }cameraResolution;

    typedef struct strDiffProj{
        int x;
        int y;
        int rx;
        int ry;
        int ux;
        int uy;
        int lx;
        int ly;
        int dx;
        int dy;
        float wavelength;
    }strDiffProj;

    typedef struct strAllLinReg{
        double horizA;
        double horizB;
        double vertA;
        double vertB;
        double waveHorizA;
        double waveHorizB;
        double waveVertA;
        double waveVertB;
        double deltaHorizA;
        double deltaHorizB;
        double deltaVertA;
        double deltaVertB;
    }strAllLinReg;

    typedef struct strLimits{
        int rightInf;
        int rightSup;
        int upInf;
        int upSup;
        int leftInf;
        int leftSup;
        int downInf;
        int downSup;
        int sourceX;
        int sourceY;
    }strLimits;

    typedef struct strDiffPix{
        int x;
        int y;
        int rightX;
        int rightY;
        int upX;
        int upY;
        int leftX;
        int leftY;
        int downX;
        int downY;
    }strDiffPix;

    typedef struct lstDoubleAxisCalibration{
        QString         bkgPath;
        int             W;
        int             H;
        float           bigX;
        float           bigY;
        float           bigW;
        float           bigH;
        float           squareX;
        float           squareY;
        float           squareW;
        float           squareH;
        int             squarePixX;
        int             squarePixY;
        int             squarePixW;
        int             squarePixH;
        int             squareUsableX;      //Respect to the snapshot
        int             squareUsableY;      //Respect to the snapshot
        int             squareUsableW;      //Respect to the snapshot
        int             squareUsableH;      //Respect to the snapshot
        float           minWavelength;
        float           maxWavelength;
        int             maxNumBands;
        double          minSpecRes;
        strAllLinReg    LR;
        QList<double>   Sr;
        QList<double>   Sg;
        QList<double>   Sb;
    }lstDoubleAxisCalibration;

    typedef struct lstCalibFileNames{
        //Miscelaneas
        QString bkgPath;
        int W;
        int H;
        float bigX;
        float bigY;
        float bigW;
        float bigH;
        float squareX;
        float squareY;
        float squareW;
        float squareH;
        //Source
        QString source;
        QString sourceHalogen;
        //Blue
        QString blueRightDown;
        QString blueRight;
        QString blueRightUp;
        QString blueUp;
        QString blueLeftUp;
        QString blueLeft;
        QString blueLeftDown;
        QString blueDown;
        //Green
        QString greenRightDown;
        QString greenRight;
        QString greenRightUp;
        QString greenUp;
        QString greenLeftUp;
        QString greenLeft;
        QString greenLeftDown;
        QString greenDown;
        //Red
        QString redRightDown;
        QString redRight;
        QString redRightUp;
        QString redUp;
        QString redLeftUp;
        QString redLeft;
        QString redLeftDown;
        QString redDown;
        //Limits
        QString limR;
        QString limU;
        QString limL;
        QString limD;
    }lstCalibFileNames;

    typedef struct customLineParameters{
        bool movible = false;
        bool rotate = false;
        int orientation; //1:Horizontal | 2:Vertical
        int lenght;
        float wavelength=0.0; //Nanometers
        QString name;
        int originalW;
        int originalH;
    }customLineParameters;

    typedef struct customRectParameters{
        bool movible = false;
        bool scalable = false;
        int analCentroid = 0;//0:No | 1:Red | 2:Green | 3:Blue | 4:source(white[All RGB])
        QString name;
        QString backgroundPath;
        GraphicsView *canvas;
        int W;//Canvas width
        int H;//Canvas height
        //int x;
        //int y;
        //int w;
        //int h;
    }customRectParameters;

    typedef struct linearRegresion{
        float a;
        float b;
    }linearRegresion;

    typedef struct structVerticalCalibration{
        int imgW;
        int imgH;
        int x1;
        int y1;
        int x2;
        int y2;
        float minWave;
        float maxWave;
        linearRegresion wavelengthLR;
        linearRegresion wave2DistLR;
        linearRegresion dist2WaveLR;
        linearRegresion vertLR;
    }structVerticalCalibration;

    typedef struct structHorizontalCalibration{
        int   imgW;
        int   imgH;
        int   H;
        float a;
        float b;
    }structHorizontalCalibration;

    typedef struct structSlideStrSens
    {
        int     filled=0;
        QString normedOrigR;
        QString normedOrigG;
        QString normedOrigB;

        QString normedRalfR;
        QString normedRalfG;
        QString normedRalfB;
        QString normedRalfH;

        QString originalR;
        QString originalG;
        QString originalB;
        QString originalH;

        QString wR;
        QString wG;
        QString wB;
    }structSlideStrSens;

    typedef struct structMaximumColors
    {
        float   maxSR=0;
        float   maxSG=0;
        float   maxSB=0;
        float   maxMaxS=0;
    }structMaximumColors;

    typedef struct structSlideSensitivities
    {
        int          filled=0;

        QList<float> normedOrigR;
        QList<float> normedOrigG;
        QList<float> normedOrigB;

        QList<float> normedRalfR;
        QList<float> normedRalfG;
        QList<float> normedRalfB;
        QList<float> normedRalfH;

        QList<float> originalR;
        QList<float> originalG;
        QList<float> originalB;
        QList<float> originalH;

        QList<float> wR;
        QList<float> wG;
        QList<float> wB;

        structMaximumColors maximumColors;

    }structSlideSensitivities;

    typedef struct structSlideCalibration{
        int     imgW;
        int     imgH;
        int     originX;
        int     originY;
        int     originH;
        int     maxNumCols;
        float   originWave;
        float   maxWave;
        QString xyMatrix;
        linearRegresion dist2WaveLR;
        linearRegresion wave2DistLR;
        linearRegresion vertLR;
        linearRegresion horizLR;
        QTransform translation;
        structSlideSensitivities sensitivities;
    }structSlideCalibration;

    typedef struct colorAnalyseResult{
        int maxRx;
        int maxR;
        int maxGx;
        int maxG;
        int maxBx;
        int maxB;
        int maxMax;
        int maxMaxx;
        int maxMaxColor;
        int *Red;
        int *Green;
        int *Blue;
    }colorAnalyseResult;

    typedef struct calcAndCropSnap{
        int r;
        int g;
        int b;
        int x1;
        int y1;
        int x2;
        int y2;
        int X1;
        int Y1;
        int X2;
        int Y2;
        int lenW;
        int lenH;
        int canvasW;
        int canvasH;
        int origImgW;
        int origImgH;
    }calcAndCropSnap;

    typedef struct frameHeader{
        unsigned char idMsg;		// Id instruction
        unsigned int consecutive;	// Consecutive
        unsigned int numTotMsg;		// Total number of message to send
        unsigned int bodyLen;		// Message lenght
        int trigeredTime = 0;           // Time before to apply console command
    }frameHeader;

    typedef struct frameStruct{
      frameHeader header;      
      char msg[frameBodyLen];				// Usable message
    }frameStruct;

    typedef struct structRaspistillCommand{
      unsigned char idMsg;					// Id instruction
      char fileName[100];
      char raspiCommand[frameBodyLen-100];
    }structRaspistillCommand;

    typedef struct strReqFileInfo{
        u_int8_t    idMsg;
        int         fileNameLen;
        char        fileName[frameBodyLen - sizeof(u_int8_t) - sizeof(int)];
    }strReqFileInfo;

    typedef struct camSettings{
      char idMsg;
      char IP[15];
      char Alias[20];
    }camSettings;

    typedef struct structRaspcamSettings{
        //int         width;                  // 1280 to 2592
        //int         height;                 // 960 to 1944
        unsigned char AWB[20];                // OFF,AUTO,SUNLIGHT,CLOUDY,TUNGSTEN,FLUORESCENT,INCANDESCENT,FLASH,HORIZON
        unsigned char Exposure[20];           // OFF,AUTO,NIGHT,NIGHTPREVIEW,BACKLIGHT,SPOTLIGHT,SPORTS,SNOW,BEACH,VERYLONG,FIXEDFPS,ANTISHAKE,FIREWORKS
        u_int8_t      Brightness;             // 0 to 100
        int           Sharpness;              // -100 to 100
        int           Contrast;               // -100 to 100
        int           Saturation;             // -100 to 100
        int           ShutterSpeedMs;         // microsecs (0 - 3000000)
        int           SquareShutterSpeedMs;   // microsecs (0 - 3000000)
        int           ISO;                    // 100 to 800
        int           ExposureCompensation;   // -10 to 10
        int           TimelapseInterval_ms;   // 100-5000 milliseconds
        int           TimelapseDurationSecs;  // 2-86400 seconds
        int           VideoDurationSecs;      // 2-86400 seconds
        u_int8_t      Format;                 // 1->raspicam::RASPICAM_FORMAT_GRAY | 2->raspicam::RASPICAM_FORMAT_YUV420
        u_int8_t      Red;                    // 0 to 8 set the value for the RED component of white balance
        u_int8_t      Green;                  // 0 to 8 set the value for the GREEN component of white balance
        u_int8_t      Preview;                // 0v1: Request a preview
        u_int8_t      OneShot;                // 0: Video streaming | 1:Snapshot
        u_int8_t      TriggeringTimeSecs;     // 0 to 500 Seconds before to take a photo
        u_int8_t      Denoise;                // 0v1: Denoise efx
        u_int8_t      FullPhoto;              // 1: No Cut | 0: Require cut
        u_int8_t      ColorBalance;           // 0v1: ColorBalance efx
        u_int8_t      CameraMp;               // 5: 5Mp | 8:8Mp
        u_int8_t      Flipped;                // 1: Horizontal Flipped | 0:Not flipped
    }structRaspcamSettings;

    typedef struct squareAperture{
        int canvasW;
        int canvasH;
        int rectX;//Begin
        int rectY;//Begin
        int rectW;//Len
        int rectH;//Len
    }squareAperture;

    typedef struct structSubimage{
        unsigned char idMsg;
        squareAperture frame;
        char fileName[100];
    }structSubimage;

    typedef struct strSlideSettings
    {
        int x1;         //Slide
        int y1;         //Slide
        int rows1;      //Slide
        int cols1;      //Slide
        int x2;         //Difraction
        int y2;         //Difraction
        int rows2;      //Difraction
        int cols2;      //Difraction
        int degreeIni;  //Start point
        int degreeEnd;  //Finish point
        int degreeJump; //Degrees at each step
        int speed;      //time lapse in ms, recomended at least 800
        //int duration;   //time in ms of the total time snapshing
    }strSlideSettings;

    typedef struct strSlideMotorWalk{
        int16_t degreeIni;
        int16_t degreeEnd;
        int16_t durationMs;
        int16_t stabilizingMs;
    }strSlideMotorWalk;

    typedef struct strReqImg{
        unsigned char idMsg;
        unsigned char stabSec;
        int imgCols;
        int imgRows;
        bool needCut;
        bool squApert;
        bool fullFrame;
        bool isSlide;
        structRaspcamSettings raspSett;
        squareAperture sqApSett;
        squareAperture diffArea;
        strSlideSettings slide;
        structRaspivid video;
        strSlideMotorWalk motorWalk;
    }strReqImg;

    typedef struct structSettings{
        bool setted;
        float version;
        unsigned int inputPort;
        unsigned int outputPort;
        unsigned int tcpPort;
    }structSettings;

    typedef struct structCamSelected{
        bool isConnected;
        bool On;
        bool stream;
        unsigned int tcpPort;
        unsigned char IP[15];
    }structCamSelected;

    typedef struct fileInfo{
        unsigned int fileLen;
        char *rawFile;
    }fileInfo;


    typedef struct colSpaceXYZ{
        float X;
        float Y;
        float Z;
        float x;
        float y;
        float z;
    }CIELab;

    typedef struct structAnalysePlotSaved
    {
        int red;
        int green;
        int blue;
        int canvasW;
        int canvasH;
        int originalW;
        int originalH;
    }structAnalysePlotSaved;

    typedef struct structSlideHypCube
    {
        bool    rotateLeft = false;
        int     width=9;
        float   extraW=0.3;
        float   shiftAllowed;//%[0,0.8]: Recomended [0,0.5]
    }structSlideHypCube;


    typedef struct structSlideShifting
    {
        QPoint shifting;
        QImage imgLeft;
        QImage imgRight;
        int extraWPix;
    }structSlideShifting;

    typedef struct structLine
    {
        int     originalW;
        int     originalH;
        int     canvasW;
        int     canvasH;
        int     x1;
        int     y1;
        int     x2;
        int     y2;
        int     colorR;
        int     colorG;
        int     colorB;
        int     oritation;
        float   wavelength;
        //float   m;//Slope
    }structLine;

    typedef struct structExportHypcubeSettings
    {
        float spectralResolution;
        int   spatialResolution;
        float spatialOverlap;
        int   flip;
        float expMinWave;
        float expMaxWave;
    }structExportHypcubeSettings;

    typedef struct structVerticalCalibrationSettings
    {
        float maxWavelength = 0.0;
    }structVerticalCalibrationSettings;

    typedef struct structSlideHypCubeSize
    {
        int hypcubeW;
        int hypcubeH;
        int hypcubeL;
        int imgW;
        int imgH;
        int slideW;
        int slideH;
        int overlapW;
        int notOverlapW;
        float specW;
        float specResolution;
        float minSpecRes;
        float* lstWavelengths;
        int* lstOverlapPos;
        int* lstNotOverlapPos;
        int* lstDestineInitX;
    }structSlideHypCubeSize;

    typedef struct strDenoisedColorSelected{
        int     colorID;
        float   wS=1.0;
    }strDenoisedColorSelected;



    typedef struct strCubeParameters{
        int   W;
        int   H;
        int   L;
        float initWavelength;
        float spectralRes;
    }strCubeParameters;

    typedef struct strSlidePlotSettings{
        float minWave;
        float maxWave;
        float XFrame;
        float YFrame;
        float XStep;
        float YStep;
    }strSlidePlotSettings;


    typedef struct strlstRemarkedPix{
        int x;
        int y;
        int thumbX;
        int thumbY;
        QColor color;
        float waveToPixeRatio;
    }strlstRemarkedPix;



    const unsigned int streamLen = sizeof(frameStruct);










#endif //LSTSTRUCTS_H
