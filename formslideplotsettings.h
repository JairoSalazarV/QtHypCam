#ifndef FORMSLIDEPLOTSETTINGS_H
#define FORMSLIDEPLOTSETTINGS_H

#include <QDialog>
#include <lstStructs.h>

namespace Ui {
class formSlidePlotSettings;
}

class formSlidePlotSettings : public QDialog
{
    Q_OBJECT

public:
    explicit formSlidePlotSettings(QWidget *parent = 0);
    ~formSlidePlotSettings();    

    void readPlotSettings( strSlidePlotSettings* slideSettings );

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void reloadPlotSettings();


private:
    Ui::formSlidePlotSettings *ui;
};

#endif // FORMSLIDEPLOTSETTINGS_H
