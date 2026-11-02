#include "QOpenGLBufferWrapper.hpp"

#include <QOpenGLBuffer>

QOpenGLBufferWrapper::QOpenGLBufferWrapper(QOpenGLBuffer::Type type) : QOpenGLBuffer(type)
{
}

QOpenGLBufferWrapper::~QOpenGLBufferWrapper()
{
    if (isCreated())
    {
        destroy();
    }
}

QOpenGLBufferWrapper::QOpenGLBufferWrapper(QOpenGLBufferWrapper &&other) noexcept : QOpenGLBuffer(other.type())
{
    QOpenGLBuffer::operator=(static_cast<const QOpenGLBuffer &>(other));

    static_cast<QOpenGLBuffer &>(other) = QOpenGLBuffer();
}

QOpenGLBufferWrapper &QOpenGLBufferWrapper::operator=(QOpenGLBufferWrapper &&other) noexcept
{
    if (this != &other)
    {
        if (isCreated())
        {
            destroy();
        }

        QOpenGLBuffer::operator=(static_cast<const QOpenGLBuffer &>(other));

        static_cast<QOpenGLBuffer &>(other) = QOpenGLBuffer();
    }
    return *this;
}