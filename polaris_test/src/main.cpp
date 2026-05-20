#include <polaris/polaris.hpp>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>

#define WIN32_LEAN_AND_MEAN
#include <glfw/glfw3native.h>

#include <meshopt/meshoptimizer.h>

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

struct Mesh {
	u64 drawCount;
	pl::Buffer vertexBuffer;
	pl::Buffer triangleBuffer;
	pl::Buffer meshletBuffer;
	pl::Texture texture;
};

Mesh makeMesh(const pl::Device& device) {
	pl::Queue uploadQueue(pl::QueueCreateInfo{
		.device = device,
		.type = pl::QueueType::DMA
	});

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	// load mesh
	std::vector<Vertex> vertices1;
	tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "viking_room.obj");
	for(const auto& shape : shapes) {
		for(const auto& index : shape.mesh.indices) {
			vertices1.emplace_back(
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

	// index mesh
	u64 indexCount = vertices1.size();
	std::vector<u32> remap(indexCount);
	u64 vertexCount = meshopt_generateVertexRemap(remap.data(), nullptr, indexCount, vertices1.data(), indexCount, sizeof(Vertex));

	std::vector<u32> indices(indexCount);
	meshopt_remapIndexBuffer(indices.data(), nullptr, indexCount, remap.data());

	std::vector<Vertex> vertices2(vertexCount);
	meshopt_remapVertexBuffer(vertices2.data(), vertices1.data(), indexCount, sizeof(Vertex), remap.data());

	// meshletize mesh
	u64 maxMeshlets = meshopt_buildMeshletsBound(indices.size(), Meshlet::MaxVertexCount, Meshlet::MaxTriangleCount);

	std::vector<meshopt_Meshlet> meshlets(maxMeshlets);
	std::vector<u32> meshletVertices(indices.size());
	std::vector<u8> meshletTriangles(indices.size());

	u64 meshletCount = meshopt_buildMeshlets(meshlets.data(), meshletVertices.data(), meshletTriangles.data(), indices.data(), indices.size(),
											 reinterpret_cast<f32*>(vertices2.data()), vertices2.size(), sizeof(Vertex), Meshlet::MaxVertexCount, Meshlet::MaxTriangleCount, 0.0f);
	
	meshlets.resize(meshletCount);

	meshopt_Meshlet last = meshlets.back();
	meshletVertices.resize(last.vertex_offset + last.vertex_count);
	meshletTriangles.resize(last.triangle_offset + last.triangle_count * 3);

	// optimize meshlets
	for(meshopt_Meshlet meshlet : meshlets) {
		meshopt_optimizeMeshlet(meshletVertices.data() + meshlet.vertex_offset, meshletTriangles.data() + meshlet.triangle_offset, meshlet.triangle_count, meshlet.vertex_count);
	}

	// create final vertex buffer
	std::vector<Vertex> vertices3(meshletVertices.size());
	for(u64 i = 0; i < meshletVertices.size(); i++) {
		vertices3[i] = vertices2[meshletVertices[i]];
	}

	pl::Buffer vertexBuffer(pl::BufferCreateInfo{
		.device = device,
		.size = vertices3.size() * sizeof(Vertex)
	});

	pl::Buffer triangleBuffer(pl::BufferCreateInfo{
		.device = device,
		.size = meshletTriangles.size() * sizeof(u8)
	});

	pl::Buffer meshletBuffer(pl::BufferCreateInfo{
		.device = device,
		.size = meshlets.size() * sizeof(Meshlet)
	});

	std::vector<Meshlet> meshlets2(meshlets.size());
	for(u64 i = 0; i < meshlets.size(); i++) {
		meshlets2[i] = Meshlet{
			vertexBuffer.deviceAddress<Vertex>() + meshlets[i].vertex_offset,
			triangleBuffer.deviceAddress<vec3u8>() + meshlets[i].triangle_offset / 3,
			static_cast<u8>(meshlets[i].vertex_count),
			static_cast<u8>(meshlets[i].triangle_count)
		};
	}

	i32 x, y;
	byte* textureData = stbi_load("viking_room.png", &x, &y, nullptr, 4);
	
	pl::Texture texture(pl::TextureCreateInfo{
		.device = device,
		.format = pl::Format::RGBA8_SRGB,
		.width = static_cast<u32>(x),
		.height = static_cast<u32>(y),
	});
	
	pl::CommandBuffer upload = uploadQueue.beginRecording();
	upload.writeBuffer(vertexBuffer.deviceAddress(), pl::View<const Vertex>(vertices3));
	upload.writeBuffer(triangleBuffer.deviceAddress(), pl::View<const u8>(meshletTriangles));
	upload.writeBuffer(meshletBuffer.deviceAddress(), pl::View<const Meshlet>(meshlets2));
	upload.writeTexture(texture, pl::View<const byte>(textureData, x * y * 4));
	uploadQueue.submit(pl::SubmitInfo{ .commandBuffer = std::move(upload) }).wait();
	
	stbi_image_free(textureData);
	
	return Mesh{
		meshlets.size(),
		std::move(vertexBuffer),
		std::move(triangleBuffer),
		std::move(meshletBuffer),
		std::move(texture)
	};
}

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
		.vsync = true
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

	Mesh mesh = makeMesh(device);
	pl::TextureHandle albedoHandle = mesh.texture.makeTextureHandle();

	pl::Sampler sampler(pl::SamplerCreateInfo{
		.device = device,
		.minFilter = pl::Filter::Linear,
		.magFilter = pl::Filter::Linear,
		.anisotropy = 16.0f,
	});

	// shaders
	std::vector<u32> shaderCode = getShaderSource("shaders/tri.spv");

	pl::Shader meshShader(pl::ShaderCreateInfo{
		.device = device,
		.stage = pl::ShaderStage::Mesh,
		.entryPoint = "msmain",
		.code = shaderCode
	});

	pl::Shader fragmentShader(pl::ShaderCreateInfo{
		.device = device,
		.stage = pl::ShaderStage::Fragment,
		.entryPoint = "fsmain",
		.code = shaderCode
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

		cmd.setDepthStencilState(pl::DepthStencilState{
			.depthTestEnable = true,
			.depthWriteEnable = true,
			.depthCompareOp = pl::CompareOp::Less
		});
		cmd.setRasterizerState(pl::RasterizerState{
			.cullMode = pl::Face::Back,
			.windingOrder = pl::WindingOrder::CCW
		});
		cmd.setViewport(pl::Rect3D<f32>{ .width = 1920.0f, .height = 1080.0f, .depth = 1.0f });
		cmd.setScissor(pl::Rect2D<u32>{ .width = 1920u, .height = 1080u });
		cmd.bindShaders({ meshShader, fragmentShader });
		cmd.pushConstants(PushConstants{
			.meshlets = mesh.meshletBuffer.deviceAddress<Meshlet>(),
			.mvp = projection * view * model,
			.texture = pl::TextureHandle(albedoHandle, sampler)
		});
		
		cmd.barrier(pl::PipelineStage::Present, pl::PipelineStage::RenderTargetWrite);
		cmd.beginRenderPass(pl::RenderPassBeginInfo{
			.renderArea = { .width = 1920, .height = 1080 },
			.colorTargets = pl::RenderTargetInfo{
				.target = colorTarget,
				.loadOp = pl::LoadOp::Clear,
				.storeOp = pl::StoreOp::Store,
				.clearValue = pl::ClearValue{ .fColor = { 0.0f, 0.0f, 0.0f, 1.0f } }
			},
			.depthTarget = pl::RenderTargetInfo{
				.target = depthTarget,
				.loadOp = pl::LoadOp::Clear,
				.storeOp = pl::StoreOp::Discard,
				.clearValue = pl::ClearValue{ .depthStencil = { 1.0f, 0u } }
			}
		});
		cmd.draw(mesh.drawCount);
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