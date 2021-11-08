#include "device.h"

#include <stdio.h>

typedef struct
{
    uint32_t familiesSet;
    uint32_t graphicsFamily;
} ignisQueueFamilyIndices;

static bool ignisQueueFamilyIndicesComplete(ignisQueueFamilyIndices indices)
{
    return indices.familiesSet | VK_QUEUE_GRAPHICS_BIT;
}

static ignisQueueFamilyIndices ignisFindQueueFamilies(VkPhysicalDevice device)
{
    ignisQueueFamilyIndices indices = { 0 };

    uint32_t property_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &property_count, NULL);

    VkQueueFamilyProperties* properties = malloc(sizeof(VkQueueFamilyProperties) * property_count);
    if (!properties) return indices;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &property_count, properties);

    for (uint32_t i = 0; i < property_count; ++i)
    {
        if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        if (ignisQueueFamilyIndicesComplete(indices))
            break;
    }

    return indices;
}

static bool ignisIsDeviceSuitable(VkPhysicalDevice device)
{
    ignisQueueFamilyIndices indices = ignisFindQueueFamilies(device);
    return ignisQueueFamilyIndicesComplete(indices);
}

VkPhysicalDevice ignisPickPhysicalDevice(VkInstance instance)
{
    VkPhysicalDevice device = VK_NULL_HANDLE;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);
    if (device_count == 0)
    {
        fprintf(stderr, "failed to find GPUs with Vulkan support.");
        return VK_NULL_HANDLE;
    }

    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * device_count);
    if (!devices)
    {
        fprintf(stderr, "failed to allocate memory for physical devices.");
        return VK_NULL_HANDLE;
    }
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    for (uint32_t i = 0; i < device_count; ++i)
    {
        if (ignisIsDeviceSuitable(devices[i]))
        {
            device = devices[i];
            break;
        }
    }

    if (device == VK_NULL_HANDLE)
    {
        fprintf(stderr, "failed to find a suitable GPU.");
    }

    free(devices);
    return device;
}


VkDevice ignisCreateLogicalDevice(VkPhysicalDevice physicalDevice, VkQueue* graphicsQueue)
{
    ignisQueueFamilyIndices indices = ignisFindQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo = { 0 };
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = { 0 };

    VkDeviceCreateInfo createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    VkDevice device;
    if (vkCreateDevice(physicalDevice, &createInfo, NULL, &device) != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create logical device!");
        return VK_NULL_HANDLE;
    }

    vkGetDeviceQueue(device, indices.graphicsFamily, 0, graphicsQueue);

    return device;
}