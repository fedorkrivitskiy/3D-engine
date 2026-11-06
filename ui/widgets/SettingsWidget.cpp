#include "SettingsWidget.hpp"
#include "ui_SettingsWidget.h"

#include "Core/Settings/SettingsModel.hpp"

#include <QColorDialog>
#include <QVector4D>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent), ui_(new Ui::SettingsWidget)
{
    ui_->setupUi(this);
}

SettingsWidget::~SettingsWidget()
{
    delete ui_;
}

void SettingsWidget::setModel(SettingsModel *settingsModel)
{
    if (settingsModel == nullptr)
    {
        return;
    }

    model_ = settingsModel;

    ui_->fovSlider->setValue(static_cast<int>(model_->getFov()));
    ui_->nearSpinBox->setValue(model_->getZNear());
    ui_->farSpinBox->setValue(model_->getZFar());

    ui_->ambientSpinBox->setValue(model_->getKAmbient());
    ui_->diffuseSpinBox->setValue(model_->getKDiffuse());
    ui_->specularSpinBox->setValue(model_->getKSpecular());

    const QColor initialColor = model_->getLightColor();

    ui_->lightColorPreview->setStyleSheet(
        QString("background-color: %1; border: 1px solid gray;").arg(initialColor.name(QColor::HexArgb)));

    connect(ui_->fovSlider, &QSlider::valueChanged, this, [this](int val) { model_->setFov(static_cast<float>(val)); });

    connect(ui_->nearSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double val) { model_->setZNear(static_cast<float>(val)); });

    connect(ui_->farSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double val) { model_->setZFar(static_cast<float>(val)); });

    connect(ui_->ambientSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) { model_->setKAmbient(static_cast<float>(value)); });

    connect(ui_->diffuseSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) { model_->setKDiffuse(static_cast<float>(value)); });

    connect(ui_->specularSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) { model_->setKSpecular(static_cast<float>(value)); });

    connect(ui_->lightColorButton, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(model_->getLightColor(), this, tr("Select Light Color"),
                                              QColorDialog::ShowAlphaChannel);

        if (color.isValid())
        {
            model_->setLightColor(color);
        }
    });

    connect(model_, &SettingsModel::fovChanged, this, [this](float fov) {
        QSignalBlocker blocker{ui_->fovSlider};
        ui_->fovSlider->setValue(static_cast<int>(fov));
    });

    connect(model_, &SettingsModel::zNearChanged, this, [this](float zNear) {
        QSignalBlocker blocker{ui_->nearSpinBox};
        ui_->nearSpinBox->setValue(zNear);
    });

    connect(model_, &SettingsModel::zFarChanged, this, [this](float zFar) {
        QSignalBlocker blocker{ui_->farSpinBox};
        ui_->farSpinBox->setValue(zFar);
    });

    connect(model_, &SettingsModel::kAmbientChanged, this, [this](float kAmbient) {
        QSignalBlocker blocker{ui_->ambientSpinBox};
        ui_->ambientSpinBox->setValue(kAmbient);
    });

    connect(model_, &SettingsModel::diffuseChanged, this, [this](float kDiffuse) {
        QSignalBlocker blocker{ui_->diffuseSpinBox};
        ui_->diffuseSpinBox->setValue(kDiffuse);
    });

    connect(model_, &SettingsModel::specularChanged, this, [this](float kSpecular) {
        QSignalBlocker blocker{ui_->specularSpinBox};
        ui_->specularSpinBox->setValue(kSpecular);
    });

    connect(model_, &SettingsModel::lightColorChanged, this, [this](const QColor &color) {
        ui_->lightColorPreview->setStyleSheet(
            QString("background-color: %1; border: 1px solid gray;").arg(color.name(QColor::HexArgb)));
    });
}