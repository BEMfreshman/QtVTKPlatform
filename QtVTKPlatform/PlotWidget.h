#pragma once

#include <QWidget>
//#include <QVTKOpenGLWidget.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>

class vtkActor;
class vtkRenderer;
class vtkGenericOpenGLRenderWindow;
class vtkCamera;
class QVTKInteractor;
class CusInteractorStyle;

namespace MK {
    class FEMInfo;
}

class PlotWidget :
    public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    PlotWidget(QWidget* parent = nullptr);
    void UpdateFEMInfo(MK::FEMInfo* pFI);


public slots:

    void onShowPoint(bool);

    void onDefaultBtn(bool);
    void onMoveBtn(bool);
    void onRotateBtn(bool);

    void onFitSize();

    void onLeftView();
    void onRightView();

    void onUpView();
    void onDownView();

    void onFrontView();
    void onBackView();

protected:
    int DrawModel();
    int DrawAxes();


protected:
    void closeEvent(QCloseEvent* event) override;

protected:
    
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkGenericOpenGLRenderWindow> win;
    vtkNew<vtkCamera> camera;
    vtkNew<CusInteractorStyle> style;
    //vtkNew<QVTKInteractor> renderWindowInteractor;

    vtkNew<vtkActor> ugridActor;
    vtkNew<vtkActor> ptactor;

protected:
    int m_iFIFlag;
    MK::FEMInfo* m_pFI;

    //float m_fxmin;
    //float m_fxmax;
    //float m_fymin;
    //float m_fymax;
    //float m_fzmin;
    //float m_fzmax;

};

