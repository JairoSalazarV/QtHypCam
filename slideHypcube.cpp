#include <__common.h>

#include <slideHypcube.h>
#include <iostream>
#include <fstream>
#include <QDir>

slideHypcube::slideHypcube(QObject *parent) : QDialog(){
    this->setMouseTracking(true);
}

int slideHypcube::loadHypercube(QWidget* parent)
{
    //-------------------------------------------------
    //Get cube location
    //-------------------------------------------------
    //if( funcLetUserSelectDirectory( _PATH_LAST_LOCAL_FOLDER, &dirPath ) != _OK )
    QString filePath;
    if( funcLetUserSelectFile( &filePath, "Select Hypercube File (use to be .hypercube)...", parent ) != _OK )
    {
        return _FAILURE;
    }

    QFileInfo tmpFileSel(filePath);
    dirPath = tmpFileSel.absolutePath() + "/";


    mouseCursorWait();

    emit signalProgBarValue(0,"Loading Hypercube into Memory...");


    //-------------------------------------------------
    //Read Hypercube Parameters
    //-------------------------------------------------
    if( readHypCubeParameters( dirPath+"info.xml", &cubeParam ) != _OK )
    {
        funcShowMsgERROR("Reading Hypercube Parameters");
        return _ERROR;
    }

    //-------------------------------------------------
    //Put Cube into Memory
    //-------------------------------------------------
    std::cout << "W: " << cubeParam.W << std::endl;
    std::cout << "H: " << cubeParam.H << std::endl;
    std::cout << "L: " << cubeParam.L << std::endl;
    std::cout << "initWave: " << cubeParam.initWavelength << std::endl;
    std::cout << "specRes: " << cubeParam.spectralRes << std::endl;
    //Reserve Memory
    int x, y;
    HypCube = (u_int8_t***)malloc( cubeParam.W * sizeof(u_int8_t**) );
    int percentage = 0;
    for( x=0; x<cubeParam.W; x++ )
    {
        HypCube[x] = (u_int8_t**)malloc( cubeParam.H * sizeof(u_int8_t*) );
        for( y=0; y<cubeParam.H; y++ )
        {
            HypCube[x][y] = (u_int8_t*)malloc( cubeParam.L * sizeof(u_int8_t) );
        }
    }
    //Read Hypercube
    QString cubeFilename = dirPath+"slideHypcube.hypercube";
    std::ifstream binCubeFile;
    binCubeFile.open(
                        cubeFilename.toStdString(),
                        std::ios::out | std::ios::app | std::ios::binary
                    );
    for( x=0; x<cubeParam.W; x++ )
    {
        for( y=0; y<cubeParam.H; y++ )
        {
            //binCubeFile.read( (std::basic_istream<char>::char_type*)&HypCube[x][y][0], cubeParam.L );
            binCubeFile.read( (std::basic_istream<char>::char_type*)&HypCube[x][y][0], cubeParam.L );
        }
        //Update progress bar
        percentage = round( ((float)(x+1) / (float)cubeParam.W) * 100.0 );
        emit signalProgBarValue(percentage,"Loading Hypercube into Memory...");
    }

    //-------------------------------------------------
    //Save Thumbs into HDD
    //-------------------------------------------------

    //Check if folder exists
    QString tmpFileFolder;
    tmpFileFolder.clear();
    tmpFileFolder.append( tmpFileSel.absoluteFilePath() );
    tmpFileFolder.replace(".hypercube","");
    tmpFileFolder.append("/");
    QDir tmpDir(tmpFileFolder);
    if( !tmpDir.exists() )
    {
        tmpDir.mkdir(tmpFileFolder);
    }
    //std::cout << "(int)tmpDir.count() " << (int)tmpDir.count() << std::endl;
    if( ( (int)tmpDir.count() - 2 ) != cubeParam.L )
    {
        //Cube to PNG
        emit signalProgBarValue(0,"Extracting Thumbs...");
        lstCubeThumbs.clear();
        int l;
        for( l=0; l<cubeParam.L; l++ )
        {
            lstCubeThumbs.append( slideImgFromCube(l) );
            lstCubeThumbs.at(l).save(tmpFileFolder +  QString::number(l) + ".png");
            percentage = round( ((float)(l+1) / (float)cubeParam.L) * 100.0 );
            emit signalProgBarValue(percentage,"Extracting Thumbs...");
        }
        emit signalProgBarValue(101,"");
    }
    else
    {
        emit signalProgBarValue(0,"Loading Thumbs...");
        lstCubeThumbs.clear();
        int l;
        for( l=0; l<cubeParam.L; l++ )
        {
            lstCubeThumbs.append( QImage( tmpFileFolder + QString::number(l) + ".png" ) );
            emit signalProgBarValue(percentage,"Loading Thumbs...");
        }
        emit signalProgBarValue(101,"");
    }




    cubeStatus = cubeLoaded;

    mouseCursorReset();

    return _OK;
}

QImage slideHypcube::slideImgFromCube( const int &l )
{
    //-------------------------------------------------
    //From Hypercube to Image
    //-------------------------------------------------
    QImage tmpImg( cubeParam.W, cubeParam.H, QImage::Format_Grayscale8 );
    int x, y;
    for( x=0; x< cubeParam.W; x++ )
    {
        for( y=0; y<cubeParam.H; y++ )
        {
            tmpImg.setPixelColor( x, y, QColor(
                                                    HypCube[x][y][l],
                                                    HypCube[x][y][l],
                                                    HypCube[x][y][l]
                                               )
                                );
            //std::cout << x << ", " << y << " -> " << HypCube[x][y][l] << std::endl;
        }
    }

    //-------------------------------------------------
    //Calculate Thumb Size
    //-------------------------------------------------
    int maxImgW, maxImgH, frameW;
    frameW = 20;
    QRect screenRes = screenResolution(this);
    maxImgW = screenRes.width() - frameW;
    maxImgH = round( (float)screenRes.height() / 2.0 );
    QImage thumbImg = tmpImg.scaled( maxImgW, maxImgH, Qt::KeepAspectRatio );

    //Return
    return thumbImg;

}
