#ifndef GENERATEHYPERCUBE_H
#define GENERATEHYPERCUBE_H

#include <QDialog>

namespace Ui {
class generateHypercube;
}

class generateHypercube : public QDialog
{
    Q_OBJECT

public:
    explicit generateHypercube(QWidget *parent = 0);
    ~generateHypercube();

private:
    Ui::generateHypercube *ui;
};

#endif // GENERATEHYPERCUBE_H
