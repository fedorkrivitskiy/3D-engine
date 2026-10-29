#include "RenderWidget.hpp"

#include <QMouseEvent>
#include <QOpenGLShader>
#include <QQuaternion>
#include <QVector2D>
#include <QtTypes>

#include <cmath>

#include "Camera.hpp"

RenderWidget::~RenderWidget()
{
    makeCurrent();

    delete geometries;

    doneCurrent();
}

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();

    geometries = new GeometryEngine();
}

void RenderWidget::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    if (!program.link())
        close();

    if (!program.bind())
        close();
}

void RenderWidget::setCamera(Camera *camera)
{
    camera_ = camera;
}

void RenderWidget::setZNear(float zNear)
{
    zNear_ = zNear;

    updateProjectionMatrix();
}

void RenderWidget::setZFar(float zFar)
{
    zFar_ = zFar;

    updateProjectionMatrix();
}

void RenderWidget::setFov(float fov)
{
    fov_ = fov;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    program.bind();

    if (!camera_)
    {
        return;
    }

    QMatrix4x4 cameraMatrix;
    cameraMatrix.lookAt(camera_->getEye(), camera_->getTarget(), camera_->getUp());

    program.setUniformValue("mvp_matrix", projection_ * cameraMatrix);

    geometries->drawModel(&program);

    geometries->updateModel();
}
