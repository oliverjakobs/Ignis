#include "shader.h"

#include "../Ignis.h"

IgnisShader ignisCreateShadervf(const char* vert, const char* frag)
{
    char* vert_src = ignisReadFile(vert, NULL);
    if (!vert_src)
    {
        IGNIS_ERROR("[SHADER] Failed to read file: %s", vert);
        return IGNIS_FAILURE;
    }

    char* frag_src = ignisReadFile(frag, NULL);
    if (!frag_src)
    {
        IGNIS_ERROR("[SHADER] Failed to read file: %s", frag);
        return IGNIS_FAILURE;
    }

    IgnisShader shader = ignisCreateShaderSrcvf(vert_src, frag_src);

    ignisFree(vert_src);
    ignisFree(frag_src);

    return shader;
}

IgnisShader ignisCreateShadervgf(const char* vert, const char* geom, const char* frag)
{
    char* vert_src = ignisReadFile(vert, NULL);
    if (!vert_src)
    {
        IGNIS_ERROR("[SHADER] Failed to read file: %s", vert);
        return IGNIS_FAILURE;
    }

    char* geom_src = ignisReadFile(geom, NULL);
    if (!geom_src)
    {
        IGNIS_ERROR("[SHADER] Failed to read file: %s", geom);
        return IGNIS_FAILURE;
    }

    char* frag_src = ignisReadFile(frag, NULL);
    if (!frag_src)
    {
        IGNIS_ERROR("[SHADER] Failed to read file: %s", frag);
        return IGNIS_FAILURE;
    }

    IgnisShader shader = ignisCreateShaderSrcvgf(vert_src, geom_src, frag_src);

    ignisFree(vert_src);
    ignisFree(geom_src);
    ignisFree(frag_src);

    return shader;
}

IgnisShader ignisCreateShaderSrcvf(const char* vert, const char* frag)
{
    if (!(vert && frag)) return IGNIS_FAILURE;

    IgnisShaderType types[] = { IGNIS_VERTEX_SHADER, IGNIS_FRAGMENT_SHADER };
    const char* sources[] = { vert, frag };

    return (IgnisShader)ignisCreateGLShaderProgram(types, sources, 2);
}

IgnisShader ignisCreateShaderSrcvgf(const char* vert, const char* geom, const char* frag)
{
    if (!(vert && geom && frag)) return IGNIS_FAILURE;

    IgnisShaderType types[] = { IGNIS_VERTEX_SHADER, IGNIS_GEOMETRY_SHADER, IGNIS_FRAGMENT_SHADER };
    const char* sources[] = { vert, geom, frag };

    return (IgnisShader)ignisCreateGLShaderProgram(types, sources, 3);
}

void ignisDeleteShader(IgnisShader shader)
{
    glDeleteProgram(shader);
}

void ignisUseShader(IgnisShader shader)
{
    /* check if program handle refer to an object generated by OpenGL */
    if (shader) IGNIS_ASSERT(glIsProgram(shader));
    glUseProgram(shader);
}

GLint ignisGetUniformLocation(IgnisShader shader, const char* name)
{
    GLint location = glGetUniformLocation(shader, name);

    if (location < 0) 
        IGNIS_WARN("[SHADER] Uniform %s not found", name);

    return location;
}

void ignisSetUniformi(IgnisShader shader, const char* name, int value)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniformil(shader, location, value);
}

void ignisSetUniformui(IgnisShader shader, const char* name, uint32_t value)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniformuil(shader, location, value);
}

void ignisSetUniformf(IgnisShader shader, const char* name, float value)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniformfl(shader, location, value);
}

void ignisSetUniform1i(IgnisShader shader, const char* name, GLsizei count, const int* value)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniform1il(shader, location, count, value);
}

void ignisSetUniform1f(IgnisShader shader, const char* name, GLsizei count, const float* value)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniform1fl(shader, location, count, value);
}

void ignisSetUniform2f(IgnisShader shader, const char* name, GLsizei count, const float* values)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniform2fl(shader, location, count, values);
}

void ignisSetUniform3f(IgnisShader shader, const char* name, GLsizei count, const float* values)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniform3fl(shader, location, count, values);
}

void ignisSetUniform4f(IgnisShader shader, const char* name, GLsizei count, const float* values)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniform4fl(shader, location, count, values);
}

void ignisSetUniformMat2(IgnisShader shader, const char* name, GLsizei count, const float* values)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniformMat2l(shader, location, count, values);
}

void ignisSetUniformMat3(IgnisShader shader, const char* name, GLsizei count, const float* values)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniformMat3l(shader, location, count, values);
}

void ignisSetUniformMat4(IgnisShader shader, const char* name, GLsizei count, const float* values)
{
    GLint location = ignisGetUniformLocation(shader, name);
    if (location >= 0) ignisSetUniformMat4l(shader, location, count, values);
}

void ignisSetUniformil(IgnisShader shader, GLint location, int value)
{
    glProgramUniform1i(shader, location, value);
}

void ignisSetUniformuil(IgnisShader shader, GLint location, uint32_t value)
{
    glProgramUniform1ui(shader, location, value);
}

void ignisSetUniformfl(IgnisShader shader, GLint location, float value)
{
    glProgramUniform1f(shader, location, value);
}

void ignisSetUniform1il(IgnisShader shader, GLint location, GLsizei count, const int* value)
{
    glProgramUniform1iv(shader, location, count, value);
}

void ignisSetUniform1fl(IgnisShader shader, GLint location, GLsizei count, const float* value)
{
    glProgramUniform1fv(shader, location, count, value);
}

void ignisSetUniform2fl(IgnisShader shader, GLint location, GLsizei count, const float* values)
{
    glProgramUniform2fv(shader, location, count, values);
}

void ignisSetUniform3fl(IgnisShader shader, GLint location, GLsizei count, const float* values)
{
    glProgramUniform3fv(shader, location, count, values);
}

void ignisSetUniform4fl(IgnisShader shader, GLint location, GLsizei count, const float* values)
{
    glProgramUniform4fv(shader, location, count, values);
}

void ignisSetUniformMat2l(IgnisShader shader, GLint location, GLsizei count, const float* values)
{
    glProgramUniformMatrix2fv(shader, location, count, GL_FALSE, values);
}

void ignisSetUniformMat3l(IgnisShader shader, GLint location, GLsizei count, const float* values)
{
    glProgramUniformMatrix3fv(shader, location, count, GL_FALSE, values);
}

void ignisSetUniformMat4l(IgnisShader shader, GLint location, GLsizei count, const float* values)
{
    glProgramUniformMatrix4fv(shader, location, count, GL_FALSE, values);
}

GLuint ignisCreateGLShaderProgram(IgnisShaderType* types, const char** sources, size_t count)
{
    IgnisShader program = glCreateProgram();

    GLuint attached_shader[6];
    GLuint shader_count = 0;

    for (size_t i = 0; i < count; i++)
    {
        GLuint shader = ignisCompileGLShader(types[i], sources[i]);

        if (shader == 0)
        {
            glDeleteProgram(program);
            return IGNIS_FAILURE;
        }

        glAttachShader(program, shader);
        attached_shader[shader_count] = shader;
        shader_count++;
    }

    glLinkProgram(program);

    for (size_t i = 0; i < shader_count; i++)
    {
        glDeleteShader(attached_shader[i]);
        glDetachShader(program, attached_shader[i]);
    }

    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        IGNIS_ERROR("[SHADER] Linking Error");
        ignisPrintProgramLog(program);
        glDeleteProgram(program);
        return IGNIS_FAILURE;
    }

    glValidateProgram(program);

    result = GL_FALSE;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
    if (result == GL_FALSE)
    {
        IGNIS_ERROR("[SHADER] Validating Error");
        ignisPrintProgramLog(program);
        glDeleteProgram(program);

        return IGNIS_FAILURE;
    }

    return program;
}

static const char* ignisGetShaderTypeName(IgnisShaderType type)
{
    switch (type)
    {
    case IGNIS_COMPUTE_SHADER:      return "COMPUTE_SHADER";
    case IGNIS_VERTEX_SHADER:       return "VERTEX_SHADER";
    case IGNIS_TESS_CONTROL_SHADER: return "TESS_CONTROL_SHADER";
    case IGNIS_TESS_EVAL_SHADER:    return "TESS_EVALUATION_SHADER";
    case IGNIS_GEOMETRY_SHADER:     return "GEOMETRY_SHADER";
    case IGNIS_FRAGMENT_SHADER:     return "FRAGMENT_SHADER";
    }

    return "INVALID_SHADER_TYPE";
}

GLuint ignisCompileGLShader(IgnisShaderType type, const char* source)
{
    if (source[0] == '\0')
    {
        IGNIS_ERROR("[SHADER] Shader source is missing for %s", ignisGetShaderTypeName(type));
        return IGNIS_FAILURE;
    }

    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        IGNIS_ERROR("[SHADER] Compiling Error (%s)", ignisGetShaderTypeName(type));
        ignisPrintShaderLog(shader);
        glDeleteShader(shader);

        return IGNIS_FAILURE;
    }

    return shader;
}

void ignisPrintShaderLog(GLuint shader)
{
    if (!glIsShader(shader))
    {
        IGNIS_ERROR("[SHADER] Failed to log: Object is not a shader");
        return;
    }

    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    char* log_buffer = ignisMalloc(log_length);
    glGetShaderInfoLog(shader, log_length, &log_length, log_buffer);

    IGNIS_ERROR("[SHADER] %.*s", log_length, log_buffer);

    ignisFree(log_buffer);
}

void ignisPrintProgramLog(GLuint program)
{
    if (!glIsProgram(program))
    {
        IGNIS_ERROR("[SHADER] Failed to log: Object is not a program");
        return;
    }

    GLint log_length = 0;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &log_length);
    char* log_buffer = ignisMalloc(log_length);
    glGetShaderInfoLog(program, log_length, &log_length, log_buffer);

    IGNIS_ERROR("[SHADER] %.*s", log_length, log_buffer);

    ignisFree(log_buffer);
}