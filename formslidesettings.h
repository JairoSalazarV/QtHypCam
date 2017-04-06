#ifndef FORMSLIDESETTINGS_H
#define FORMSLIDESETTINGS_H

#include <QDialog>

namespace Ui {
class formSlideSettings;
}

class formSlideSettings : public QDialog
{
    Q_OBJECT

public:
    explicit formSlideSettings(QWidget *parent = 0);
    ~formSlideSettings();

private slots:
    void on_pbClose_clicked();

    void on_pbSave_clicked();

private:
    Ui::formSlideSettings *ui;
};

#endif // FORMSLIDESETTINGS_H
