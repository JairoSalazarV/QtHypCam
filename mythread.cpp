#include "mythread.h"
#include <QDebug>
#include <QMutex>

MyThread::MyThread(QObject *parent, bool b) :
    QThread(parent), Stop(b)
{
}

// run() will be called when a thread starts
void MyThread::run(QString text, int s)
{
    /*
    QMutex mutex;
    // prevent other threads from changing the "Stop" value
    mutex.lock();
    if(this->Stop) break;
    mutex.unlock();
    */

    // emit the signal for the count label
    emit valueChanged(text,s--);

}
