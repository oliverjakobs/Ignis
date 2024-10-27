#include "renderer.h"

#include "../ignis.h"

#include <math.h>

/* ---------------------| shader |---------------------------------------------*/
static const char* vert_src = "#version 330 core \n \
layout(location = 0) in vec3 a_Pos;                 \
layout(location = 1) in vec4 a_Color;               \
uniform mat4 u_ViewProj;                            \
out vec4 v_Color;                                   \
void main() {                                       \
    gl_Position = u_ViewProj * vec4(a_Pos, 1.0);    \
    v_Color = a_Color;                              \
}";

static const char* geom_src = "#version 330 core \n \
layout(points) in;                                  \
layout (line_strip, max_vertices = 121) out;        \
uniform mat4 u_ViewProj;                            \
out vec4 v_Color;                                   \
void main() {                                       \
    for (float x = -5.0f; x <= 5.0f; x += 1.0f) {   \
        v_Color = vec4(1.0f);                                                           \
        gl_Position = gl_in[0].gl_Position + u_ViewProj * vec4(x, -5.0f, 0.0f, 0.0f);   \
        EmitVertex();                                                                   \
        gl_Position = gl_in[0].gl_Position + u_ViewProj * vec4(x, 5.0f, 0.0f, 0.0f);    \
        EmitVertex();                                                                   \
        EndPrimitive();                                                                 \
        v_Color = vec4(1.0f);                                                           \
        gl_Position = gl_in[0].gl_Position + u_ViewProj * vec4(-5.0f, x, 0.0f, 0.0f);   \
        EmitVertex();                                                                   \
        gl_Position = gl_in[0].gl_Position + u_ViewProj * vec4(5.0f, x, 0.0f, 0.0f);    \
        EmitVertex();                                                                   \
        EndPrimitive();                                                                 \
    } \
}";

static const char* frag_src = "#version 330 core \n \
layout(location = 0) out vec4 f_Color;              \
in vec4 v_Color;                                    \
void main() {                                       \
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

    IgnisShader primitive_shader;
    GLint uniform_location_view_proj;

    IgnisVertexArray grid_vao;
    IgnisShader grid_shader;
} IgnisDebugRendererStorage;

static IgnisDebugRendererStorage render_data;

static void ignisPrimitivesDataCreate(IgnisPrimitivesData* prim)
{
    ignisGenerateVertexArray(&prim->vao, 1);

    IgnisBufferElement layout[] = {
        {IGNIS_FLOAT, 3, GL_FALSE},
        {IGNIS_FLOAT, 4, GL_FALSE}
    };
    ignisLoadArrayBuffer(&prim->vao, 0, IGNIS_PRIMITIVES2D_BUFFER_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    ignisSetVertexLayout(&prim->vao, 0, layout, 2);

    prim->index = 0;
}

static void ignisPrimitivesDataFlush(IgnisPrimitivesData* prim, GLenum mode)
{
    if (prim->index == 0) return;

    ignisBindVertexArray(&prim->vao);
    ignisBufferSubData(&prim->vao.buffers[0], 0, prim->index * sizeof(float), prim->vertices);

    ignisUseShader(render_data.primitive_shader);

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

void ignisDebugRendererInit()
{
    ignisPrimitivesDataCreate(&render_data.lines);
    ignisPrimitivesDataCreate(&render_data.triangles);

    render_data.primitive_shader = ignisCreateShaderSrcvf(vert_src, frag_src);
    render_data.uniform_location_view_proj = ignisGetUniformLocation(render_data.primitive_shader, "u_ViewProj");

    render_data.grid_shader = ignisCreateShaderSrcvgf(vert_src, geom_src, frag_src);

    ignisGenerateVertexArray(&render_data.grid_vao, 1);

    IgnisBufferElement layout[] = { {IGNIS_FLOAT, 3, GL_FALSE} };
    ignisLoadArrayBuffer(&render_data.grid_vao, 0, 3 * sizeof(float), NULL, GL_STATIC_DRAW);
    ignisSetVertexLayout(&render_data.grid_vao, 0, layout, 1);
}

void ignisDebugRendererDestroy()
{
    ignisDeleteVertexArray(&render_data.lines.vao);
    ignisDeleteVertexArray(&render_data.triangles.vao);
    ignisDeleteShader(render_data.primitive_shader);

    ignisDeleteVertexArray(&render_data.grid_vao);
    ignisDeleteShader(render_data.grid_shader);
}

void ignisDebugRendererSetViewProjection(const float* view_proj)
{
    ignisSetUniformMat4l(render_data.primitive_shader, render_data.uniform_location_view_proj, 1, view_proj);
    ignisSetUniformMat4l(render_data.grid_shader, render_data.uniform_location_view_proj, 1, view_proj);
}

void ignisDebugRendererFlush()
{
    ignisPrimitivesDataFlush(&render_data.lines, GL_LINES);
    ignisPrimitivesDataFlush(&render_data.triangles, GL_TRIANGLES);
}

void ignisRenderLine2D(float x0, float y0, float x1, float y1, IgnisColorRGBA color)
{
    if (render_data.lines.index + (2 * IGNIS_PRIMITIVES2D_VERTEX_SIZE) > IGNIS_PRIMITIVES2D_BUFFER_SIZE)
        ignisPrimitivesDataFlush(&render_data.lines, GL_LINES);

    ignisPrimitivesPushVertex(&render_data.lines, x0, y0, 0.0f, color);
    ignisPrimitivesPushVertex(&render_data.lines, x1, y1, 0.0f, color);
}

void ignisRenderRect(float x, float y, float w, float h, IgnisColorRGBA color)
{
    ignisRenderLine2D(x + 0, y + 0, x + w, y + 0, color);
    ignisRenderLine2D(x + w, y + 0, x + w, y + h, color);
    ignisRenderLine2D(x + w, y + h, x + 0, y + h, color);
    ignisRenderLine2D(x + 0, y + h, x + 0, y + 0, color);
}

void ignisRenderPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color)
{
    if (!vertices || count < 2) return;

    float p1x = x + vertices[count - 2];
    float p1y = y + vertices[count - 1];

    size_t i = 0;
    while (i < count - 1)
    {
        float p2x = x + vertices[i++];
        float p2y = y + vertices[i++];

        ignisRenderLine2D(p1x, p1y, p2x, p2y, color);

        p1x = p2x;
        p1y = p2y;
    }
}

void ignisRenderCircle(float x, float y, float radius, IgnisColorRGBA color)
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

        ignisRenderLine2D(v1x, v1y, v2x, v2y, color);

        r1x = r2x;
        r1y = r2y;
        v1x = v2x;
        v1y = v2y;
    }
}

void ignisFillTriangle(float x0, float y0, float x1, float y1, float x2, float y2, IgnisColorRGBA color)
{
    if (render_data.triangles.index + (3 * IGNIS_PRIMITIVES2D_VERTEX_SIZE) > IGNIS_PRIMITIVES2D_BUFFER_SIZE)
        ignisPrimitivesDataFlush(&render_data.triangles, GL_TRIANGLES);

    ignisPrimitivesPushVertex(&render_data.triangles, x0, y0, 0.0f, color);
    ignisPrimitivesPushVertex(&render_data.triangles, x1, y1, 0.0f, color);
    ignisPrimitivesPushVertex(&render_data.triangles, x2, y2, 0.0f, color);
}

void ignisFillRect(float x, float y, float w, float h, IgnisColorRGBA color)
{
    ignisFillTriangle(x + 0, y + 0, x + w, y + 0, x + w, y + h, color);
    ignisFillTriangle(x + w, y + h, x + 0, y + h, x + 0, y + 0, color);
}

void ignisFillPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color)
{
    if (!vertices || count < 2) return;

    float x0 = x + vertices[0], y0 = y + vertices[1];
    for (size_t i = 2; i < count - 3; ++i)
    {
        float x1 = x + vertices[i + 0], y1 = y + vertices[i + 1];
        float x2 = x + vertices[i + 2], y2 = y + vertices[i + 3];
        ignisFillTriangle(x0, y0, x1, y1, x2, y2, color);
    }
}

void ignisFillCircle(float x, float y, float radius, IgnisColorRGBA color)
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

        ignisFillTriangle(x, y, v1x, v1y, v2x, v2y, color);

        r1x = r2x;
        r1y = r2y;
        v1x = v2x;
        v1y = v2y;
    }
}

void ignisRenderLine3D(float start[3], float end[3], IgnisColorRGBA color)
{
    if (render_data.lines.index + (2 * IGNIS_PRIMITIVES2D_VERTEX_SIZE) > IGNIS_PRIMITIVES2D_BUFFER_SIZE)
        ignisPrimitivesDataFlush(&render_data.lines, GL_LINES);

    ignisPrimitivesPushVertex(&render_data.lines, start[0], start[1], start[2], color);
    ignisPrimitivesPushVertex(&render_data.lines, end[0], end[1], end[2], color);
}

void ignisRenderTriangle3D(float v0[3], float v1[3], float v2[3], IgnisColorRGBA color)
{
    ignisRenderLine3D(v0, v1, color);
    ignisRenderLine3D(v1, v2, color);
    ignisRenderLine3D(v2, v0, color);
}

void ignisRenderBox(float min[3], float max[3], IgnisColorRGBA color)
{
    float nnx[3] = { min[0], min[1], max[2] };
    float xnx[3] = { max[0], min[1], max[2] };
    float xnn[3] = { max[0], min[1], min[2] };
    float nxx[3] = { min[0], max[1], max[2] };
    float xxn[3] = { max[0], max[1], min[2] };
    float nxn[3] = { min[0], max[1], min[2] };

    // lower rect
    ignisRenderLine3D(min, nnx, color);
    ignisRenderLine3D(nnx, xnx, color);
    ignisRenderLine3D(xnx, xnn, color);
    ignisRenderLine3D(xnn, min, color);

    // connecting lines
    ignisRenderLine3D(min, nxn, color);
    ignisRenderLine3D(nnx, nxx, color);
    ignisRenderLine3D(xnx, max, color);
    ignisRenderLine3D(xnn, xxn, color);

    // upper rect
    ignisRenderLine3D(max, xxn, color);
    ignisRenderLine3D(xxn, nxn, color);
    ignisRenderLine3D(nxn, nxx, color);
    ignisRenderLine3D(nxx, max, color);
}


void ignisRenderDebugGrid(float w, float h, float gap)
{
    ignisBindVertexArray(&render_data.grid_vao);
    ignisUseShader(render_data.grid_shader);

    glDrawArrays(GL_POINTS, 0, 1);
}