#include "selcolor.h"
#include "ui_selcolor.h"
#include "__common.h"

selColor::selColor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selColor)
{
    ui->setupUi(this);
}

selColor::~selColor()
{
    delete ui;
}

void selColor::on_pushButton_clicked()
{
    QString color = ui->lineEdit->text();
    emit signalColorSelected(color);
    this->close();
}

/*
void signalColorSelected(QString color){
    color = color;
}
*/
