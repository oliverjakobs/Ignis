#ifndef IGNIS_TEXTURE_H
#define IGNIS_TEXTURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../glad/glad.h"

#include <stdint.h>

typedef struct
{
    GLint internal_format;
    GLenum format;

    GLint min_filter;
    GLint mag_filter;

    GLint wrap_s;
    GLint wrap_t;

    uint8_t flip_on_load;
} IgnisTextureConfig;

GLuint ignisGenerateTexture(GLuint target, int w, int h, const void* pixels, IgnisTextureConfig config);

#define IGNIS_DEFAULT_CONFIG (IgnisTextureConfig){ GL_RGBA8, GL_RGBA, GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT, 0 }

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