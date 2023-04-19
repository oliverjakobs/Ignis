#include "renderer.h"

#include "../ignis.h"

/* ---------------------| shader |---------------------------------------------*/
static const char* vert_src = "#version 330 core \n             \
layout(location = 0) in vec2 a_Pos;                             \
layout(location = 1) in vec2 a_TexCoord;                        \
uniform mat4 u_ViewProj;                                        \
uniform mat4 u_Model;                                           \
out vec2 v_TexCoord;                                            \
void main()                                                     \
{                                                               \
    gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 0.0, 1.0); \
    v_TexCoord = a_TexCoord;                                    \
}";

static const char* frag_src = "#version 330 core \n     \
layout(location = 0) out vec4 f_Color;                  \
in vec4 v_Color;                                        \
in vec2 v_TexCoord;                                     \
uniform vec4 u_Color;                                   \
uniform sampler2D u_Texture;                            \
void main()                                             \
{                                                       \
    f_Color = texture(u_Texture, v_TexCoord) * u_Color; \
}";
/* ---------------------| !shader |--------------------------------------------*/

typedef struct
{
    IgnisQuad quad;

    IgnisShader default_shader;
    IgnisShader shader;

    GLint uniform_location_view_proj;
    GLint uniform_location_color;
    GLint uniform_location_model;
} IgnisRenderer2DStorage;

static IgnisRenderer2DStorage render_data;

void ignisRenderer2DInit()
{
    ignisCreateQuadTextured(&render_data.quad, GL_STATIC_DRAW);

    render_data.default_shader = ignisCreateShaderSrcvf(vert_src, frag_src);
    ignisRenderer2DSetShader(0);
}

void ignisRenderer2DDestroy()
{
    ignisDeleteShader(render_data.default_shader);
    ignisDeleteQuad(&render_data.quad);
}

void ignisRenderer2DSetShader(IgnisShader shader)
{
    if (shader) render_data.shader = shader;
    else        render_data.shader = render_data.default_shader;

    render_data.uniform_location_view_proj = ignisGetUniformLocation(render_data.shader, "u_ViewProj");
    render_data.uniform_location_color = ignisGetUniformLocation(render_data.shader, "u_Color");
    render_data.uniform_location_model = ignisGetUniformLocation(render_data.shader, "u_Model");

    ignisSetUniformi(render_data.shader, "u_Texture", 0);
}

void ignisRenderer2DSetViewProjection(const float* view_proj)
{
    ignisSetUniformMat4l(render_data.shader, render_data.uniform_location_view_proj, 1, view_proj);
}

void ignisRenderer2DRenderTexture(const IgnisTexture2D* texture, float x, float y)
{
    ignisRenderer2DRenderTextureColor(texture, x, y, (float)texture->width, (float)texture->height, IGNIS_WHITE);
}

void ignisRenderer2DRenderTextureScale(const IgnisTexture2D* texture, float x, float y, float w, float h)
{
    ignisRenderer2DRenderTextureColor(texture, x, y, w, h, IGNIS_WHITE);
}

void ignisRenderer2DRenderTextureColor(const IgnisTexture2D* texture, float x, float y, float w, float h, IgnisColorRGBA color)
{
    /* creating a mat4 that translates and scales */
    float model[16];
    model[0] = w;       model[4] = 0.0f;    model[8] = 0.0f;    model[12] = x;
    model[1] = 0.0f;    model[5] = h;       model[9] = 0.0f;    model[13] = y;
    model[2] = 0.0f;    model[6] = 0.0f;    model[10] = 1.0f;   model[14] = 0.0f;
    model[3] = 0.0f;    model[7] = 0.0f;    model[11] = 0.0f;   model[15] = 1.0f;

    ignisRenderer2DRenderTextureModel(texture, model, color);
}

void ignisRenderer2DRenderTextureModel(const IgnisTexture2D* texture, const float* model, IgnisColorRGBA color)
{
    ignisUseShader(render_data.shader);

    ignisSetUniformMat4l(render_data.shader, render_data.uniform_location_model, 1, model);
    ignisSetUniform4fl(render_data.shader, render_data.uniform_location_color, 1, &color.r);

    ignisBindTexture2D(texture, 0);

    ignisDrawQuadElements(&render_data.quad, GL_TRIANGLES);
}

/* ---------------------| quad |-----------------------------------------------*/
int ignisCreateQuad(IgnisQuad* quad, GLfloat* vertices, size_t vertex_count, GLenum usage, IgnisBufferElement* layout, size_t layout_size, GLuint* indices, GLsizei element_count)
{
    if (ignisGenerateVertexArray(&quad->vao) == IGNIS_SUCCESS)
    {
        GLsizeiptr size = sizeof(GLfloat) * vertex_count;
        if (ignisAddArrayBufferLayout(&quad->vao, size, vertices, usage, 0, layout, layout_size) == IGNIS_SUCCESS)
        {
            quad->vertex_count = vertex_count;
            return ignisLoadElementBuffer(&quad->vao, indices, element_count, GL_STATIC_DRAW);
        }
    }

    return IGNIS_FAILURE;
}

int ignisCreateQuadTextured(IgnisQuad* quad, GLenum usage)
{
    GLfloat vertices[] =
    {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f
    };

    IgnisBufferElement layout[] =
    {
        {GL_FLOAT, 2, GL_FALSE},
        {GL_FLOAT, 2, GL_FALSE}
    };

    GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
    return ignisCreateQuad(quad, vertices, 4 * 4, usage, layout, 2, indices, 6);
}

void ignisGenerateQuadIndices(GLuint* indices, size_t max)
{
    GLuint offset = 0;
    for (size_t i = 0; i < max - 6; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }
}

void ignisDeleteQuad(IgnisQuad* quad)
{
    ignisDeleteVertexArray(&quad->vao);
}

void ignisBindQuad(IgnisQuad* quad)
{
    ignisBindVertexArray((quad) ? &quad->vao : NULL);
}

void ignisDrawQuadElements(IgnisQuad* quad, GLenum mode)
{
    ignisBindQuad(quad);
    glDrawElements(mode, quad->vao.element_count, GL_UNSIGNED_INT, NULL);
}

void ignisDrawQuadElementsInstanced(IgnisQuad* quad, GLenum mode, GLsizei primcount)
{
    ignisBindQuad(quad);
    glDrawElementsInstanced(mode, quad->vao.element_count, GL_UNSIGNED_INT, NULL, primcount);
}

/* ---------------------| rect |-----------------------------------------------*/
IgnisRect ignisGetTexture2DSrcRect(const IgnisTexture2D* texture, uint32_t cols, uint32_t rows, uint32_t frame)
{
    IgnisRect src;
    src.w = 1.0f / cols;
    src.h = 1.0f / rows;
    src.x = (frame % cols) * src.w;
    src.y = 1 - src.h - ((frame / cols) * src.h);
    return src;
}

/* ---------------------| color |----------------------------------------------*/
IgnisColorRGBA IGNIS_WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
IgnisColorRGBA IGNIS_BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
IgnisColorRGBA IGNIS_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
IgnisColorRGBA IGNIS_GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
IgnisColorRGBA IGNIS_BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
IgnisColorRGBA IGNIS_CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };
IgnisColorRGBA IGNIS_MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };
IgnisColorRGBA IGNIS_YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };

IgnisColorRGBA IGNIS_DARK_GREY = { 0.2f, 0.2f, 0.2f, 1.0f };
IgnisColorRGBA IGNIS_LIGHT_GREY = { 0.75f, 0.75f, 0.75f, 1.0f };

IgnisColorRGBA* ignisBlendColorRGBA(IgnisColorRGBA* color, float alpha)
{
    color->a = alpha;
    return color;
}