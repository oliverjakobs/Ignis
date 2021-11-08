#include "layer.h"

const char* const validationLayers[] = {
    "VK_LAYER_KHRONOS_validation"
};

const uint32_t validationLayerCount = sizeof(validationLayers) / sizeof(validationLayers[0]);

bool ignisCheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* availableLayers = malloc(sizeof(VkLayerProperties) * layerCount);
    if (!availableLayers) return false;

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (size_t i = 0; i < validationLayerCount; ++i)
    {
        bool layerFound = false;
        const char* name = validationLayers[i];
        size_t name_len = strlen(name);

        for (size_t available = 0; available < layerCount; ++available)
        {
            if (strncmp(name, availableLayers[available].layerName, name_len) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            free(availableLayers);
            return false;
        }
    }

    free(availableLayers);
    return true;
}

const char* const* ignisGetEnabledLayerNames(bool enable_validation, uint32_t* layer_count)
{
    if (!enable_validation)
    {
        *layer_count = 0;
        return NULL;
    }

    *layer_count = validationLayerCount;
    return validationLayers;
}