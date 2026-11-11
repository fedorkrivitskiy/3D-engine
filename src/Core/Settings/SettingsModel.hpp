#pragma once

#include <QColor>
#include <QObject>
#include <QString>

struct CameraSettings
{
    float fov{};
    float zNear{};
    float zFar{};
};

struct ModelSettings
{
    QString modelPath{};
};

struct SceneSettings
{
    QColor lightColor{};
    float kAmbient{};
    float kDiffuse{};
    float kSpecular{};
};

struct MovementSettings
{
    float movementSpeed{};
    float acceleration{};
    float mouseSensitivity{};
    float rotationSmoothing{};
};

class SettingsModel final : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(SettingsModel)

  public:
    explicit SettingsModel(QObject *parent = nullptr);
    ~SettingsModel() override;

    void load();
    void save() const;

    void setFov(float fov);
    [[nodiscard]] float getFov() const;

    void setZNear(float zNear);
    [[nodiscard]] float getZNear() const;

    void setZFar(float zFar);
    [[nodiscard]] float getZFar() const;

    void setModelPath(const QString &path);
    [[nodiscard]] QString getModelPath() const;

    void setLightColor(const QColor &color);
    [[nodiscard]] QColor getLightColor() const;

    void setKAmbient(float kAmbient);
    [[nodiscard]] float getKAmbient() const;

    void setKDiffuse(float kDiffuse);
    [[nodiscard]] float getKDiffuse() const;

    void setKSpecular(float kSpecular);
    [[nodiscard]] float getKSpecular() const;

    void setMovementSpeed(float movementSpeed);
    [[nodiscard]] float getMovementSpeed() const;

    void setAcceleration(float acceleration);
    [[nodiscard]] float getAcceleration() const;

    void setMouseSensitivity(float mouseSensitivity);
    [[nodiscard]] float getMouseSensitivity() const;

    void setRotationSmoothing(float rotationSmoothing);
    [[nodiscard]] float getRotationSmoothing() const;

  signals:
    void fovChanged(float fov);
    void zNearChanged(float zNear);
    void zFarChanged(float zFar);

    void modelPathChanged(const QString &modelPath);

    void lightColorChanged(const QColor &lightColor);
    void kAmbientChanged(float kAmbient);
    void diffuseChanged(float kDiffuse);
    void specularChanged(float kSpecular);

    void movementSpeedChanged(float movementSpeed);
    void accelerationChanged(float acceleration);
    void mouseSensitivityChanged(float mouseSensitivity);
    void rotationSmoothingChanged(float rotationSmoothing);

  private:
    CameraSettings cameraSettings_{};
    ModelSettings modelSettings_{};
    SceneSettings sceneSettings_{};
    MovementSettings movementSettings_{};
};
