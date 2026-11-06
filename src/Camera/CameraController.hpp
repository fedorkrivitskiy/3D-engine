#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QPointF>
#include <QSet>
#include <QVector3D>

class Camera;
class QKeyEvent;
class QMouseEvent;

class CameraController : public QObject
{
    Q_OBJECT
  public:
    explicit CameraController(Camera *camera, QObject *parent = nullptr);

    void handleKeyPress(QKeyEvent *event);
    void handleKeyRelease(QKeyEvent *event);
    void handleMousePress(QMouseEvent *event);
    void handleMouseRelease(QMouseEvent *event);
    void handleMouseMove(QMouseEvent *event);

    void update();

  private:
    void updateMovement(float deltaTime);
    void updateRotation(float deltaTime);

    [[nodiscard]] QVector3D computeInputDirection() const;

  private:
    Camera *camera_{nullptr};
    QElapsedTimer deltaTimer_;

    QSet<int> pressedKeys_;
    QVector3D currentVelocity_{0.0f, 0.0f, 0.0f};

    bool isMouseLookActive_{false};
    QPointF lastMousePos_;
    QPointF pendingMouseDelta_{0.0f, 0.0f};

    // TODO: move to settings
    float movementSpeed_{10.0f};
    float acceleration_{15.0f};
    float mouseSensitivity_{0.002f};
    float rotationSmoothing_{25.0f};
};