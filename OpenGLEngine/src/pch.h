#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

#define HZ_ASSERT(x, ...) { if(!(x)) { LOGGER_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }