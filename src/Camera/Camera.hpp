#pragma once

#include <QVector3D>
#include <numbers>

class Camera final
{
  public:
    Camera();

    void move(const QVector3D &direction);

    void rotate(const QVector3D &rotation);

    void setScale(float scale);

    [[nodiscard]] QVector3D getEye() const;

    [[nodiscard]] QVector3D getTarget() const;

    [[nodiscard]] QVector3D getUp() const;

    [[nodiscard]] float getScale() const;

  private:
    void updateForward();

    QVector3D forward_{0, 0, 0};
    QVector3D eye_{0.0, 0.0, 0.0};
    QVector3D target_{0, 0, 0};
    QVector3D up_{0, 1, 0};

    float scale_{kDefaultScale};
    float yaw_{kDefaultYaw};
    float pitch_{kDefaultPitch};

    static constexpr float kDefaultScale = 0.F;
    static constexpr float kDefaultYaw = -std::numbers::pi_v<float> / 2.F;
    static constexpr float kDefaultPitch = 1.0F;
};