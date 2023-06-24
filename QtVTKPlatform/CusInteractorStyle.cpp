#include "CusInteractorStyle.h"

vtkStandardNewMacro(CusInteractorStyle);

void CusInteractorStyle::OnLeftButtonDown()
{
	if (m_bEnableRotation) {
		// original left button
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
	else if (m_bEnableMove) {
		// original middle button
		vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
	}
	else if (m_bEnableZoom) {
		// original right button
		vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	}
}
