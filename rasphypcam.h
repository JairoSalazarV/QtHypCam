#ifndef RASPHYPCAM_H
#define RASPHYPCAM_H

#include <string>
#include <iostream>


void debugMsg( std::string msg );

int readParamFromFile( std::string fileName, std::string* stringContain );

int funcRaspSocketConnect();

int funcRaspFileExists( std::string fileName );
int funcRaspFileExists( int socketID, std::string fileName );

u_int8_t* funcRaspReceiveFile(std::string fileNameRequested , int* fileLen);

bool funcRaspIsIP( std::string ipCandidate );

int funcReceiveOnePositiveInteger(int sockfd);

#endif // RASPHYPCAM_H
