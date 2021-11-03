#ifndef IGNIS_DEBUG_H
#define IGNIS_DEBUG_H

#include "common.h"

VkDebugUtilsMessengerCreateInfoEXT ignisGetDebugMessengerCreateInfo(PFN_vkDebugUtilsMessengerCallbackEXT callback);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* messenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* allocator);


#endif // !IGNIS_DEBUG_H
