#include "RenderWidget.hpp"

#include <QMouseEvent>
#include <QOpenGLShader>
#include <QQuaternion>
#include <QVector2D>

#include <cmath>
#include <memory>

#include "Camera/Camera.hpp"
#include "Camera/CameraController.hpp"
#include "Core/Core.hpp"
#include "Core/Settings/SettingsModel.hpp"
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

    geometries_ = std::make_unique<GeometryEngine>();
}

void RenderWidget::initShaders()
{
    if (!program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl")) {
        close();
    }

    if (!program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl")) {
        close();
    }

    if (!program_.link()) {
        close();
    }

    if (!program_.bind()) {
        close();
    }
}

QMatrix4x4 RenderWidget::getProjectionMatrix()
{
    QMatrix4x4 projection;
    projection.setToIdentity();

    SettingsModel *settingsModel = Core::get().getSettingsModel();

    auto [w, h] = size();
    float aspectRatio = static_cast<float>(w) / static_cast<float>(h > 0 ? h : 1);

    projection.perspective(settingsModel->getFov(), aspectRatio, settingsModel->getZNear(), settingsModel->getZFar());

    return projection;
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

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(camera_->getEye(), camera_->getTarget(), camera_->getUp());

    QMatrix4x4 projection = getProjectionMatrix();

    SettingsModel *settingsModel = Core::get().getSettingsModel();

    program_.setUniformValue("ambientStrength", settingsModel->getKAmbient());
    program_.setUniformValue("diffuseStrength", settingsModel->getKDiffuse());
    program_.setUniformValue("specularStrength", settingsModel->getKSpecular());

    program_.setUniformValue("lightPos", camera_->getEye());
    program_.setUniformValue("viewPos", camera_->getEye());

    QColor c = settingsModel->getLightColor();
    QVector4D lightColorVec(c.redF(), c.greenF(), c.blueF(), c.alphaF());
    program_.setUniformValue("lightColor", lightColorVec);

    program_.setUniformValue("model_matrix", modelMatrix);
    program_.setUniformValue("view_matrix", viewMatrix);
    program_.setUniformValue("projection_matrix", projection);

    geometries_->drawModel(&program_);

    geometries_->updateModel();
}
