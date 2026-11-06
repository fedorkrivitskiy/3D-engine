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

class SettingsModel final : public QObject
{
    Q_OBJECT

  public:
    explicit SettingsModel(QObject *parent = nullptr);
    ~SettingsModel() override;

    void load();
    void save();

    void setFov(float fov);
    float getFov() const;

    void setZNear(float zNear);
    float getZNear() const;

    void setZFar(float zFar);
    float getZFar() const;

    void setModelPath(const QString &path);
    QString getModelPath() const;

    void setLightColor(const QColor &color);
    QColor getLightColor() const;

    void setKAmbient(float kAmbient);
    float getKAmbient() const;

    void setKDiffuse(float kDiffuse);
    float getKDiffuse() const;

    void setKSpecular(float kSpecular);
    float getKSpecular() const;

  signals:
    void fovChanged(float fov);
    void zNearChanged(float zNear);
    void zFarChanged(float zFar);

    void modelPathChanged(const QString &modelPath);

    void lightColorChanged(const QColor &lightColor);
    void kAmbientChanged(float kAmbient);
    void diffuseChanged(float kDiffuse);
    void specularChanged(float kSpecular);

  private:
    CameraSettings cameraSettings_{};
    ModelSettings modelSettings_{};
    SceneSettings sceneSettings_{};
};
