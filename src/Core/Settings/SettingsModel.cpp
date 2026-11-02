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
}

void SettingsModel::save()
{
    QSettings settings{kCompanyName, kApplicationName};

    settings.setValue("camera/fov", cameraSettings_.fov);
    settings.setValue("camera/zFar", cameraSettings_.zFar);
    settings.setValue("camera/zNear", cameraSettings_.zNear);

    settings.setValue("model/modelPath", modelSettings_.modelPath);
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
    modelSettings_.modelPath = modelPath;
    emit modelPathChanged(modelPath);
}

QString SettingsModel::getModelPath() const
{
    return modelSettings_.modelPath;
}