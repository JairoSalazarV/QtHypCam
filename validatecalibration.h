#ifndef VALIDATECALIBRATION_H
#define VALIDATECALIBRATION_H

#include <QDialog>

namespace Ui {
class validateCalibration;
}

class validateCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit validateCalibration(QWidget *parent = 0);
    ~validateCalibration();


private:
    Ui::validateCalibration *ui;
};

#endif // VALIDATECALIBRATION_H
