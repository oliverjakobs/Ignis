#include "buffer.h"

#include "../Ignis.h"

int ignisGenerateBuffer(IgnisBuffer* buffer, IgnisBufferTarget target)
{
    GLuint name = 0;

    switch (target)
    {
    case IGNIS_TEXTURE_BUFFER:
        glGenTextures(1, &name);
        break;
    case IGNIS_RENDERBUFFER:
        glGenRenderbuffers(1, &name);
        break;
    case IGNIS_ARRAY_BUFFER:
    case IGNIS_ELEMENT_ARRAY_BUFFER:
        glGenBuffers(1, &name);
        break;
    default:
        IGNIS_ERROR("[Buffer] Unsupported buffer target (%d)", buffer->target);
        return IGNIS_FAILURE;
    }

    if (buffer)
    {
        buffer->name = name;
        buffer->target = target;
    }

    return name;
}

int ignisGenerateArrayBuffer(IgnisBuffer* buffer, GLsizeiptr size, const void* data, IgnisUsage usage)
{
    if (buffer && ignisGenerateBuffer(buffer, IGNIS_ARRAY_BUFFER))
    {
        ignisBufferData(buffer, size, data, usage);
        return IGNIS_SUCCESS;
    }

    return IGNIS_FAILURE;
}

int ignisGenerateElementBuffer(IgnisBuffer* buffer, GLsizei count, const GLuint* data, IgnisUsage usage)
{
    if (buffer && ignisGenerateBuffer(buffer, IGNIS_ELEMENT_ARRAY_BUFFER))
    {
        ignisElementBufferData(buffer, count, data, usage);
        return IGNIS_SUCCESS;
    }

    return IGNIS_FAILURE;
}

int ignisGenerateTextureBuffer(IgnisBuffer* tex_buffer, GLenum format, GLuint buffer)
{
    if (tex_buffer && ignisGenerateBuffer(tex_buffer, GL_TEXTURE_BUFFER))
    {
        glBindTexture(tex_buffer->target, tex_buffer->name);
        glTexBuffer(tex_buffer->target, format, buffer);
        return IGNIS_SUCCESS;
    }

    return IGNIS_FAILURE;
}

int ignisGenerateRenderBuffer(IgnisBuffer* buffer)
{
    if (buffer && ignisGenerateBuffer(buffer, IGNIS_RENDERBUFFER))
        return IGNIS_SUCCESS;

    return IGNIS_FAILURE;
}

void ignisDeleteBuffer(IgnisBuffer* buffer)
{
    if (!buffer->name)
    {
        IGNIS_WARN("[Buffer] Trying to delete uninitialized buffer");
        return;
    }

    switch (buffer->target)
    {
    case IGNIS_TEXTURE_BUFFER:
        glDeleteTextures(1, &buffer->name);
        break;
    case IGNIS_RENDERBUFFER:
        glBindRenderbuffer(buffer->target, 0);
        glDeleteRenderbuffers(1, &buffer->name);
        break;
    case IGNIS_ARRAY_BUFFER:
    case IGNIS_ELEMENT_ARRAY_BUFFER:
        glBindBuffer(buffer->target, 0);
        glDeleteBuffers(1, &buffer->name);
        break;
    default:
        IGNIS_ERROR("[Buffer] Unsupported buffer target (%d)", buffer->target);
    }

    buffer->name = 0;
    buffer->target = 0;
}

void ignisBindBuffer(IgnisBuffer* buffer, IgnisBufferTarget target)
{
    if (buffer && buffer->target != target)
    {
        IGNIS_ERROR("[Buffer] Buffer target missmatch (%d)", target);
        return;
    }

    GLuint name = buffer ? buffer->name : 0;
    switch (target)
    {
    case IGNIS_RENDERBUFFER:
        glBindRenderbuffer(target, name);
        return;
    case IGNIS_TEXTURE_BUFFER:
        glBindTexture(target, name);
    case IGNIS_ARRAY_BUFFER:
    case IGNIS_ELEMENT_ARRAY_BUFFER:
        glBindBuffer(target, name);
        return;
    }

    IGNIS_ERROR("[Buffer] Unsupported buffer target (%d)", target);
}

void ignisBufferData(IgnisBuffer* buffer, GLsizeiptr size, const void* data, IgnisUsage usage)
{
    ignisBindBuffer(buffer, buffer->target);
    glBufferData(buffer->target, size, data, usage);
}

void ignisBufferSubData(IgnisBuffer* buffer, GLintptr offset, GLsizeiptr size, const void* data)
{
    ignisBindBuffer(buffer, buffer->target);
    glBufferSubData(buffer->target, offset, size, data);
}

void ignisElementBufferData(IgnisBuffer* buffer, GLsizei count, const GLuint* data, IgnisUsage usage)
{
    if (buffer->target != IGNIS_ELEMENT_ARRAY_BUFFER)
    {
        IGNIS_ERROR("[Buffer] Buffer target is not IGNIS_ELEMENT_ARRAY_BUFFER");
        return;
    }

    ignisBindBuffer(buffer, buffer->target);
    glBufferData(buffer->target, count * sizeof(GLuint), data, usage);
}

void ignisBindImageTexture(IgnisBuffer* buffer, GLuint unit, GLenum access, GLenum format)
{
    if (buffer->target != IGNIS_TEXTURE_BUFFER)
    {
        IGNIS_ERROR("[Buffer] Buffer target is not GL_TEXTURE_BUFFER");
        return;
    }

    glBindImageTexture(unit, buffer->name, 0, GL_FALSE, 0, access, format);
}

void ignisRenderBufferStorage(IgnisBuffer* buffer, GLenum format, GLsizei width, GLsizei height)
{
    if (buffer->target != IGNIS_RENDERBUFFER)
    {
        IGNIS_ERROR("[Buffer] Buffer target is not GL_RENDERBUFFER");
        return;
    }

    ignisBindBuffer(buffer, buffer->target);
    glRenderbufferStorage(buffer->target, format, width, height);
}

void* ignisMapBuffer(IgnisBuffer* buffer, GLenum access)
{
    ignisBindBuffer(buffer, buffer->target);
    return glMapBuffer(buffer->target, access);
}

void* ignisMapBufferRange(IgnisBuffer* buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
    ignisBindBuffer(buffer, buffer->target);
    return glMapBufferRange(buffer->target, offset, length, access);
}

void ignisUnmapBuffer(IgnisBuffer* buffer)
{
    if (glUnmapBuffer(buffer->target) == GL_FALSE)
        IGNIS_CRITICAL("[Buffer] Buffer data has become corrupt");
}