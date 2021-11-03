#ifndef IGNIS_LAYER_H
#define IGNIS_LAYER_H

#include "common.h"

bool ignisCheckValidationLayerSupport();

const char* const* ignisGetEnabledLayerNames(uint32_t* layer_count);

#endif // !IGNIS_LAYER_H
