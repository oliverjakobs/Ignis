#ifndef IGNIS_BUFFER_H
#define IGNIS_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../glad/glad.h"
#include "types.h"

#define IGNIS_BUFFER_OFFSET(offset) ((void*) (offset))

/* Buffer */
typedef struct
{
    GLuint name;
    IgnisBufferTarget target;
} IgnisBuffer;

/* buffer generation */
int ignisGenerateBuffer(IgnisBuffer* buffer, IgnisBufferTarget target);
int ignisGenerateArrayBuffer(IgnisBuffer* buffer, GLsizeiptr size, const void* data, IgnisUsage usage);
int ignisGenerateElementBuffer(IgnisBuffer* buffer, GLsizei count, const GLuint* data, IgnisUsage usage);
int ignisGenerateTextureBuffer(IgnisBuffer* tex_buffer, GLenum format, GLuint buffer);
int ignisGenerateRenderBuffer(IgnisBuffer* buffer);

void ignisDeleteBuffer(IgnisBuffer* buffer);

/* buffer binding */
void ignisBindBuffer(IgnisBuffer* buffer, IgnisBufferTarget target);

/* buffer data */
void ignisBufferData(IgnisBuffer* buffer, GLsizeiptr size, const void* data, IgnisUsage usage);
void ignisBufferSubData(IgnisBuffer* buffer, GLintptr offset, GLsizeiptr size, const void* data);

void ignisElementBufferData(IgnisBuffer* buffer, GLsizei count, const GLuint* data, IgnisUsage usage);

void ignisBindImageTexture(IgnisBuffer* buffer, GLuint unit, GLenum access, GLenum format);
void ignisRenderBufferStorage(IgnisBuffer* buffer, GLenum format, GLsizei width, GLsizei height);

/* buffer mapping */
void* ignisMapBuffer(IgnisBuffer* buffer, GLenum access);
void* ignisMapBufferRange(IgnisBuffer* buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
void ignisUnmapBuffer(IgnisBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_BUFFER_H */