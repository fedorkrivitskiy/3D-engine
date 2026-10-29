#pragma once

#include "geometryengine.h"

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

class GeometryEngine;
class Camera;

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

  public:
    using QOpenGLWidget::QOpenGLWidget;

    RenderWidget() = default;

    ~RenderWidget();

    void setCamera(Camera *camera);

    void setZNear(float zNear);

    void setZFar(float zFar);

    void setFov(float fov);

  protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

  private:
    void updateProjectionMatrix();

    void initShaders();

  private:
    QOpenGLShaderProgram program;

    GeometryEngine *geometries = nullptr;
    Camera *camera_ = nullptr;

    float zNear_{}, zFar_{}, fov_{};
    float aspectRatio_{};

    QMatrix4x4 projection_{};
};
