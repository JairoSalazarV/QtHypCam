#ifndef GENCALIBXML_H
#define GENCALIBXML_H

#include <QDialog>

namespace Ui {
class genCalibXML;
}

class genCalibXML : public QDialog
{
    Q_OBJECT

public:
    explicit genCalibXML(QWidget *parent = 0);
    ~genCalibXML();

private:
    Ui::genCalibXML *ui;
};

#endif // GENCALIBXML_H
