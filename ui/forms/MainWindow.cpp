#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

#include <QKeyEvent>
#include <QTimer>

#include "Core/FPSCounter.hpp"

#include "Core/Core.hpp"
#include "Core/Settings/SettingsModel.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow), updateTimer_(new QTimer(this))
{
    ui_->setupUi(this);

    showFullScreen();

    counter_ = std::make_unique<FPSCounter>();

    connect(ui_->quitButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(ui_->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    initStatisticsWidget();

    initSettingsWidget();

    initRenderWidget();

    connect(updateTimer_, &QTimer::timeout, this, &MainWindow::onFrameUpdate);

    updateTimer_->setInterval(0);
    updateTimer_->start();
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onFrameUpdate()
{
    counter_->update();
    ui_->statsWidget->updateStats();
    ui_->renderWidget->update();
}

void MainWindow::onSettingsButtonClicked()
{
    isSettingsWidgetVisible_ = !isSettingsWidgetVisible_;

    ui_->settingsWidget->setVisible(isSettingsWidgetVisible_);
}

void MainWindow::initSettingsWidget()
{
    SettingsModel *settingsModel = Core::get().getSettingsModel();
    ui_->settingsWidget->setModel(settingsModel);

    ui_->settingsWidget->setVisible(isSettingsWidgetVisible_);
}

void MainWindow::initStatisticsWidget()
{
    // ui->statsWidget->setCamera(camera_.get());
    ui_->statsWidget->setFPSCounter(counter_.get());

    ui_->statsWidget->raise();
}

void MainWindow::initRenderWidget()
{
}