#include "CameraController.hpp"
#include "Camera.hpp"

#include <QKeyEvent>
#include <QMouseEvent>
#include <cmath>

CameraController::CameraController(Camera *camera, QObject *parent) : QObject(parent), camera_(camera)
{
    deltaTimer_.start();
}

void CameraController::handleKeyPress(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        pressedKeys_.insert(event->key());
    }
}

void CameraController::handleKeyRelease(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        pressedKeys_.remove(event->key());
    }
}

void CameraController::handleMousePress(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isMouseLookActive_ = true;
        lastMousePos_ = event->position();
    }
}

void CameraController::handleMouseRelease(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isMouseLookActive_ = false;
    }
}

void CameraController::handleMouseMove(QMouseEvent *event)
{
    if (!isMouseLookActive_)
    {
        return;
    }

    QPointF currentPos = event->position();
    QPointF delta = currentPos - lastMousePos_;
    lastMousePos_ = currentPos;

    pendingMouseDelta_ += delta;
}

void CameraController::update()
{
    constexpr float kMillisecondsInSecond = 1000.f;

    float deltaTime = static_cast<float>(deltaTimer_.restart()) / kMillisecondsInSecond;

    updateMovement(deltaTime);
    updateRotation(deltaTime);
}

QVector3D CameraController::computeInputDirection() const
{
    float left = 0.0f;
    float forward = 0.0f;
    float up = 0.0f;

    if (pressedKeys_.contains(Qt::Key_A))
        left += 1.0f;
    if (pressedKeys_.contains(Qt::Key_D))
        left -= 1.0f;

    if (pressedKeys_.contains(Qt::Key_W))
        forward += 1.0f;
    if (pressedKeys_.contains(Qt::Key_S))
        forward -= 1.0f;

    if (pressedKeys_.contains(Qt::Key_Space))
        up += 1.0f;
    if (pressedKeys_.contains(Qt::Key_Control))
        up -= 1.0f;

    QVector3D direction{left, forward, up};
    if (!direction.isNull())
    {
        direction.normalize();
    }

    return direction;
}

void CameraController::updateMovement(float deltaTime)
{
    if (!camera_)
        return;

    QVector3D targetVelocity = computeInputDirection() * movementSpeed_;

    const float lerpFactor = 1.0f - std::exp(-acceleration_ * deltaTime);
    currentVelocity_ += (targetVelocity - currentVelocity_) * lerpFactor;

    constexpr float velocityThreshold = 0.00001f;

    if (currentVelocity_.lengthSquared() < velocityThreshold)
    {
        return;
    }

    camera_->move(currentVelocity_ * deltaTime);
}

void CameraController::updateRotation(float dt)
{
    if (!camera_)
        return;

    const float lerpFactor = 1.0f - std::exp(-rotationSmoothing_ * dt);
    QPointF currentStep = pendingMouseDelta_ * lerpFactor;
    pendingMouseDelta_ -= currentStep;

    const float deltaYaw = static_cast<float>(currentStep.x()) * mouseSensitivity_;
    const float deltaPitch = static_cast<float>(currentStep.y()) * mouseSensitivity_;

    constexpr float deltaThreshold = 0.00001f;

    if (std::abs(deltaYaw) > deltaThreshold || std::abs(deltaPitch) > deltaThreshold)
    {
        camera_->rotate(QVector3D{-deltaYaw, -deltaPitch, 0.0f});
    }
}