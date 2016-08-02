#ifndef LSTSTRUCTS_H
#define LSTSTRUCTS_H

    #include <string>
    #include <sys/types.h>
    #include <QString>    

    #include <graphicsview.h>

    //#include <string>


    const unsigned int frameBodyLen = 1024;
    #define FRAME_COMM_LEN 1024;
    #define _BIG_WIDTH 2592 //2592 | 640 | 320
    #define _BIG_HEIGHT 1944 //1944 | 480 | 240    
    #define _FRAME_THUMB_W 500
    #define _FRAME_THUMB_H 375
    #define _GRAPH_HEIGHT 440
    #define _GRAPH_CALIB_HEIGHT 590    
    #define  _USE_CAM true
    #define _FACT_MULT 3
    #define _AUTOCONNECT false
    #define PI 3.14159265

    #define _RED_WAVELENGHT 618
    #define _GREEN_WAVELENGHT 542
    #define _BLUE_WAVELENGHT 438

    #define _PATH_DISPLAY_IMAGE             "./tmpImages/tmpImg2Disp.png"
    //#define _PATH_LAST_PREVIEW            "./tmpImages/lastPeview.png"
    #define _PATH_LAST_ROTATION             "./settings/lastRotation.hypcam"
    #define _PATH_LAST_SNAPPATH             "./settings/lastSnapPath.hypcam"    
    #define _PATH_AUX_IMG                   "./tmpImages/tmp.png"
    #define _PATH_IMAGE_RECEIVED            "./tmpImages/tmpImgRec.RGB888"
    #define _PATH_SQUARE_APERTURE           "./XML/squareAperture.xml"
    #define _PATH_SQUARE_USABLE             "./XML/squareUsable.xml"
    #define _PATH_REGION_OF_INTERES         "./XML/regionOfInteres.xml"
    #define _PATH_CALIBRATION_FILE          "./XML/hypcalib.xml"
    #define _PATH_CALBKG                    "./settings/Calib/backgroundPath.hypcam"
    #define _PATH_CALIB                     "./settings/Calib/"
    #define _PATH_WAVE_OPTIONS              "./settings/waveOptions.hypcam"
    #define _PATH_WAVE_CHOISES              "./settings/waveChoises.hypcam"
    #define _PATH_CUSTOM_GV_DEFAULT         "./tmpImages/customGraphicView.png"
    #define _PATH_HALOGEN_FUNCTION          "./settings/halogenFunction.hypcam"

    #define _PATH_RED_RESPONSE              "./settings/Calib/responses/redResponse.hypcam"
    #define _PATH_GREEN_RESPONSE            "./settings/Calib/responses/greenResponse.hypcam"
    #define _PATH_BLUE_RESPONSE             "./settings/Calib/responses/blueResponse.hypcam"

    #define _PATH_RED_SENSITIV              "./settings/Calib/responses/redSensivility.hypcam"
    #define _PATH_GREEN_SENSITIV            "./settings/Calib/responses/greenSensivility.hypcam"
    #define _PATH_BLUE_SENSITIV             "./settings/Calib/responses/blueSensivility.hypcam"

    #define _PATH_RED_SENS_NORM             "./settings/Calib/responses/redSensivilityNorm.hypcam"
    #define _PATH_GREEN_SENS_NORM           "./settings/Calib/responses/greenSensivilityNorm.hypcam"
    #define _PATH_BLUE_SENS_NORM            "./settings/Calib/responses/blueSensivilityNorm.hypcam"

    #define _PATH_HALOGEN_IRRADIATION       "./settings/Calib/responses/halogenIrradiation.hypcam"
    #define _PATH_RGB_SENSIVILITIES         "./settings/Calib/responses/sensitivities.hypcam"

    #define _PATH_LIMIT_R                   "./settings/Calib/limR.hypcam"
    #define _PATH_LIMIT_U                   "./settings/Calib/limU.hypcam"
    #define _PATH_LIMIT_L                   "./settings/Calib/limL.hypcam"
    #define _PATH_LIMIT_D                   "./settings/Calib/limD.hypcam"
    #define _PATH_LIMIT_S                   "./settings/Calib/sourceHalogen.hypcam"

    #define _EXTENTION                      ".hypcam"

    #define _RIGHT  1
    #define _UP     2
    #define _LEFT   3
    #define _DOWN   4

    #define _RED    1
    #define _GREEN  2
    #define _BLUE   3
    #define _RGB    7

    #define _ERROR_FILE_NOTEXISTS           "~FileDoesNotExists"
    #define _ERROR_FILE                     "~UnknowError"

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
        double wavelength;
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
        QString bkgPath;
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
        int            *sensitivity;
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
    }lstCalibFileNames;

    typedef struct customLineParameters{
        bool movible = false;
        bool rotate = false;
        int orientation; //0:Rotated | 1:Horizontal | 2:Vertical
        int lenght;
        QString name;
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
        int origImgW;
        int origImgH;
    }calcAndCropSnap;

    typedef struct frameHeader{
        unsigned char idMsg;					// Id instruction
        unsigned int consecutive;	// Consecutive
        unsigned int numTotMsg;		// Total number of message to send
        unsigned int bodyLen;		// Message lenght
    }frameHeader;

    typedef struct frameStruct{
      frameHeader header;
      char msg[frameBodyLen];				// Usable message
    }frameStruct;

    typedef struct camSettings{
      char idMsg;
      char IP[15];
      char Alias[20];
    }camSettings;

    typedef struct structRaspcamSettings{
        //int             width;                // 1280 to 2592
        //int             height;               // 960 to 1944
        unsigned char   AWB[20];                // OFF,AUTO,SUNLIGHT,CLOUDY,TUNGSTEN,FLUORESCENT,INCANDESCENT,FLASH,HORIZON
        unsigned char   Exposure[20];           // OFF,AUTO,NIGHT,NIGHTPREVIEW,BACKLIGHT,SPOTLIGHT,SPORTS,SNOW,BEACH,VERYLONG,FIXEDFPS,ANTISHAKE,FIREWORKS
        u_int8_t        Brightness;             // 0 to 100
        int             Sharpness;              // -100 to 100
        int             Contrast;               // -100 to 100
        int             Saturation;             // -100 to 100
        int             ShutterSpeed;           // microsecs (0 - 3000000)
        int             ShutterSpeedSmall;      // microsecs (1 - 95000)
        int             ISO;                    // 100 to 800
        int             ExposureCompensation;   // -10 to 10
        u_int8_t        Format;                 // 1->raspicam::RASPICAM_FORMAT_GRAY | 2->raspicam::RASPICAM_FORMAT_YUV420
        u_int8_t        Red;                    // 0 to 8 set the value for the RED component of white balance
        u_int8_t        Green;                  // 0 to 8 set the value for the GREEN component of white balance
        u_int8_t        Preview;                // 0v1: Request a preview
        u_int8_t        OneShot;                // 0: Video streaming | 1:Snapshot
        u_int8_t        TriggerTime;            // Seconds before to take a photo
        u_int8_t        Denoise;                // 0v1: Denoise efx
        u_int8_t        ColorBalance;           // 0v1: ColorBalance efx
    }structRaspcamSettings;

    typedef struct squareAperture{
        int canvasW;
        int canvasH;
        int rectX;//Begin
        int rectY;//Begin
        int rectW;//Len
        int rectH;//Len
    }squareAperture;

    typedef struct strReqImg{
        unsigned char idMsg;
        unsigned char stabSec;
        int imgCols;
        int imgRows;
        bool needCut;
        structRaspcamSettings raspSett;
        squareAperture sqApSett;
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

    const unsigned int streamLen = sizeof(frameStruct);





#endif //LSTSTRUCTS_H
