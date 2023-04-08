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
    gl_Position = u_Proj * vec4(a_Pos, 1.0, 1.0);   \
    v_TexCoord = a_TexCoord;                        \
}";

static const char* frag_src = "#version 330 core \n\
layout(location = 0) out vec4 f_Color;          \
uniform sampler2D u_Tex;                        \
uniform vec4 u_Color;                           \
in vec2 v_TexCoord;                             \
void main()                                     \
{                                               \
    float alpha = texture(u_Tex, v_TexCoord).r; \
    f_Color = vec4(u_Color.xyz, alpha);         \
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
    ignisGenerateVertexArray(&render_data.vao);

    size_t size = IGNIS_FONTRENDERER_BUFFER_SIZE * sizeof(float);
    IgnisBufferElement layout[] = { {GL_FLOAT, 2, GL_FALSE}, {GL_FLOAT, 2, GL_FALSE} };
    ignisAddArrayBufferLayout(&render_data.vao, size, NULL, GL_DYNAMIC_DRAW, 0, layout, 2);

    GLuint indices[IGNIS_FONTRENDERER_INDEX_COUNT];
    ignisGenerateQuadIndices(indices, IGNIS_FONTRENDERER_INDEX_COUNT);

    ignisLoadElementBuffer(&render_data.vao, indices, IGNIS_FONTRENDERER_INDEX_COUNT, GL_STATIC_DRAW);

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

    ignisSetUniform4fl(render_data.shader, render_data.uniform_location_color, &render_data.color.r);
}

void ignisFontRendererSetProjection(const float* proj)
{
    ignisSetUniformMat4l(render_data.shader, render_data.uniform_location_proj, proj);
}

void ignisFontRendererFlush()
{
    if (render_data.index == 0) return;

    ignisBindFont(render_data.font, 0);
    ignisBindVertexArray(&render_data.vao);
    ignisBufferSubData(&render_data.vao.array_buffers[0], 0, render_data.index * sizeof(float), render_data.vertices);

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

    y += ignisFontGetHeight(render_data.font);

    for (size_t i = 0; i < strlen(text); i++)
    {
        if (render_data.index + IGNIS_FONTRENDERER_QUAD_SIZE >= IGNIS_FONTRENDERER_BUFFER_SIZE)
            ignisFontRendererFlush();

        if (!ignisFontLoadCharQuad(render_data.font, text[i], &x, &y, render_data.vertices, render_data.index))
            IGNIS_WARN("[FontRenderer] Failed to load quad for %c", text[i]);

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
    text_field.line_height = render_data.font ? ignisFontGetHeight(render_data.font) : 0.0f;
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
