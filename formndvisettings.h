#ifndef FORMNDVISETTINGS_H
#define FORMNDVISETTINGS_H

#include <QDialog>

namespace Ui {
class formNDVISettings;
}

class formNDVISettings : public QDialog
{
    Q_OBJECT

public:
    explicit formNDVISettings(QWidget *parent = 0);
    ~formNDVISettings();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::formNDVISettings *ui;
};

#endif // FORMNDVISETTINGS_H
