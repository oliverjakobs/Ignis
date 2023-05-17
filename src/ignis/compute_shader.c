#include "compute_shader.h"

#include "ignis.h"

int ignisCreateComputeShader(IgnisComputeShader* shader, const char* path)
{
    char* src = ignisReadFile(path, NULL);
    if (!src)
    {
        IGNIS_ERROR("[SHADER] Failed to read file: %s", path);
        return IGNIS_FAILURE;
    }

    int status = ignisCreateComputeShaderSrc(shader, src);

    ignisFree(src);

    return IGNIS_SUCCESS;
}

int ignisCreateComputeShaderSrc(IgnisComputeShader* shader, const char* src)
{
    if (!(shader && src)) return IGNIS_FAILURE;

    IgnisShaderType types[] = { IGNIS_COMPUTE_SHADER };
    const char* sources[] = { src };

    shader->program = ignisCreateGLShaderProgram(types, sources, 1);

    /* set shader info */
    glGetProgramiv(shader->program, GL_COMPUTE_WORK_GROUP_SIZE, shader->work_group_size);

    return IGNIS_SUCCESS;
}

void ignisDeleteComputeShader(IgnisComputeShader* shader)
{
    ignisDeleteShader(shader->program);
}

void ignisDispatchCompute(IgnisComputeShader* shader, GLuint groups_x, GLuint groups_y, GLuint groups_z)
{
    glDispatchCompute(groups_x, groups_y, groups_z);
}
