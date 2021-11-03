#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ignis/layer.h"
#include "ignis/debug.h"

GLFWwindow* window = NULL;
VkInstance instance = NULL;

VkDebugUtilsMessengerEXT debugMessenger;

char** extensions = NULL;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data)
{
    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        fprintf(stderr, "validation layer: %s\n", callback_data->pMessage);

    return VK_FALSE;
}

const char** getRequiredExtensions(uint32_t* count)
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    extensions = malloc(sizeof(char*) * ((size_t)glfwExtensionCount + 1));

    if (!extensions) return NULL;

    memcpy(extensions, glfwExtensions, sizeof(char*) * glfwExtensionCount);

    if (enableValidationLayers)
        extensions[glfwExtensionCount++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

    *count = glfwExtensionCount;
    return extensions;
}

bool appLoad(const char* title, uint32_t width, uint32_t height)
{
    if (glfwInit() != GLFW_TRUE)
    {
        fprintf(stderr, "Failed to initialize glfw.");
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create glfw window.");
        return false;
    }

    /* create vulkan instance */
    if (enableValidationLayers && !ignisCheckValidationLayerSupport())
    {
        fprintf(stderr, "validation layers requested, but not available!");
        return false;
    }

    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    createInfo.ppEnabledExtensionNames = getRequiredExtensions(&createInfo.enabledExtensionCount);

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = ignisGetDebugMessengerCreateInfo(debugCallback);
    if (enableValidationLayers)
    {
        createInfo.ppEnabledLayerNames = ignisGetEnabledLayerNames(&createInfo.enabledLayerCount);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.ppEnabledLayerNames = NULL;
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = NULL;
    }

    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS)
    {
        fprintf(stderr, "Failed to create vulkan instance.");
        return false;
    }

    /* setup debug messenger */
    if (enableValidationLayers)
    {
        if (CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, NULL, &debugMessenger) != VK_SUCCESS)
        {
            fprintf(stderr, "failed to set up debug messenger!");
            return false;
        }
    }

    return true;
}

void appRun()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void appClose()
{
    if (enableValidationLayers) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);

    if (extensions) free(extensions);
    if (instance) vkDestroyInstance(instance, NULL);

    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

int main()
{
    if (appLoad("Vulkan", 800, 600))
        appRun();

    appClose();

    return 0;
}