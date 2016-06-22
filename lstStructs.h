#ifndef LSTSTRUCTS_H
#define LSTSTRUCTS_H

    const unsigned int frameBodyLen = 512;

    typedef struct frameHeader{
        char idMsg;					// Id instruction
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
      char IP[20];
      char Alias[20];
    }camSettings;

    typedef struct structSettings{
        bool setted;
        float version;
        unsigned int inputPort;
        unsigned int outputPort;
    }structSettings;

    const unsigned int streamLen = sizeof(frameStruct);

#endif //LSTSTRUCTS_H
