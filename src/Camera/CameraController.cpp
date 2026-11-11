#include "CameraController.hpp"

#include <QKeyEvent>
#include <QMouseEvent>
#include <cmath>

#include "Camera.hpp"
#include "Core/Core.hpp"
#include "Core/Settings/SettingsModel.hpp"

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
    constexpr float kMillisecondsInSecond = 1000.F;

    float deltaTime = static_cast<float>(deltaTimer_.restart()) / kMillisecondsInSecond;

    updateMovement(deltaTime);
    updateRotation(deltaTime);
}

QVector3D CameraController::computeInputDirection() const
{
    float leftDirection = 0.0F;
    float forwardDirection = 0.0F;
    float upDirection = 0.0F;

    if (pressedKeys_.contains(Qt::Key_A))
    {
        leftDirection += 1.0F;
    }
    if (pressedKeys_.contains(Qt::Key_D))
    {
        leftDirection -= 1.0F;
    }

    if (pressedKeys_.contains(Qt::Key_W))
    {
        forwardDirection += 1.0F;
    }
    if (pressedKeys_.contains(Qt::Key_S))
    {
        forwardDirection -= 1.0F;
    }

    if (pressedKeys_.contains(Qt::Key_Space))
    {
        upDirection += 1.0F;
    }
    if (pressedKeys_.contains(Qt::Key_Control))
    {
        upDirection -= 1.0F;
    }

    QVector3D direction{leftDirection, forwardDirection, upDirection};
    if (!direction.isNull())
    {
        direction.normalize();
    }

    return direction;
}

void CameraController::updateMovement(float deltaTime)
{
    if (camera_ == nullptr)
    {
        return;
    }

    SettingsModel *settingsModel = Core::get().getSettingsModel();
    if (settingsModel == nullptr)
    {
        return;
    }

    const float acceleration = settingsModel->getAcceleration();
    const float movementSpeed = settingsModel->getMovementSpeed();

    QVector3D targetVelocity = computeInputDirection() * movementSpeed;

    const float lerpFactor = 1.0F - std::exp(-acceleration * deltaTime);
    currentVelocity_ += (targetVelocity - currentVelocity_) * lerpFactor;

    constexpr float velocityThreshold = 0.00001F;

    if (currentVelocity_.lengthSquared() < velocityThreshold)
    {
        return;
    }

    camera_->move(currentVelocity_ * deltaTime);
}

void CameraController::updateRotation(float deltaTime)
{
    if (camera_ == nullptr)
    {
        return;
    }

    SettingsModel *settingsModel = Core::get().getSettingsModel();
    if (settingsModel == nullptr)
    {
        return;
    }

    const float rotationSmoothing = settingsModel->getRotationSmoothing();
    const float mouseSensitivity = settingsModel->getMouseSensitivity();

    const float lerpFactor = 1.0F - std::exp(-rotationSmoothing * deltaTime);
    QPointF currentStep = pendingMouseDelta_ * lerpFactor;
    pendingMouseDelta_ -= currentStep;

    const float deltaYaw = static_cast<float>(currentStep.x()) * mouseSensitivity;
    const float deltaPitch = static_cast<float>(currentStep.y()) * mouseSensitivity;

    constexpr float deltaThreshold = 0.00001F;

    if (std::abs(deltaYaw) > deltaThreshold || std::abs(deltaPitch) > deltaThreshold)
    {
        camera_->rotate(QVector3D{-deltaYaw, -deltaPitch, 0.0F});
    }
}