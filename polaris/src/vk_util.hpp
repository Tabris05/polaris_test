#pragma once

#include <initializer_list>
#include <polaris/pod_types.hpp>

template<typename T>
T* ptr(T&& val) {
    return &val;
}

template<typename T>
const T* ptr(std::initializer_list<T> val) {
    return val.begin();
}

inline VkFormat vkFormat(pl::Format format) {
    switch(format) {
        case pl::Format::None: return VK_FORMAT_UNDEFINED;
        case pl::Format::RG4_UNORM: return VK_FORMAT_R4G4_UNORM_PACK8;
        case pl::Format::RGBA4_UNORM: return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
        case pl::Format::RGB565_UNORM: return VK_FORMAT_R5G6B5_UNORM_PACK16;
        case pl::Format::A1RGB5_UNORM: return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
        case pl::Format::R8_UNORM: return VK_FORMAT_R8_UNORM;
        case pl::Format::R8_SNORM: return VK_FORMAT_R8_SNORM;
        case pl::Format::R8_UINT: return VK_FORMAT_R8_UINT;
        case pl::Format::R8_SINT: return VK_FORMAT_R8_SINT;
        case pl::Format::R8_SRGB: return VK_FORMAT_R8_SRGB;
        case pl::Format::RG8_UNORM: return VK_FORMAT_R8G8_UNORM;
        case pl::Format::RG8_SNORM: return VK_FORMAT_R8G8_SNORM;
        case pl::Format::RG8_UINT: return VK_FORMAT_R8G8_UINT;
        case pl::Format::RG8_SINT: return VK_FORMAT_R8G8_SINT;
        case pl::Format::RGBA8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
        case pl::Format::RGBA8_SNORM: return VK_FORMAT_R8G8B8A8_SNORM;
        case pl::Format::RGBA8_UINT: return VK_FORMAT_R8G8B8A8_UINT;
        case pl::Format::RGBA8_SINT: return VK_FORMAT_R8G8B8A8_SINT;
        case pl::Format::RGBA8_SRGB: return VK_FORMAT_R8G8B8A8_SRGB;
        case pl::Format::A2BGR10_UNORM: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        case pl::Format::A2BGR10_UINT: return VK_FORMAT_A2B10G10R10_UINT_PACK32;
        case pl::Format::B10GR11_UFLOAT: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
        case pl::Format::E5BGR9_UFLOAT: return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
        case pl::Format::R16_UNORM: return VK_FORMAT_R16_UNORM;
        case pl::Format::R16_SNORM: return VK_FORMAT_R16_SNORM;
        case pl::Format::R16_UINT: return VK_FORMAT_R16_UINT;
        case pl::Format::R16_SINT: return VK_FORMAT_R16_SINT;
        case pl::Format::R16_SFLOAT: return VK_FORMAT_R16_SFLOAT;
        case pl::Format::RG16_UNORM: return VK_FORMAT_R16G16_UNORM;
        case pl::Format::RG16_SNORM: return VK_FORMAT_R16G16_SNORM;
        case pl::Format::RG16_UINT: return VK_FORMAT_R16G16_UINT;
        case pl::Format::RG16_SINT: return VK_FORMAT_R16G16_SINT;
        case pl::Format::RG16_SFLOAT: return VK_FORMAT_R16G16_SFLOAT;
        case pl::Format::RGBA16_UNORM: return VK_FORMAT_R16G16B16A16_UNORM;
        case pl::Format::RGBA16_SNORM: return VK_FORMAT_R16G16B16A16_SNORM;
        case pl::Format::RGBA16_UINT: return VK_FORMAT_R16G16B16A16_UINT;
        case pl::Format::RGBA16_SINT: return VK_FORMAT_R16G16B16A16_SINT;
        case pl::Format::RGBA16_SFLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;
        case pl::Format::R32_UINT: return VK_FORMAT_R32_UINT;
        case pl::Format::R32_SINT: return VK_FORMAT_R32_SINT;
        case pl::Format::R32_SFLOAT: return VK_FORMAT_R32_SFLOAT;
        case pl::Format::RG32_UINT: return VK_FORMAT_R32G32_UINT;
        case pl::Format::RG32_SINT: return VK_FORMAT_R32G32_SINT;
        case pl::Format::RG32_SFLOAT: return VK_FORMAT_R32G32_SFLOAT;
        case pl::Format::RGBA32_UINT: return VK_FORMAT_R32G32B32A32_UINT;
        case pl::Format::RGBA32_SINT: return VK_FORMAT_R32G32B32A32_SINT;
        case pl::Format::RGBA32_SFLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
        case pl::Format::R64_UINT: return VK_FORMAT_R64_UINT;
        case pl::Format::R64_SINT: return VK_FORMAT_R64_SINT;
        case pl::Format::D16_UNORM: return VK_FORMAT_D16_UNORM;
        case pl::Format::X8_D24_UNORM: return VK_FORMAT_X8_D24_UNORM_PACK32;
        case pl::Format::D32_SFLOAT: return VK_FORMAT_D32_SFLOAT;
        case pl::Format::S8_UINT: return VK_FORMAT_S8_UINT;
        case pl::Format::D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
        case pl::Format::D32_SFLOAT_S8_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
        case pl::Format::BC1_RGB_UNORM: return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
        case pl::Format::BC1_RGB_SRGB: return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
        case pl::Format::BC1_RGBA_UNORM: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
        case pl::Format::BC1_RGBA_SRGB: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
        case pl::Format::BC2_UNORM: return VK_FORMAT_BC2_UNORM_BLOCK;
        case pl::Format::BC2_SRGB: return VK_FORMAT_BC2_SRGB_BLOCK;
        case pl::Format::BC3_UNORM: return VK_FORMAT_BC3_UNORM_BLOCK;
        case pl::Format::BC3_SRGB: return VK_FORMAT_BC3_SRGB_BLOCK;
        case pl::Format::BC4_UNORM: return VK_FORMAT_BC4_UNORM_BLOCK;
        case pl::Format::BC4_SNORM: return VK_FORMAT_BC4_SNORM_BLOCK;
        case pl::Format::BC5_UNORM: return VK_FORMAT_BC5_UNORM_BLOCK;
        case pl::Format::BC5_SNORM: return VK_FORMAT_BC5_SNORM_BLOCK;
        case pl::Format::BC6_UFLOAT: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
        case pl::Format::BC6_SFLOAT: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
        case pl::Format::BC7_UNORM: return VK_FORMAT_BC7_UNORM_BLOCK;
        case pl::Format::BC7_SRGB: return VK_FORMAT_BC7_SRGB_BLOCK;
    }
}

inline VkImageType vkImageType(pl::TextureType type) {
    switch(type) {
        case pl::TextureType::Type1D: return VK_IMAGE_TYPE_1D;
        case pl::TextureType::Type2D: return VK_IMAGE_TYPE_2D;
        case pl::TextureType::Type3D: return VK_IMAGE_TYPE_3D;
    }
}

inline VkImageViewType vkImageViewType(pl::TextureViewType type) {
    switch(type) {
        case pl::TextureViewType::Type1D: return VK_IMAGE_VIEW_TYPE_1D;
        case pl::TextureViewType::Type1DArray: return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        case pl::TextureViewType::Type2D: return VK_IMAGE_VIEW_TYPE_2D;
        case pl::TextureViewType::Type2DArray: return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case pl::TextureViewType::Cubemap: return VK_IMAGE_VIEW_TYPE_CUBE;
        case pl::TextureViewType::CubemapArray: return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		case pl::TextureViewType::Type3D: return VK_IMAGE_VIEW_TYPE_3D;
    }
}

inline VkComponentSwizzle vkSwizzle(pl::Swizzle swizzle) {
    switch(swizzle) {
        case pl::Swizzle::Identity: return VK_COMPONENT_SWIZZLE_IDENTITY;
        case pl::Swizzle::Zero: return VK_COMPONENT_SWIZZLE_ZERO;
        case pl::Swizzle::One: return VK_COMPONENT_SWIZZLE_ONE;
        case pl::Swizzle::Red: return VK_COMPONENT_SWIZZLE_R;
        case pl::Swizzle::Green: return VK_COMPONENT_SWIZZLE_G;
        case pl::Swizzle::Blue: return VK_COMPONENT_SWIZZLE_B;
        case pl::Swizzle::Alpha: return VK_COMPONENT_SWIZZLE_A;
    }
}

inline VkFilter vkFilter(pl::Filter filter) {
    switch(filter) {
        case pl::Filter::Nearest: return VK_FILTER_NEAREST;
        case pl::Filter::Linear: return VK_FILTER_LINEAR;
    }
}

inline VkSamplerMipmapMode vkMipFilter(pl::Filter filter) {
    switch(filter) {
        case pl::Filter::Nearest: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case pl::Filter::Linear: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    }
}

inline VkSamplerAddressMode vkWrapMode(pl::WrapMode wrap) {
    switch(wrap) {
        case pl::WrapMode::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case pl::WrapMode::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case pl::WrapMode::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case pl::WrapMode::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    }
}

inline VkSamplerReductionMode vkReductionMode(pl::ReductionMode mode) {
    switch(mode) {
        case pl::ReductionMode::Average: return VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE;
        case pl::ReductionMode::Min: return VK_SAMPLER_REDUCTION_MODE_MIN;
        case pl::ReductionMode::Max: return VK_SAMPLER_REDUCTION_MODE_MAX;
    }
}

inline VkCompareOp vkCompareOp(pl::CompareOp op) {
    switch(op) {
        case pl::CompareOp::None: return VK_COMPARE_OP_ALWAYS;
        case pl::CompareOp::Never: return VK_COMPARE_OP_NEVER;
        case pl::CompareOp::NotEqual: return VK_COMPARE_OP_NOT_EQUAL;
        case pl::CompareOp::Less: return VK_COMPARE_OP_LESS;
        case pl::CompareOp::LessEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
        case pl::CompareOp::Equal: return VK_COMPARE_OP_EQUAL;
        case pl::CompareOp::GreaterEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case pl::CompareOp::Greater: return VK_COMPARE_OP_GREATER;
        case pl::CompareOp::Always: return VK_COMPARE_OP_ALWAYS;
    }
}

inline VkImageAspectFlags vkAspectMask(VkFormat format) {
    switch(format) {
        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_X8_D24_UNORM_PACK32:
        case VK_FORMAT_D32_SFLOAT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case VK_FORMAT_S8_UINT:
            return VK_IMAGE_ASPECT_STENCIL_BIT;
        case VK_FORMAT_D24_UNORM_S8_UINT:
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        default:
            return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

inline bool vkFormatIsDepthOrStencil(VkFormat format) {
    switch(format) {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

inline u32 vkFormatTexelBlockSize(VkFormat format) {
    switch(format) {
    case VK_FORMAT_UNDEFINED:
        return 0;
    case VK_FORMAT_R4G4_UNORM_PACK8:
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8_SRGB:
    case VK_FORMAT_S8_UINT:
        return 1;
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SNORM:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16_SFLOAT:
    case VK_FORMAT_D16_UNORM:
        return 2;
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_R8G8B8A8_SNORM:
    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_R8G8B8A8_SINT:
    case VK_FORMAT_R8G8B8A8_SRGB:
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
    case VK_FORMAT_R16G16_UNORM:
    case VK_FORMAT_R16G16_SNORM:
    case VK_FORMAT_R16G16_UINT:
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16_SFLOAT:
    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32_SFLOAT:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT:
        return 4;
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SNORM:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32_SFLOAT:
    case VK_FORMAT_R64_UINT:
    case VK_FORMAT_R64_SINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
        return 8;
    case VK_FORMAT_R32G32B32A32_UINT:
    case VK_FORMAT_R32G32B32A32_SINT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
    case VK_FORMAT_BC2_UNORM_BLOCK:
    case VK_FORMAT_BC2_SRGB_BLOCK:
    case VK_FORMAT_BC3_UNORM_BLOCK:
    case VK_FORMAT_BC3_SRGB_BLOCK:
    case VK_FORMAT_BC4_UNORM_BLOCK:
    case VK_FORMAT_BC4_SNORM_BLOCK:
    case VK_FORMAT_BC5_UNORM_BLOCK:
    case VK_FORMAT_BC5_SNORM_BLOCK:
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
    case VK_FORMAT_BC7_UNORM_BLOCK:
    case VK_FORMAT_BC7_SRGB_BLOCK:
        return 16;
    }
}

inline VkFormatFeatureFlagBits vkImageUsageToFormatFeatures(VkImageUsageFlagBits usage) {
    switch(usage) {
        case VK_IMAGE_USAGE_TRANSFER_SRC_BIT: return VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
        case VK_IMAGE_USAGE_TRANSFER_DST_BIT: return VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
        case VK_IMAGE_USAGE_SAMPLED_BIT: return VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
        case VK_IMAGE_USAGE_STORAGE_BIT: return VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
        case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT: return VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
        case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT: return VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
        default: return VkFormatFeatureFlagBits(0);
	}
}

inline VkPresentModeKHR vkPresentMode(pl::PresentMode mode) {
    switch(mode) {
        case pl::PresentMode::Immediate: return VK_PRESENT_MODE_IMMEDIATE_KHR;
        case pl::PresentMode::VSync: return VK_PRESENT_MODE_FIFO_KHR;
        case pl::PresentMode::FastSync: return VK_PRESENT_MODE_MAILBOX_KHR;
    }
}

inline VkShaderStageFlagBits vkShaderStage(pl::ShaderStage stage) {
    switch(stage) {
        case pl::ShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
        case pl::ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
        case pl::ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
}

inline VkAttachmentLoadOp vkLoadOp(pl::LoadOp op) {
    switch(op) {
        case pl::LoadOp::Load: return VK_ATTACHMENT_LOAD_OP_LOAD;
        case pl::LoadOp::Clear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
        case pl::LoadOp::None: return VK_ATTACHMENT_LOAD_OP_NONE;
    }
}

inline VkAttachmentStoreOp vkStoreOp(pl::StoreOp op) {
    switch(op) {
        case pl::StoreOp::Store: return VK_ATTACHMENT_STORE_OP_STORE;
        case pl::StoreOp::Discard: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        case pl::StoreOp::None: return VK_ATTACHMENT_STORE_OP_NONE;
    }
}

inline VkPipelineStageFlags2 vkStageMask(pl::PipelineStage stage) {
    VkPipelineStageFlags2 flags = 0;
    if((stage & pl::PipelineStage::PreRaster) != pl::PipelineStage::None) {
        flags |= VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT | VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_TESSELLATION_CONTROL_SHADER_BIT
              | VK_PIPELINE_STAGE_2_TESSELLATION_EVALUATION_SHADER_BIT | VK_PIPELINE_STAGE_2_GEOMETRY_SHADER_BIT;
    }
    if((stage & pl::PipelineStage::Fragment) != pl::PipelineStage::None) {
        flags |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    }
    if((stage & pl::PipelineStage::Compute) != pl::PipelineStage::None) {
        flags |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    if((stage & pl::PipelineStage::Copy) != pl::PipelineStage::None) {
        flags |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    }
    if((stage & pl::PipelineStage::Depth) != pl::PipelineStage::None) {
        flags |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
	}
    if((stage & pl::PipelineStage::Color) != pl::PipelineStage::None) {
        flags |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
    if((stage & pl::PipelineStage::IndirectRead) != pl::PipelineStage::None) {
        flags |= VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT;
    }

	return flags;
}

inline VkAccessFlags2 vkAccessMask(pl::PipelineStage stage) {
    VkAccessFlags2 flags = 0;
    if((stage & pl::PipelineStage::ShaderRead) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_SHADER_READ_BIT;
    }
    if((stage & pl::PipelineStage::ShaderWrite) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_SHADER_WRITE_BIT;
    }
    if((stage & pl::PipelineStage::CopyRead) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_TRANSFER_READ_BIT;
    }
    if((stage & pl::PipelineStage::CopyWrite) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
    }
    if((stage & pl::PipelineStage::DepthRead) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    }
    if((stage & pl::PipelineStage::DepthWrite) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    if((stage & pl::PipelineStage::ColorRead) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;
    }
    if((stage & pl::PipelineStage::ColorWrite) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if((stage & pl::PipelineStage::IndirectRead) != pl::PipelineStage::None) {
        flags |= VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT;
	}

	return flags;
}

inline u8 getMemoryTypeIndex(VkPhysicalDeviceMemoryProperties memProps, u32 mask, bool host = false) {
    VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    if(host) {
        flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    }

    for(u8 idx = 0; idx < memProps.memoryTypeCount; idx++) {
        if(((1 << idx) & mask) && (memProps.memoryTypes[idx].propertyFlags & flags) == flags) {
            return idx;
        }
    }
    return ~0;
}