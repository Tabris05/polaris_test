#include "device_impl.hpp"
#include "vk_util.hpp"
#include <volk/volk.h>
#include <tabris/vec.hpp>
#include <ranges>

namespace pl {
	void Device::waitIdle() {
		vkDeviceWaitIdle(m_device);
	}

	Device::Device(const DeviceCreateInfo& ci) {
		volkInitialize();
		
		u32 count;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		tbrs::Vec<VkExtensionProperties> props(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

		tbrs::Vec<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
		for(const VkExtensionProperties& prop : props) {
			if(strcmp(prop.extensionName, VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME) == 0) {
				instanceExtensions.push(prop.extensionName);
			}
			if(strcmp(prop.extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0) {
				instanceExtensions.push(prop.extensionName);
			}
			if(strcmp(prop.extensionName, VK_KHR_XCB_SURFACE_EXTENSION_NAME) == 0) {
				instanceExtensions.push(prop.extensionName);
			}
			if(strcmp(prop.extensionName, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME) == 0) {
				instanceExtensions.push(prop.extensionName);
			}
			if(strcmp(prop.extensionName, VK_EXT_METAL_SURFACE_EXTENSION_NAME) == 0) {
				instanceExtensions.push(prop.extensionName);
			}
			if(strcmp(prop.extensionName, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME) == 0) {
				instanceExtensions.push(prop.extensionName);
			}
			if(strcmp(prop.extensionName, VK_NN_VI_SURFACE_EXTENSION_NAME) == 0) {
				instanceExtensions.push(prop.extensionName);
			}
		}
	
		vkCreateInstance(ptr(VkInstanceCreateInfo{
				.pApplicationInfo = ptr(VkApplicationInfo{
				.apiVersion = VK_API_VERSION_1_4
			}),
			.enabledExtensionCount = static_cast<u32>(instanceExtensions.count()),
			.ppEnabledExtensionNames = instanceExtensions.data()
		}), nullptr, &m_instance);
	
		volkLoadInstanceOnly(m_instance);
	
		// foo: eventually should "properly" select physical device
		vkEnumeratePhysicalDevices(m_instance, ptr(1u), &m_physicalDevice);
	
		tbrs::Vec<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_MAINTENANCE_9_EXTENSION_NAME,
			VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME,
			VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
		};
		
		f32 one = 1.0f;
		VkDeviceQueueCreateInfo vkqci{
			.queueCount = 1,
			.pQueuePriorities = &one
		};
		tbrs::Vec<VkDeviceQueueCreateInfo> queueCIs;

		VkQueueFlags include[3] = { VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT };
		VkQueueFlags exclude = 0;
		for(u8 i = 0; i < 3; i++) {
			if(ci.requestedQueueTypes.contains(static_cast<pl::QueueType>(i))) {
				u32 family = getQueueFamilyIndex(include[i], exclude);
				exclude |= include[i];

				m_queues[i].family = family;
				vkqci.queueFamilyIndex = family;
				queueCIs.push(vkqci);
			}
		}
	
		// foo: determine which device features/extensions are definitely required (and think of a way to optionally enable more)
		vkCreateDevice(m_physicalDevice, ptr(VkDeviceCreateInfo{
			.pNext = ptr(VkPhysicalDeviceFeatures2{
				.pNext = ptr(VkPhysicalDeviceVulkan11Features{
					.pNext = ptr(VkPhysicalDeviceVulkan12Features{
						.pNext = ptr(VkPhysicalDeviceVulkan13Features{
							.pNext = ptr(VkPhysicalDeviceVulkan14Features{
								.pNext = ptr(VkPhysicalDeviceMaintenance9FeaturesKHR{
									.pNext = ptr(VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR{
										.pNext = ptr(VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT{
											.mutableDescriptorType = true
										}),
										.shaderMaximalReconvergence = true
									}),
									.maintenance9 = true
								}),
								.indexTypeUint8 = true,
								.maintenance5 = true,
							}),
							.synchronization2 = true,
							.dynamicRendering = true,
						}),
						.drawIndirectCount = true,
						.shaderSampledImageArrayNonUniformIndexing = true,
						.shaderStorageImageArrayNonUniformIndexing = true,
						.descriptorBindingSampledImageUpdateAfterBind = true,
						.descriptorBindingStorageBufferUpdateAfterBind = true,
						.descriptorBindingPartiallyBound = true,
						.runtimeDescriptorArray = true,
						.samplerFilterMinmax = true,
						.scalarBlockLayout = true,
						.timelineSemaphore = true,
						.bufferDeviceAddress = true,
						.vulkanMemoryModel = true,
						.vulkanMemoryModelDeviceScope = true,
						.vulkanMemoryModelAvailabilityVisibilityChains = true
					}),
					.shaderDrawParameters = true,
				}),
				.features{
					.multiDrawIndirect = true,
					.drawIndirectFirstInstance = true,
					.samplerAnisotropy = true,
					.fragmentStoresAndAtomics = true,
					.shaderStorageImageReadWithoutFormat = true,
					.shaderStorageImageWriteWithoutFormat = true,
				}
			}),
			.queueCreateInfoCount = static_cast<u32>(queueCIs.count()),
			.pQueueCreateInfos = queueCIs.data(),
			.enabledExtensionCount = static_cast<u32>(deviceExtensions.count()),
			.ppEnabledExtensionNames = deviceExtensions.data(),
		}), nullptr, &m_device);
		volkLoadDevice(m_device);

		for(u8 i = 0; i < 3; i++) {
			if(ci.requestedQueueTypes.contains(static_cast<pl::QueueType>(i))) {
				vkGetDeviceQueue(m_device, m_queues[i].family, 0, &m_queues[i].queue);
				m_queues[i].submissionLock = new std::mutex;
			}
		}

		m_heap = new DescriptorHeap(m_device);
		m_allocator = new DeviceMemoryAllocator(m_physicalDevice, m_device);
	}

	Device::Device(Device&& src) {
		memcpy(this, &src, sizeof(Device));
		memset(&src, 0, sizeof(Device));
	}

	Device& Device::operator=(Device&& src) {
		this->~Device();
		new (this) Device(std::move(src));

		return *this;
	}

	Device::~Device() {
		delete m_allocator;
		delete m_heap;

		for(u8 i = 0; i < 3; i++) {
			delete m_queues[i].submissionLock;
		}

		vkDestroyDevice(m_device, nullptr);
		vkDestroyInstance(m_instance, nullptr);
	}

	VkInstance Device::vkInstance() const {
		return m_instance;
	}

	VkPhysicalDevice Device::vkPhysicalDevice() const {
		return m_physicalDevice;
	}

	VkDevice Device::vkDevice() const {
		return m_device;
	}

	u32 Device::vkQueueFamily(QueueType type) const {
		return m_queues[static_cast<u8>(type)].family;
	}

	VkQueue Device::vkQueue(QueueType type) const {
		return m_queues[static_cast<u8>(type)].queue;
	}

	std::mutex* Device::vkQueueSubmissionLock(QueueType type) const {
		return m_queues[static_cast<u8>(type)].submissionLock;
	}

	DescriptorHeap* Device::descriptorHeap() const {
		return m_heap;
	}

	DeviceMemoryAllocator* Device::deviceMemoryAllocator() const {
		return m_allocator;
	}

	u32 Device::getQueueFamilyIndex(VkQueueFlags include, VkQueueFlags exclude) {
		u32 size = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &size, nullptr);

		tbrs::Vec<VkQueueFamilyProperties> queueProperties(size);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &size, queueProperties.data());

		// foo: should decouple this from std::ranges maybe
		for (auto [idx, queueFamily] : std::views::enumerate(queueProperties)) {
			if ((queueFamily.queueFlags & include) && !(queueFamily.queueFlags & exclude)) {
				return static_cast<u32>(idx);
			}
		}

		return ~0;
	}
}