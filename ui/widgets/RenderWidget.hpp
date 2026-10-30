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

  public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget() override;

    void setFov(float fov);
    void setPlanes(float zNear, float zFar);

  signals:
    void frameRendered(float frameTimeMs);

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

  private:
    void updateProjectionMatrix();
    void initShaders();

    std::unique_ptr<Camera> camera_;
    std::unique_ptr<CameraController> cameraController_;
    std::unique_ptr<GeometryEngine> geometries_;

    QOpenGLShaderProgram program_;
    QMatrix4x4 projection_;

    float fov_{60.0f};
    float zNear_{0.1f};
    float zFar_{100.0f};
    float aspectRatio_{1.0f};
};
