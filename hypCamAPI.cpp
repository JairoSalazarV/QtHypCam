#include "lstStructs.h"
#include "hypCamAPI.h"

#include <unistd.h>
#include <netdb.h>
#include <QFile>
#include <fstream>
#include <math.h>

#include <QDebug>

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


