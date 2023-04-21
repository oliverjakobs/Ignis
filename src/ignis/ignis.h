#ifndef IGNIS_H
#define IGNIS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


/* You can #define IGNIS_ASSERT(x) before the #include to avoid using assert.h */
#ifndef IGNIS_ASSERT
#include <assert.h>
#define IGNIS_ASSERT(x) assert(x)
#endif

/* Ignis version numbers */
#define IGNIS_VERSION_MAJOR       1
#define IGNIS_VERSION_MINOR       0
#define IGNIS_VERSION_REVISION    0

/*
 * --------------------------------------------------------------
 *                          defines
 * --------------------------------------------------------------
 */
#define IGNIS_SUCCESS   1
#define IGNIS_FAILURE   0

#define IGNIS_PI        3.14159265359f

#define IGNIS_VERTICES_PER_QUAD  4
#define IGNIS_INDICES_PER_QUAD   6

#define IGNIS_FILE_BUFFER_SIZE  1024

/*
 * --------------------------------------------------------------
 *                          core
 * --------------------------------------------------------------
 */
#include "core/texture.h"
#include "core/shader.h"
#include "core/buffer.h"

/*
 * --------------------------------------------------------------
 *                          font
 * --------------------------------------------------------------
 */
#define IGNIS_FONT_FIRST_CHAR       32
#define IGNIS_FONT_NUM_CHARS        96  /* ASCII 32..126 is 95 glyphs */
#define IGNIS_FONT_BITMAP_WIDTH     512
#define IGNIS_FONT_BITMAP_HEIGHT    512

#include "font.h"

/*
 * --------------------------------------------------------------
 *                          vertex array
 * --------------------------------------------------------------
 */
#define IGNIS_BUFFER_ARRAY_INITIAL_SIZE     4
#define IGNIS_BUFFER_ARRAY_GROWTH_FACTOR    2

#include "vertex_array.h"

/*
 * --------------------------------------------------------------
 *                          renderer
 * --------------------------------------------------------------
 */

/* batch renderer */
#define IGNIS_BATCH2D_MAX_QUADS   32
#define IGNIS_BATCH2D_VERTEX_SIZE (3 + 2 + 1)

#define IGNIS_BATCH2D_QUAD_SIZE   (IGNIS_VERTICES_PER_QUAD * IGNIS_BATCH2D_VERTEX_SIZE)
#define IGNIS_BATCH2D_INDEX_COUNT (IGNIS_BATCH2D_MAX_QUADS * IGNIS_INDICES_PER_QUAD)
#define IGNIS_BATCH2D_BUFFER_SIZE (IGNIS_BATCH2D_MAX_QUADS * IGNIS_BATCH2D_QUAD_SIZE)

#define IGNIS_BATCH2D_TEXTURES    8

/* font renderer */
#define IGNIS_FONTRENDERER_MAX_QUADS    32
#define IGNIS_FONTRENDERER_VERTEX_SIZE  (2 + 2) /* 2f: vec; 2f: tex */

#define IGNIS_FONTRENDERER_QUAD_SIZE    (IGNIS_VERTICES_PER_QUAD * IGNIS_FONTRENDERER_VERTEX_SIZE)
#define IGNIS_FONTRENDERER_INDEX_COUNT  (IGNIS_FONTRENDERER_MAX_QUADS * IGNIS_INDICES_PER_QUAD)
#define IGNIS_FONTRENDERER_BUFFER_SIZE  (IGNIS_FONTRENDERER_MAX_QUADS * IGNIS_FONTRENDERER_QUAD_SIZE)

#define IGNIS_FONTRENDERER_MAX_LINE_LENGTH    128

/* primitives */
#define IGNIS_PRIMITIVES2D_MAX_VERTICES   3 * 1024
#define IGNIS_PRIMITIVES2D_VERTEX_SIZE    (3 + 4) /* 2f: position; 4f color */
#define IGNIS_PRIMITIVES2D_BUFFER_SIZE    (IGNIS_PRIMITIVES2D_VERTEX_SIZE * IGNIS_PRIMITIVES2D_MAX_VERTICES)

 /* primitives circle */
#define IGNIS_PRIMITIVES2D_PI             3.14159265359f
#define IGNIS_PRIMITIVES2D_K_SEGMENTS     36
#define IGNIS_PRIMITIVES2D_K_INCREMENT    2.0f * IGNIS_PRIMITIVES2D_PI / IGNIS_PRIMITIVES2D_K_SEGMENTS

#include "renderer/renderer.h"


/*
 * --------------------------------------------------------------
 *                          setup + utility
 * --------------------------------------------------------------
 */

#define IGNIS_TEXTFORMAT_BUFFERS    4       // number of static buffers for text formatting
#define IGNIS_FORMAT_BUFFER_LENGTH  512     // size of static text buffer

int ignisInit(int debug);
void ignisDestroy();

int ignisEnableBlend(GLenum sfactor, GLenum dfactor);

void ignisSetClearColor(IgnisColorRGBA color);
void ignisClearColorBuffer(IgnisColorRGBA color);

GLuint ignisGetOpenGLTypeSize(GLenum type);
char* ignisReadFile(const char* path, size_t* sizeptr);

const char* ignisTextFormat(const char* fmt, ...);

void ignisGetVersion(int* major, int* minor, int* rev);
const char* ignisGetVersionString();
const char* ignisGetGLVersion();
const char* ignisGetGLVendor();
const char* ignisGetGLRenderer();
const char* ignisGetGLSLVersion();

/*
 * --------------------------------------------------------------
 *                          logging
 * --------------------------------------------------------------
 */
#ifndef IGNIS_DISABLE_LOGGING

#define IGNIS_CRITICAL(s, ...)  _ignisError(IGNIS_LVL_CRITICAL, s, __VA_ARGS__)
#define IGNIS_ERROR(s, ...)     _ignisError(IGNIS_LVL_CRITICAL, s, __VA_ARGS__)
#define IGNIS_WARN(s, ...)      _ignisError(IGNIS_LVL_WARN, s, __VA_ARGS__)

#else

#define IGNIS_CRITICAL(s, ...)
#define IGNIS_ERROR(s, ...)
#define IGNIS_WARN(s, ...)

#endif

typedef enum
{
    IGNIS_LVL_WARN,
    IGNIS_LVL_ERROR,
    IGNIS_LVL_CRITICAL
} ignisErrorLevel;

void ignisSetErrorCallback(void (*callback)(ignisErrorLevel, const char*));
void _ignisError(ignisErrorLevel level, const char* fmt, ...);

/*
 * --------------------------------------------------------------
 *                          memory
 * --------------------------------------------------------------
 */
typedef void* (*ignisMallocCb) (void* allocator, size_t size);
typedef void* (*ignisReallocCb)(void* allocator, void* block, size_t size);
typedef void  (*ignisFreeCb)   (void* allocator, void* block);

void ignisSetAllocator(void* allocator, ignisMallocCb malloc, ignisReallocCb realloc, ignisFreeCb free);

void* ignisMalloc(size_t size);
void* ignisRealloc(void* block, size_t size);
void  ignisFree(void* block);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_H */