#ifndef IGNIS_BUFFER_H
#define IGNIS_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../glad/glad.h"

#define IGNIS_BUFFER_OFFSET(offset) ((void*) (offset))

/* Buffer */
typedef struct
{
	GLuint name;
	GLenum target;
} IgnisBuffer;

/* buffer generation */
int ignisGenerateBuffer(IgnisBuffer* buffer, GLenum target);
int ignisGenerateArrayBuffer(IgnisBuffer* buffer, GLsizeiptr size, const void* data, GLenum usage);
int ignisGenerateElementBuffer(IgnisBuffer* buffer, GLsizei count, const GLuint* data, GLenum usage);
int ignisGenerateTextureBuffer(IgnisBuffer* tex_buffer, GLenum format, GLuint buffer);
int ignisGenerateRenderBuffer(IgnisBuffer* buffer);

void ignisDeleteBuffer(IgnisBuffer* buffer);

/* buffer binding */
void ignisBindBuffer(IgnisBuffer* buffer, GLenum target);

/* buffer data */
void ignisBufferData(IgnisBuffer* buffer, GLsizeiptr size, const void* data, GLenum usage);
void ignisBufferSubData(IgnisBuffer* buffer, GLintptr offset, GLsizeiptr size, const void* data);

void ignisElementBufferData(IgnisBuffer* buffer, GLsizei count, const GLuint* data, GLenum usage);

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