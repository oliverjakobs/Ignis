#ifndef IGNISRENDERER_H
#define IGNISRENDERER_H

#include <stdint.h>

#include "../Ignis.h"
#include "../VertexArray.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IGNIS_VERTICES_PER_QUAD  4
#define IGNIS_INDICES_PER_QUAD   6

typedef struct
{
    IgnisVertexArray vao;
    size_t vertex_count;
} IgnisQuad;

int ignisCreateQuad(IgnisQuad* quad, GLfloat* vertices, size_t vertex_count, GLenum usage, IgnisBufferElement* layout, size_t layout_size, GLuint* indices, GLsizei element_count);
int ignisCreateQuadTextured(IgnisQuad* quad, GLenum usage);

void ignisDeleteQuad(IgnisQuad* quad);

void ignisBindQuad(IgnisQuad* quad);

void ignisDrawQuadElements(IgnisQuad* quad, GLenum mode);
void ignisDrawQuadElementsInstanced(IgnisQuad* quad, GLenum mode, GLsizei primcount);

typedef struct
{
    float x, y, w, h;
} IgnisRect;

/*
 * --------------------------------------------------------------
 *                          BatchRenderer2D
 * --------------------------------------------------------------
 */
#define IGNIS_BATCH2D_MAX_QUADS   32
#define IGNIS_BATCH2D_VERTEX_SIZE (3 + 2 + 1)

#define IGNIS_BATCH2D_QUAD_SIZE   (IGNIS_VERTICES_PER_QUAD * IGNIS_BATCH2D_VERTEX_SIZE)
#define IGNIS_BATCH2D_INDEX_COUNT (IGNIS_BATCH2D_MAX_QUADS * IGNIS_INDICES_PER_QUAD)
#define IGNIS_BATCH2D_BUFFER_SIZE (IGNIS_BATCH2D_MAX_QUADS * IGNIS_BATCH2D_QUAD_SIZE)

#define IGNIS_BATCH2D_TEXTURES    8

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

#define IGNIS_FONTRENDERER_MAX_QUADS    32
#define IGNIS_FONTRENDERER_VERTEX_SIZE  (2 + 2) /* 2f: vec; 2f: tex */

#define IGNIS_FONTRENDERER_QUAD_SIZE    (IGNIS_VERTICES_PER_QUAD * IGNIS_FONTRENDERER_VERTEX_SIZE)
#define IGNIS_FONTRENDERER_INDEX_COUNT  (IGNIS_FONTRENDERER_MAX_QUADS * IGNIS_INDICES_PER_QUAD)
#define IGNIS_FONTRENDERER_BUFFER_SIZE  (IGNIS_FONTRENDERER_MAX_QUADS * IGNIS_FONTRENDERER_QUAD_SIZE)

#define IGNIS_FONTRENDERER_MAX_LINE_LENGTH    128

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

#define IGNIS_PRIMITIVES2D_MAX_VERTICES   3 * 1024
#define IGNIS_PRIMITIVES2D_VERTEX_SIZE    (2 + 4) /* 2f: position; 4f color */
#define IGNIS_PRIMITIVES2D_BUFFER_SIZE    (IGNIS_PRIMITIVES2D_VERTEX_SIZE * IGNIS_PRIMITIVES2D_MAX_VERTICES)

 /* Circle */
#define IGNIS_PRIMITIVES2D_PI             3.14159265359f
#define IGNIS_PRIMITIVES2D_K_SEGMENTS     36
#define IGNIS_PRIMITIVES2D_K_INCREMENT    2.0f * IGNIS_PRIMITIVES2D_PI / IGNIS_PRIMITIVES2D_K_SEGMENTS

void ignisPrimitives2DInit();
void ignisPrimitives2DDestroy();

void ignisPrimitives2DSetViewProjection(const float* view_proj);

void ignisPrimitives2DFlush();

void ignisPrimitives2DRenderLine(float x1, float y1, float x2, float y2, IgnisColorRGBA color);
void ignisPrimitives2DRenderRect(float x, float y, float w, float h, IgnisColorRGBA color);
void ignisPrimitives2DRenderPoly(const float* vertices, size_t count, float x, float y, IgnisColorRGBA color);
void ignisPrimitives2DRenderCircle(float x, float y, float radius, IgnisColorRGBA color);
void ignisPrimitives2DRenderRhombus(float x, float y, float width, float height, IgnisColorRGBA color);

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

void ignisRenderer2DSetShader(IgnisShader* shader);
void ignisRenderer2DSetViewProjection(const float* view_proj);

void ignisRenderer2DRenderTexture(const IgnisTexture2D* texture, float x, float y);
void ignisRenderer2DRenderTextureScale(const IgnisTexture2D* texture, float x, float y, float w, float h);
void ignisRenderer2DRenderTextureColor(const IgnisTexture2D* texture, float x, float y, float w, float h, IgnisColorRGBA color);
void ignisRenderer2DRenderTextureModel(const IgnisTexture2D* texture, const float* model, IgnisColorRGBA color);

/*
 * --------------------------------------------------------------
 *                          Utility
 * --------------------------------------------------------------
 */
void ignisGenerateQuadIndices(GLuint* indices, size_t max);
IgnisRect ignisGetTexture2DSrcRect(const IgnisTexture2D* texture, uint32_t frame);

#ifdef __cplusplus
}
#endif

#endif /* !IGNISRENDERER_H */
