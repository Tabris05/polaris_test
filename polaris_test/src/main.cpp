#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>

#define WIN32_LEAN_AND_MEAN

#include <glfw/glfw3native.h>
#include <polaris/polaris.hpp>

#include <vector>
#include <fstream>

#include <format>

std::vector<u32> getShaderSource(const char* path) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::vector<u32> ret(file.tellg() / sizeof(u32));
	file.seekg(0);
	file.read(reinterpret_cast<char*>(ret.data()), ret.size() * sizeof(u32));
	return ret;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Polaris Test", nullptr, nullptr);

	pl::Device device(pl::DeviceCreateInfo{
		.requestedQueueTypes = { pl::QueueType::Universal, pl::QueueType::DMA }
	});

	pl::Queue gfxQueue(pl::QueueCreateInfo{
		.device = device,
		.type = pl::QueueType::Universal
	});

	pl::Queue transferQueue(pl::QueueCreateInfo{
		.device = device,
		.type = pl::QueueType::DMA
	});

	pl::Texture tex(pl::TextureCreateInfo{
		.device = device,
		.format = pl::Format::RGBA8_SRGB,
		.width = 1920,
		.height = 1080,
	});

	pl::RenderTargetHandle rtHandle = tex.getRenderTargetHandle();
	pl::SampledHandle sampledHandle = tex.getSampledHandle(pl::TextureView{ .format = pl::Format::RGBA8_UNORM });

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
		.colorFormats = { pl::Format::RGBA8_SRGB }
	});

	std::vector<f32> vertices{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	};

	pl::Buffer buffer(pl::BufferCreateInfo{
		.device = device,
		.size = vertices.size() * sizeof(f32)
	});

	pl::CommandBuffer cmd = transferQueue.beginRecording();
	cmd.writeBuffer(buffer, pl::View<const f32>(vertices));
	transferQueue.submit(pl::SubmitInfo{ .commandBuffer = std::move(cmd) }).wait();

	pl::Event fif[2] = {};
	u64 frame = 0;

	std::vector<u8> clearColor(1920 * 1080 * 4);
	std::fill(clearColor.begin(), clearColor.end(), 0xFF);

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		fif[frame % 2].wait();

		pl::CommandBuffer cmd = gfxQueue.beginRecording();
		cmd.setViewport(pl::Rect2D<f32>{ .width = 1920.0f, .height = 1080.0f });
		cmd.setScissor(pl::Rect2D<u32>{ .width = 1920u, .height = 1080u });
		cmd.bindPipeline(pipeline);
		cmd.pushConstants(buffer.deviceAddress<f32>());
		cmd.barrier(pl::PipelineStage::Present, pl::PipelineStage::CopyWrite);
		cmd.writeTexture(tex, pl::View<const u8>(clearColor));
		cmd.barrier(pl::PipelineStage::CopyWrite, pl::PipelineStage::ColorRead);
		cmd.beginRenderPass(pl::RenderPassBeginInfo{
			.renderArea = {.width = 1920, .height = 1080 },
			.colorTargets = { { rtHandle } }
		});
		cmd.draw(6);
		cmd.endRenderPass();

		pl::Event event = gfxQueue.submit(pl::SubmitInfo{ .commandBuffer = std::move(cmd) });
		fif[frame % 2] = event;

		swapchain.present(pl::PresentInfo{
			.texture = sampledHandle,
			.queue = gfxQueue,
			.waitEvent = pl::EventInfo{
				.stage = pl::PipelineStage::ColorWrite,
				.event = event
			}
		});

		frame++;
	}

	device.waitIdle();
}