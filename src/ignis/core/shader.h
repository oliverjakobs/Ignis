#ifndef IGNIS_SHADER_H
#define IGNIS_SHADER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../glad/glad.h"

typedef GLuint IgnisShader;

IgnisShader ignisCreateShadervf(const char* vert, const char* frag);
IgnisShader ignisCreateShadervgf(const char* vert, const char* geom, const char* frag);
IgnisShader ignisCreateShaderSrcvf(const char* vert, const char* frag);
IgnisShader ignisCreateShaderSrcvgf(const char* vert, const char* geom, const char* frag);
void ignisDeleteShader(IgnisShader shader);

void ignisUseShader(IgnisShader shader);

GLint ignisGetUniformLocation(IgnisShader shader, const char* name);

void ignisSetUniform1i(IgnisShader shader, const char* name, int value);
void ignisSetUniform1f(IgnisShader shader, const char* name, float value);
void ignisSetUniform2f(IgnisShader shader, const char* name, const float* values);
void ignisSetUniform3f(IgnisShader shader, const char* name, const float* values);
void ignisSetUniform4f(IgnisShader shader, const char* name, const float* values);
void ignisSetUniformMat2(IgnisShader shader, const char* name, const float* values);
void ignisSetUniformMat3(IgnisShader shader, const char* name, const float* values);
void ignisSetUniformMat4(IgnisShader shader, const char* name, const float* values);

void ignisSetUniform1iv(IgnisShader shader, const char* name, GLsizei count, const int* values);
void ignisSetUniform1fv(IgnisShader shader, const char* name, GLsizei count, const float* values);

/* unchecked location */
void ignisSetUniform1il(IgnisShader shader, GLint location, int value);
void ignisSetUniform1fl(IgnisShader shader, GLint location, float value);
void ignisSetUniform2fl(IgnisShader shader, GLint location, const float* values);
void ignisSetUniform3fl(IgnisShader shader, GLint location, const float* values);
void ignisSetUniform4fl(IgnisShader shader, GLint location, const float* values);
void ignisSetUniformMat2l(IgnisShader shader, GLint location, const float* values);
void ignisSetUniformMat3l(IgnisShader shader, GLint location, const float* values);
void ignisSetUniformMat4l(IgnisShader shader, GLint location, const float* values);

void ignisSetUniform1ivl(IgnisShader shader, GLint location, GLsizei count, const int* values);
void ignisSetUniform1fvl(IgnisShader shader, GLint location, GLsizei count, const float* values);

GLuint ignisCreateGLShaderProgram(GLenum* types, const char** sources, size_t count);
GLuint ignisCompileGLShader(GLenum type, const char* source);

void ignisPrintShaderLog(GLuint shader);
void ignisPrintProgramLog(GLuint program);

const char* ignisGetShaderType(GLenum type);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_SHADER_H */