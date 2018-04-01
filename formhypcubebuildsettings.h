#ifndef FORMHYPCUBEBUILDSETTINGS_H
#define FORMHYPCUBEBUILDSETTINGS_H

#include <QDialog>
#include <__common.h>

namespace Ui {
class formHypcubeBuildSettings;
}

class formHypcubeBuildSettings : public QDialog
{
    Q_OBJECT

public:
    explicit formHypcubeBuildSettings(QWidget *parent = 0);
    ~formHypcubeBuildSettings();

    int funcReadSettings(structExportHypcubeSettings* hypcubeSettings);

private slots:
    void on_pbSave_clicked();

private:
    Ui::formHypcubeBuildSettings *ui;
};

#endif // FORMHYPCUBEBUILDSETTINGS_H
