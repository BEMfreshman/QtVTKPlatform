#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtVTKPlatform.h"

#include <QMenu>
#include <QAction>
#include <qactiongroup.h>

#include <QMenubar>
#include <QStatusbar>
#include <qprogressbar.h>

#include <QMdiArea>

#include "PlotWidget.h"
#include "MessageDockWidget.h"
#include "TableDockWidget.h"

#include <qfuture.h>
#include <QtConcurrent/QtConcurrent>

namespace MK {
    class FEMInfo;
};

class FileReader;

class QtVTKPlatform : public QMainWindow
{
    Q_OBJECT

public:
    QtVTKPlatform(QWidget *parent = nullptr);
    ~QtVTKPlatform();

protected:

    void InitLayout();
    void InitWatcher();

    void CreateStatusBar();
    void CreateMenuBar();
    void CreateToolBar();
    void CreateProgressBar();

protected:

    void InitConnection();

protected slots:
    void onNewFile();
    void onOpenFile();
    void onOpenRstFile();

protected:
    void HandleReadResult();

protected:

    QMdiArea* m_pMdiAreaWidget;
    PlotWidget* m_pPlotWidget;
    MessageDockWidget* m_pMessageDockWidget;
    TableDockWidget* m_pTableDockWidget;


    QMenuBar* m_pMenuBar{ nullptr };
    QStatusBar* m_pStatusBar{ nullptr };
    QToolBar* m_pSelectToolBar{ nullptr };
    QToolBar* m_pViewToolBar{ nullptr };
    QProgressBar* m_pProgressBar{ nullptr };

    QMenu* m_pFileMenu;
    QAction* m_pNewFile;
    QAction* m_pOpenFile;
    QAction* m_pOpenRstFile;

    // QAction for select tool bar
    QAction* m_pShowPoint;
    QAction* m_pPickPoint;


    // QAction for view tool bar
    QActionGroup* m_pActGroup;
    QAction* m_pDefault;
    QAction* m_pMove;
    QAction* m_pRotate;

    QAction* m_pFitSize;
    QAction* m_pLeftView;
    QAction* m_pRightView;
    QAction* m_pUpView;
    QAction* m_pDownView;
    QAction* m_pFrontView;
    QAction* m_pBackView;


    // FileReader
    FileReader* m_pReader{ nullptr };

    // bdf model
    MK::FEMInfo* m_pFI{nullptr};


    // watcher
    QFutureWatcher<int> *m_pReadFileWatcher{nullptr};

private:
    Ui::QtVTKPlatformClass ui;
};
