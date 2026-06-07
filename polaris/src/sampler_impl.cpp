#include "sampler_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl
{
	Sampler::Sampler(const SamplerCreateInfo& ci) : m_handle(Device::get().descriptorHeap().allocSamplerHandle(&VkSamplerCreateInfo{
		.pNext = &VkSamplerReductionModeCreateInfo{ .reductionMode = vkReductionMode(ci.reductionMode) },
		.magFilter = vkFilter(ci.magFilter),
		.minFilter = vkFilter(ci.minFilter),
		.mipmapMode = vkMipFilter(ci.mipFilter),
		.addressModeU = vkWrapMode(ci.wrapU),
		.addressModeV = vkWrapMode(ci.wrapV),
		.addressModeW = vkWrapMode(ci.wrapW),
		.mipLodBias = ci.lodBias,
		.anisotropyEnable = true,
		.maxAnisotropy = ci.anisotropy,
		.compareEnable = ci.compareOp != CompareOp::None,
		.compareOp = vkCompareOp(ci.compareOp),
		.minLod = ci.minLod,
		.maxLod = ci.maxLod
	})) {}

	Sampler::Sampler(Sampler&& src) {
		memcpy(this, &src, sizeof(Sampler));
		memset(&src, 0xFF, sizeof(Sampler));
	}

	Sampler& Sampler::operator=(Sampler&& src) {
		this->~Sampler();
		new (this) Sampler(std::move(src));
		return *this;
	}

	Sampler::~Sampler() {
		if(m_handle != 0xFFFF) {
			Device::get().descriptorHeap().freeSamplerHandle(m_handle);
		}
	}

	u16 Sampler::handle() const {
		return m_handle;
	}
}