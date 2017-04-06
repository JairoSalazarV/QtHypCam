#include "arduinomotor.h"
#include <string>
#include <rasphypcam.h>
#include <__common.h>

arduinoMotor::arduinoMotor()
{

}

void arduinoMotor::setAWalk( int ini, int end, int step, int speed )
{
    this->internIni     = ini;
    this->internEnd     = end;
    this->internStep    = step;
    this->internSpeed   = speed;
}

void arduinoMotor::doAWalk()
{

    if( goTo( internIni ) == -1 )
    {
        //return -1;
    }
    else
    {
        int pos = internIni;
        while( pos < internEnd )
        {
            QtDelay(internSpeed);
            goForward(internStep);
            pos += internStep;
        }
        //QtDelay(internSpeed);
        QThread::msleep(internSpeed);
        goToZero();
    }
    //return 1;
}

std::string arduinoMotor::padding( int direction, int step )
{
    std::string tmp;
    tmp.assign("");
    if( step < 1 )
        return tmp;
    else
    {
        for(int i=1; i<=step; i++)
        {
            if( direction == 1 )//Forward
                tmp.append("1");
            else if( direction == 2 )//Backward
                tmp.append("2");
            else
                return tmp;
        }
    }

    return tmp;
}

int arduinoMotor::goTo( int degrees )
{
    if( goToZero() == -1 )
    {
        return -1;
    }
    else
    {
        if( goForward( degrees ) == -1 )
            return -1;
    }
    return 1;
}

int arduinoMotor::goToZero()
{
    QSerialPort* serialPort = connectSerialPort();

    if( serialPort->isOpen() )
    {
        std::string tmp;
        tmp.assign("");
        for( int i=1; i<=380; i++ )
            tmp.append( "2" );
        serialPort->write(tmp.c_str(),tmp.size());
        serialPort->waitForBytesWritten(100);
        serialPort->close();
    }
    else
        return -1;


    return 1;
}

int arduinoMotor::goBackward( int degrees )
{

    QSerialPort* serialPort = connectSerialPort();

    if( serialPort->isOpen() )
    {
        std::string tmp;
        tmp.assign("");
        for( int i=1; i<=degrees; i++ )
            tmp.append( "2" );
        serialPort->write(tmp.c_str(),tmp.size());
        serialPort->waitForBytesWritten(100);
        serialPort->close();
    }
    else
        return -1;


    return 1;
}

int arduinoMotor::goForward( int degrees )
{

    QSerialPort* serialPort = connectSerialPort();

    if( serialPort->isOpen() )
    {
        std::string tmp;
        tmp.assign("");
        for( int i=1; i<=degrees; i++ )
            tmp.append( "1" );
        serialPort->write(tmp.c_str(),tmp.size());
        serialPort->waitForBytesWritten(100);
        serialPort->close();
    }
    else
        return -1;


    return 1;
}

QSerialPort* arduinoMotor::connectSerialPort()
{
    QSerialPort* serialPort = new QSerialPort();
    serialPort->setBaudRate(9600);
    serialPort->setPortName("ttyUSB0");
    if( serialPort->open(QIODevice::ReadWrite) )
    {
        debugMsg("Connected to Motor Successfully");
    }
    else
    {
        debugMsg("Error: connecting to Arduino Motor");
    }
    return serialPort;
}
