#include "chosewavetoextract.h"
#include "ui_chosewavetoextract.h"



choseWaveToExtract::choseWaveToExtract(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choseWaveToExtract)
{
    ui->setupUi(this);


    funcGetCalibration(&daCalib);
    fillOptions();

}

choseWaveToExtract::~choseWaveToExtract()
{
    delete ui;
}

void choseWaveToExtract::fillOptions(){
    //Get options and choises
    //..
    if( !QFile::exists(_PATH_WAVE_OPTIONS) )
    {
        iniOptsAndChois(true);
    }
    else
    {
        refreshOptChoi();
    }
}


void choseWaveToExtract::refreshOptChoi()
{
    //Clear tables
    //..
    while (ui->tableOptions->rowCount() > 0)
    {
        ui->tableOptions->removeRow(0);
    }
    while (ui->tableChoises->rowCount() > 0)
    {
        ui->tableChoises->removeRow(0);
    }

    //Fill tables
    //..
    QString options, choises;
    options = readAllFile(_PATH_WAVE_OPTIONS);
    choises = readAllFile(_PATH_WAVE_CHOISES);
    QList<QString> lstOptions = options.split(",");
    QList<QString> lstChoises = choises.split(",");
    std::sort(lstOptions.begin(), lstOptions.end());
    std::sort(lstChoises.begin(), lstChoises.end());

    Q_FOREACH(const QString option, lstOptions)
    {
        insertRow(option,ui->tableOptions);
    }

    Q_FOREACH(const QString choise, lstChoises)
    {
        insertRow(choise,ui->tableChoises);
    }
}

void choseWaveToExtract::iniOptsAndChois(bool allOptions)
{
    float tmpWave;
    tmpWave = daCalib.minWavelength;
    QString options;
    options.append(QString::number(tmpWave));
    while( tmpWave < daCalib.maxWavelength - daCalib.minSpecRes )
    {
        tmpWave += daCalib.minSpecRes;
        options.append("," + QString::number(tmpWave));
    }

    if(allOptions)
    {
        saveFile(_PATH_WAVE_OPTIONS,options);
        saveFile(_PATH_WAVE_CHOISES,"");
    }
    else
    {
        saveFile(_PATH_WAVE_CHOISES,options);
        saveFile(_PATH_WAVE_OPTIONS,"");
    }

    refreshOptChoi();
}

void choseWaveToExtract::insertRow(QString wave, QTableWidget *table )
{
    if( !wave.isEmpty() && wave != "\n" )
    {
        int row;
        row = table->rowCount();
        table->insertRow(row);
        table->setItem(row,0,new QTableWidgetItem(wave));
    }
}

void choseWaveToExtract::switchSelected( QTableWidget *tableOrig, QTableWidget *tableDest)
{
    int id;
    QString waveSelected;
    id = tableOrig->currentRow();
    waveSelected = tableOrig->item(id,0)->text();
    tableOrig->removeRow(id);
    insertRow(waveSelected,tableDest);
    fromTablesToFiles();
    refreshOptChoi();
}

void choseWaveToExtract::fromTablesToFiles()
{
    QString options, choises;
    while (ui->tableOptions->rowCount() > 0)
    {
        options.append("," + ui->tableOptions->item(0,0)->text());
        ui->tableOptions->removeRow(0);
    }
    while (ui->tableChoises->rowCount() > 0)
    {
        choises.append("," + ui->tableChoises->item(0,0)->text());
        ui->tableChoises->removeRow(0);
    }
    saveFile(_PATH_WAVE_OPTIONS,options);
    saveFile(_PATH_WAVE_CHOISES,choises);
}

void choseWaveToExtract::on_pbRemoveAll_clicked()
{
    iniOptsAndChois(true);
}

void choseWaveToExtract::on_pbAddAll_clicked()
{
    iniOptsAndChois(false);
}

void choseWaveToExtract::on_pbAdd_clicked()
{
    switchSelected( ui->tableOptions, ui->tableChoises );
}

void choseWaveToExtract::on_pbRemove_clicked()
{
    switchSelected( ui->tableChoises, ui->tableOptions );
}
