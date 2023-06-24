#pragma once

#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkInteractorStyleTrackballCamera.h>

class CusInteractorStyle :
    public vtkInteractorStyleTrackballCamera
{
public:
    static CusInteractorStyle* New();
    vtkTypeMacro(CusInteractorStyle, vtkInteractorStyleTrackballCamera);

    void OnLeftButtonDown() override;
    void OnRightButtonDown() override { }
    void OnMiddleButtonDown() override { }

    void EnableRotation() { m_bEnableRotation = true; }
    void DisableRotation() { m_bEnableRotation = false; }

    void EnableMove() { m_bEnableMove = true; }
    void DisableMove() { m_bEnableMove = false; }

    void EnableZoom() { m_bEnableZoom = true; }
    void DisableZoom() { m_bEnableZoom = false; }

protected:

    bool m_bEnableRotation = false;
    bool m_bEnableMove = false;
    bool m_bEnableZoom = false;

};

