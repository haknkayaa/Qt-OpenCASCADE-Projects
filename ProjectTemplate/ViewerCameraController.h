
#ifndef  VIEWERCAMERACONTROLLER_H
#define  VIEWERCAMERACONTROLLER_H

#include  <V3d_View.hxx>
#include  <QtCore/QAbstractAnimation>
#include  <QtCore/QEasingCurve>
#include  <functional>

class ViewerCameraController : public QAbstractAnimation {
public:
    ViewerCameraController(Handle_V3d_View view, QObject *parent = nullptr);

    int duration() const override;

    void setDuration(int msecs);

    void setCameraStart(const Handle_Graphic3d_Camera &camera);

    void setCameraEnd(const Handle_Graphic3d_Camera &camera);

    const QEasingCurve &easingCurve() const;

    void setEasingCurve(const QEasingCurve &easingCurve);

    void configure(const std::function<void(Handle_V3d_View)> &fnViewChange);

protected:
    void updateCurrentTime(int currentTime) override;

private:
    Handle_V3d_View m_view;
    Handle_Graphic3d_Camera m_cameraStart;
    Handle_Graphic3d_Camera m_cameraEnd;
    QEasingCurve m_easingCurve; // Linear by default
    int m_duration_ms = 1000;
};


#endif //PROJECT_VIEWERCAMERACONTROLLER_H
