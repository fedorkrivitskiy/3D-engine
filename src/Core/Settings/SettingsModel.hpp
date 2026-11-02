#pragma once

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

  signals:
    void fovChanged(float fov);
    void zNearChanged(float zNear);
    void zFarChanged(float zFar);

    void modelPathChanged(const QString &modelPath);

  private:
    CameraSettings cameraSettings_{};
    ModelSettings modelSettings_{};
};
