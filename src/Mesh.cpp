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

Mesh::Mesh() : indexBuf_(QOpenGLBuffer::IndexBuffer)
{
    indexBuf_.create();
}

void Mesh::setVertices(const std::vector<std::vector<VertexData>> &vertices)
{
    arrayBuf_.clear();

    std::ranges::transform(vertices, std::back_inserter(arrayBuf_), [](const std::vector<VertexData> &frameData) {
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
    indexBuf_.bind();
    indexBuf_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(Index)));
    indexBuf_.release();
}

void Mesh::setColorTexture(const QString &path)
{
    colorTexture_ = std::make_unique<QOpenGLTexture>(QImage(path).flipped());

    colorTexture_->setMinificationFilter(QOpenGLTexture::Nearest);
    colorTexture_->setMagnificationFilter(QOpenGLTexture::Linear);
    colorTexture_->setWrapMode(QOpenGLTexture::Repeat);
}

void Mesh::setNormalsTexture(const QString &path)
{
    normalTexture_ = std::make_unique<QOpenGLTexture>(QImage(path).flipped());

    normalTexture_->setMinificationFilter(QOpenGLTexture::Nearest);
    normalTexture_->setMagnificationFilter(QOpenGLTexture::Linear);
    normalTexture_->setWrapMode(QOpenGLTexture::Repeat);
}

void Mesh::draw(QOpenGLShaderProgram *program, int frameIndex)
{
    const int vertexLocation = program->attributeLocation("a_position");
    const int texcoordLocation = program->attributeLocation("a_texcoord");

    program->enableAttributeArray(vertexLocation);
    program->enableAttributeArray(texcoordLocation);

    arrayBuf_[frameIndex].bind();
    indexBuf_.bind();

    colorTexture_->bind(0);
    program->setUniformValue("colorTexture", 0);

    // normalTexture_->bind(1);
    // program->setUniformValue("normalTexture", 1);

    quintptr offset = 0;

    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    const int count = static_cast<int>(indexBuf_.size() / sizeof(Index));
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

    program->disableAttributeArray(vertexLocation);
    program->disableAttributeArray(texcoordLocation);

    indexBuf_.release();
    arrayBuf_[frameIndex].release();
}