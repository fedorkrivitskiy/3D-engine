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

    void draw(QOpenGLShaderProgram *program, int frameIndex);

  private:
    std::vector<QOpenGLBufferWrapper> arrayBuf_;
    QOpenGLBufferWrapper indexBuf_;

    std::unique_ptr<QOpenGLTexture> colorTexture_;
    std::unique_ptr<QOpenGLTexture> normalTexture_;
};
