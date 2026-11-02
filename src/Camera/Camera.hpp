#pragma once

#include <QVector3D>

class Camera
{
  public:
    Camera();

    virtual ~Camera() = default;

    void move(const QVector3D &direction);

    void rotate(const QVector3D &rotation);

    void setScale(float scale);

    QVector3D getEye() const;

    QVector3D getTarget() const;

    QVector3D getUp() const;

    float getScale() const;

  private:
    void updateForward();

  private:
    QVector3D forward_{0, 0, 0};
    QVector3D eye_{0.0, 0.0, 5.0};
    QVector3D target_{0, 0, 0};
    QVector3D up_{0, 1, 0};

    float scale_{1.0f};
    float yaw_{-std::numbers::pi_v<float> / 2};
    float pitch_{};
};