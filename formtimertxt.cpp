#include "formtimertxt.h"
#include "ui_formtimertxt.h"
#include <ctime>
#include <QThread>

#include <QThread>

formTimerTxt::formTimerTxt(QWidget *parent, QString text, int seconds) :
    QDialog(parent),
    ui(new Ui::formTimerTxt)
{
    ui->setupUi(this);

    // connect signal/slot
    connect(this, SIGNAL(signalTimerRefresh(int)),
            this, SLOT(updateTimer(int)));

    this->setWindowTitle(text);
    emit signalTimerRefresh(seconds);

}

void formTimerTxt::startMyTimer(int seconds)
{
    int i;
    for(i=seconds; i>=0; i--)
    {        
        emit signalTimerRefresh(i);
        QtDelay(1000);
    }
    this->close();
}

formTimerTxt::~formTimerTxt()
{
    delete ui;
}

void formTimerTxt::on_pushButton_clicked()
{
    this->close();
}

void formTimerTxt::updateTimer(int s)
{

    QString strClock;
    int h, m;

    strClock.clear();
    //Hours
    h =     floor( (float)s/(float)(60*60) );
    s -=    60*60*h;
    //Minuts
    m =     floor( (float)s/(float)(60) );
    s -=    60*m;
    //Delay
    strClock = (h<10)?strClock+"0"+QString::number(h):QString::number(h);
    strClock.append(":");
    strClock = (m<10)?strClock+"0"+QString::number(m):strClock+QString::number(m);
    strClock.append(":");
    strClock = (s<10)?strClock+"0"+QString::number(s):strClock+QString::number(s);
    //Refresh clock    
    ui->label->setText(strClock);
    ui->label->update();    
    this->update();
    qDebug() << "strClock: " << strClock;
}
