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
#include "shader.hpp"

using vec3u8 = pl::vec3<u8>;

using vec2f32 = pl::vec2<f32>;
using vec3f32 = pl::vec3<f32>;
using vec4f32 = pl::vec4<f32>;

template<typename T> using Texture1D = pl::TextureHandle;
template<typename T> using Texture1DArray = pl::TextureHandle;
template<typename T> using Texture2D = pl::TextureHandle;
template<typename T> using Texture2DArray = pl::TextureHandle;
template<typename T> using Texture2DMS = pl::TextureHandle;
template<typename T> using Texture2DMSArray = pl::TextureHandle;
template<typename T> using TextureCube = pl::TextureHandle;
template<typename T> using TextureCubeArray = pl::TextureHandle;
template<typename T> using Texture3D = pl::TextureHandle;

template<typename T> using Image1D = pl::ImageHandle;
template<typename T> using Image1DArray = pl::ImageHandle;
template<typename T> using Image2D = pl::ImageHandle;
template<typename T> using Image2DArray = pl::ImageHandle;
template<typename T> using Image2DMS = pl::ImageHandle;
template<typename T> using Image2DMSArray = pl::ImageHandle;
template<typename T> using ImageCube = pl::ImageHandle;
template<typename T> using ImageCubeArray = pl::ImageHandle;
template<typename T> using Image3D = pl::ImageHandle;

#else

import polaris;

#endif