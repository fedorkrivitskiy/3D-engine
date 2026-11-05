#pragma once

#include <QString>
#include <QVector2D>
#include <QVector3D>

#include <vector>

#include "QOpenGLBufferWrapper.hpp"

class QOpenGLTexture;
class QOpenGLShaderProgram;

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
    QVector3D tangent;
};

using Index = GLuint;

class Mesh final
{
  public:
    Mesh();

    void setVertices(const std::vector<std::vector<VertexData>> &vertices);
    void setIndices(const std::vector<Index> &indices);

    void setColorTexture(const QString &path);
    void setNormalsTexture(const QString &path);
    void setGlossinessTexture(const QString &path);
    void setSpecularTexture(const QString &path);

    void draw(QOpenGLShaderProgram *program, int frameIndex);

  private:
    std::vector<QOpenGLBufferWrapper> vertices_;
    QOpenGLBufferWrapper indexes_;

    std::unique_ptr<QOpenGLTexture> colorTexture_;
    std::unique_ptr<QOpenGLTexture> normalTexture_;
    std::unique_ptr<QOpenGLTexture> glossinessTexture_;
    std::unique_ptr<QOpenGLTexture> specularTexture_;
};
