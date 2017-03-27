#ifndef ARDUINOMOTOR_H
#define ARDUINOMOTOR_H

#include <QSerialPort>
#include <QThread>
#include <QDebug>

/*
class Thread : public QThread
{
private:
    void run()
    {
        qDebug()<<"From worker thread: "<<currentThreadId();
    }
};
*/
class arduinoMotor: public QThread
{
    //Q_OBJECT

    int internIni;
    int internEnd;
    int internStep;
    int internSpeed;

public:
    arduinoMotor();

    void setAWalk( int ini, int end, int step, int speed );

    void doAWalk();

    std::string padding( int direction, int step );

    int goTo( int degrees );

    int goToZero();

    int goBackward( int degrees );

    int goForward( int degrees );

    QSerialPort* connectSerialPort();

private:
    void run()
    {
        doAWalk();
    }
};

#endif // ARDUINOMOTOR_H
