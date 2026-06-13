#include "device_impl.hpp"
#include "vk_util.hpp"
#include <volk/volk.h>
#include <tabris/vec.hpp>
#include <ranges>

namespace pl {

	void Device::initialize(const DeviceCreateInfo& ci) {
		Device::get().initializeImpl(ci);
	}

	void Device::idle() {
		vkDeviceWaitIdle(Device::get().vkDevice());
	}

	Device& Device::get() {
		static Device device;
		return device;
	}

	void Device::initializeImpl(const DeviceCreateInfo& ci) {
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

		vkCreateInstance(&VkInstanceCreateInfo{
				.pApplicationInfo = &VkApplicationInfo{
				.apiVersion = VK_API_VERSION_1_4
			},
			.enabledExtensionCount = static_cast<u32>(instanceExtensions.count()),
			.ppEnabledExtensionNames = instanceExtensions.data()
		}, nullptr, &m_instance);
	
		volkLoadInstanceOnly(m_instance);
	
		// foo: eventually should "properly" select physical device
		{
			u32 one = 1;
			vkEnumeratePhysicalDevices(m_instance, &one, &m_physicalDevice);
			vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);
		}
		
		tbrs::Vec<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_RAY_QUERY_EXTENSION_NAME,
			VK_KHR_MAINTENANCE_9_EXTENSION_NAME,
			VK_KHR_MAINTENANCE_10_EXTENSION_NAME,
			VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
			VK_KHR_DEVICE_ADDRESS_COMMANDS_EXTENSION_NAME,
			VK_KHR_SHADER_UNTYPED_POINTERS_EXTENSION_NAME,
			VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
			VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME,
			VK_KHR_INTERNALLY_SYNCHRONIZED_QUEUES_EXTENSION_NAME,
			VK_EXT_MESH_SHADER_EXTENSION_NAME,
			VK_EXT_SHADER_OBJECT_EXTENSION_NAME,
			VK_EXT_DESCRIPTOR_HEAP_EXTENSION_NAME,
			VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
		};
		
		f32 one = 1.0f;
		VkDeviceQueueCreateInfo queueCIs[3] = {
			{
				.flags = VK_DEVICE_QUEUE_CREATE_INTERNALLY_SYNCHRONIZED_BIT_KHR,
				.queueFamilyIndex = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT),
				.queueCount = 1,
				.pQueuePriorities = &one
			},
			{
				.flags = VK_DEVICE_QUEUE_CREATE_INTERNALLY_SYNCHRONIZED_BIT_KHR,
				.queueFamilyIndex = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT, VK_QUEUE_GRAPHICS_BIT),
				.queueCount = 1,
				.pQueuePriorities = &one
			},
			{
				.flags = VK_DEVICE_QUEUE_CREATE_INTERNALLY_SYNCHRONIZED_BIT_KHR,
				.queueFamilyIndex = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT),
				.queueCount = 1,
				.pQueuePriorities = &one
			},
		};
	
		// foo: determine which device features/extensions are definitely required (and think of a way to optionally enable more)
		vkCreateDevice(m_physicalDevice, &VkDeviceCreateInfo{
			.pNext = &VkPhysicalDeviceFeatures2{
				.pNext = &VkPhysicalDeviceVulkan11Features{
					.pNext = &VkPhysicalDeviceVulkan12Features{
						.pNext = &VkPhysicalDeviceVulkan13Features{
							.pNext = &VkPhysicalDeviceVulkan14Features{
								.pNext = &VkPhysicalDeviceRayQueryFeaturesKHR{
									.pNext = &VkPhysicalDeviceMaintenance9FeaturesKHR{
										.pNext = &VkPhysicalDeviceMaintenance10FeaturesKHR{
											.pNext = &VkPhysicalDeviceAccelerationStructureFeaturesKHR{
												.pNext = &VkPhysicalDeviceDeviceAddressCommandsFeaturesKHR{
													.pNext = &VkPhysicalDeviceShaderUntypedPointersFeaturesKHR{
														.pNext = &VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR{
															.pNext = &VkPhysicalDeviceInternallySynchronizedQueuesFeaturesKHR{
																.pNext = &VkPhysicalDeviceMeshShaderFeaturesEXT{
																	.pNext = &VkPhysicalDeviceShaderObjectFeaturesEXT{
																		.pNext = &VkPhysicalDeviceDescriptorHeapFeaturesEXT{
																			.pNext = &VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT{
																				.fragmentShaderPixelInterlock = true
																			},
																			.descriptorHeap = true
																		},
																		.shaderObject = true
																	},
																	.meshShader = true
																},
																.internallySynchronizedQueues = true
															},
															.shaderMaximalReconvergence = true
														},
														.shaderUntypedPointers = true
													},
													.deviceAddressCommands = true
												},
												.accelerationStructure = true
											},
											.maintenance10 = true
										},
										.maintenance9 = true
									},
									.rayQuery = true
								},
								.shaderSubgroupRotate = true,
								.shaderSubgroupRotateClustered = true,
								.indexTypeUint8 = true,
								.maintenance5 = true,
								.hostImageCopy = true
							},
							.shaderDemoteToHelperInvocation = true,
							.shaderTerminateInvocation = true,
							.synchronization2 = true,
							.dynamicRendering = true,
							.shaderIntegerDotProduct = true,
							.maintenance4 = true,
						},
						.samplerMirrorClampToEdge = true,
						.drawIndirectCount = true,
						.storageBuffer8BitAccess = true,
						.shaderBufferInt64Atomics = true,
						.shaderSharedInt64Atomics = true,
						.shaderFloat16 = true,
						.shaderInt8 = true,
						.samplerFilterMinmax = true,
						.scalarBlockLayout = true,
						.shaderSubgroupExtendedTypes = true,
						.separateDepthStencilLayouts = true,
						.hostQueryReset = true,
						.timelineSemaphore = true,
						.bufferDeviceAddress = true,
						.vulkanMemoryModel = true,
						.vulkanMemoryModelDeviceScope = true,
						.shaderOutputLayer = true,
						.subgroupBroadcastDynamicId = true
					},
					.storageBuffer16BitAccess = true,
					.storagePushConstant16 = true,
					.variablePointersStorageBuffer = true,
					.variablePointers = true,
					.shaderDrawParameters = true,
				},
				.features{
				    .imageCubeArray = true,
					.independentBlend = true,
					.sampleRateShading = true,
					.dualSrcBlend = true,
					.logicOp = true,
					.multiDrawIndirect = true,
					.drawIndirectFirstInstance = true,
					.depthClamp = true,
					.depthBiasClamp = true,
					.fillModeNonSolid = true,
					.depthBounds = true,
					.wideLines = true,
					.largePoints = true,
					.samplerAnisotropy = true,
					.textureCompressionBC = true,
					.occlusionQueryPrecise = true,
					.pipelineStatisticsQuery = true,
					.fragmentStoresAndAtomics = true,
					.shaderImageGatherExtended = true,
					.shaderStorageImageMultisample = true,
					.shaderStorageImageReadWithoutFormat = true,
					.shaderStorageImageWriteWithoutFormat = true,
					.shaderClipDistance = true,
					.shaderCullDistance = true,
					.shaderFloat64 = true,
					.shaderInt64 = true,
				    .shaderInt16 = true,
					.shaderResourceMinLod = true,
				}
			},
			.queueCreateInfoCount = 3,
			.pQueueCreateInfos = queueCIs,
			.enabledExtensionCount = static_cast<u32>(deviceExtensions.count()),
			.ppEnabledExtensionNames = deviceExtensions.data(),
		}, nullptr, &m_device);
		volkLoadDevice(m_device);

		for(u8 i = 0; i < 3; i++) {
			vkGetDeviceQueue2(m_device, &VkDeviceQueueInfo2{
				.flags = VK_DEVICE_QUEUE_CREATE_INTERNALLY_SYNCHRONIZED_BIT_KHR,
				.queueFamilyIndex = m_queues[i].family,
				.queueIndex = 0,
			},  &m_queues[i].queue);
		}

		m_allocator.initialize();
		m_heap.initialize();
	}

	Device::~Device() {
		m_heap.finalize();
		m_allocator.finalize();

		vkDestroyDevice(m_device, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		volkFinalize();
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

	DescriptorHeap& Device::descriptorHeap() {
		return m_heap;
	}

	DeviceMemoryAllocator& Device::deviceMemoryAllocator() {
		return m_allocator;
	}

	u32 Device::getQueueFamilyIndex(VkQueueFlags include, VkQueueFlags exclude) {
		u32 size = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &size, nullptr);

		tbrs::Vec<VkQueueFamilyProperties> queueProperties(size);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &size, queueProperties.data());

		// foo: should decouple this from std::ranges maybe
		for (auto [idx, queueFamily] : std::views::enumerate(queueProperties)) {
			if ((queueFamily.queueFlags & include) == include && (queueFamily.queueFlags & exclude) == 0) {
				return static_cast<u32>(idx);
			}
		}

		return ~0;
	}

	BufferBindResult Device::bindBufferMemory(VkBuffer buffer, b8 deviceLocal) {
		BufferBindResult ret;

		VkMemoryRequirements mrq;
		vkGetBufferMemoryRequirements(m_device, buffer, &mrq);

		u32 memoryTypeIndex = ~0u;
		VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		if(deviceLocal) {
			flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}
		else {
			flags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		}

		for(u8 idx = 0; idx < m_memoryProperties.memoryTypeCount; idx++) {
			if(((1 << idx) & mrq.memoryTypeBits) && (m_memoryProperties.memoryTypes[idx].propertyFlags & flags) == flags) {
				memoryTypeIndex = idx;
				break;
			}
		}

		vkAllocateMemory(m_device, &VkMemoryAllocateInfo{
			.pNext = &VkMemoryAllocateFlagsInfo{.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT },
			.allocationSize = mrq.size,
			.memoryTypeIndex = memoryTypeIndex
		}, nullptr, &ret.memory);

		vkBindBufferMemory(m_device, buffer, ret.memory, 0);

		ret.deviceAddress = vkGetBufferDeviceAddress(m_device, &VkBufferDeviceAddressInfo{ .buffer = buffer });
		vkMapMemory(m_device, ret.memory, 0, mrq.size, 0, reinterpret_cast<void**>(&ret.hostAddress));

		return ret;
	}
}