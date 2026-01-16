#pragma once

#ifdef __cplusplus

#include "util_types.hpp"
#include "pod_types.hpp"
#include "device.hpp"
#include "queue.hpp"
#include "sync.hpp"
#include "command_buffer.hpp"
#include "texture.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"

using vec2f32 = pl::vec2<f32>;
using vec3f32 = pl::vec3<f32>;
using vec4f32 = pl::vec4<f32>;

template<typename T>
using Texture2D = pl::TextureHandle;

#else

import polaris;

#endif