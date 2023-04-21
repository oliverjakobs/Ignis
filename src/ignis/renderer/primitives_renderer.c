#include "renderer.h"

#include "../ignis.h"

#include <math.h>

/* ---------------------| shader |---------------------------------------------*/
static const char* vert_src = "#version 330 core \n \
layout(location = 0) in vec3 a_Pos;                 \
layout(location = 1) in vec4 a_Color;               \
uniform mat4 u_ViewProj;                            \
out vec4 v_Color;                                   \
void main()                                         \
{                                                   \
    gl_Position = u_ViewProj * vec4(a_Pos, 1.0);    \
    v_Color = a_Color;                              \
}";

static const char* frag_src = "#version 330 core \n \
layout(location = 0) out vec4 f_Color;              \
in vec4 v_Color;                                    \
void main()                                         \
{                                                   \
    f_Color = v_Color;                              \
}";
/* ---------------------| !shader |--------------------------------------------*/


typedef struct
{
    IgnisVertexArray vao;
    float vertices[IGNIS_PRIMITIVES2D_BUFFER_SIZE];
    GLsizei index;
} IgnisPrimitivesData;

typedef struct
{
    IgnisPrimitivesData lines;
    IgnisPrimitivesData triangles;

    IgnisShader shader;
    GLint uniform_location_view_proj;
} IgnisPrimitivesRendererStorage;

static IgnisPrimitivesRendererStorage render_data;

static void ignisPrimitivesDataCreate(IgnisPrimitivesData* prim)
{
    IgnisBufferElement layout[] = { {GL_FLOAT, 3, GL_FALSE}, {GL_FLOAT, 4, GL_FALSE} };

    ignisGenerateVertexArray(&prim->vao);
    ignisAddArrayBufferLayout(&prim->vao, IGNIS_PRIMITIVES2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW, 0, layout, 2);

    prim->index = 0;
}

static void ignisPrimitivesDataFlush(IgnisPrimitivesData* prim, GLenum mode)
{
    if (prim->index == 0) return;

    ignisBindVertexArray(&prim->vao);
    ignisBufferSubData(&prim->vao.array_buffers[0], 0, prim->index * sizeof(float), prim->vertices);

    ignisUseShader(render_data.shader);

    glDrawArrays(mode, 0, prim->index / IGNIS_PRIMITIVES2D_VERTEX_SIZE);

    prim->index = 0;
}

static void ignisPrimitivesPushVertex(IgnisPrimitivesData* prim, float x, float y, float z, IgnisColorRGBA color)
{
    prim->vertices[prim->index++] = x;
    prim->vertices[prim->index++] = y;
    prim->vertices[prim->index++] = z;

    prim->vertices[prim->index++] = color.r;
    prim->vertices[prim->index++] = color.g;
    prim->vertices[prim->index++] = color.b;
    prim->vertices[prim->index++] = color.a;
}

void ignisPrimitivesRendererInit()
{
    ignisPrimitivesDataCreate(&render_data.lines);
    ignisPrimitivesDataCreate(&render_data.triangles);

    render_data.shader = ignisCreateShaderSrcvf(vert_src, frag_src);
    render_data.uniform_location_view_proj = ignisGetUniformLocation(render_data.shader, "u_ViewProj");
}

void ignisPrimitivesRendererDestroy()
{
    ignisDeleteVertexArray(&render_data.lines.vao);
    ignisDeleteVertexArray(&render_data.triangles.vao);
    ignisDeleteShader(render_data.shader);
}

void ignisPrimitivesRendererSetViewProjection(const float* view_proj)
{
    ignisSetUniformMat4l(render_data.shader, render_data.uniform_location_view_proj, 1, view_proj);
}

void ignisPrimitivesRendererFlush()
{
    ignisPrimitivesDataFlush(&render_data.lines, GL_LINES);
    ignisPrimitivesDataFlush(&render_data.triangles, GL_TRIANGLES);
}

void ignisPrimitives2DRenderLine(float x0, float y0, float x1, float y1, IgnisColorRGBA color)
{
    if (render_data.lines.index + (2 * IGNIS_PRIMITIVES2D_VERTEX_SIZE) > IGNIS_PRIMITIVES2D_BUFFER_SIZE)
        ignisPrimitivesDataFlush(&render_data.lines, GL_LINES);

    ignisPrimitivesPushVertex(&render_data.lines, x0, y0, 0.0f, color);
    ignisPrimitivesPushVertex(&render_data.lines, x1, y1, 0.0f, color);
}

void ignisPrimitives2DRenderRect(float x, float y, float w, float h, IgnisColorRGBA color)
{
    ignisPrimitives2DRenderLine(x + 0, y + 0, x + w, y + 0, color);
    ignisPrimitives2DRenderLine(x + w, y + 0, x + w, y + h, color);
    ignisPrimitives2DRenderLine(x + w, y + h, x + 0, y + h, color);
    ignisPrimitives2DRenderLine(x + 0, y + h, x + 0, y + 0, color);
}

void ignisPrimitives2DRenderPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color)
{
    if (!vertices || count < 2) return;

    float p1x = x + vertices[count - 2];
    float p1y = y + vertices[count - 1];

    size_t i = 0;
    while (i < count - 1)
    {
        float p2x = x + vertices[i++];
        float p2y = y + vertices[i++];

        ignisPrimitives2DRenderLine(p1x, p1y, p2x, p2y, color);

        p1x = p2x;
        p1y = p2y;
    }
}

void ignisPrimitives2DRenderCircle(float x, float y, float radius, IgnisColorRGBA color)
{
    float sinInc = sinf(IGNIS_PRIMITIVES2D_K_INCREMENT);
    float cosInc = cosf(IGNIS_PRIMITIVES2D_K_INCREMENT);

    float r1x = 1.0f;
    float r1y = 0.0f;

    float v1x = x + radius * r1x;
    float v1y = y + radius * r1y;

    for (int i = 0; i < IGNIS_PRIMITIVES2D_K_SEGMENTS; ++i)
    {
        /* Perform rotation to avoid additional trigonometry. */
        float r2x = cosInc * r1x - sinInc * r1y;
        float r2y = sinInc * r1x + cosInc * r1y;

        float v2x = x + radius * r2x;
        float v2y = y + radius * r2y;

        ignisPrimitives2DRenderLine(v1x, v1y, v2x, v2y, color);

        r1x = r2x;
        r1y = r2y;
        v1x = v2x;
        v1y = v2y;
    }
}

void ignisPrimitives2DFillTriangle(float x0, float y0, float x1, float y1, float x2, float y2, IgnisColorRGBA color)
{
    if (render_data.triangles.index + (3 * IGNIS_PRIMITIVES2D_VERTEX_SIZE) > IGNIS_PRIMITIVES2D_BUFFER_SIZE)
        ignisPrimitivesDataFlush(&render_data.triangles, GL_TRIANGLES);

    ignisPrimitivesPushVertex(&render_data.triangles, x0, y0, 0.0f, color);
    ignisPrimitivesPushVertex(&render_data.triangles, x1, y1, 0.0f, color);
    ignisPrimitivesPushVertex(&render_data.triangles, x2, y2, 0.0f, color);
}

void ignisPrimitives2DFillRect(float x, float y, float w, float h, IgnisColorRGBA color)
{
    ignisPrimitives2DFillTriangle(x + 0, y + 0, x + w, y + 0, x + w, y + h, color);
    ignisPrimitives2DFillTriangle(x + w, y + h, x + 0, y + h, x + 0, y + 0, color);
}

void ignisPrimitives2DFillPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color)
{
    if (!vertices || count < 2) return;

    float x0 = x + vertices[0], y0 = y + vertices[1];
    for (size_t i = 2; i < count - 3; ++i)
    {
        float x1 = x + vertices[i + 0], y1 = y + vertices[i + 1];
        float x2 = x + vertices[i + 2], y2 = y + vertices[i + 3];
        ignisPrimitives2DFillTriangle(x0, y0, x1, y1, x2, y2, color);
    }
}

void ignisPrimitives2DFillCircle(float x, float y, float radius, IgnisColorRGBA color)
{
    float sinInc = sinf(IGNIS_PRIMITIVES2D_K_INCREMENT);
    float cosInc = cosf(IGNIS_PRIMITIVES2D_K_INCREMENT);

    float r1x = 1.0f;
    float r1y = 0.0f;

    float v1x = x + radius * r1x;
    float v1y = y + radius * r1y;

    for (int i = 0; i < IGNIS_PRIMITIVES2D_K_SEGMENTS; ++i)
    {
        /* Perform rotation to avoid additional trigonometry. */
        float r2x = cosInc * r1x - sinInc * r1y;
        float r2y = sinInc * r1x + cosInc * r1y;

        float v2x = x + radius * r2x;
        float v2y = y + radius * r2y;

        ignisPrimitives2DFillTriangle(x, y, v1x, v1y, v2x, v2y, color);

        r1x = r2x;
        r1y = r2y;
        v1x = v2x;
        v1y = v2y;
    }
}

void ignisPrimitives3DRenderLine(float start[3], float end[3], IgnisColorRGBA color)
{
    if (render_data.lines.index + (2 * IGNIS_PRIMITIVES2D_VERTEX_SIZE) > IGNIS_PRIMITIVES2D_BUFFER_SIZE)
        ignisPrimitivesDataFlush(&render_data.lines, GL_LINES);

    ignisPrimitivesPushVertex(&render_data.lines, start[0], start[1], start[2], color);
    ignisPrimitivesPushVertex(&render_data.lines, end[0], end[1], end[2], color);
}

void ignisPrimitives3DRenderTriangle(float v0[3], float v1[3], float v2[3], IgnisColorRGBA color)
{
    ignisPrimitives3DRenderLine(v0, v1, color);
    ignisPrimitives3DRenderLine(v1, v2, color);
    ignisPrimitives3DRenderLine(v2, v0, color);
}

void ignisPrimitives3DRenderBox(float min[3], float max[3], IgnisColorRGBA color)
{
    float nnx[3] = { min[0], min[1], max[2] };
    float xnx[3] = { max[0], min[1], max[2] };
    float xnn[3] = { max[0], min[1], min[2] };
    float nxx[3] = { min[0], max[1], max[2] };
    float xxn[3] = { max[0], max[1], min[2] };
    float nxn[3] = { min[0], max[1], min[2] };

    // lower rect
    ignisPrimitives3DRenderLine(min, nnx, color);
    ignisPrimitives3DRenderLine(nnx, xnx, color);
    ignisPrimitives3DRenderLine(xnx, xnn, color);
    ignisPrimitives3DRenderLine(xnn, min, color);

    // connecting lines
    ignisPrimitives3DRenderLine(min, nxn, color);
    ignisPrimitives3DRenderLine(nnx, nxx, color);
    ignisPrimitives3DRenderLine(xnx, max, color);
    ignisPrimitives3DRenderLine(xnn, xxn, color);

    // upper rect
    ignisPrimitives3DRenderLine(max, xxn, color);
    ignisPrimitives3DRenderLine(xxn, nxn, color);
    ignisPrimitives3DRenderLine(nxn, nxx, color);
    ignisPrimitives3DRenderLine(nxx, max, color);
}