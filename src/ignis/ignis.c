#include "Ignis.h"

void APIENTRY _ignisDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    /* ignore non-significant error/warning codes */
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    IGNIS_ERROR("[OpenGL] Debug output (%d):", id);
    IGNIS_ERROR("[OpenGL] %s", message);

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:               IGNIS_ERROR("[OpenGL] Source: API"); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     IGNIS_ERROR("[OpenGL] Source: Window System"); break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:   IGNIS_ERROR("[OpenGL] Source: Shader Compiler"); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:       IGNIS_ERROR("[OpenGL] Source: Third Party"); break;
    case GL_DEBUG_SOURCE_APPLICATION:       IGNIS_ERROR("[OpenGL] Source: Application"); break;
    case GL_DEBUG_SOURCE_OTHER:             IGNIS_ERROR("[OpenGL] Source: Other"); break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               IGNIS_ERROR("[OpenGL] Type: Error"); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: IGNIS_ERROR("[OpenGL] Type: Deprecated Behaviour"); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  IGNIS_ERROR("[OpenGL] Type: Undefined Behaviour"); break;
    case GL_DEBUG_TYPE_PORTABILITY:         IGNIS_ERROR("[OpenGL] Type: Portability"); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         IGNIS_ERROR("[OpenGL] Type: Performance"); break;
    case GL_DEBUG_TYPE_MARKER:              IGNIS_ERROR("[OpenGL] Type: Marker"); break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          IGNIS_ERROR("[OpenGL] Type: Push Group"); break;
    case GL_DEBUG_TYPE_POP_GROUP:           IGNIS_ERROR("[OpenGL] Type: Pop Group"); break;
    case GL_DEBUG_TYPE_OTHER:               IGNIS_ERROR("[OpenGL] Type: Other"); break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:            IGNIS_ERROR("[OpenGL] Severity: high"); break;
    case GL_DEBUG_SEVERITY_MEDIUM:          IGNIS_ERROR("[OpenGL] Severity: medium"); break;
    case GL_DEBUG_SEVERITY_LOW:             IGNIS_ERROR("[OpenGL] Severity: low"); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:    IGNIS_ERROR("[OpenGL] Severity: notification"); break;
    }
}

int ignisInit(int debug)
{
    /* loading glad */
    if (!gladLoadGL())
    {
        IGNIS_ERROR("[GLAD] Failed to initialize GLAD");
        return 0;
    }

    if (debug)
    {
        /* Set up opengl debug output */
        GLint flags, minor, major;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_MAJOR_VERSION, &major);

        if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) && (major >= 4 && minor >= 3))
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(_ignisDebugOutput, NULL);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        }
        else
        {
            IGNIS_ERROR("[OpenGL] Could not create debug context");
        }
    }

    /* set up default texture */
    unsigned char default_pixels[4] = { 255, 255, 255, 255 };
    if (!ignisGenerateTexture2D(&IGNIS_DEFAULT_TEXTURE2D, 1, 1, default_pixels, NULL))
    {
        IGNIS_WARN("[Ignis] Failed to create default texture2D");
    }

    return 1;
}

void ignisDestroy()
{
    ignisDeleteTexture2D(&IGNIS_DEFAULT_TEXTURE2D);
}

static void (*s_ignisErrorCb)(ignisErrorLevel level, const char* fmt);

void ignisSetErrorCallback(void (*callback)(ignisErrorLevel, const char*))
{
    s_ignisErrorCb = callback;
}

void _ignisError(ignisErrorLevel level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
    char* buffer = ignisMalloc(buffer_size + 1);
    vsnprintf(buffer, buffer_size + 1, fmt, args);
    va_end(args);

    if (s_ignisErrorCb) s_ignisErrorCb(level, buffer);

    ignisFree(buffer);
}

int ignisEnableBlend(GLenum sfactor, GLenum dfactor)
{
    if (sfactor == GL_NONE && dfactor == GL_NONE)
    {
        glDisable(GL_BLEND);
        return 0;
    }

    glEnable(GL_BLEND);
    glBlendFunc(sfactor, dfactor);
    return 1;
}

void ignisSetClearColor(IgnisColorRGBA color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void ignisClearColorBuffer(IgnisColorRGBA color)
{
    glClearBufferfv(GL_COLOR, 0, &color.r);
}

size_t ignisGetTypeSize(IgnisType type)
{
    switch (type)
    {
    case IGNIS_INT8:    return sizeof(int8_t);
    case IGNIS_UINT8:   return sizeof(uint8_t);
    case IGNIS_INT16:   return sizeof(int16_t);
    case IGNIS_UINT16:  return sizeof(uint16_t);
    case IGNIS_INT32:   return sizeof(int32_t);
    case IGNIS_UINT32:  return sizeof(uint32_t);
    case IGNIS_FLOAT:   return sizeof(float);
    default: return 0;
    }
}

char* ignisReadFile(const char* path, size_t* sizeptr)
{
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        IGNIS_ERROR("[Ignis] Failed to open file: %s", path);
        return NULL;
    }

    /* find file size */
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);


    char* buffer = ignisMalloc(size + 1);
    if (buffer)
    {
        if (fread(buffer, size, 1, file) != 1)
        {
            IGNIS_ERROR("[Ignis] Failed to read file: %s", path);
            ignisFree(buffer);
            fclose(file);
            return NULL;
        }

        buffer[size] = '\0'; /* zero terminate buffer */
        if (sizeptr) *sizeptr = size + 1;
    }
    else
    {
        IGNIS_ERROR("[Ignis] Failed to allocate memory for file: %s", path);
    }

    fclose(file);
    return buffer;
}

const char* ignisTextFormat(const char* fmt, ...)
{
    static char buffers[IGNIS_TEXTFORMAT_BUFFERS][IGNIS_FORMAT_BUFFER_LENGTH] = { 0 };
    static uint8_t index = 0;

    char* current = buffers[index++];
    memset(current, 0, IGNIS_FORMAT_BUFFER_LENGTH);

    va_list args;
    va_start(args, fmt);
    vsnprintf(current, IGNIS_FORMAT_BUFFER_LENGTH, fmt, args);
    va_end(args);

    // reset index after IGNIS_TEXTFORMAT_BUFFERS calls
    if (index >= IGNIS_TEXTFORMAT_BUFFERS) index = 0;
    return current;
}

void ignisGetVersion(int* major, int* minor, int* rev)
{
    if (major != NULL) *major = IGNIS_VERSION_MAJOR;
    if (minor != NULL) *minor = IGNIS_VERSION_MINOR;
    if (rev != NULL)   *rev = IGNIS_VERSION_REVISION;
}

#define IGNIS_CONCAT_VERSION(m, n, r) #m "." #n "." #r
#define IGNIS_MAKE_VERSION_STR(m, n, r) IGNIS_CONCAT_VERSION(m, n, r)

const char* ignisGetVersionString(void)
{
    return IGNIS_MAKE_VERSION_STR(IGNIS_VERSION_MAJOR, IGNIS_VERSION_MINOR, IGNIS_VERSION_REVISION);
}

const char* ignisGetGLVersion()   { return glGetString(GL_VERSION); }
const char* ignisGetGLVendor()    { return glGetString(GL_VENDOR); }
const char* ignisGetGLRenderer()  { return glGetString(GL_RENDERER); }
const char* ignisGetGLSLVersion() { return glGetString(GL_SHADING_LANGUAGE_VERSION); }

static void* ignis_allocator;
static ignisMallocCb  ignis_malloc;
static ignisReallocCb ignis_realloc;
static ignisFreeCb    ignis_free;

void ignisSetAllocator(void* allocator, ignisMallocCb malloc, ignisReallocCb realloc, ignisFreeCb free)
{
    ignis_allocator = allocator;
    ignis_malloc = malloc;
    ignis_realloc = realloc;
    ignis_free = free;
}

void* ignisMalloc(size_t size)
{
    return ignis_malloc ? ignis_malloc(ignis_allocator, size) : malloc(size);
}

void* ignisRealloc(void* block, size_t size)
{
    return ignis_realloc ? ignis_realloc(ignis_allocator, block, size) : realloc(block, size);
}

void ignisFree(void* block)
{
    if (ignis_free) ignis_free(ignis_allocator, block);
    else            free(block);
}
