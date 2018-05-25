#include "formmerge3grayintoargb.h"
#include "ui_formmerge3grayintoargb.h"
#include <__common.h>

formMerge3GrayIntoARgb::formMerge3GrayIntoARgb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formMerge3GrayIntoARgb)
{
    ui->setupUi(this);
}

formMerge3GrayIntoARgb::~formMerge3GrayIntoARgb()
{
    delete ui;
}

void formMerge3GrayIntoARgb::on_pbRed_clicked()
{
    QString defaVideoPath;
    if( funcLetUserSelectFile( &defaVideoPath, "Select Red Source", this ) != _OK )
    {
        funcShowMsgERROR_Timeout("Select Red Image Source");
        return (void)false;
    }
    ui->txtRed->setText(defaVideoPath);
}

void formMerge3GrayIntoARgb::on_pbGreen_clicked()
{
    QString defaVideoPath;
    if( funcLetUserSelectFile( &defaVideoPath, "Select Green Source", this ) != _OK )
    {
        funcShowMsgERROR_Timeout("Select Green Image Source");
        return (void)false;
    }
    ui->txtGreen->setText(defaVideoPath);
}

void formMerge3GrayIntoARgb::on_pbBlue_clicked()
{
    QString defaVideoPath;
    if( funcLetUserSelectFile( &defaVideoPath, "Select Blue Source", this ) != _OK )
    {
        funcShowMsgERROR_Timeout("Select Vlue Image Source");
        return (void)false;
    }
    ui->txtBlue->setText(defaVideoPath);
}

void formMerge3GrayIntoARgb::on_pbSave_clicked()
{
    if(
            ui->txtRed->text().trimmed().isEmpty()      ||
            ui->txtGreen->text().trimmed().isEmpty()    ||
            ui->txtBlue->text().trimmed().isEmpty()
    ){
        funcShowMsgERROR_Timeout("All Parameters Required");
        return (void)false;
    }

    //Validatas Images Size
    QImage srcRed( ui->txtRed->text().trimmed() );
    QImage srcGreen( ui->txtGreen->text().trimmed() );
    QImage srcBlue( ui->txtBlue->text().trimmed() );
    QImage tmpRGB(QSize(srcRed.width(),srcRed.height()),QImage::Format_RGB32);

    //Copi colors
    int x, y;
    QColor tmpRed, tmpGreen, tmpBlue;
    for(x=0; x<srcRed.width(); x++)
    {
        for(y=0; y<srcRed.height(); y++)
        {
            tmpRed      = srcRed.pixelColor(x,y);
            tmpGreen    = srcGreen.pixelColor(x,y);
            tmpBlue     = srcBlue.pixelColor(x,y);
            tmpRGB.setPixelColor(
                                    QPoint(x,y),
                                    QColor(
                                                tmpRed.red(),
                                                tmpGreen.green(),
                                                tmpBlue.blue()
                                           )
                                 );
        }
    }

    //Save Image
    QString outputFileName;
    if(
            funcLetUserDefineFile(
                                    &outputFileName,
                                    "Define the Output fileName",
                                    ".png",
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    new QString(_PATH_LAST_PATH_OPENED),
                                    this
                                 ) != _OK
    ){
        funcShowMsgERROR_Timeout("Output Filename Wrong");
        return (void)false;
    }
    tmpRGB.save(outputFileName);
    funcShowMsgSUCCESS_Timeout("Image RGB Saved Successfully");
    this->close();
    /*
    QString* filePath,
    QString title,
    QString extension,
    QString* pathLocation,
    QString pathOfInterest,
    QWidget* parent
    */
    //tmpRGB


}
