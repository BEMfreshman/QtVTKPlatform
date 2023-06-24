#include "QtVTKPlatform.h"

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>

#include <vtkCamera.h>

#include <vtkNamedColors.h>

#include <vtkCellType.h>
#include <vtkPoints.h>

#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qmessagebox.h>

// bdf reader
#include "AbstractFileReader.h"
//#include "NASTRANFileReader.h"

#include "FileReader.h"

#include "FEMInfo.h"
#include "Vertex.h"
#include "MatProp.h"
#include "EleProp.h"
#include "Element.h"

#include "UtilityFun.h"
    
QtVTKPlatform::QtVTKPlatform(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_pReader = new FileReader();

    InitWatcher();
    InitLayout();
    InitConnection();
}

QtVTKPlatform::~QtVTKPlatform()
{

    m_pReadFileWatcher->cancel();
    m_pReadFileWatcher->waitForFinished();
}

void QtVTKPlatform::InitLayout()
{
    m_pMdiAreaWidget = new QMdiArea(this);
    this->setCentralWidget(m_pMdiAreaWidget);

    m_pPlotWidget = new PlotWidget(this);
    m_pPlotWidget->setAttribute(Qt::WA_DeleteOnClose);

    m_pMessageDockWidget = new MessageDockWidget(this);
    m_pMessageDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    m_pMessageDockWidget->setWindowTitle(QString::fromLocal8Bit("Message"));
    m_pMessageDockWidget->setAcceptDrops(false);
    m_pMessageDockWidget->setAttribute(Qt::WA_DeleteOnClose);

    m_pTableDockWidget = new TableDockWidget(this);
    m_pTableDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    m_pTableDockWidget->setWindowTitle(QString::fromLocal8Bit("Table"));
    m_pTableDockWidget->setAcceptDrops(false);
    m_pTableDockWidget->setAttribute(Qt::WA_DeleteOnClose);

    addDockWidget(Qt::BottomDockWidgetArea, m_pMessageDockWidget);
    addDockWidget(Qt::BottomDockWidgetArea, m_pTableDockWidget);

    tabifyDockWidget(m_pMessageDockWidget, m_pTableDockWidget);

    m_pMdiAreaWidget->addSubWindow(m_pPlotWidget);
    m_pPlotWidget->showMaximized();

    CreateStatusBar();
    CreateMenuBar();
    CreateToolBar();
    CreateProgressBar();

    showMaximized();

}

void QtVTKPlatform::InitWatcher()
{
    m_pReadFileWatcher = new QFutureWatcher<int>(this);
}

void QtVTKPlatform::CreateStatusBar()
{
    m_pStatusBar = new QStatusBar(this);
    //m_pStatusBar->setFixedHeight(40);
    this->setStatusBar(m_pStatusBar);
}

void QtVTKPlatform::CreateMenuBar()
{
    m_pMenuBar = new QMenuBar(this);

    m_pFileMenu = new QMenu(m_pMenuBar);
    m_pFileMenu->setTitle(QString::fromLocal8Bit("File"));

    m_pNewFile = new QAction(tr("New"), m_pFileMenu);
    m_pNewFile->setStatusTip(QString::fromLocal8Bit("Install New"));

    m_pOpenFile = new QAction(tr("Open"), m_pFileMenu);
    m_pOpenFile->setStatusTip(QString::fromLocal8Bit("Open Model File"));

    m_pOpenRstFile = new QAction(tr("Open Rst"), m_pFileMenu);
    m_pOpenRstFile->setStatusTip(QString::fromLocal8Bit("Open Result File"));
    
    m_pFileMenu->addAction(m_pNewFile);
    m_pFileMenu->addAction(m_pOpenFile);
    m_pFileMenu->addAction(m_pOpenRstFile);

    m_pMenuBar->addMenu(m_pFileMenu);

    setMenuBar(m_pMenuBar);
}

void QtVTKPlatform::CreateToolBar()
{
    m_pViewToolBar = new QToolBar(this);
    this->addToolBar(Qt::RightToolBarArea, m_pViewToolBar);

    m_pSelectToolBar = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, m_pSelectToolBar);

    m_pActGroup = new QActionGroup(this);
    m_pActGroup->setExclusive(true);

    m_pDefault = new QAction(m_pViewToolBar);
    m_pDefault->setCheckable(true);
    m_pDefault->setText(QString::fromLocal8Bit("Select"));
    m_pDefault->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/MouseDefault.png"));

    m_pMove = new QAction(m_pViewToolBar);
    m_pMove->setCheckable(true);
    m_pMove->setText(QString::fromLocal8Bit("Move"));
    m_pMove->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/start.png"));

    m_pRotate = new QAction(m_pViewToolBar);
    m_pRotate->setCheckable(true);
    m_pRotate->setText(QString::fromLocal8Bit("Rotate"));
    m_pRotate->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/start.png"));

    m_pShowPoint = new QAction(m_pSelectToolBar);
    m_pShowPoint->setCheckable(true);
    m_pShowPoint->setText(QString::fromLocal8Bit("Show Point"));
    m_pShowPoint->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/ShowPoint.png"));

    m_pPickPoint = new QAction(m_pSelectToolBar);
    m_pPickPoint->setText(QString::fromLocal8Bit("Pick Point"));
    m_pPickPoint->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/pickPoint.png"));

    m_pActGroup->addAction(m_pDefault);
    m_pActGroup->addAction(m_pMove);
    m_pActGroup->addAction(m_pRotate);

    m_pFitSize = new QAction(m_pViewToolBar);
    m_pFitSize->setText(QString::fromLocal8Bit("Fit"));
    m_pFitSize->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/FitSize.png"));

    m_pLeftView = new QAction(m_pViewToolBar);
    m_pLeftView->setText(QString::fromLocal8Bit("Left"));
    m_pLeftView->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/Left.png"));

    m_pRightView = new QAction(m_pViewToolBar);
    m_pRightView->setText(QString::fromLocal8Bit("Right"));
    m_pRightView->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/Right.png"));

    m_pUpView = new QAction(m_pViewToolBar);
    m_pUpView->setText(QString::fromLocal8Bit("Up"));
    m_pUpView->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/Up.png"));

    m_pDownView = new QAction(m_pViewToolBar);
    m_pDownView->setText(QString::fromLocal8Bit("Down"));
    m_pDownView->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/Down.png"));

    m_pFrontView = new QAction(m_pViewToolBar);
    m_pFrontView->setText(QString::fromLocal8Bit("Front"));
    m_pFrontView->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/Front.png"));

    m_pBackView = new QAction(m_pViewToolBar);
    m_pBackView->setText(QString::fromLocal8Bit("Back"));
    m_pBackView->setIcon(QIcon(":/ViewToolBar/Icon/ViewToolBar/Back.png"));

    m_pViewToolBar->addAction(m_pDefault);
    m_pViewToolBar->addAction(m_pMove);
    m_pViewToolBar->addAction(m_pRotate);
    m_pViewToolBar->addSeparator();

    m_pViewToolBar->addAction(m_pFitSize);
    m_pViewToolBar->addSeparator();

    m_pViewToolBar->addAction(m_pLeftView);
    m_pViewToolBar->addAction(m_pRightView);

    m_pViewToolBar->addAction(m_pUpView);
    m_pViewToolBar->addAction(m_pDownView);

    m_pViewToolBar->addAction(m_pFrontView);
    m_pViewToolBar->addAction(m_pBackView);

    m_pSelectToolBar->addAction(m_pShowPoint);
    m_pSelectToolBar->addAction(m_pPickPoint);
}

void QtVTKPlatform::CreateProgressBar()
{
    m_pProgressBar = new QProgressBar(m_pStatusBar);
    m_pStatusBar->addPermanentWidget(m_pProgressBar);

    m_pProgressBar->setAlignment(Qt::AlignRight);
    m_pProgressBar->setOrientation(Qt::Horizontal);
    m_pProgressBar->setFixedWidth(150);
    m_pProgressBar->setMinimum(0);
    m_pProgressBar->setMaximum(0);
    m_pProgressBar->setVisible(false);

    m_pProgressBar->setStyleSheet("QProgressBar::chunk {"
        "background-color: #2196F3;"
        "width: 10px;"
        "margin: 0.5px;}"
    );
}

void QtVTKPlatform::InitConnection()
{
    connect(m_pOpenFile, SIGNAL(triggered()), this, SLOT(onOpenFile()));

    // select tool bar
    connect(m_pShowPoint, SIGNAL(toggled(bool)), m_pPlotWidget, SLOT(onShowPoint(bool)));

    connect(m_pDefault, SIGNAL(toggled(bool)), m_pPlotWidget, SLOT(onDefaultBtn(bool)));
    connect(m_pMove, SIGNAL(toggled(bool)), m_pPlotWidget, SLOT(onMoveBtn(bool)));
    connect(m_pRotate, SIGNAL(toggled(bool)), m_pPlotWidget, SLOT(onRotateBtn(bool)));
    
    // view action
    connect(m_pFitSize, SIGNAL(triggered()), m_pPlotWidget, SLOT(onFitSize()));
    
    connect(m_pLeftView, SIGNAL(triggered()), m_pPlotWidget, SLOT(onLeftView()));
    connect(m_pRightView, SIGNAL(triggered()), m_pPlotWidget, SLOT(onRightView()));

    connect(m_pUpView, SIGNAL(triggered()), m_pPlotWidget, SLOT(onUpView()));
    connect(m_pDownView, SIGNAL(triggered()), m_pPlotWidget, SLOT(onDownView()));

    connect(m_pFrontView, SIGNAL(triggered()), m_pPlotWidget, SLOT(onFrontView()));
    connect(m_pBackView, SIGNAL(triggered()), m_pPlotWidget, SLOT(onBackView()));


    connect(m_pReadFileWatcher, &QFutureWatcher<int>::started, this, [&]() {
        //qDebug() << "Read start";
        this->m_pProgressBar->setVisible(true);
    });

    connect(m_pReadFileWatcher, &QFutureWatcher<int>::finished, this, [&]() {
        //qDebug() << "Read end";
        HandleReadResult();
        this->m_pProgressBar->setVisible(false);
    });

}

void QtVTKPlatform::onNewFile() {

}

void QtVTKPlatform::onOpenFile() {
    
    QFileInfo fileinfo = QFileDialog::getOpenFileName(this,
        QDir::currentPath(), QDir::currentPath(),
        QString("NASTRAN Files(*.bdf);;"
            "Optistruct Files(*.fem);;"
            "Files(*.*)"));

    if (fileinfo.filePath().isEmpty()) return;

    QString filepath = fileinfo.filePath();

    if (m_pReadFileWatcher->isRunning()) {
        m_pReadFileWatcher->cancel();
        m_pReadFileWatcher->waitForFinished();
    }

    m_pReadFileWatcher->setFuture(QtConcurrent::run(m_pReader,
        &FileReader::Read, filepath.toStdString()));
}

void QtVTKPlatform::onOpenRstFile()
{
    /*QFileInfo fileinfo = QFileDialog::getOpenFileName(this,
        QDir::currentPath(), QDir::currentPath(),
        QString("Op2 Files(*.op2);;"));

    if (fileinfo.filePath().isEmpty()) return;

    QString filepath = fileinfo.filePath();*/

}

void QtVTKPlatform::HandleReadResult()
{

    int ret = m_pReadFileWatcher->result();

    if (ret != EXIT_SUCCESS) {

        QString msg = "Failed to read bdf file, exit !";
        QMessageBox box;
        box.critical(nullptr, "Error", msg);

        return;
    }

    if (m_pFI) {
        delete m_pFI;
        m_pFI = nullptr;
    }

    m_pFI = new MK::FEMInfo(m_pReader->GetReader());
    m_pReader->GetReader()->FillFEMInfo(m_pFI);

    m_pPlotWidget->UpdateFEMInfo(m_pFI);
}
