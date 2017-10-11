#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0, bool b = false);
    void run(QString text, int s);

    // if Stop = true, the thread will break
    // out of the loop, and will be disposed
    bool Stop;

signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void valueChanged(QString,int);

public slots:

};

#endif // MYTHREAD_H
