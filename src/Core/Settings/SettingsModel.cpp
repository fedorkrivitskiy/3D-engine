#include "SettingsModel.hpp"

#include <QSettings>

#include "Defaults.hpp"

namespace
{
constexpr auto kCompanyName = "Default";
constexpr auto kApplicationName = "3D_Engine";
} // namespace

SettingsModel::SettingsModel(QObject *parent) : QObject(parent)
{
    load();
}

SettingsModel::~SettingsModel()
{
    save();
}

void SettingsModel::load()
{
    QSettings settings{kCompanyName, kApplicationName};

    cameraSettings_.fov = settings.value("camera/fov", Defaults::Camera::kFov).toFloat();
    cameraSettings_.zFar = settings.value("camera/zFar", Defaults::Camera::kZFar).toFloat();
    cameraSettings_.zNear = settings.value("camera/zNear", Defaults::Camera::kZNear).toFloat();

    modelSettings_.modelPath = settings.value("model/modelPath", Defaults::Model::kModelPath).toString();

    sceneSettings_.lightColor = settings.value("scene/lightColor", Defaults::Scene::kLightColor).value<QColor>();
    sceneSettings_.kAmbient = settings.value("scene/kAmbient", Defaults::Scene::kAmbient).toFloat();
    sceneSettings_.kDiffuse = settings.value("scene/kDiffuse", Defaults::Scene::kDiffuse).toFloat();
    sceneSettings_.kSpecular = settings.value("scene/kSpecular", Defaults::Scene::kSpecular).toFloat();
}

void SettingsModel::save()
{
    QSettings settings{kCompanyName, kApplicationName};

    settings.setValue("camera/fov", cameraSettings_.fov);
    settings.setValue("camera/zFar", cameraSettings_.zFar);
    settings.setValue("camera/zNear", cameraSettings_.zNear);

    settings.setValue("model/modelPath", modelSettings_.modelPath);

    settings.setValue("scene/lightColor", sceneSettings_.lightColor);
    settings.setValue("scene/kAmbient", sceneSettings_.kAmbient);
    settings.setValue("scene/kDiffuse", sceneSettings_.kDiffuse);
    settings.setValue("scene/kSpecular", sceneSettings_.kSpecular);
}

void SettingsModel::setFov(float fov)
{
    if (qFuzzyCompare(fov, cameraSettings_.fov))
    {
        return;
    }

    cameraSettings_.fov = fov;
    emit fovChanged(fov);
}

float SettingsModel::getFov() const
{
    return cameraSettings_.fov;
}

void SettingsModel::setZNear(float zNear)
{
    if (qFuzzyCompare(zNear, cameraSettings_.zNear))
    {
        return;
    }

    cameraSettings_.zNear = zNear;
    emit zNearChanged(zNear);
}

float SettingsModel::getZNear() const
{
    return cameraSettings_.zNear;
}

void SettingsModel::setZFar(float zFar)
{
    if (qFuzzyCompare(zFar, cameraSettings_.zFar))
    {
        return;
    }

    cameraSettings_.zFar = zFar;
    emit zFarChanged(zFar);
}

float SettingsModel::getZFar() const
{
    return cameraSettings_.zFar;
}

void SettingsModel::setModelPath(const QString &modelPath)
{
    if (modelSettings_.modelPath == modelPath)
    {
        return;
    }

    modelSettings_.modelPath = modelPath;
    emit modelPathChanged(modelPath);
}

QString SettingsModel::getModelPath() const
{
    return modelSettings_.modelPath;
}

void SettingsModel::setLightColor(const QColor &color)
{
    if (sceneSettings_.lightColor == color)
    {
        return;
    }

    sceneSettings_.lightColor = color;
    emit lightColorChanged(color);
}

QColor SettingsModel::getLightColor() const
{
    return sceneSettings_.lightColor;
}

void SettingsModel::setKAmbient(float kAmbient)
{
    if (qFuzzyCompare(kAmbient, sceneSettings_.kAmbient))
    {
        return;
    }

    sceneSettings_.kAmbient = kAmbient;
    emit kAmbientChanged(kAmbient);
}

float SettingsModel::getKAmbient() const
{
    return sceneSettings_.kAmbient;
}

void SettingsModel::setKDiffuse(float kDiffuse)
{
    if (qFuzzyCompare(kDiffuse, sceneSettings_.kDiffuse))
    {
        return;
    }

    sceneSettings_.kDiffuse = kDiffuse;
    emit diffuseChanged(kDiffuse);
}

float SettingsModel::getKDiffuse() const
{
    return sceneSettings_.kDiffuse;
}

void SettingsModel::setKSpecular(float kSpecular)
{
    if (qFuzzyCompare(kSpecular, sceneSettings_.kSpecular))
    {
        return;
    }

    sceneSettings_.kSpecular = kSpecular;
    emit specularChanged(kSpecular);
}

float SettingsModel::getKSpecular() const
{
    return sceneSettings_.kSpecular;
}