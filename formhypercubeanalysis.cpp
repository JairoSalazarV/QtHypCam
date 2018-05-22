#include "formhypercubeanalysis.h"
#include "ui_formhypercubeanalysis.h"

#include <QVBoxLayout>

formHypercubeAnalysis::formHypercubeAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formHypercubeAnalysis)
{
    ui->setupUi(this);

    ui->progressBar->setVisible(false);


    connect(
                mainGV,
                SIGNAL(signalMouseMove(QMouseEvent*)),
                this,
                SLOT(updateSignature(QMouseEvent*))
           );
}

formHypercubeAnalysis::~formHypercubeAnalysis()
{
    delete ui;
}

void formHypercubeAnalysis::updateSignature(QMouseEvent* e)
{
    int cubeX, cubeY;
    cubeX = round( ((float)e->x() / (float)mainGV->width() ) * (float)mainGV->cubeParam.W );
    cubeY = round( ((float)e->y() / (float)mainGV->height()) * (float)mainGV->cubeParam.H );
    //std::cout << "coor(" << e->x() << ", " << e->y() << ")"
    //          << " cube(" << cubeX  << ", " << cubeY  << ")" << std::endl;

}

void formHypercubeAnalysis::on_slideTmpImg_valueChanged(const int &value)
{
    updateLabel(value);
    mainGV->displayInternCubeThumb( value );
}

void formHypercubeAnalysis::updateLabel(const int &value)
{
    float tmpWavelen;
    tmpWavelen = mainGV->cubeParam.initWavelength + ( mainGV->cubeParam.spectralRes * value);
    ui->labelImgNumber->setText( "Wavelength: " + QString::number(tmpWavelen) + " nm" );
}

void formHypercubeAnalysis::on_pbCubeToImg_clicked()
{
    //-------------------------------------------------
    //Sel Dir
    //-------------------------------------------------
    QString dirPath;
    if( funcLetUserSelectDirectory(_PATH_LAST_PATH_OPENED,&dirPath) != _OK )
    {
        return (void)false;
    }

    //-------------------------------------------------
    //Create Thumbs Images From Hypcube
    //-------------------------------------------------
    if( mainGV->lstCubeThumbs.size() == 0 )
    {
        int l;
        for( l=0; l<mainGV->cubeParam.L; l++ )
        {
            mainGV->lstCubeThumbs.append( mainGV->slideImgFromCube(l) );
        }

        for( l=0; l<mainGV->cubeParam.L; l++ )
        {
             mainGV->lstCubeThumbs.at(l).save( dirPath + QString::number(l) + ".png" );
        }
    }

    /*
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(thumbImg));
    this->scene()->addItem( item );
    item->setPos(0,0);
    this->setFixedSize( thumbImg.width(), thumbImg.height() );
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    return _OK;
    */
}

void formHypercubeAnalysis::on_pbLoadCube_clicked()
{
    //------------------------------------------------
    //Load Hypercube
    //------------------------------------------------

    connect( mainGV, SIGNAL(signalProgBarValue(int,QString)), this, SLOT(updateProgressBar(int,QString)) );

    //Load Hypercube
    if( mainGV->loadHypercube() != _OK )
    {
        //funcShowMsgERROR("Loading Hypercube");
        return (void)false;
    }
    ui->slideTmpImg->setMaximum( mainGV->cubeParam.L-1 );
    ui->slideTmpImg->setMinimum( 0 );
    updateProgressBar(101,"");

    //Show Thumbnail
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(mainGV);
    this->setLayout(layout);
    mainGV->displayInternCubeThumb();

}

void formHypercubeAnalysis::updateProgressBar(int value, QString label)
{
    if( value > 100 || value < 0 )
    {
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(false);
        ui->labelProgressBar->setText("");
        return (void)true;
    }

    if( value >= 0 && value <= 100 )
    {
        ui->labelProgressBar->setText(label);
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(value);
        ui->progressBar->update();
        if( ui->progressBar->value() != value )
        {
            QtDelay(15);
        }
    }
}
