#ifndef HYPCAMAPI_H
#define HYPCAMAPI_H

    #include "lstStructs.h"
    #include "__common.h"
    #include <string>

    void funcValCam( std::string IP, int portno, camSettings *tmpCamSett );

    void funcRGBImageToArray(uchar **r, uchar **g, uchar **b, QImage* img );

    u_int8_t funcSaveGrayImage( QString fileName, uchar** color, int numRows, int numCols );

#endif // HYPCAMAPI_H
