#include "renderer.h"

#include "../ignis.h"

typedef struct
{
    IgnisShader shader;
    GLint uniform_location_view_proj;

    IgnisVertexArray vao;
    float vertices[IGNIS_BATCH2D_BUFFER_SIZE];
    uint32_t vertex_index;

    GLsizei quad_count;

    GLuint texture_slots[IGNIS_BATCH2D_TEXTURES];
    uint32_t texture_slot_index;
} IgnisBatch2DStorage;

static IgnisBatch2DStorage render_data;

void ignisBatch2DInit(const char* vert, const char* frag)
{
    ignisGenerateVertexArray(&render_data.vao);

    IgnisBufferElement layout[] =
    {
        {GL_FLOAT, 3, GL_FALSE},    /* position */
        {GL_FLOAT, 2, GL_FALSE},    /* Texture coords*/
        {GL_FLOAT, 1, GL_FALSE}     /* Texture index */
    };

    ignisAddArrayBufferLayout(&render_data.vao, IGNIS_BATCH2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, 0, layout, 3);

    GLuint indices[IGNIS_BATCH2D_INDEX_COUNT];
    ignisGenerateQuadIndices(indices, IGNIS_BATCH2D_INDEX_COUNT);

    ignisLoadElementBuffer(&render_data.vao, indices, IGNIS_BATCH2D_INDEX_COUNT, GL_STATIC_DRAW);

    render_data.shader = ignisCreateShadervf(vert, frag);

    ignisUseShader(render_data.shader);
    int samplers[IGNIS_BATCH2D_TEXTURES];
    for (int i = 0; i < IGNIS_BATCH2D_TEXTURES; i++)
        samplers[i] = i;

    ignisSetUniform1i(render_data.shader, "u_Textures", IGNIS_BATCH2D_TEXTURES, samplers);

    render_data.uniform_location_view_proj = ignisGetUniformLocation(render_data.shader, "u_ViewProjection");

    render_data.vertex_index = 0;
    render_data.quad_count = 0;

    render_data.texture_slot_index = 0;
}

void ignisBatch2DDestroy()
{
    ignisDeleteShader(render_data.shader);
    ignisDeleteVertexArray(&render_data.vao);
}

void ignisBatch2DSetViewProjection(const float* view_proj)
{
    ignisSetUniformMat4l(render_data.shader, render_data.uniform_location_view_proj, 1, view_proj);
}

void ignisBatch2DFlush()
{
    if (render_data.vertex_index == 0) return;

    ignisBindVertexArray(&render_data.vao);
    ignisBufferSubData(&render_data.vao.array_buffers[0], 0, render_data.vertex_index * sizeof(float), render_data.vertices);

    ignisUseShader(render_data.shader);

    /* bind textures */
    for (size_t i = 0; i < render_data.texture_slot_index; i++)
    {
        glActiveTexture(GL_TEXTURE0 + (GLenum)i);
        glBindTexture(GL_TEXTURE_2D, render_data.texture_slots[i]);
    }

    glDrawElements(GL_TRIANGLES, IGNIS_INDICES_PER_QUAD * render_data.quad_count, GL_UNSIGNED_INT, NULL);

    render_data.vertex_index = 0;
    render_data.quad_count = 0;

    /* textures */
    render_data.texture_slot_index = 0;
}

void ignisBatch2DPushVertex(float x, float y, float src_x, float src_y, uint32_t texture_index)
{
    render_data.vertices[render_data.vertex_index++] = x;
    render_data.vertices[render_data.vertex_index++] = y;
    render_data.vertices[render_data.vertex_index++] = 0.0f;

    render_data.vertices[render_data.vertex_index++] = src_x;
    render_data.vertices[render_data.vertex_index++] = src_y;

    render_data.vertices[render_data.vertex_index++] = (float)texture_index;
}

static int ignisFindTexture(GLuint name, uint32_t* index)
{
    for (uint32_t i = 0; i < render_data.texture_slot_index; i++)
    {
        if (render_data.texture_slots[i] == name)
        {
            *index = i;
            return 1;
        }
    }
    return 0;
}

void ignisBatch2DRenderTexture(const IgnisTexture2D* texture, IgnisRect rect)
{
    ignisBatch2DRenderTextureSrc(texture, rect, (IgnisRect) { 0.0f, 0.0f, 1.0f, 1.0f });
}

void ignisBatch2DRenderTextureFrame(const IgnisTexture2D* texture, IgnisRect rect, uint32_t frame)
{
    ignisBatch2DRenderTextureSrc(texture, rect, ignisGetTexture2DSrcRect(texture, 1, 1, frame));
}

void ignisBatch2DRenderTextureSrc(const IgnisTexture2D* texture, IgnisRect rect, IgnisRect src)
{
    if (render_data.vertex_index + IGNIS_BATCH2D_QUAD_SIZE >= IGNIS_BATCH2D_BUFFER_SIZE)
        ignisBatch2DFlush();

    uint32_t texture_index = 0;
    if (!ignisFindTexture(texture->name, &texture_index))
    {
        texture_index = render_data.texture_slot_index;

        if (render_data.texture_slot_index >= IGNIS_BATCH2D_TEXTURES)
        {
            ignisBatch2DFlush();
            render_data.texture_slot_index = 0; // supress warning
        }

        render_data.texture_slots[render_data.texture_slot_index++] = texture->name;
    }

    ignisBatch2DPushVertex(rect.x,          rect.y,          src.x,         src.y,         texture_index);
    ignisBatch2DPushVertex(rect.x + rect.w, rect.y,          src.x + src.w, src.y,         texture_index);
    ignisBatch2DPushVertex(rect.x + rect.w, rect.y + rect.h, src.x + src.w, src.y + src.h, texture_index);
    ignisBatch2DPushVertex(rect.x,          rect.y + rect.h, src.x,         src.y + src.h, texture_index);

    render_data.quad_count++;
}
