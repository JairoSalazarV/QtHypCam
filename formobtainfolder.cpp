#include "formobtainfolder.h"
#include "ui_formobtainfolder.h"

#include <__common.h>
#include <mainwindow.h>


structCamSelected *formObtFol_CamSel;
MainWindow* formObtFol_Parent;

formObtainFolder::formObtainFolder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formObtainFolder)
{
    ui->setupUi(this);


    //------------------------------------
    //Read last execution
    //------------------------------------
    QString lastRemoteFolder;
    QString lastLocalFolder;
    readFileParam(_PATH_LAST_REMOTE_FOLDER,&lastRemoteFolder);
    readFileParam(_PATH_LAST_LOCAL_FOLDER,&lastLocalFolder);
    ui->txtSource->setText(lastRemoteFolder);
    ui->txtDestination->setText(lastLocalFolder);

    //------------------------------------
    //Get Camera From MainWindow
    //------------------------------------
    formObtFol_CamSel = (structCamSelected*)malloc(sizeof(structCamSelected));
    formObtFol_Parent = qobject_cast<MainWindow*>(parent);
    formObtFol_CamSel = formObtFol_Parent->getCameraSelected();

}

formObtainFolder::~formObtainFolder()
{
    delete ui;
}

void formObtainFolder::on_pbFolderDestination_clicked()
{
    //-----------------------------------------
    //Get Last Local Directory
    //-----------------------------------------
    QString lastLocalDir;
    if( readFileParam(_PATH_LAST_LOCAL_FOLDER,&lastLocalDir) == _ERROR )
    {
        lastLocalDir = "./";
    }

    //-----------------------------------------
    //Refresh Selected Form
    //-----------------------------------------
    QString selectedDir = funcShowSelDir(lastLocalDir);
    if( selectedDir.isEmpty() || selectedDir.isNull() )
    {
        saveFile(_PATH_LAST_LOCAL_FOLDER,_PATH_VIDEO_FRAMES);
        ui->txtDestination->setText( _PATH_VIDEO_FRAMES );
    }
    else
    {
        ui->txtDestination->setText( selectedDir );
    }
}

void formObtainFolder::on_buttonBox_accepted()
{
    //-----------------------------------------
    //Save Execution
    //-----------------------------------------
    saveFile(_PATH_LAST_REMOTE_FOLDER,ui->txtSource->text());
    saveFile(_PATH_LAST_LOCAL_FOLDER,ui->txtDestination->text());

    //-----------------------------------------
    //Obtain Remote Folder
    //-----------------------------------------
    emit signalObtainRemoteFolder(ui->txtSource->text(),ui->txtDestination->text());

}
