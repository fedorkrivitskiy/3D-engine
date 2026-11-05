#include "RenderWidget.hpp"

#include <QMouseEvent>
#include <QOpenGLShader>
#include <QQuaternion>
#include <QVector2D>
#include <QtTypes>

#include <cmath>

#include "Camera/Camera.hpp"
#include "Camera/CameraController.hpp"
#include "Renderer/GeometryEngine.hpp"

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

    const QVector4D lightColor = QVector4D{1.0, 1.0, 1.0, 1.0};
    const float ambientStrength = 0.15f;  // TODO: fix
    const float diffuseStrength = 1.0f;   // TODO: fix
    const float specularStrength = 0.15f; // TODO: fix

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(camera_->getEye(), camera_->getTarget(), camera_->getUp());

    program_.setUniformValue("ambientStrength", ambientStrength);
    program_.setUniformValue("diffuseStrength", diffuseStrength);
    program_.setUniformValue("specularStrength", specularStrength);

    program_.setUniformValue("lightPos", camera_->getEye());
    program_.setUniformValue("viewPos", camera_->getEye());
    program_.setUniformValue("lightColor", lightColor);

    program_.setUniformValue("model_matrix", modelMatrix);
    program_.setUniformValue("view_matrix", viewMatrix);
    program_.setUniformValue("projection_matrix", projection_);

    geometries_->drawModel(&program_);

    geometries_->updateModel();
}
