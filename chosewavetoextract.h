#ifndef CHOSEWAVETOEXTRACT_H
#define CHOSEWAVETOEXTRACT_H

#include <QDialog>

#include <__common.h>
#include <QTableWidget>

namespace Ui {
class choseWaveToExtract;
}

class choseWaveToExtract : public QDialog
{
    Q_OBJECT

    lstDoubleAxisCalibration daCalib;

public:
    explicit choseWaveToExtract(QWidget *parent = 0);
    ~choseWaveToExtract();    

private slots:

    void addByStep();

    void on_pbAdd_clicked();

    void switchSelected( QTableWidget *tableOrig, QTableWidget *tableDest);

    void fromTablesToFiles();

    void on_pbRemoveAll_clicked();

    void on_pbAddAll_clicked();

    void on_pbRemove_clicked();

    void on_tableOptions_doubleClicked(const QModelIndex &index);

    void on_tableChoises_doubleClicked(const QModelIndex &index);

    void iniOptsAndChois(bool allOptions);

private:
    Ui::choseWaveToExtract *ui;

    void fillOptions();

    void insertRow(QString wave , QTableWidget *table);

    void refreshOptChoi();



};

#endif // CHOSEWAVETOEXTRACT_H
