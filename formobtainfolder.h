#ifndef FORMOBTAINFOLDER_H
#define FORMOBTAINFOLDER_H

#include <QDialog>

namespace Ui {
class formObtainFolder;
}

class formObtainFolder : public QDialog
{
    Q_OBJECT

public:
    explicit formObtainFolder(QWidget *parent = 0);
    ~formObtainFolder();

private slots:
    void on_pbFolderDestination_clicked();

    void on_buttonBox_accepted();

private:
    Ui::formObtainFolder *ui;

signals:
    void signalObtainRemoteFolder( QString remoteFolder, QString localFolder );


};

#endif // FORMOBTAINFOLDER_H
