#ifndef IGNIS_VERTEX_ARRAY_H
#define IGNIS_VERTEX_ARRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core/buffer.h"

typedef struct
{
    GLenum type;
    GLsizei count;
    GLboolean normalized;
} IgnisBufferElement;

typedef struct
{
    GLuint name;

    /* dynamic buffer array */
    IgnisBuffer* array_buffers;
    size_t array_buffer_count;

    IgnisBuffer element_buffer;
    size_t element_count;
} IgnisVertexArray;

int ignisGenerateVertexArray(IgnisVertexArray* vao, size_t buffer_count);
void ignisDeleteVertexArray(IgnisVertexArray* vao);

void ignisBindVertexArray(IgnisVertexArray* vao);

int ignisLoadArrayBuffer(IgnisVertexArray* vao, size_t index, GLsizeiptr size, const void* data, GLenum usage);
int ignisLoadArrayBufferLayout(IgnisVertexArray* vao, size_t index, GLsizeiptr size, const void* data, GLenum usage, GLuint vertex_attrib_index, IgnisBufferElement* layout, size_t count);
int ignisLoadElementBuffer(IgnisVertexArray* vao, const GLuint* indices, GLsizei count, GLenum usage);

/* vertex attrib pointer */
void ignisVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);
void ignisVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset);
void ignisVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset);

void ignisVertexAttribFloat(GLuint index, GLint size, GLboolean normalized, GLsizei stride, GLintptr offset);

void ignisVertexAttribUnsignedByte(GLuint index, GLint size, GLsizei stride, GLintptr offset);
void ignisVertexAttribUnsignedShort(GLuint index, GLint size, GLsizei stride, GLintptr offset);
void ignisVertexAttribUnsignedInt(GLuint index, GLint size, GLsizei stride, GLintptr offset);
void ignisVertexAttribByte(GLuint index, GLint size, GLsizei stride, GLintptr offset);
void ignisVertexAttribShort(GLuint index, GLint size, GLsizei stride, GLintptr offset);
void ignisVertexAttribInt(GLuint index, GLint size, GLsizei stride, GLintptr offset); 

void ignisVertexAttribDouble(GLuint index, GLint size, GLsizei stride, GLintptr offset);

void ignisVertexAttribDivisor(GLuint index, GLuint divisor);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_VERTEX_ARRAY_H */