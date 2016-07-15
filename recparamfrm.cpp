#include "recparamfrm.h"
#include "ui_recparamfrm.h"
#include <__common.h>
recParamFrm::recParamFrm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recParamFrm)
{
    ui->setupUi(this);
}

recParamFrm::~recParamFrm()
{
    delete ui;
}

void recParamFrm::on_pbSendParam_clicked()
{
    if( ui->txtParam->text().isEmpty() ){
        funcShowMsg("Lack","Empty param");
        return (void)NULL;
    }
    else
    {
        this->close();
        this->update();
        emit paramGenerated(ui->txtParam->text());        
    }

}
