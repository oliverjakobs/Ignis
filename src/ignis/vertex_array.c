#include "vertex_array.h"

#include "ignis.h"

int ignisGenerateVertexArray(IgnisVertexArray* vao, size_t buffer_count)
{
    if (!vao || vao->name) return IGNIS_FAILURE;

    vao->buffers = ignisMalloc(buffer_count * sizeof(IgnisBuffer));
    if (!vao->buffers)
    {
        IGNIS_ERROR("[VertexArray] Failed to allocate memeory for buffers");
        return IGNIS_FAILURE;
    }

    memset(vao->buffers, 0, buffer_count * sizeof(IgnisBuffer));
    vao->buffer_count = buffer_count;

    glGenVertexArrays(1, &vao->name);
    glBindVertexArray(vao->name);

    return IGNIS_SUCCESS;
}

void ignisDeleteVertexArray(IgnisVertexArray* vao)
{
    for (size_t i = 0; i < vao->buffer_count; ++i)
        if (vao->buffers[i].name) ignisDeleteBuffer(&vao->buffers[i]);

    ignisFree(vao->buffers);
    vao->buffer_count = 0;

    glDeleteVertexArrays(1, &vao->name);
    glBindVertexArray(0);
}

void ignisBindVertexArray(IgnisVertexArray* vao)
{
    glBindVertexArray((vao) ? vao->name : 0);
}

int ignisLoadArrayBuffer(IgnisVertexArray* vao, size_t index, size_t size, const void* data, IgnisUsage usage)
{
    if (index >= vao->buffer_count)
    {
        IGNIS_ERROR("[VertexArray] Buffer index (%d) out if bounds", index);
        return IGNIS_FAILURE;
    }

    ignisBindVertexArray(vao);

    if (ignisGenerateArrayBuffer(&vao->buffers[index], size, data, usage) != IGNIS_SUCCESS)
    {
        IGNIS_ERROR("[VertexArray] Failed to generate array buffer");
        return IGNIS_FAILURE;
    }

    return IGNIS_SUCCESS;
}

int ignisLoadElementBuffer(IgnisVertexArray* vao, size_t index, const GLuint* indices, size_t count, IgnisUsage usage)
{
    if (index >= vao->buffer_count)
    {
        IGNIS_ERROR("[VertexArray] Buffer index (%d) out if bounds", index);
        return IGNIS_FAILURE;
    }

    ignisBindVertexArray(vao);

    if (ignisGenerateElementBuffer(&vao->buffers[index], count, indices, usage) != IGNIS_SUCCESS)
    {
        IGNIS_ERROR("[VertexArray] Failed to generate element buffer");
        return IGNIS_FAILURE;
    }

    return IGNIS_SUCCESS;
}

int ignisSetVertexLayout(IgnisVertexArray* vao, GLuint attrib_index, IgnisBufferElement* layout, size_t count)
{
    size_t stride = 0;
    for (size_t i = 0; i < count; i++)
        stride += ignisGetTypeSize(layout[i].type) * layout[i].count;

    size_t offset = 0;
    for (size_t i = 0; i < count; i++)
    {
        ignisVertexAttribPointer(attrib_index, layout[i].count, layout[i].type, layout[i].normalized, stride, IGNIS_BUFFER_OFFSET(offset));

        offset += ignisGetTypeSize(layout[i].type) * layout[i].count;
        attrib_index++;
    }

    return IGNIS_SUCCESS;
}

void ignisVertexAttribPointer(GLuint index, GLint size, IgnisType type, GLboolean normalized, GLsizei stride, const void* offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, offset);
}

void ignisVertexAttribIPointer(GLuint index, GLint size, IgnisType type, GLsizei stride, const void* offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribIPointer(index, size, type, stride, offset);
}

void ignisVertexAttribLPointer(GLuint index, GLint size, IgnisType type, GLsizei stride, const void* offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribLPointer(index, size, type, stride, offset);
}

void ignisVertexAttribDivisor(GLuint index, GLuint divisor)
{
    glVertexAttribDivisor(index, divisor);
}