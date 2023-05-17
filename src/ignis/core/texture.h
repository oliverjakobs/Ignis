#ifndef IGNIS_TEXTURE_H
#define IGNIS_TEXTURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../glad/glad.h"
#include "types.h"

typedef struct
{
    GLint internal_format;
    IgnisTextureFormat format;

    IgnisTextureFilter min_filter;
    IgnisTextureFilter mag_filter;

    IgnisTextureWrap wrap_s;
    IgnisTextureWrap wrap_t;

    uint8_t flip_on_load;
} IgnisTextureConfig;

GLuint ignisGenerateTexture(IgnisTextureTarget target, int w, int h, const void* pixels, IgnisTextureConfig config);

#define IGNIS_DEFAULT_CONFIG (IgnisTextureConfig){ GL_RGBA8, IGNIS_RGBA, IGNIS_LINEAR, IGNIS_LINEAR, IGNIS_REPEAT, IGNIS_REPEAT, 0 }

typedef struct
{
    GLuint name;
    int width, height;
} IgnisTexture2D;

int ignisGenerateTexture2D(IgnisTexture2D* texture, int w, int h, const void* pixels, IgnisTextureConfig* config);
int ignisGenerateTexStorage2D(IgnisTexture2D* texture, int width, int height, GLenum internal_format);

int ignisCreateTexture2D(IgnisTexture2D* texture, const char* path, IgnisTextureConfig* config);
int ignisCreateTexture2DSrc(IgnisTexture2D* texture, const uint8_t* data, size_t size, IgnisTextureConfig* config);

void ignisDeleteTexture2D(IgnisTexture2D* texture);

void ignisBindTexture2D(const IgnisTexture2D* texture, GLuint slot);

/* default texture */
extern IgnisTexture2D IGNIS_DEFAULT_TEXTURE2D;
int ignisIsDefaultTexture2D(IgnisTexture2D tex);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_TEXTURE_H */