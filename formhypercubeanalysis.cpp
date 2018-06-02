#include "formhypercubeanalysis.h"
#include "ui_formhypercubeanalysis.h"

#include <QVBoxLayout>
#include <QMainWindow>
#include <QGraphicsLineItem>
#include <QPen>

#include <QFormLayout>
#include <formslideplotsettings.h>

QWidget *parentHypercubeAnalysis;

formHypercubeAnalysis::formHypercubeAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formHypercubeAnalysis)
{    
    ui->setupUi(this);

    ui->progressBar->setVisible(false);

    parentHypercubeAnalysis = qobject_cast<QMainWindow*>(parent);

    ui->slideTmpImg->setEnabled(false);

    connect(
                gvImg,
                SIGNAL(signalPixeleSelected(QMouseEvent*)),
                this,
                SLOT(addRemarkedPixel(QMouseEvent*))
           );

}

formHypercubeAnalysis::~formHypercubeAnalysis()
{
    delete ui;
}

void formHypercubeAnalysis::addRemarkedPixel(QMouseEvent* e)
{
    //Calc Real Pixel Coordinates
    int cubeX, cubeY;
    cubeX = round( ((float)e->x() / (float)gvPlot->width()  ) * (float)slideHypCube->cubeParam.W );
    cubeY = round( ((float)e->y() / (float)gvPlot->height() ) * (float)slideHypCube->cubeParam.H );
    //Save Pixel
    strlstRemarkedPix remarkedPix;
    QString newColor    = funcGetParam("Color","#0F0");
    remarkedPix.x       = cubeX;
    remarkedPix.y       = cubeY;
    remarkedPix.thumbX  = e->x();
    remarkedPix.thumbY  = e->y();
    remarkedPix.color   = QColor(newColor.trimmed());
    lstRemarkedPix.append(remarkedPix);
}

/*
void formHypercubeAnalysis::refreshGVGeometry()
{
    //Calc Coordinates Sizes
    int tmpW, tmpH, frameW, frameH;
    QRect screenRes = screenResolution(parentHypercubeAnalysis);
    frameW  = 30;
    frameH  = 80;
    tmpW    = screenRes.width() - frameW;
    tmpH    = round( ((float)screenRes.height() - frameH) * 0.4 );

    //Modify Geometries
    mainGV->setFixedSize(tmpW,mainGV->height());
    plotGV->setFixedSize(tmpW,tmpH);

    mainGV->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    mainGV->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    plotGV->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    plotGV->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    std::cout << "refreshGVGeometry" << std::endl;
}*/

void formHypercubeAnalysis::updateSignature(QMouseEvent* e)
{

    //QGraphicsScene* tmpScene = new QGraphicsScene(this);

    //Get Cube Postion
    int cubeX, cubeY;
    cubeX = round( ((float)e->x() / (float)gvPlot->width()  ) * (float)slideHypCube->cubeParam.W );
    cubeY = round( ((float)e->y() / (float)gvPlot->height() ) * (float)slideHypCube->cubeParam.H );
    //std::cout << "coor(" << e->x() << ", " << e->y() << ")"
    //          << " cube(" << cubeX  << ", " << cubeY  << ")" << std::endl;
    if(
            cubeX < 0 || cubeX > slideHypCube->cubeParam.W-1 ||
            cubeY < 0 || cubeY > slideHypCube->cubeParam.H-1
    ){
        msg("Wrong coordinates");
        std::cout << "cubeX: " << cubeX << " cubeY: " << cubeY << std::endl;
        std::cout << "maxX: " << slideHypCube->cubeParam.W << " maxY: " << slideHypCube->cubeParam.H << std::endl;
        return (void)false;
    }

    int frameW = plotSettings.XFrame;
    int frameH = plotSettings.YFrame;
    int W = gvPlot->width();
    int H = gvPlot->height();
    int yRealArea = H-frameH;
    int xRealArea = W-frameW;
    QPen tmpPen = QPen(QBrush(Qt::white),2.0);
    //gvImg->scene()->removeItem(tmpRect);
    gvPlot->scene()->clear();            
    //X-Axix
    gvPlot->scene()->addLine( 0, yRealArea, W, yRealArea, tmpPen );
    //Y-Axis
    gvPlot->scene()->addLine( frameW, 0, frameW, H, tmpPen );

    //-----------------------------------------------------------
    //Refresh Thumb Image
    //-----------------------------------------------------------
    //gvImg->scene()->clear();
    //updateLabel(ui->slideTmpImg->value());
    //updateSlidePicture(ui->slideTmpImg->value());

    //=================================================================
    //DRAW AXIS
    //=================================================================
    //-------------------------------
    //Read Settings
    //-------------------------------
    float minWave   = plotSettings.minWave;
    float maxWave   = plotSettings.maxWave;

    //-------------------------------
    //Y-Bars
    //-------------------------------
    //Prepare font
    QFont font;
    font.setPixelSize(14);
    font.setBold(false);
    font.setFamily("Times");
    QPen penTxt(Qt::white,1.5);
    //Prepare coordinates
    int x1, y1, x2, y2;
    int yBarW = 7;
    int yStep = round((float)yRealArea / (plotSettings.YStep+0.3));//Each 20
    int numYSteps = ceil( 100.0 / plotSettings.YStep );
    int i;
    for( i=1; i<=numYSteps; i++ )
    {
        //Line
        x1 = frameW - yBarW;
        y1 = H - frameH - (i*yStep);
        x2 = frameW + yBarW;
        y2 = y1;
        gvPlot->scene()->addLine( x1, y1, x2, y2, tmpPen );
        //Text
        QGraphicsSimpleTextItem* tmpTxt = new QGraphicsSimpleTextItem(QString::number(i*plotSettings.YStep));
        tmpTxt->setPos(x2-40,y2-10);
        tmpTxt->setFont(font);
        tmpTxt->setPen(penTxt);
        gvPlot->scene()->addItem(tmpTxt);
    }

    //-------------------------------
    //X-Bars
    //-------------------------------
    int xH          = 7;
    float xStep     = plotSettings.XStep;
    float range     = maxWave - minWave;
    int numSteps    = ceil( range / xStep );
    int xPixStep    = (float)xRealArea / (float)numSteps;
    for( i=1; i<numSteps; i++ )
    {
        //Line
        x1  = frameW + (i*xPixStep);
        y1  = H - frameH - xH;
        x2  = x1;
        y2  = H - frameH + xH;
        gvPlot->scene()->addLine( x1, y1, x2, y2, tmpPen );
        //Text
        QGraphicsSimpleTextItem* tmpTxt = new QGraphicsSimpleTextItem(QString::number(i*xStep)+" nm");
        tmpTxt->setPos(x1-10,y1+15);
        tmpTxt->setFont(font);
        tmpTxt->setPen(penTxt);
        gvPlot->scene()->addItem(tmpTxt);
    }

    //-------------------------------
    //Calc Wave to Pixel Ratio
    //-------------------------------
    float waveToPixeRatio = (float)xRealArea / range;

    //msg("Updating");


    //=================================================================
    //=================================================================
    //Plot Actual Pixel
    strlstRemarkedPix tmpPix;
    tmpPix.x                = cubeX;
    tmpPix.y                = cubeY;
    tmpPix.thumbX           = e->x();
    tmpPix.thumbY           = e->y();
    tmpPix.color            = Qt::white;
    tmpPix.waveToPixeRatio  = waveToPixeRatio;
    plotPixel( tmpPix, yRealArea );

    //Plot Lst Remarked Pixel
    for(i=0; i<lstRemarkedPix.size(); i++)
    {
        tmpPix                  = lstRemarkedPix.at(i);
        tmpPix.waveToPixeRatio  = waveToPixeRatio;
        plotPixel( tmpPix, yRealArea );
    }

}

void formHypercubeAnalysis::plotPixel(const strlstRemarkedPix &remarkedPix, const int &boundArea)
{
    //-----------------------------------------------------------
    //Plot Spectral Signature
    //-----------------------------------------------------------
    int l;
    float initWave  = slideHypCube->cubeParam.initWavelength;
    float specRes   = slideHypCube->cubeParam.spectralRes;
    float tmpWave;
    int xPos1, yPos1, xPos2, yPos2;
    QPen tmpPenSig;
    tmpPenSig.setBrush(QBrush(remarkedPix.color));
    tmpPenSig.setWidth(1);
    float tmpVal;
    for(l=0; l<slideHypCube->cubeParam.L-1; l++)
    {
        //Actual Point
        tmpWave     = initWave + (specRes*l);
        xPos1       = plotSettings.XFrame + round( remarkedPix.waveToPixeRatio * tmpWave );
        tmpVal      = (float)slideHypCube->HypCube[remarkedPix.x][remarkedPix.y][l];
        if( tmpVal > 0 )yPos1 = round(( tmpVal / 255.0) * (float)boundArea);
        else yPos1 = 0;
        yPos1       = boundArea - yPos1;
        //Next Point
        tmpWave     = initWave + (specRes*(l+1));
        xPos2       = plotSettings.XFrame + round( remarkedPix.waveToPixeRatio * tmpWave );
        tmpVal      = (float)slideHypCube->HypCube[remarkedPix.x][remarkedPix.y][l+1];
        if(tmpVal>0)yPos2 = round((tmpVal / 255.0) * (float)boundArea);
        else yPos2  = 0;
        yPos2       = boundArea - yPos2;
        //Add Line
        gvPlot->scene()->addLine(xPos1,yPos1,xPos2,yPos2,tmpPenSig);
    }

    //-----------------------------------------------------------
    //Plot Mouse Position
    //-----------------------------------------------------------
    /*
    int recW = 5;
    gvImg->scene()->addRect(
                                remarkedPix.thumbX-recW,
                                remarkedPix.thumbY-recW,
                                (recW*2),
                                (recW*2),
                                QPen(remarkedPix.color)
                            );*/

}

void formHypercubeAnalysis::on_slideTmpImg_valueChanged(const int &value)
{
    updateLabel(value);
    updateSlidePicture(value);
}

void formHypercubeAnalysis::updateSlidePicture(const int &l)
{    
    gvImg->scene()->clear();
    gvImg->scene()->addPixmap(QPixmap::fromImage(slideHypCube->lstCubeThumbs.at(l)));
    gvImg->update();
    QtDelay(15);
}

void formHypercubeAnalysis::updateLabel(const int &value)
{
    float tmpWavelen;
    tmpWavelen = slideHypCube->cubeParam.initWavelength + ( slideHypCube->cubeParam.spectralRes * value);
    ui->labelImgNumber->setText( "Wavelength: " + QString::number(tmpWavelen) + " nm" );
}

void formHypercubeAnalysis::on_pbCubeToImg_clicked()
{
    formSlidePlotSettings* tmpForm = new formSlidePlotSettings(this);
    tmpForm->setModal(true);
    tmpForm->show();
}

void formHypercubeAnalysis::on_pbLoadCube_clicked()
{
    //------------------------------------------------
    //Load Hypercube
    //------------------------------------------------
    connect(
                slideHypCube,
                SIGNAL(signalProgBarValue(int,QString)),
                this,
                SLOT(updateProgressBar(int,QString))
           );

    //Load Hypercube
    if( slideHypCube->loadHypercube(this) != _OK )
    {
        return (void)false;
    }
    ui->slideTmpImg->setMaximum( slideHypCube->cubeParam.L-1 );
    ui->slideTmpImg->setMinimum( 0 );
    updateProgressBar(101,"");
    updateLabel(0);

    //---------------------------------------------------------
    //Create Layout and Set Image into GV
    //---------------------------------------------------------
    int frameH = 105;
    //Add to layout
    QFormLayout* tmpLayout = new QFormLayout(this);

    QImage tmpImg = slideHypCube->slideImgFromCube(20);
    if( 0 )
    {
        std::cout << "imgW: " << tmpImg.width() << " imgH: " << tmpImg.height() << std::endl;
        displayImageFullScreen(tmpImg);
    }

    //Add a Vertical spacer
    QSpacerItem* spacer = new QSpacerItem(tmpImg.width(), frameH, QSizePolicy::Minimum, QSizePolicy::Expanding);
    tmpLayout->addItem(spacer);

    tmpLayout->addWidget(gvImg);

    if( this->layout()->isEmpty() )
    {
        this->setLayout(tmpLayout);
    }

    //Put Image into GV    
    QGraphicsScene* imgScene = new QGraphicsScene(this);
    gvImg->setScene(imgScene);
    gvImg->scene()->addPixmap(QPixmap::fromImage(tmpImg));
    gvImg->setGeometry(QRect(0,0,tmpImg.width(),tmpImg.height()));
    gvImg->setMouseTracking(true);
    gvImg->setVisible(true);
    gvImg->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvImg->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvImg->move(10,frameH);

    //---------------------------------------------------------
    //Create Layout and Set Image into GV
    //---------------------------------------------------------
    int frameH2 = 30;
    //Add to layout
    QRect screenRes = screenResolution(parentHypercubeAnalysis);
    //QFormLayout* tmpLayout2 = new QFormLayout(this);
    tmpLayout->addWidget(gvPlot);
    //this->setLayout(tmpLayout2);
    QGraphicsScene* imgScene2 = new QGraphicsScene(this);
    gvPlot->setScene(imgScene2);
    gvPlot->scene()->setBackgroundBrush(QBrush(QColor(45,45,45)));
    gvPlot->setGeometry(QRect(0,0,tmpImg.width(),round(screenRes.height()*0.45)));
    gvPlot->setMouseTracking(true);
    gvPlot->setVisible(true);
    gvPlot->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvPlot->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    gvPlot->move(10,frameH+frameH2+tmpImg.height());

    ui->slideTmpImg->setEnabled(true);

    //---------------------------------------------------------
    //Reload Settings
    //---------------------------------------------------------
    formSlidePlotSettings tmpFormSettings;
    tmpFormSettings.readPlotSettings(&plotSettings);

    //---------------------------------------------------------
    //Connect
    //---------------------------------------------------------
    connect(
                gvImg,
                SIGNAL(signalMouseMove(QMouseEvent*)),
                this,
                SLOT(updateSignature(QMouseEvent*))
           );
    ui->pbLoadCube->setEnabled(false);

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

/*
void formHypercubeAnalysis::resizeEvent( QResizeEvent * event )
{
    event = event;
    std::cout << "ResizeEvent" << std::endl;
}*/


void formHypercubeAnalysis::on_pbReset_clicked()
{
    ui->slideTmpImg->setValue(0);
    lstRemarkedPix.clear();
    gvPlot->scene()->clear();
    QMouseEvent eve(
                        QEvent::MouseMove,
                        QPoint(
                                    round((float)gvImg->width()*0.5),
                                    round((float)gvImg->height()*0.5)
                              ),
                        Qt::NoButton,
                        Qt::NoButton,
                        Qt::NoModifier
                   );
    updateSignature(&eve);
}
