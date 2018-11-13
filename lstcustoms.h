#ifndef LSTCUSTOMS_H
#define LSTCUSTOMS_H


    #define _VERSION_TXT                    "Beta 1.0"

    const unsigned int frameBodyLen = 1024;
    #define FRAME_COMM_LEN                  1024
    //#define _BIG_WIDTH 2592 //2592 | 640 | 320
    //#define _BIG_HEIGHT 1944 //1944 | 480 | 240
    #define _FRAME_THUMB_W                  500
    #define _FRAME_THUMB_H                  500
    #define _GRAPH_HEIGHT                   440
    #define _GRAPH_CALIB_HEIGHT             590
    #define  _USE_CAM                       true
    #define _FACT_MULT                      3
    //#define _AUTOCONNECT true
    #define PI                              3.14159265

    #define _PADDING_W                      50
    #define _PADDING_H                      160

    //#define _INVERTED_CAMERA                1

    //#define _RED_WAVELENGHT                 618
    //#define _GREEN_WAVELENGHT               542
    //#define _BLUE_WAVELENGHT                438

    #define _EXTENTION                      ".hypcam"
    #define _EXT_HYPERCUBE                  ".hypercube"

    #define _H264                           0
    #define _MJPEG                          1

    #define _VIDEO_FRAME_RATE               30
    #define _STABILIZATION_SECS             3

    #define _RIGHT                          1
    #define _ABOVE                          2
    #define _LEFT                           3
    #define _DOWN                           4

    #define _RED                            1
    #define _GREEN                          2
    #define _BLUE                           3
    #define _RGB                            7

    #define _OK                             1
    #define _FAILURE                        0
    #define _ERROR                          -1

    #define _YES                            1;
    #define _NO                             0;
    #define _NOT_ANSWER                     -1;


    #define _HORIZONTAL                     1
    #define _VERTICAL                       2


    #define _ERROR_FILE_NOTEXISTS           "~FileDoesNotExists"
    #define _ERROR_FILE                     "~UnknowError"

    #define SQUARE_BICUBIC_ITERATIONS       1        //Apply Bicubic k times

    #define _FRAME_EXTENSION                ".png"
    #define _FRAME_RECEIVED_EXTENSION       "RGB888"
    #define _VIDEO_EXTENSION                ".H264"
    #define _SNAPSHOT_REMOTE_EXTENSION      ".RGB888"


    #define _MSG_PROGBAR_STABILIZING        "Stabilizing Remote Camera"
    #define _MSG_PROGBAR_TRANSF_FILE        "Transferring Requested File"
    #define _MSG_PROGBAR_RESET              " "

    #define _RASPBERRY_STABILIZATION_TIME   2000 //Seconds

    #define _RASP_CAM_5MP_IMAGE_W           2592
    #define _RASP_CAM_5MP_IMAGE_H           1944
    #define _RASP_CAM_5MP_VIDEO_W           1920 //1920 | 1640
    #define _RASP_CAM_5MP_VIDEO_H           1080 //1080 | 1232    

    #define _RASP_CAM_8MP_IMAGE_W           3240
    #define _RASP_CAM_8MP_IMAGE_H           2464
    #define _RASP_CAM_8MP_VIDEO_W           1640 //1920 | 1640
    #define _RASP_CAM_8MP_VIDEO_H           1232 //1080 | 1232

    #define _RASP_CAM_MAX_WAVELENGTH        950  //Used During Slide Clibration
    #define _RASP_CAM_MIN_WAVELENGTH        300  //Used During Slide Clibration


#endif // LSTCUSTOMS_H
