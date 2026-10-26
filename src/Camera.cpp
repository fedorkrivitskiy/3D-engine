#include "Camera.hpp"

#include <algorithm>
#include <cmath>

#include <glm/detail/func_geometric.inl>

#include "Constants.hpp"

namespace
{
constexpr glm::vec3 world_up{0.0, 1.0, 0.0};
} // namespace

Camera::Camera()
{
    updateForward();
}

void Camera::move(const glm::vec3 &move_vector)
{
    glm::vec3 left = glm::normalize(glm::cross(up_, forward_));

    glm::vec3 left_move_vector = left * move_vector.x;
    glm::vec3 forward_move_vector = forward_ * move_vector.y;
    glm::vec3 up_move_vector = world_up * move_vector.z;
    glm::vec3 result_vector = left_move_vector + forward_move_vector + up_move_vector;

    eye_ += result_vector;
    target_ += result_vector;

    updateForward();
}

void Camera::rotate(const glm::vec3 &rotation_vector)
{
    yaw_ -= rotation_vector.x;

    pitch_ =
        std::clamp(pitch_ + rotation_vector.y, -Constants::kMaxCameraPitchRadians, Constants::kMaxCameraPitchRadians);

    forward_ = glm::normalize(
        glm::vec3{std::cos(yaw_) * std::cos(pitch_), std::sin(pitch_), std::sin(yaw_) * std::cos(pitch_)});

    glm::vec3 left = glm::normalize(glm::cross(forward_, world_up));
    up_ = glm::normalize(glm::cross(left, forward_));
    target_ = eye_ + forward_;
}

void Camera::setScale(float scale)
{
    scale_ = scale;
}

glm::vec3 Camera::getEye() const
{
    return eye_;
}

glm::vec3 Camera::getTarget() const
{
    return target_;
}

glm::vec3 Camera::getUp() const
{
    return up_;
}

float Camera::getScale() const
{
    return scale_;
}

void Camera::updateForward()
{
    forward_ = glm::normalize(target_ - eye_);
}
