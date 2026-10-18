#pragma once

#include <glm/vec3.hpp>

class Camera
{
  public:
    Camera();

    virtual ~Camera() = default;

    void move(const glm::vec3 &direction);

    void rotate(const glm::vec3 &rotation);

    void setScale(float scale);

    glm::vec3 getEye() const;

    glm::vec3 getTarget() const;

    glm::vec3 getUp() const;

    float getScale() const;

  private:
    void updateForward();

  private:
    glm::vec3 forward_{0, 0, 0};
    glm::vec3 eye_{0.0, 0.0, 5.0};
    glm::vec3 target_{0, 0, 0};
    glm::vec3 up_{0, 1, 0};

    float scale_{1.0f};
    float yaw_{-std::numbers::pi_v<float> / 2};
    float pitch_{};
};