#pragma once

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <memory>

class GeometryEngine;
class Camera;
class CameraController;

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(RenderWidget)

  public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget() override;

  signals:
    void frameRendered(float frameTimeMs);

  protected:
    void initializeGL() override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

  private:
    QMatrix4x4 getProjectionMatrix();
    void initShaders();

    std::unique_ptr<Camera> camera_;
    std::unique_ptr<CameraController> cameraController_;
    std::unique_ptr<GeometryEngine> geometries_;

    QOpenGLShaderProgram program_;
};
