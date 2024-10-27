#ifndef IGNISRENDERER_H
#define IGNISRENDERER_H

#include <stdint.h>

#include "../core/texture.h"
#include "../core/shader.h"
#include "../vertex_array.h"
#include "../font.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * --------------------------------------------------------------
 *                          quad
 * --------------------------------------------------------------
 */
typedef struct
{
    IgnisVertexArray vao;
    size_t vertex_count;
    size_t element_count;
} IgnisQuad;

int ignisCreateQuad(IgnisQuad* quad, GLfloat* vertices, size_t vertex_count, GLenum usage, IgnisBufferElement* layout, size_t layout_size, GLuint* indices, GLsizei element_count);
int ignisCreateQuadTextured(IgnisQuad* quad, GLenum usage);

void ignisGenerateQuadIndices(GLuint* indices, size_t max);

void ignisDeleteQuad(IgnisQuad* quad);

void ignisBindQuad(IgnisQuad* quad);

void ignisDrawQuadElements(IgnisQuad* quad, GLenum mode);
void ignisDrawQuadElementsInstanced(IgnisQuad* quad, GLenum mode, GLsizei primcount);

/*
 * --------------------------------------------------------------
 *                          rect
 * --------------------------------------------------------------
 */
typedef struct
{
    float x, y, w, h;
} IgnisRect;

IgnisRect ignisGetTexture2DSrcRect(const IgnisTexture2D* texture, uint32_t cols, uint32_t rows, uint32_t frame);
IgnisRect ignisGetTextureAtlas2DSrcRect(const IgnisTextureAtlas2D* atlas, uint32_t frame);

/*
 * --------------------------------------------------------------
 *                          color
 * --------------------------------------------------------------
 */
typedef struct
{
    float r, g, b, a;
} IgnisColorRGBA;

extern IgnisColorRGBA IGNIS_WHITE;
extern IgnisColorRGBA IGNIS_BLACK;
extern IgnisColorRGBA IGNIS_RED;
extern IgnisColorRGBA IGNIS_GREEN;
extern IgnisColorRGBA IGNIS_BLUE;
extern IgnisColorRGBA IGNIS_CYAN;
extern IgnisColorRGBA IGNIS_MAGENTA;
extern IgnisColorRGBA IGNIS_YELLOW;

extern IgnisColorRGBA IGNIS_DARK_GREY;
extern IgnisColorRGBA IGNIS_LIGHT_GREY;

IgnisColorRGBA* ignisBlendColorRGBA(IgnisColorRGBA* color, float alpha);

/*
 * --------------------------------------------------------------
 *                          BatchRenderer2D
 * --------------------------------------------------------------
 */
void ignisBatch2DInit(const char* vert, const char* frag);
void ignisBatch2DDestroy();

void ignisBatch2DSetViewProjection(const float* mat_view_proj);

void ignisBatch2DFlush();

void ignisBatch2DRenderTexture(const IgnisTexture2D* texture, IgnisRect rect);
void ignisBatch2DRenderTextureSrc(const IgnisTexture2D* texture, IgnisRect rect, IgnisRect src);

/*
 * --------------------------------------------------------------
 *                          FontRenderer
 * --------------------------------------------------------------
 */
void ignisFontRendererInit();
void ignisFontRendererDestroy();

void ignisFontRendererBindFont(IgnisFont* font);
void ignisFontRendererBindFontColor(IgnisFont* font, IgnisColorRGBA color);

void ignisFontRendererSetProjection(const float* proj);

void ignisFontRendererFlush();

void ignisFontRendererRenderText(float x, float y, float height, const char* text);
void ignisFontRendererRenderTextFormat(float x, float y, float height, const char* fmt, ...);

void ignisFontRendererTextFieldBegin(float x, float y, float line_height, float spacing);
void ignisFontRendererTextFieldLine(const char* fmt, ...);

/*
 * --------------------------------------------------------------
 *                          DebugRenderer
 * --------------------------------------------------------------
 */
void ignisDebugRendererInit();
void ignisDebugRendererDestroy();

void ignisDebugRendererSetViewProjection(const float* view_proj);

void ignisDebugRendererFlush();

void ignisRenderLine2D(float x1, float y1, float x2, float y2, IgnisColorRGBA color);
void ignisRenderRect(float x, float y, float w, float h, IgnisColorRGBA color);
void ignisRenderPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color);
void ignisRenderCircle(float x, float y, float radius, IgnisColorRGBA color);

void ignisFillTriangle(float x0, float y0, float x1, float y1, float x2, float y2, IgnisColorRGBA color);
void ignisFillRect(float x, float y, float w, float h, IgnisColorRGBA color);
void ignisFillPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color);
void ignisFillCircle(float x, float y, float radius, IgnisColorRGBA color);

void ignisRenderLine3D(float start[3], float end[3], IgnisColorRGBA color);
void ignisRenderTriangle3D(float v0[3], float v1[3], float v2[3], IgnisColorRGBA color);
void ignisRenderBox(float min[3], float max[3], IgnisColorRGBA color);

void ignisRenderDebugGrid(float w, float h, float gap);

/*
 * --------------------------------------------------------------
 *                          Renderer2D
 * --------------------------------------------------------------
 */
void ignisRenderer2DInit();
void ignisRenderer2DDestroy();

void ignisRenderer2DSetShader(IgnisShader shader);
void ignisRenderer2DSetViewProjection(const float* view_proj);

void ignisRenderer2DRenderTexture(const IgnisTexture2D* texture, float x, float y);
void ignisRenderer2DRenderTextureScale(const IgnisTexture2D* texture, float x, float y, float w, float h);
void ignisRenderer2DRenderTextureColor(const IgnisTexture2D* texture, float x, float y, float w, float h, IgnisColorRGBA color);
void ignisRenderer2DRenderTextureModel(const IgnisTexture2D* texture, const float* model, IgnisColorRGBA color);

#ifdef __cplusplus
}
#endif

#endif /* !IGNISRENDERER_H */
