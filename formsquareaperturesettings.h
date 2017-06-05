#ifndef FORMSQUAREAPERTURESETTINGS_H
#define FORMSQUAREAPERTURESETTINGS_H

#include <QDialog>

namespace Ui {
class formSquareApertureSettings;
}

class formSquareApertureSettings : public QDialog
{
    Q_OBJECT

public:
    explicit formSquareApertureSettings(QWidget *parent = 0);
    ~formSquareApertureSettings();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::formSquareApertureSettings *ui;
};

#endif // FORMSQUAREAPERTURESETTINGS_H
