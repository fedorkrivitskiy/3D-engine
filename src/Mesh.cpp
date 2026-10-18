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

Mesh::Mesh() : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    indexBuf.create();
}

void Mesh::setVertices(const std::vector<std::vector<VertexData>> &vertices)
{
    arrayBuf.clear();

    std::ranges::transform(vertices, std::back_inserter(arrayBuf), [](const std::vector<VertexData> &frameData) {
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
    indexBuf.bind();
    indexBuf.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(Index)));
    indexBuf.release();
}

void Mesh::setTexture(const QString &path)
{
    texture = std::make_unique<QOpenGLTexture>(QImage(path).flipped());

    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Mesh::draw(QOpenGLShaderProgram *program, int frameIndex)
{
    const int vertexLocation = program->attributeLocation("a_position");
    const int texcoordLocation = program->attributeLocation("a_texcoord");

    program->enableAttributeArray(vertexLocation);
    program->enableAttributeArray(texcoordLocation);

    arrayBuf[frameIndex].bind();
    indexBuf.bind();

    texture->bind(0);
    program->setUniformValue("texture", 0);

    quintptr offset = 0;

    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    const int count = static_cast<int>(indexBuf.size() / sizeof(Index));
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

    program->disableAttributeArray(vertexLocation);
    program->disableAttributeArray(texcoordLocation);

    indexBuf.release();
    arrayBuf[frameIndex].release();
}