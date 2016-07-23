#include "generatehypercube.h"
#include "ui_generatehypercube.h"

#include <QDesktopWidget>
#include <lstStructs.h>

generateHypercube::generateHypercube(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::generateHypercube)
{
    ui->setupUi(this);



}

generateHypercube::~generateHypercube()
{
    delete ui;
}
