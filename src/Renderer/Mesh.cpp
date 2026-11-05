#include "Mesh.hpp"

#include <QImage>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>

#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

#include "QOpenGLBufferWrapper.hpp"

namespace
{
std::unique_ptr<QOpenGLTexture> loadTextureFromFile(const QString &path)
{
    std::unique_ptr<QOpenGLTexture> texture = std::make_unique<QOpenGLTexture>(QImage(path).flipped());

    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}
} // namespace

Mesh::Mesh() : indexes_(QOpenGLBuffer::IndexBuffer)
{
    indexes_.create();
}

void Mesh::setVertices(const std::vector<std::vector<VertexData>> &vertices)
{
    vertices_.clear();

    std::ranges::transform(vertices, std::back_inserter(vertices_), [](const std::vector<VertexData> &frameData) {
        QOpenGLBufferWrapper buf(QOpenGLBuffer::VertexBuffer);
        buf.create();
        buf.bind();
        buf.allocate(frameData.data(), static_cast<int>(frameData.size() * sizeof(VertexData)));
        buf.release();

        return buf;
    });
}

void Mesh::setIndices(const std::vector<Index> &indices)
{
    indexes_.bind();
    indexes_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(Index)));
    indexes_.release();
}

void Mesh::setColorTexture(const QString &path)
{
    colorTexture_ = loadTextureFromFile(path);
}

void Mesh::setNormalsTexture(const QString &path)
{
    normalTexture_ = loadTextureFromFile(path);
}

void Mesh::setGlossinessTexture(const QString &path)
{
    glossinessTexture_ = loadTextureFromFile(path);
}

void Mesh::setSpecularTexture(const QString &path)
{
    specularTexture_ = loadTextureFromFile(path);
}

void Mesh::draw(QOpenGLShaderProgram *program, int frameIndex)
{
    const int vertexLocation = program->attributeLocation("a_position");
    const int texcoordLocation = program->attributeLocation("a_texcoord");
    const int normalLocation = program->attributeLocation("a_normal");
    const int tangentLocation = program->attributeLocation("a_tangent");

    program->enableAttributeArray(vertexLocation);
    program->enableAttributeArray(texcoordLocation);
    program->enableAttributeArray(normalLocation);
    program->enableAttributeArray(tangentLocation);

    vertices_[frameIndex].bind();
    indexes_.bind();

    colorTexture_->bind(0);
    program->setUniformValue("colorTexture", 0);

    normalTexture_->bind(1);
    program->setUniformValue("normalTexture", 1);

    glossinessTexture_->bind(2);
    program->setUniformValue("glossinessTexture", 2);

    specularTexture_->bind(3);
    program->setUniformValue("specularTexture", 3);

    quintptr offset = 0;

    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    program->setAttributeBuffer(tangentLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    const int count = static_cast<int>(indexes_.size() / sizeof(Index));
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

    program->disableAttributeArray(vertexLocation);
    program->disableAttributeArray(texcoordLocation);
    program->disableAttributeArray(normalLocation);
    program->disableAttributeArray(tangentLocation);

    indexes_.release();
    vertices_[frameIndex].release();
}