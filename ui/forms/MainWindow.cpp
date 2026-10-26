#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QImage>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QTimer>

#include "Camera.hpp"
#include "FPSCounter.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showFullScreen();

    camera_ = std::make_unique<Camera>();
    counter_ = std::make_unique<FPSCounter>();

    ui->statsWidget->setCamera(camera_.get());
    ui->statsWidget->setFPSCounter(counter_.get());

    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, [this]() {
        counter_->update();
        ui->statsWidget->updateStats();

        ui->renderWidget->update();
        ui->renderWidget->updatePhysics();
    });

    timer_->setInterval(0);
    timer_->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        camera_->move({0, 1, 0});
        break;
    case Qt::Key_S:
        camera_->move({0, -1, 0});
        break;
    case Qt::Key_A:
        camera_->move({1, 0, 0});
        break;
    case Qt::Key_D:
        camera_->move({-1, 0, 0});
        break;
    case Qt::Key_Shift:
        camera_->move({0, 0, 1});
        break;
    case Qt::Key_Space:
        camera_->move({0, 0, -1});
        break;
    default:
        break;
    }
}
