#ifndef IGNIS_LAYER_H
#define IGNIS_LAYER_H

#include "common.h"

bool ignisCheckValidationLayerSupport();

const char* const* ignisGetEnabledLayerNames(bool enable_validation, uint32_t* layer_count);

#endif // !IGNIS_LAYER_H
