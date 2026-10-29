#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QElapsedTimer>
#include <QKeyEvent>
#include <QTimer>

#include "Camera.hpp"
#include "FPSCounter.hpp"

#include "Core/Core.hpp"
#include "Settings/SettingsModel.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showFullScreen();

    camera_ = std::make_unique<Camera>();
    counter_ = std::make_unique<FPSCounter>();

    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    initStatisticsWidget();

    initSettingsWidget();

    initRenderWidget();

    frameUpdateTimer_ = new QTimer(this);

    connect(frameUpdateTimer_, &QTimer::timeout, this, &MainWindow::onFrameUpdate);

    frameUpdateTimer_->setInterval(0);
    frameUpdateTimer_->start();

    deltaTimer_ = new QElapsedTimer();
    deltaTimer_->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        pressedKeys_.insert(event->key());
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        pressedKeys_.remove(event->key());
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        isMouseLookActive_ = true;
        lastMousePos_ = event->position();
        setCursor(Qt::BlankCursor);
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        isMouseLookActive_ = false;
        unsetCursor();
    }
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isMouseLookActive_)
    {
        QPointF delta = event->position() - lastMousePos_;
        lastMousePos_ = event->position();

        constexpr float sensitivity = 0.0025f;

        pendingRotation_ += QVector3D(delta.x() * sensitivity, -delta.y() * sensitivity, 0.0f);
    }

    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::onFrameUpdate()
{
    float dt = deltaTimer_->restart() / 1000.0f;

    updateCameraMovement(dt);
    updateCameraRotation(dt);

    counter_->update();
    ui->statsWidget->updateStats();
    ui->renderWidget->update();
}

void MainWindow::updateCameraMovement(float deltaTime)
{
    QVector3D direction(0.0f, 0.0f, 0.0f);

    if (pressedKeys_.contains(Qt::Key_W))
        direction += QVector3D(0, 1, 0);
    if (pressedKeys_.contains(Qt::Key_S))
        direction += QVector3D(0, -1, 0);
    if (pressedKeys_.contains(Qt::Key_A))
        direction += QVector3D(1, 0, 0);
    if (pressedKeys_.contains(Qt::Key_D))
        direction += QVector3D(-1, 0, 0);
    if (pressedKeys_.contains(Qt::Key_Shift))
        direction += QVector3D(0, 0, 1);
    if (pressedKeys_.contains(Qt::Key_Space))
        direction += QVector3D(0, 0, -1);

    if (!direction.isNull())
    {
        direction.normalize();
    }

    constexpr float moveSpeed = 10.0f;
    constexpr float acceleration = 12.0f;

    QVector3D targetVelocity = direction * moveSpeed;
    float blendFactor = 1.0f - std::exp(-acceleration * deltaTime);
    currentVelocity_ += (targetVelocity - currentVelocity_) * blendFactor;

    constexpr float velocityTreshold = 0.0001f;

    if (currentVelocity_.lengthSquared() > velocityTreshold)
    {
        auto vec = currentVelocity_ * deltaTime;
        camera_->move({vec.x(), vec.y(), vec.z()});
    }
}

void MainWindow::updateCameraRotation(float deltaTime)
{
    constexpr float rotationTreshold = 0.000001f;

    if (pendingRotation_.lengthSquared() < rotationTreshold)
    {
        return;
    }

    const float rotationSmoothing = 25.0f;
    float blendFactor = 1.0f - std::exp(-rotationSmoothing * deltaTime);

    QVector3D stepRotation = pendingRotation_ * blendFactor;
    pendingRotation_ -= stepRotation;

    camera_->rotate(stepRotation);
}

void MainWindow::onSettingsButtonClicked()
{
    isSettingsWidgetVisible = !isSettingsWidgetVisible;

    ui->settingsWidget->setVisible(isSettingsWidgetVisible);
}

void MainWindow::initSettingsWidget()
{
    SettingsModel *settingsModel = Core::get().getSettingsModel();
    ui->settingsWidget->setModel(settingsModel);

    ui->settingsWidget->setVisible(isSettingsWidgetVisible);
}

void MainWindow::initStatisticsWidget()
{
    ui->statsWidget->setCamera(camera_.get());
    ui->statsWidget->setFPSCounter(counter_.get());

    ui->statsWidget->raise();
}

void MainWindow::initRenderWidget()
{
    ui->renderWidget->setCamera(camera_.get());

    SettingsModel *settingsModel = Core::get().getSettingsModel();

    ui->renderWidget->setFov(settingsModel->getFov());
    ui->renderWidget->setZNear(settingsModel->getZNear());
    ui->renderWidget->setZFar(settingsModel->getZFar());

    connect(settingsModel, &SettingsModel::fovChanged, this, [this](float fov) { ui->renderWidget->setFov(fov); });
    connect(settingsModel, &SettingsModel::zNearChanged, this,
            [this](float zNear) { ui->renderWidget->setZNear(zNear); });
    connect(settingsModel, &SettingsModel::zFarChanged, this, [this](float zFar) { ui->renderWidget->setFov(zFar); });
}