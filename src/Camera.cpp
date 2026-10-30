#include "Camera.hpp"

#include <QVector3D>
#include <QtMath>
#include <algorithm>

namespace
{
constexpr QVector3D kWorldUp{0.0f, 1.0f, 0.0f};

constexpr float kMaxCameraPitch = 89.f;
constexpr float kMaxCameraPitchRadians = std::numbers::pi_v<float> * kMaxCameraPitch / 180.f;
} // namespace

Camera::Camera()
{
    updateForward();
}

void Camera::move(const QVector3D &move_vector)
{
    QVector3D left = QVector3D::crossProduct(up_, forward_).normalized();

    QVector3D left_move_vector = left * move_vector.x();
    QVector3D forward_move_vector = forward_ * move_vector.y();
    QVector3D up_move_vector = kWorldUp * move_vector.z();
    QVector3D result_vector = left_move_vector + forward_move_vector + up_move_vector;

    eye_ += result_vector;
    target_ += result_vector;

    updateForward();
}

void Camera::rotate(const QVector3D &rotation_vector)
{
    yaw_ -= rotation_vector.x();

    pitch_ = std::clamp(pitch_ + rotation_vector.y(), -kMaxCameraPitchRadians, kMaxCameraPitchRadians);

    forward_ =
        QVector3D{std::cos(yaw_) * std::cos(pitch_), std::sin(pitch_), std::sin(yaw_) * std::cos(pitch_)}.normalized();

    QVector3D left = QVector3D::crossProduct(forward_, kWorldUp).normalized();
    up_ = QVector3D::crossProduct(left, forward_).normalized();
    target_ = eye_ + forward_;
}

void Camera::setScale(float scale)
{
    scale_ = scale;
}

QVector3D Camera::getEye() const
{
    return eye_;
}

QVector3D Camera::getTarget() const
{
    return target_;
}

QVector3D Camera::getUp() const
{
    return up_;
}

float Camera::getScale() const
{
    return scale_;
}

void Camera::updateForward()
{
    forward_ = (target_ - eye_).normalized();
}