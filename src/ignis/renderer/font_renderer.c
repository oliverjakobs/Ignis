#include "renderer.h"

#include "../ignis.h"

/* ---------------------| shader |---------------------------------------------*/
static const char* vert_src = "#version 330 core \n \
layout(location = 0) in vec2 a_Pos;                 \
layout(location = 1) in vec2 a_TexCoord;            \
uniform mat4 u_Proj;                                \
out vec2 v_TexCoord;                                \
void main()                                         \
{                                                   \
    gl_Position = u_Proj * vec4(a_Pos, 0.0, 1.0);   \
    v_TexCoord = a_TexCoord;                        \
}";

static const char* frag_src = "#version 330 core \n \
layout(location = 0) out vec4 f_Color;              \
uniform sampler2D u_Tex;                            \
uniform vec4 u_Color;                               \
in vec2 v_TexCoord;                                 \
void main()                                         \
{                                                   \
    f_Color = u_Color * texture(u_Tex, v_TexCoord); \
}";
/* ---------------------| !shader |--------------------------------------------*/

typedef struct 
{
    IgnisVertexArray vao;
    IgnisShader shader;

    IgnisFont* font;
    IgnisColorRGBA color;

    float vertices[IGNIS_FONTRENDERER_BUFFER_SIZE];
    size_t index;
    GLsizei quad_count;

    char line_buffer[IGNIS_FONTRENDERER_MAX_LINE_LENGTH];

    GLint uniform_location_proj;
    GLint uniform_location_color;
} IgnisFontRendererStorage;

static IgnisFontRendererStorage render_data;

void ignisFontRendererInit()
{
    ignisGenerateVertexArray(&render_data.vao, 2);

    size_t size = IGNIS_FONTRENDERER_BUFFER_SIZE * sizeof(float);
    ignisLoadArrayBuffer(&render_data.vao, 0, size, NULL, IGNIS_DYNAMIC_DRAW);

    IgnisBufferElement layout[] = { {IGNIS_FLOAT, 2, GL_FALSE}, {IGNIS_FLOAT, 2, GL_FALSE} };
    ignisSetVertexLayout(&render_data.vao, 0, layout, 2);

    GLuint indices[IGNIS_FONTRENDERER_INDEX_COUNT];
    ignisGenerateQuadIndices(indices, IGNIS_FONTRENDERER_INDEX_COUNT);

    ignisLoadElementBuffer(&render_data.vao, 1, indices, IGNIS_FONTRENDERER_INDEX_COUNT, IGNIS_STATIC_DRAW);

    render_data.index = 0;
    render_data.quad_count = 0;

    render_data.font = NULL;
    render_data.color = IGNIS_WHITE;

    render_data.shader = ignisCreateShaderSrcvf(vert_src, frag_src);

    render_data.uniform_location_proj = ignisGetUniformLocation(render_data.shader, "u_Proj");
    render_data.uniform_location_color = ignisGetUniformLocation(render_data.shader, "u_Color");
}

void ignisFontRendererDestroy()
{
    ignisDeleteShader(render_data.shader);
    ignisDeleteVertexArray(&render_data.vao);
}

void ignisFontRendererBindFont(IgnisFont* font)
{
    render_data.font = font;
}

void ignisFontRendererBindFontColor(IgnisFont* font, IgnisColorRGBA color)
{
    render_data.font = font;
    render_data.color = color;

    ignisSetUniform4fl(render_data.shader, render_data.uniform_location_color, 1, &render_data.color.r);
}

void ignisFontRendererSetProjection(const float* proj)
{
    ignisSetUniformMat4l(render_data.shader, render_data.uniform_location_proj, 1, proj);
}

void ignisFontRendererFlush()
{
    if (render_data.index == 0) return;

    ignisBindTexture2D(render_data.font->texture);
    ignisBindVertexArray(&render_data.vao);
    ignisBufferSubData(&render_data.vao.buffers[0], 0, render_data.index * sizeof(float), render_data.vertices);

    ignisUseShader(render_data.shader);

    glDrawElements(GL_TRIANGLES, IGNIS_INDICES_PER_QUAD * render_data.quad_count, GL_UNSIGNED_INT, NULL);

    render_data.index = 0;
    render_data.quad_count = 0;
}

void ignisFontRendererRenderText(float x, float y, const char* text)
{
    if (!render_data.font)
    {
        IGNIS_WARN("[FontRenderer] No font bound");
        return;
    }

    for (size_t i = 0; i < strlen(text); i++)
    {
        if (render_data.index + IGNIS_FONTRENDERER_QUAD_SIZE >= IGNIS_FONTRENDERER_BUFFER_SIZE)
            ignisFontRendererFlush();

        const IgnisGlyph* glyph = ignisFontFindGlyph(render_data.font, text[i]);
        if (!glyph)
        {
            IGNIS_WARN("[FontRenderer] Failed to load quad for %c", text[i]);
            continue;
        }

        if (glyph == render_data.font->fallback)
            IGNIS_WARN("[FontRenderer] Used fallback for %c", text[i]);

        render_data.vertices[render_data.index + 0] = x + glyph->x0;
        render_data.vertices[render_data.index + 1] = y + glyph->y0;
        render_data.vertices[render_data.index + 2] = glyph->u0;
        render_data.vertices[render_data.index + 3] = glyph->v0;
        render_data.vertices[render_data.index + 4] = x + glyph->x0;
        render_data.vertices[render_data.index + 5] = y + glyph->y1;
        render_data.vertices[render_data.index + 6] = glyph->u0;
        render_data.vertices[render_data.index + 7] = glyph->v1;
        render_data.vertices[render_data.index + 8] = x + glyph->x1;
        render_data.vertices[render_data.index + 9] = y + glyph->y1;
        render_data.vertices[render_data.index + 10] = glyph->u1;
        render_data.vertices[render_data.index + 11] = glyph->v1;
        render_data.vertices[render_data.index + 12] = x + glyph->x1;
        render_data.vertices[render_data.index + 13] = y + glyph->y0;
        render_data.vertices[render_data.index + 14] = glyph->u1;
        render_data.vertices[render_data.index + 15] = glyph->v0;

        x += glyph->xadvance;
        render_data.index += IGNIS_FONTRENDERER_QUAD_SIZE;
        render_data.quad_count++;
    }
}

static void ignisFontRendererRenderTextVA(float x, float y, const char* fmt, va_list args)
{
    size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
    vsnprintf(render_data.line_buffer, buffer_size + 1, fmt, args);

    ignisFontRendererRenderText(x, y, render_data.line_buffer);
}

void ignisFontRendererRenderTextFormat(float x, float y, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ignisFontRendererRenderTextVA(x, y, fmt, args);
    va_end(args);
}

typedef struct 
{
    float x;
    float y;
    float line_height;
} IgnisTextField;

static IgnisTextField text_field;

void ignisFontRendererTextFieldBegin(float x, float y, float spacing)
{
    text_field.x = x;
    text_field.y = y;
    text_field.line_height = render_data.font ? render_data.font->size : 0.0f;
    text_field.line_height += spacing;
}

void ignisFontRendererTextFieldLine(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ignisFontRendererRenderTextVA(text_field.x, text_field.y, fmt, args);
    va_end(args);

    text_field.y += text_field.line_height;
}
