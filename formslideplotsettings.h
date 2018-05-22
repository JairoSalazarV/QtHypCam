#ifndef FORMSLIDEPLOTSETTINGS_H
#define FORMSLIDEPLOTSETTINGS_H

#include <QDialog>

namespace Ui {
class formSlidePlotSettings;
}

class formSlidePlotSettings : public QDialog
{
    Q_OBJECT

public:
    explicit formSlidePlotSettings(QWidget *parent = 0);
    ~formSlidePlotSettings();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted(bool showMsg=true);

    void reloadPlotSettings();

private:
    Ui::formSlidePlotSettings *ui;
};

#endif // FORMSLIDEPLOTSETTINGS_H
