/********************************************************************************
** Form generated from reading UI file 'showancalchrres.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWANCALCHRRES_H
#define UI_SHOWANCALCHRRES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_showAnCalChrRes
{
public:
    QGraphicsView *canvasCroped;
    QFrame *frame;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QCheckBox *chbBlue;
    QPushButton *pbSaveAnalysis;
    QPushButton *pbClearCalib;
    QCheckBox *chbGreen;
    QCheckBox *chbRed;
    QLineEdit *txtQuadFilename;
    QLabel *label;
    QCheckBox *chbRedLine;
    QCheckBox *chbGreenLine;
    QCheckBox *chbBlueLine;
    QPushButton *pbSaveScene;
    QPushButton *pbCloseThis;

    void setupUi(QDialog *showAnCalChrRes)
    {
        if (showAnCalChrRes->objectName().isEmpty())
            showAnCalChrRes->setObjectName(QStringLiteral("showAnCalChrRes"));
        showAnCalChrRes->resize(507, 407);
        canvasCroped = new QGraphicsView(showAnCalChrRes);
        canvasCroped->setObjectName(QStringLiteral("canvasCroped"));
        canvasCroped->setGeometry(QRect(0, 0, 511, 211));
        frame = new QFrame(showAnCalChrRes);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(14, 240, 401, 151));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(6, 3, 386, 142));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        chbBlue = new QCheckBox(layoutWidget);
        chbBlue->setObjectName(QStringLiteral("chbBlue"));
        chbBlue->setChecked(true);

        gridLayout->addWidget(chbBlue, 2, 2, 1, 1);

        pbSaveAnalysis = new QPushButton(layoutWidget);
        pbSaveAnalysis->setObjectName(QStringLiteral("pbSaveAnalysis"));
        pbSaveAnalysis->setEnabled(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/icons/imagenInte/add-484.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbSaveAnalysis->setIcon(icon);

        gridLayout->addWidget(pbSaveAnalysis, 2, 3, 1, 1);

        pbClearCalib = new QPushButton(layoutWidget);
        pbClearCalib->setObjectName(QStringLiteral("pbClearCalib"));
        pbClearCalib->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/icons/imagenInte/edit-clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbClearCalib->setIcon(icon1);

        gridLayout->addWidget(pbClearCalib, 2, 4, 1, 1);

        chbGreen = new QCheckBox(layoutWidget);
        chbGreen->setObjectName(QStringLiteral("chbGreen"));
        chbGreen->setChecked(true);

        gridLayout->addWidget(chbGreen, 2, 1, 1, 1);

        chbRed = new QCheckBox(layoutWidget);
        chbRed->setObjectName(QStringLiteral("chbRed"));
        chbRed->setChecked(true);

        gridLayout->addWidget(chbRed, 2, 0, 1, 1);

        txtQuadFilename = new QLineEdit(layoutWidget);
        txtQuadFilename->setObjectName(QStringLiteral("txtQuadFilename"));

        gridLayout->addWidget(txtQuadFilename, 1, 0, 1, 5);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 5);

        chbRedLine = new QCheckBox(layoutWidget);
        chbRedLine->setObjectName(QStringLiteral("chbRedLine"));
        chbRedLine->setChecked(true);

        gridLayout->addWidget(chbRedLine, 3, 0, 1, 1);

        chbGreenLine = new QCheckBox(layoutWidget);
        chbGreenLine->setObjectName(QStringLiteral("chbGreenLine"));
        chbGreenLine->setChecked(true);

        gridLayout->addWidget(chbGreenLine, 3, 1, 1, 1);

        chbBlueLine = new QCheckBox(layoutWidget);
        chbBlueLine->setObjectName(QStringLiteral("chbBlueLine"));
        chbBlueLine->setChecked(true);

        gridLayout->addWidget(chbBlueLine, 3, 2, 1, 1);

        pbSaveScene = new QPushButton(layoutWidget);
        pbSaveScene->setObjectName(QStringLiteral("pbSaveScene"));
        pbSaveScene->setEnabled(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/icons/imagenInte/guardar.gif"), QSize(), QIcon::Normal, QIcon::Off);
        pbSaveScene->setIcon(icon2);

        gridLayout->addWidget(pbSaveScene, 3, 3, 1, 1);

        pbCloseThis = new QPushButton(showAnCalChrRes);
        pbCloseThis->setObjectName(QStringLiteral("pbCloseThis"));
        pbCloseThis->setEnabled(true);
        pbCloseThis->setGeometry(QRect(0, 0, 21, 21));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/new/icons/imagenInte/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbCloseThis->setIcon(icon3);
        pbCloseThis->setIconSize(QSize(18, 18));

        retranslateUi(showAnCalChrRes);

        QMetaObject::connectSlotsByName(showAnCalChrRes);
    } // setupUi

    void retranslateUi(QDialog *showAnCalChrRes)
    {
        showAnCalChrRes->setWindowTitle(QApplication::translate("showAnCalChrRes", "Analysis results", Q_NULLPTR));
        chbBlue->setText(QApplication::translate("showAnCalChrRes", "Blue", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        pbSaveAnalysis->setToolTip(QApplication::translate("showAnCalChrRes", "<html><head/><body><p>Save parameters into XML</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pbSaveAnalysis->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbClearCalib->setToolTip(QApplication::translate("showAnCalChrRes", "<html><head/><body><p>Clear all calibration settings</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pbClearCalib->setText(QString());
        chbGreen->setText(QApplication::translate("showAnCalChrRes", "Green", Q_NULLPTR));
        chbRed->setText(QApplication::translate("showAnCalChrRes", "Red", Q_NULLPTR));
        label->setText(QApplication::translate("showAnCalChrRes", "<html><head/><body><p align=\"center\"><span style=\" font-weight:600;\">Quadrant file-name</span></p></body></html>", Q_NULLPTR));
        chbRedLine->setText(QApplication::translate("showAnCalChrRes", "Red line", Q_NULLPTR));
        chbGreenLine->setText(QApplication::translate("showAnCalChrRes", "Green line", Q_NULLPTR));
        chbBlueLine->setText(QApplication::translate("showAnCalChrRes", "Blue line", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        pbSaveScene->setToolTip(QApplication::translate("showAnCalChrRes", "<html><head/><body><p>Save Graphic-view</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pbSaveScene->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbCloseThis->setToolTip(QApplication::translate("showAnCalChrRes", "<html><head/><body><p>Close</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pbCloseThis->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class showAnCalChrRes: public Ui_showAnCalChrRes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWANCALCHRRES_H
