#include "SettingsWidget.hpp"
#include "ui_SettingsWidget.h"

#include "Settings/SettingsModel.hpp"

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
    if (!settingsModel)
        return;

    model_ = settingsModel;

    ui->fovSlider->setValue(static_cast<int>(model_->getFov()));
    ui->nearSpinBox->setValue(model_->getZNear());
    ui->farSpinBox->setValue(model_->getZFar());

    connect(ui->fovSlider, &QSlider::valueChanged, this, [this](int val) { model_->setFov(static_cast<float>(val)); });

    connect(ui->nearSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double val) { model_->setZNear(static_cast<float>(val)); });

    connect(ui->farSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double val) { model_->setZFar(static_cast<float>(val)); });

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
}