#pragma once

#include <QOpenGLBuffer>

class QOpenGLBufferWrapper : public QOpenGLBuffer
{
  public:
    QOpenGLBufferWrapper() = default;
    explicit QOpenGLBufferWrapper(QOpenGLBuffer::Type type);

    ~QOpenGLBufferWrapper();

    QOpenGLBufferWrapper(const QOpenGLBufferWrapper &) = delete;
    QOpenGLBufferWrapper &operator=(const QOpenGLBufferWrapper &) = delete;

    QOpenGLBufferWrapper(QOpenGLBufferWrapper &&other) noexcept;
    QOpenGLBufferWrapper &operator=(QOpenGLBufferWrapper &&other) noexcept;
};