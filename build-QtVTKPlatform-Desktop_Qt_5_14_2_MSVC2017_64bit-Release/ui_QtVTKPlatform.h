/********************************************************************************
** Form generated from reading UI file 'QtVTKPlatform.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTVTKPLATFORM_H
#define UI_QTVTKPLATFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtVTKPlatformClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtVTKPlatformClass)
    {
        if (QtVTKPlatformClass->objectName().isEmpty())
            QtVTKPlatformClass->setObjectName(QString::fromUtf8("QtVTKPlatformClass"));
        QtVTKPlatformClass->resize(600, 400);
        menuBar = new QMenuBar(QtVTKPlatformClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        QtVTKPlatformClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtVTKPlatformClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtVTKPlatformClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtVTKPlatformClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QtVTKPlatformClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtVTKPlatformClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtVTKPlatformClass->setStatusBar(statusBar);

        retranslateUi(QtVTKPlatformClass);

        QMetaObject::connectSlotsByName(QtVTKPlatformClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtVTKPlatformClass)
    {
        QtVTKPlatformClass->setWindowTitle(QCoreApplication::translate("QtVTKPlatformClass", "QtVTKPlatform", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtVTKPlatformClass: public Ui_QtVTKPlatformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTVTKPLATFORM_H
