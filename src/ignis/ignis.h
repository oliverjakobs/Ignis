#ifndef IGNIS_H
#define IGNIS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


/* You can #define IGNIS_ASSERT(x) before the #include to avoid using assert.h */
#ifndef IGNIS_ASSERT
#include <assert.h>
#define IGNIS_ASSERT(expr) assert(expr)
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

#define IGNIS_PATH_LEN  64

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
#define IGNIS_PRIMITIVES2D_VERTEX_SIZE    (3 + 4) /* 3f: position; 4f color */
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

int ignisInit(ignisGLLoadProc loader, int debug);
void ignisDestroy();

int ignisEnableBlend(IgnisBlendFunc sfactor, IgnisBlendFunc dfactor);

void ignisSetClearColor(IgnisColorRGBA color);
void ignisClearColorBuffer(IgnisColorRGBA color);

size_t ignisGetTypeSize(IgnisType type);
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
 *                          state
 * --------------------------------------------------------------
 */
#define IGNIS_STATE_BLEND           0x01
#define IGNIS_STATE_CULL_FACE       0x02
#define IGNIS_STATE_DEPTH_TEST      0x04
#define IGNIS_STATE_SCISSOR_TEST    0x08

typedef struct
{
    uint8_t flags;

    /* blend */
    IgnisBlendEquation blend_eq;
    IgnisBlendFunc sfactor, dfactor;

    /* cull face */
    IgnisCullFaceMode cull_mode;
} IgnisState;

void ignisQueryState(IgnisState* state);
void ignisBindState(const IgnisState* state);

/*
 * --------------------------------------------------------------
 *                          logging
 * --------------------------------------------------------------
 */
#ifndef IGNIS_DISABLE_LOGGING

#define IGNIS_TRACE(s, ...)     _ignisLog(IGNIS_LOG_TRACE, s, __VA_ARGS__)
#define IGNIS_INFO(s, ...)      _ignisLog(IGNIS_LOC_INFO, s, __VA_ARGS__)
#define IGNIS_WARN(s, ...)      _ignisLog(IGNIS_LOG_WARN, s, __VA_ARGS__)
#define IGNIS_ERROR(s, ...)     _ignisLog(IGNIS_LOG_ERROR, s, __VA_ARGS__)
#define IGNIS_CRITICAL(s, ...)  _ignisLog(IGNIS_LOG_CRITICAL, s, __VA_ARGS__)

#else

#define IGNIS_TRACE(s, ...)
#define IGNIS_INFO(s, ...)
#define IGNIS_WARN(s, ...)
#define IGNIS_ERROR(s, ...)
#define IGNIS_CRITICAL(s, ...)

#endif

typedef enum
{
    IGNIS_LOG_TRACE,
    IGNIS_LOC_INFO,
    IGNIS_LOG_WARN,
    IGNIS_LOG_ERROR,
    IGNIS_LOG_CRITICAL
} IgnisLogLevel;

void ignisSetLogCallback(void (*callback)(IgnisLogLevel, const char*));
void _ignisLog(IgnisLogLevel level, const char* fmt, ...);

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