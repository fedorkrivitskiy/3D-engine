#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

#include <QKeyEvent>
#include <QTimer>

#include "Core/FPSCounter.hpp"

#include "Core/Core.hpp"
#include "Core/Settings/SettingsModel.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), updateTimer_(new QTimer(this))
{
    ui->setupUi(this);

    showFullScreen();

    counter_ = std::make_unique<FPSCounter>();

    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    initStatisticsWidget();

    initSettingsWidget();

    initRenderWidget();

    connect(updateTimer_, &QTimer::timeout, this, &MainWindow::onFrameUpdate);

    updateTimer_->setInterval(0);
    updateTimer_->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFrameUpdate()
{
    counter_->update();
    ui->statsWidget->updateStats();
    ui->renderWidget->update();
}

void MainWindow::onSettingsButtonClicked()
{
    isSettingsWidgetVisible_ = !isSettingsWidgetVisible_;

    ui->settingsWidget->setVisible(isSettingsWidgetVisible_);
}

void MainWindow::initSettingsWidget()
{
    SettingsModel *settingsModel = Core::get().getSettingsModel();
    ui->settingsWidget->setModel(settingsModel);

    ui->settingsWidget->setVisible(isSettingsWidgetVisible_);
}

void MainWindow::initStatisticsWidget()
{
    // ui->statsWidget->setCamera(camera_.get());
    ui->statsWidget->setFPSCounter(counter_.get());

    ui->statsWidget->raise();
}

void MainWindow::initRenderWidget()
{
}