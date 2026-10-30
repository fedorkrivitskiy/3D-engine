#include "RenderWidget.hpp"

#include <QMouseEvent>
#include <QOpenGLShader>
#include <QQuaternion>
#include <QVector2D>
#include <QtTypes>

#include <cmath>

#include "Camera.hpp"
#include "CameraController.hpp"
#include "geometryengine.h"

RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    camera_ = std::make_unique<Camera>();
    cameraController_ = std::make_unique<CameraController>(camera_.get(), this);
}

RenderWidget::~RenderWidget()
{
    makeCurrent();

    geometries_.reset();
    program_.removeAllShaders();

    doneCurrent();
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{
    cameraController_->handleKeyPress(event);
}

void RenderWidget::keyReleaseEvent(QKeyEvent *event)
{
    cameraController_->handleKeyRelease(event);
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    cameraController_->handleMousePress(event);
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    cameraController_->handleMouseRelease(event);
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    cameraController_->handleMouseMove(event);
}

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();

    geometries_.reset(new GeometryEngine());
}

void RenderWidget::initShaders()
{
    if (!program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    if (!program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    if (!program_.link())
        close();

    if (!program_.bind())
        close();
}

void RenderWidget::setFov(float fov)
{
    fov_ = fov;

    updateProjectionMatrix();
}

void RenderWidget::setPlanes(float zNear, float zFar)
{
    zNear_ = zNear;
    zFar_ = zFar;

    updateProjectionMatrix();
}

void RenderWidget::resizeGL(int w, int h)
{
    aspectRatio_ = float(w) / float(h ? h : 1);

    updateProjectionMatrix();
}

void RenderWidget::updateProjectionMatrix()
{
    projection_.setToIdentity();

    projection_.perspective(fov_, aspectRatio_, zNear_, zFar_);
}

void RenderWidget::paintGL()
{
    cameraController_->update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    program_.bind();

    if (!camera_)
    {
        return;
    }

    QMatrix4x4 cameraMatrix;
    cameraMatrix.lookAt(camera_->getEye(), camera_->getTarget(), camera_->getUp());

    program_.setUniformValue("mvp_matrix", projection_ * cameraMatrix);

    geometries_->drawModel(&program_);

    geometries_->updateModel();
}
