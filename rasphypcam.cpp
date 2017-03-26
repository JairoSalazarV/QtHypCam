#include <rasphypcam.h>

#include <unistd.h>
#include <ifaddrs.h>
#include <netdb.h>


#include <string.h>
#include <fstream>

#include <lstStructs.h>


int funcRaspFileExists( std::string fileName )
{
    //Ask to the HypCam if file exists
    //RETURN
    //          -1:error during the process | 0:File does not exists | 1:file exists



    //Connect to socket
    int socketID = funcRaspSocketConnect();
    if( socketID == -1 )
    {
        debugMsg("ERROR connecting to socket");
        return -1;
    }

    //
    //Check if file exist
    //

    //Prepare request
    int n;
    strReqFileInfo *reqFileInfo = (strReqFileInfo*)malloc(sizeof(strReqFileInfo));
    memset( reqFileInfo, '\0', sizeof(strReqFileInfo) );

    reqFileInfo->idMsg          = 10;
    reqFileInfo->fileNameLen    = fileName.size();

    memcpy( &reqFileInfo->fileName[0], fileName.c_str(), fileName.size() );

    //printf("Size(%d) Filename: %s\n",fileName.size(),reqFileInfo->fileName);
    //fflush(stdout);

    //Send request
    int tmpFrameLen = sizeof(strReqFileInfo);//IdMsg + lenInfo + fileLen + padding
    n = ::write(socketID,reqFileInfo,tmpFrameLen+1);
    if (n < 0){
        debugMsg("ERROR writing to socket");
        return -1;
    }

    //Receive response
    u_int8_t buffer[frameBodyLen];
    bzero(buffer,frameBodyLen);
    n = read( socketID, buffer, frameBodyLen );
    if (n < 0)
    {
        debugMsg("ERROR reading from socket");
        return -1;
    }

    //Close socket connection
    ::close(socketID);

    //Compute response
    if( buffer[0] == 0 )
        return 0;//File does not exists
    if( buffer[0] == 1 )
        return 1;//File exists

    debugMsg("ERROR unknow code response");
    return -1;//I do not know
}

void debugMsg( std::string msg )
{
    std::cout << msg << std::endl;
    fflush(stdout);
}

int readParamFromFile( std::string fileName, std::string* stringContain )
{
    //Read a file an put contain into stringContain
    //Return: -1 if error reading file | 0 if file is empty | 1 file is not empty

    stringContain->assign("");

    //Read file
    std::ifstream inputFile( fileName.c_str() );
    std::string line;
    if( !inputFile )
    {
        debugMsg("ERROR reading file");
        return 1;
    }
    else
    {
        inputFile >> line;
        stringContain->append(line);
    }
    inputFile.close();

    //Check if is empty
    if( stringContain->empty() )
    {
        return 0;
    }
    return 1;
}

int funcRaspSocketConnect()
{
    //Connect to socket
    //RETURN
    // -1: if error | socket: if connection success

    int PORT;
    std::string* IP = new std::string();
    std::string* fileBuffer = new std::string();

    //Read IP
    if( readParamFromFile( _PATH_LAST_IP, fileBuffer ) == 1 )
        IP->append( fileBuffer->c_str() );
    else
    {
        debugMsg( "ERROR reading IP or IP corrupted" );
        return -1;
    }

    //Read PORT
    if( readParamFromFile( _PATH_PORT_TO_CONNECT, fileBuffer ) == 1 )
        PORT = std::stoi( (char*)fileBuffer->c_str() );
    else
    {
        debugMsg( "ERROR reading IP or IP corrupted" );
        return -1;
    }



    //Prepare command message
    int sockfd = -1;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    std::cout << "Connecting to IP:" << IP->c_str() << " PORT: " << PORT << std::endl;

    if (sockfd < 0){
        std::cout << "ERROR opening socket IP:" << IP << " PORT: " << PORT << std::endl;
        return -1;
    }

    server = gethostbyname( (char*)IP->c_str() );
    if (server == NULL)
    {
        std::cout << "ERROR Socket, no such host:" << std::endl;
        return -1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons( PORT );

    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        std::cout << "ERROR connecting to IP:" << IP << " PORT: " << PORT << std::endl;
        return -1;
    }

    return sockfd;
}


bool funcRaspIsIP( std::string ipCandidate ){
    std::string delimiter = ".";
    ipCandidate.append(delimiter);
    size_t pos = 0;
    std::string token;
    int ipVal;
    char *pEnd;
    u_int8_t numElems = 0;
    while ((pos = ipCandidate.find(delimiter)) != std::string::npos) {
        token = ipCandidate.substr(0, pos);
        if( !token.empty() ){
            ipVal = strtol ( token.c_str(), &pEnd, 10 );
            //funcShowMsg("IP",QString::number(ipVal));
            if( ipVal < 0 || ipVal > 255 ){
                return false;
            }
            numElems++;
        }
        ipCandidate.erase(0, pos + delimiter.length());
    }
    if( numElems < 4 ){
        return false;
    }else{
        if( strtol ( token.c_str(), &pEnd, 10 ) == 254 ){
            return false;
        }else{
            return true;
        }
    }
}
