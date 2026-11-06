#include "SettingsWidget.hpp"
#include "ui_SettingsWidget.h"

#include "Core/Settings/SettingsModel.hpp"

#include <QColorDialog>
#include <QVector4D>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::setModel(SettingsModel *settingsModel)
{
    if (settingsModel == nullptr)
    {
        return;
    }

    model_ = settingsModel;

    ui->fovSlider->setValue(static_cast<int>(model_->getFov()));
    ui->nearSpinBox->setValue(model_->getZNear());
    ui->farSpinBox->setValue(model_->getZFar());

    ui->ambientSpinBox->setValue(model_->getKAmbient());
    ui->diffuseSpinBox->setValue(model_->getKDiffuse());
    ui->specularSpinBox->setValue(model_->getKSpecular());

    const QColor initialColor = model_->getLightColor();

    ui->lightColorPreview->setStyleSheet(
        QString("background-color: %1; border: 1px solid gray;").arg(initialColor.name(QColor::HexArgb)));

    connect(ui->fovSlider, &QSlider::valueChanged, this, [this](int val) { model_->setFov(static_cast<float>(val)); });

    connect(ui->nearSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double val) { model_->setZNear(static_cast<float>(val)); });

    connect(ui->farSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double val) { model_->setZFar(static_cast<float>(val)); });

    connect(ui->ambientSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) { model_->setKAmbient(static_cast<float>(value)); });

    connect(ui->diffuseSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) { model_->setKDiffuse(static_cast<float>(value)); });

    connect(ui->specularSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) { model_->setKSpecular(static_cast<float>(value)); });

    connect(ui->lightColorButton, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(model_->getLightColor(), this, tr("Select Light Color"),
                                              QColorDialog::ShowAlphaChannel);

        if (color.isValid())
        {
            model_->setLightColor(color);
        }
    });

    connect(model_, &SettingsModel::fovChanged, this, [this](float fov) {
        QSignalBlocker blocker{ui->fovSlider};
        ui->fovSlider->setValue(static_cast<int>(fov));
    });

    connect(model_, &SettingsModel::zNearChanged, this, [this](float zNear) {
        QSignalBlocker blocker{ui->nearSpinBox};
        ui->nearSpinBox->setValue(zNear);
    });

    connect(model_, &SettingsModel::zFarChanged, this, [this](float zFar) {
        QSignalBlocker blocker{ui->farSpinBox};
        ui->farSpinBox->setValue(zFar);
    });

    connect(model_, &SettingsModel::kAmbientChanged, this, [this](float kAmbient) {
        QSignalBlocker blocker{ui->ambientSpinBox};
        ui->ambientSpinBox->setValue(kAmbient);
    });

    connect(model_, &SettingsModel::diffuseChanged, this, [this](float kDiffuse) {
        QSignalBlocker blocker{ui->diffuseSpinBox};
        ui->diffuseSpinBox->setValue(kDiffuse);
    });

    connect(model_, &SettingsModel::specularChanged, this, [this](float kSpecular) {
        QSignalBlocker blocker{ui->specularSpinBox};
        ui->specularSpinBox->setValue(kSpecular);
    });

    connect(model_, &SettingsModel::lightColorChanged, this, [this](const QColor &color) {
        ui->lightColorPreview->setStyleSheet(
            QString("background-color: %1; border: 1px solid gray;").arg(color.name(QColor::HexArgb)));
    });
}