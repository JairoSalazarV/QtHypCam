#ifndef RASPHYPCAM_H
#define RASPHYPCAM_H

#include <string>
#include <iostream>


void debugMsg( std::string msg );

int readParamFromFile( std::string fileName, std::string* stringContain );

int funcRaspSocketConnect();

int funcRaspFileExists( std::string fileName );

int funcRaspFileExists( int socketID, std::string fileName );

bool funcRaspIsIP( std::string ipCandidate );

#endif // RASPHYPCAM_H
