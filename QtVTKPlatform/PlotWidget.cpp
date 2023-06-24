#include "PlotWidget.h"

#include "qevent.h"
#include "qdebug.h"

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>

#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>

#include <vtkCamera.h>
#include <vtkNamedColors.h>
#include <vtkCellType.h>
#include <vtkPoints.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

#include "Vertex.h"
#include "Element.h"
#include "FEMInfo.h"

#include "CusInteractorStyle.h"

PlotWidget::PlotWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent), m_iFIFlag(0)
{
    auto colors = vtkSmartPointer<vtkNamedColors> ::New();

    vtkNew<QVTKInteractor> iren;

    iren->SetInteractorStyle(style);
    iren->SetRenderWindow(win);
    iren->Enable();

    SetRenderWindow(win);

    camera->SetViewUp(0, 1, 0);
    camera->SetPosition(0, 0, 10);
    camera->SetFocalPoint(0, 0, 0);

    renderer->SetActiveCamera(camera);
    renderer->GradientBackgroundOn();
    renderer->SetBackground(colors->GetColor3d("RoyalBlue").GetData());
    renderer->SetBackground2(colors->GetColor3d("MistyRose").GetData());

    //DrawAxes();

    //renderWindowInteractor->SetRenderWindow(win);
    //renderWindowInteractor->Initialize();

    win->AddRenderer(renderer);
}

void PlotWidget::UpdateFEMInfo(MK::FEMInfo* pFI)
{
    m_pFI = pFI;
    m_iFIFlag = 1;

    DrawModel();
}

void PlotWidget::onLeftView()
{
    auto camera = renderer->GetActiveCamera();
    double* fp = camera->GetFocalPoint();

    //camera->SetFocalPoint(fp[0], fp[1], fp[2]);

    double upvec[3] = { 0,1,0 };
    double posvec[3] = { -1,0,0 };

    for (int i = 0; i < 3; i++) {
        //upvec[i] += fp[i];
        posvec[i] += fp[i];
    }

    camera->SetViewUp(upvec);
    camera->SetPosition(posvec);
    
    renderer->ResetCamera();
    win->Render();
}

void PlotWidget::onRightView()
{
    auto camera = renderer->GetActiveCamera();
    double* fp = camera->GetFocalPoint();

    //camera->SetFocalPoint(fp[0], fp[1], fp[2]);

    double upvec[3] = { 0,1,0 };
    double posvec[3] = { 1,0,0 };

    for (int i = 0; i < 3; i++) {
        //upvec[i] += fp[i];
        posvec[i] += fp[i];
    }

    camera->SetViewUp(upvec);
    camera->SetPosition(posvec);

    renderer->ResetCamera();
    win->Render();
}

void PlotWidget::onUpView()
{
}

void PlotWidget::onDownView()
{
}

void PlotWidget::onFrontView()
{
}

void PlotWidget::onBackView()
{
}

void PlotWidget::onFitSize()
{
    renderer->ResetCamera();
    win->Render();
}

int PlotWidget::DrawModel()
{
    vtkNew<vtkPoints> points;
    vtkNew<vtkUnstructuredGrid> ugrid;
    vtkNew<vtkDataSetMapper> ugridMapper;

    auto colors = vtkSmartPointer<vtkNamedColors> ::New();

    // points

    for (auto it = m_pFI->GetVerMapBegin();
        it != m_pFI->GetVerMapEnd(); it++) {

        MK::Vertex* ver = it->second;
        size_t giid = ver->Getiid();

        double* cord = ver->GetCoord().data();

        points->InsertPoint(giid, cord);
    }

    //T3
    for (auto it = m_pFI->GetT3VectorBegin();
        it != m_pFI->GetT3VectorEnd();
        it++) {

        long long conn[3] = {};

        for (int i = 0; i < 3; i++) {
            MK::Vertex* ver = (*it)->GetVertexInEle(i);
            size_t giid = ver->Getiid();
            conn[i] = giid;
        }

        ugrid->InsertNextCell(VTK_TRIANGLE, 3, conn);
    }

    // Q4
    for (auto it = m_pFI->GetQ4VectorBegin();
        it != m_pFI->GetQ4VectorEnd();
        it++) {

        long long conn[4] = {};

        for (int i = 0; i < 4; i++) {
            MK::Vertex* ver = (*it)->GetVertexInEle(i);
            size_t giid = ver->Getiid();
            conn[i] = giid;
        }

        ugrid->InsertNextCell(VTK_QUAD, 4, conn);
    }

    
    ugrid->SetPoints(points);
    
    ugridMapper->SetInputData(ugrid);

    ugridActor->SetMapper(ugridMapper);
    ugridActor->GetProperty()->EdgeVisibilityOn();

    renderer->AddActor(ugridActor);
    renderer->ResetCamera();

    // build sphere attached to points
    vtkNew<vtkSphereSource> sphereSource;
    vtkNew<vtkUnstructuredGrid> pts;
    vtkNew<vtkGlyph3D> glyph3d;
    vtkNew<vtkDataSetMapper> ptmapper;

    pts->SetPoints(points);
    glyph3d->SetInputData(pts);
    glyph3d->SetSourceConnection(sphereSource->GetOutputPort());
    glyph3d->SetScaleFactor(0.25);
    ptmapper->SetInputConnection(glyph3d->GetOutputPort());

    ptactor->SetMapper(ptmapper);
    ptactor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());

    //renderer->AddActor(ptactor);

    win->Render();
    return EXIT_SUCCESS;
}

void PlotWidget::onShowPoint(bool b) {

    if (b) {
        renderer->AddActor(ptactor);
    }
    else {
        renderer->RemoveActor(ptactor);
    }

    win->Render();
}

void PlotWidget::onDefaultBtn(bool b) {
    if (b) {
        style->DisableMove();
        style->DisableRotation();
    }
}

void PlotWidget::onMoveBtn(bool b)
{
    if (b) {
        style->EnableMove();
    }
    else {
        style->DisableMove();
    }
}

void PlotWidget::onRotateBtn(bool b)
{
    if (b) {
        style->EnableRotation();
    }
    else {
        style->DisableRotation();
    }
}

int PlotWidget::DrawAxes()
{
    vtkNew<vtkAxesActor> axes;
    //vtkNew<vtkOrientationMarkerWidget> widget;
    //widget->SetOrientationMarker(axes);
    // // widget->SetInteractor(renderWindowInteractor);
    //widget->SetViewport(0.0, 0.0, 0.4, 0.4);

    renderer->AddActor(axes);
    return 0;
}

void PlotWidget::closeEvent(QCloseEvent* event)
{
    event->ignore();
}
