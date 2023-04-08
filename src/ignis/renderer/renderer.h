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

/*
 * --------------------------------------------------------------
 *                          color
 * --------------------------------------------------------------
 */
typedef struct
{
    float r, g, b, a;
} IgnisColorRGBA;

extern const IgnisColorRGBA IGNIS_WHITE;
extern const IgnisColorRGBA IGNIS_BLACK;
extern const IgnisColorRGBA IGNIS_RED;
extern const IgnisColorRGBA IGNIS_GREEN;
extern const IgnisColorRGBA IGNIS_BLUE;
extern const IgnisColorRGBA IGNIS_CYAN;
extern const IgnisColorRGBA IGNIS_MAGENTA;
extern const IgnisColorRGBA IGNIS_YELLOW;

extern const IgnisColorRGBA IGNIS_DARK_GREY;
extern const IgnisColorRGBA IGNIS_LIGHT_GREY;

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
void ignisBatch2DRenderTextureFrame(const IgnisTexture2D* texture, IgnisRect rect, uint32_t frame);

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

void ignisFontRendererRenderText(float x, float y, const char* text);
void ignisFontRendererRenderTextFormat(float x, float y, const char* fmt, ...);

void ignisFontRendererTextFieldBegin(float x, float y, float spacing);
void ignisFontRendererTextFieldLine(const char* fmt, ...);

/*
 * --------------------------------------------------------------
 *                          Primitives2D
 * --------------------------------------------------------------
 */
void ignisPrimitives2DInit();
void ignisPrimitives2DDestroy();

void ignisPrimitives2DSetViewProjection(const float* view_proj);

void ignisPrimitives2DFlush();

void ignisPrimitives2DRenderLine(float x1, float y1, float x2, float y2, IgnisColorRGBA color);
void ignisPrimitives2DRenderRect(float x, float y, float w, float h, IgnisColorRGBA color);
void ignisPrimitives2DRenderPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color);
void ignisPrimitives2DRenderCircle(float x, float y, float radius, IgnisColorRGBA color);

void ignisPrimitives2DFillRect(float x, float y, float w, float h, IgnisColorRGBA color);
void ignisPrimitives2DFillPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color);
void ignisPrimitives2DFillCircle(float x, float y, float radius, IgnisColorRGBA color);

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
