#include "lstStructs.h"
#include "hypCamAPI.h"

#include <unistd.h>
#include <netdb.h>
#include <QFile>
#include <fstream>
#include <math.h>

#include <QDebug>
#include <qrgb.h>

u_int8_t funcSaveGrayImage( QString fileName, uchar** color, int numRows, int numCols )
{    
    //Validate fileName
    if( fileName.isEmpty() )
    {
        qDebug() << "grayImg fileName empty";
        return 0;
    }

    //Copy data into QImage
    QImage tmpImg(numCols, numRows, QImage::Format_RGB32);
    for (int i=0; i<numRows; i++)
    {
        for (int j=0; j<numCols; j++)
        {
            tmpImg.setPixel(j, i, qRgb(color[i][j],color[i][j],color[i][j]));
        }
    }

    //Save image
    tmpImg.save(fileName);

    //Return if success
    return 1;

}

void funcRGBImageToArray( uchar** r, uchar** g, uchar** b, QImage* img )
{
    //
    //Copy data into array
    //
    QRgb tmpPix;
    for(int row=0; row<img->height(); row++)
    {
        for(int col=0; col<img->width(); col++)
        {
            tmpPix  = img->pixel(col,row);
            r[row][col] = qRed( tmpPix );
            g[row][col] = qGreen( tmpPix );
            b[row][col] = qBlue( tmpPix );
        }
    }
}






void funcValCam( std::string IP, int portno, camSettings *tmpCamSett ){
    int sockfd, n, tmpFrameLen;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    qDebug() << "IP: " << IP.c_str();
    if (sockfd < 0){
        qDebug() << "opening socket";
    }else{
        server = gethostbyname( IP.c_str() );
        if (server == NULL) {
            qDebug() << "no such host";
        }else{
            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            bcopy((char *)server->h_addr,
                (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);
            serv_addr.sin_port = htons(portno);
            if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
                qDebug() << "connecting";
            }else{
                //Request camera settings
                char tmpIdMsg = (char)1;
                tmpFrameLen = sizeof(camSettings);
                unsigned char bufferRead[tmpFrameLen];
                n = ::write(sockfd,&tmpIdMsg,1);
                if (n < 0){
                    qDebug() << "writing to socket";
                }else{
                    //Receibing ack with file len
                    n = read(sockfd,bufferRead,tmpFrameLen);
                    if (n < 0){
                        qDebug() << "reading socket";
                    }else{
                        memcpy(tmpCamSett,&bufferRead,tmpFrameLen);
                    }
                }
            }
        }
        ::close(sockfd);
    }

}


