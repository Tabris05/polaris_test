#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>

#define WIN32_LEAN_AND_MEAN

#include <glfw/glfw3native.h>
#include <polaris/polaris.hpp>

#include <vector>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

// foo: make custom linear algebra lib with correct names
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using mat4f32 = glm::mat4;
#include "../shared/push_constants.h"

std::vector<u32> getShaderSource(const char* path) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::vector<u32> ret(file.tellg() / sizeof(u32));
	file.seekg(0);
	file.read(reinterpret_cast<char*>(ret.data()), ret.size() * sizeof(u32));
	return ret;
}

int main() {
	// init
	pl::Device device(pl::DeviceCreateInfo{
		.requestedQueueTypes = { pl::QueueType::Universal, pl::QueueType::DMA }
	});

	pl::Queue queue(pl::QueueCreateInfo{
		.device = device,
		.type = pl::QueueType::Universal
	});

	// wsi
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Polaris Test", nullptr, nullptr);
	HWND hwnd = glfwGetWin32Window(window);
	pl::Swapchain swapchain(pl::SwapchainCreateInfo{
		.device = device,
		.nativeWindow = pl::NativeWindow{
			.type = pl::NativeWindowType::Win32,
			.win32 = {
				.hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				.hwnd = hwnd
			}
		},
		.width = 1920,
		.height = 1080,
		.mode = pl::PresentMode::Immediate
	});

	pl::Texture colorBuffer(pl::TextureCreateInfo{
		.device = device,
		.format = pl::Format::RGBA8_SRGB,
		.width = 1920,
		.height = 1080,
	});

	pl::Texture depthBuffer(pl::TextureCreateInfo{
		.device = device,
		.format = pl::Format::D32_SFLOAT,
		.width = 1920,
		.height = 1080,
	});

	pl::RenderTarget colorTarget = colorBuffer.makeRenderTarget();
	pl::RenderTarget depthTarget = depthBuffer.makeRenderTarget();
	pl::TextureHandle presentHandle = colorBuffer.makeTextureHandle(pl::TextureView{ .format = pl::Format::RGBA8_UNORM });

	// upload model data
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::vector<Vertex> vertices;
	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "viking_room.obj");
	for(const auto& shape : shapes) {
		for(const auto& index : shape.mesh.indices) {
			vertices.emplace_back(
				vec3f32(
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				),

				vec2f32(
						   attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				)
			);
		}
	}

	pl::Buffer buffer(pl::BufferCreateInfo{
		.device = device,
		.size = vertices.size() * sizeof(Vertex)
	});

	i32 x, y;
	byte* texData = stbi_load("viking_room.png", &x, &y, nullptr, 4);

	pl::Texture albedo(pl::TextureCreateInfo{
		.device = device,
		.format = pl::Format::RGBA8_SRGB,
		.width = static_cast<u32>(x),
		.height = static_cast<u32>(y),
	});
	pl::CommandBuffer upload = queue.beginRecording();
	upload.writeBuffer(pl::BufferRegion{ buffer }, pl::View<const Vertex>(vertices));
	upload.writeTexture(albedo, pl::View<const byte>(texData, x * y * 4));
	queue.submit(pl::SubmitInfo{ .commandBuffer = std::move(upload) }).wait();

	stbi_image_free(texData);

	pl::TextureHandle albedoHandle = albedo.makeTextureHandle();

	pl::Sampler sampler(pl::SamplerCreateInfo{
		.device = device,
		.minFilter = pl::Filter::Linear,
		.magFilter = pl::Filter::Linear
	});

	// pipeline
	std::vector<u32> shaderCode = getShaderSource("shaders/tri.spv");

	pl::Pipeline pipeline(pl::RasterPipelineCreateInfo{
		.device = device,
		.shaders = {
			pl::Shader{
				.stage = pl::ShaderStage::Vertex,
				.entryPoint = "vsmain",
				.code = shaderCode
			},
			pl::Shader{
				.stage = pl::ShaderStage::Fragment,
				.entryPoint = "fsmain",
				.code = shaderCode
			}
		},
		.colorFormats = { pl::Format::RGBA8_SRGB },
		.depthFormat = pl::Format::D32_SFLOAT,
		.cullFace = pl::Face::Back,
		.depthCompareOp = pl::CompareOp::Less,
		.depthWriteEnable = true
	});

	pl::Event fif[2] = {};
	u64 frame = 0;

	// render loop
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		fif[frame % 2].wait();

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), static_cast<f32>(glfwGetTime()) * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		projection[1][1] *= -1.0f;

		pl::CommandBuffer cmd = queue.beginRecording();

		cmd.setViewport(pl::Rect3D<f32>{ .width = 1920.0f, .height = 1080.0f, .depth = 1.0f });
		cmd.setScissor(pl::Rect2D<u32>{ .width = 1920u, .height = 1080u });
		cmd.bindPipeline(pipeline);
		cmd.pushConstants(PushConstants{
			.vertices = buffer.deviceAddress<Vertex>(),
			.mvp = projection * view * model,
			.texture = pl::TextureHandle(albedoHandle, sampler)
		});
		
		cmd.barrier(pl::PipelineStage::Depth | pl::PipelineStage::Present, pl::PipelineStage::DepthWrite | pl::PipelineStage::ColorWrite);
		cmd.beginRenderPass(pl::RenderPassBeginInfo{
			.renderArea = {.width = 1920, .height = 1080 },
			.colorTargets = {
				pl::RenderTargetInfo{
					.target = colorTarget,
					.loadOp = pl::LoadOp::Clear,
					.storeOp = pl::StoreOp::Store,
					.clearValue = pl::ClearValue{.fColor = { 0.0f, 0.0f, 0.0f, 1.0f } }
				}
			},
			.depthTarget = pl::RenderTargetInfo{
					.target = depthTarget,
					.loadOp = pl::LoadOp::Clear,
					.storeOp = pl::StoreOp::Discard,
					.clearValue = pl::ClearValue{ .depthStencil = { 1.0f, 0u } }
				}
		});
		cmd.draw(vertices.size());
		cmd.endRenderPass();

		pl::Event event = queue.submit(pl::SubmitInfo{ .commandBuffer = std::move(cmd) });
		fif[frame % 2] = event;

		swapchain.present(pl::PresentInfo{
			.texture = presentHandle,
			.queue = queue,
			.waitEvent = event
		});

		frame++;
	}

	device.waitIdle();
}