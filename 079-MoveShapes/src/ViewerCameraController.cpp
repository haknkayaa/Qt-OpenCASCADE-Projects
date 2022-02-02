//
// Created by hakan on 6/14/21.
//

#include "ViewerCameraController.h"

#include <utility>


ViewerCameraController::ViewerCameraController(Handle_V3d_View view, QObject *parent)
        : QAbstractAnimation(parent),
          m_view(std::move(view)),
          m_cameraStart(new Graphic3d_Camera),
          m_cameraEnd(new Graphic3d_Camera) {
}

int ViewerCameraController::duration() const {
    return m_duration_ms;
}

void ViewerCameraController::setDuration(int msecs) {
    m_duration_ms = msecs;
}

void ViewerCameraController::setCameraStart(const Handle_Graphic3d_Camera &camera) {
    m_cameraStart->Copy(camera);
}

void ViewerCameraController::setCameraEnd(const Handle_Graphic3d_Camera &camera) {
    m_cameraEnd->Copy(camera);
}

const QEasingCurve &ViewerCameraController::easingCurve() const {
    return m_easingCurve;
}

void ViewerCameraController::setEasingCurve(const QEasingCurve &easingCurve) {
    m_easingCurve = easingCurve;
}

void ViewerCameraController::configure(const std::function<void(Handle_V3d_View)> &fnViewChange) {
    if (this->state() == QAbstractAnimation::Running)
        this->stop();

    const bool wasImmediateUpdateOn = m_view->SetImmediateUpdate(false);
    m_cameraStart->Copy(m_view->Camera());
    fnViewChange(m_view);
    m_cameraEnd->Copy(m_view->Camera());
    m_view->Camera()->Copy(m_cameraStart); // Restore
    m_view->SetImmediateUpdate(wasImmediateUpdateOn);
}

void ViewerCameraController::updateCurrentTime(int currentTime) {
    const double t = m_easingCurve.valueForProgress(currentTime / double(m_duration_ms));
    const bool prevImmediateUpdate = m_view->SetImmediateUpdate(false);
    const Graphic3d_CameraLerp cameraLerp(m_cameraStart, m_cameraEnd);
    Handle_Graphic3d_Camera camera = m_view->Camera();
    cameraLerp.Interpolate(t, camera);
    m_view->SetCamera(camera);
    m_view->ZFitAll();
    m_view->SetImmediateUpdate(prevImmediateUpdate);
    m_view->Update();
}


