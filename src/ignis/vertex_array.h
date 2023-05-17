#ifndef IGNIS_VERTEX_ARRAY_H
#define IGNIS_VERTEX_ARRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core/buffer.h"

typedef struct
{
    IgnisType type;
    uint32_t count;
    GLboolean normalized;
} IgnisBufferElement;

typedef struct
{
    GLuint name;

    IgnisBuffer* buffers;
    size_t buffer_count;
} IgnisVertexArray;

int ignisGenerateVertexArray(IgnisVertexArray* vao, size_t buffer_count);
void ignisDeleteVertexArray(IgnisVertexArray* vao);

void ignisBindVertexArray(IgnisVertexArray* vao);

int ignisLoadArrayBuffer(IgnisVertexArray* vao, size_t index, size_t size, const void* data, IgnisUsage usage);
int ignisLoadElementBuffer(IgnisVertexArray* vao, size_t index, const GLuint* indices, size_t count, IgnisUsage usage);

int ignisSetVertexLayout(IgnisVertexArray* vao, GLuint attrib_index, IgnisBufferElement* layout, size_t count);

/* vertex attrib pointer */
void ignisVertexAttribPointer(GLuint index, GLint size, IgnisType type, GLboolean normalized, GLsizei stride, const void* offset);
void ignisVertexAttribIPointer(GLuint index, GLint size, IgnisType type, GLsizei stride, const void* offset);
void ignisVertexAttribLPointer(GLuint index, GLint size, IgnisType type, GLsizei stride, const void* offset);

void ignisVertexAttribDivisor(GLuint index, GLuint divisor);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_VERTEX_ARRAY_H */