#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QKeyEvent>
#include <QTimer>

#include "Core/FPSCounter.hpp"

#include "Core/Core.hpp"
#include "Core/Settings/SettingsModel.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showFullScreen();

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
    // ui->statsWidget->setCamera(camera_.get());
    ui->statsWidget->setFPSCounter(counter_.get());

    ui->statsWidget->raise();
}

void MainWindow::initRenderWidget()
{
    //
}