#ifndef IGNIS_DEVICE_H
#define IGNIS_DEVICE_H

#include "common.h"

VkPhysicalDevice ignisPickPhysicalDevice(VkInstance instance);

VkDevice ignisCreateLogicalDevice(VkPhysicalDevice physicalDevice, VkQueue* graphicsQueue);

#endif // !IGNIS_DEVICE_H
