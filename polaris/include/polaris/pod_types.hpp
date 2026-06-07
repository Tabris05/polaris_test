#pragma once

#include "util_types.hpp"

typedef struct HINSTANCE__* HINSTANCE;
typedef struct HWND__* HWND;

struct xcb_connection_t;
typedef u32 xcb_window_t;

struct wl_display;
struct wl_surface;

typedef void CAMetalLayer;

struct ANativeWindow;

namespace pl {
	using DeviceAddress = u64;

	enum class QueueType : u8 {
		Universal = 0,
		Async = 1,
		DMA = 2
	};

	enum class Format : u8 {
		None,
		RG4_UNORM,
		RGBA4_UNORM,
		RGB565_UNORM,
		A1RGB5_UNORM,
		R8_UNORM,
		R8_SNORM,
		R8_UINT,
		R8_SINT,
		R8_SRGB,
		RG8_UNORM,
		RG8_SNORM,
		RG8_UINT,
		RG8_SINT,
		RGBA8_UNORM,
		RGBA8_SNORM,
		RGBA8_UINT,
		RGBA8_SINT,
		RGBA8_SRGB,
		A2BGR10_UNORM,
		A2BGR10_UINT,
		B10GR11_UFLOAT,
		E5BGR9_UFLOAT,
		R16_UNORM,
		R16_SNORM,
		R16_UINT,
		R16_SINT,
		R16_SFLOAT,
		RG16_UNORM,
		RG16_SNORM,
		RG16_UINT,
		RG16_SINT,
		RG16_SFLOAT,
		RGBA16_UNORM,
		RGBA16_SNORM,
		RGBA16_UINT,
		RGBA16_SINT,
		RGBA16_SFLOAT,
		R32_UINT,
		R32_SINT,
		R32_SFLOAT,
		RG32_UINT,
		RG32_SINT,
		RG32_SFLOAT,
		RGBA32_UINT,
		RGBA32_SINT,
		RGBA32_SFLOAT,
		R64_UINT,
		R64_SINT,
		D16_UNORM,
		X8_D24_UNORM,
		D32_SFLOAT,
		S8_UINT,
		D24_UNORM_S8_UINT,
		D32_SFLOAT_S8_UINT,
		BC1_RGB_UNORM,
		BC1_RGB_SRGB,
		BC1_RGBA_UNORM,
		BC1_RGBA_SRGB,
		BC2_UNORM,
		BC2_SRGB,
		BC3_UNORM,
		BC3_SRGB,
		BC4_UNORM,
		BC4_SNORM,
		BC5_UNORM,
		BC5_SNORM,
		BC6_UFLOAT,
		BC6_SFLOAT,
		BC7_UNORM,
		BC7_SRGB
	};

	enum class TextureType : u8 {
		Type1D = 1,
		Type2D,
		Type3D
	};

	enum class TextureViewType : u8 {
		Default,
		Type1D,
		Type2D,
		Type3D,
		TypeCubemap,
		Type1DArray,
		Type2DArray,
		TypeCubemapArray,
	};

	enum class Swizzle : u8 {
		Identity,
		Zero,
		One,
		Red,
		Green,
		Blue,
		Alpha
	};

	enum class DepthStencilAspect : u8 {
		Default,
		Depth,
		Stencil,
	};

	enum class Filter : u8 {
		Nearest,
		Linear,
	};

	enum class WrapMode : u8 {
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
	};

	enum class ReductionMode : u8 {
		Average,
		Min,
		Max
	};

	enum class CompareOp : u8 {
		None,
		Never,
		NotEqual,
		Less,
		LessEqual,
		Equal,
		GreaterEqual,
		Greater,
		Always
	};

	enum class LogicOp : u8 {
		Clear,
		Set,
		NoOp,
		Invert,
		Copy,
		CopyInverted,
		Or,
		OrReverse,
		OrInverted,
		Nor,
		And,
		AndReverse,
		AndInverted,
		Nand,
		Xor,
		Equivalent
	};

	enum class BlendFactor : u8 {
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		Src1Color,
		OneMinusSrc1Color,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		Src1Alpha,
		OneMinusSrc1Alpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate
	};

	enum class BlendOp : u8 {
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max
	};

	enum class WriteMask : u8 {
		None,
		R,
		G,
		RG,
		B,
		RB,
		GB,
		RGB,
		A,
		RA,
		GA,
		RGA,
		BA,
		RBA,
		GBA,
		RGBA
	};

	inline WriteMask operator~(WriteMask val) {
		return static_cast<WriteMask>(~static_cast<u16>(val));
	}

	inline WriteMask operator&(WriteMask lhs, WriteMask rhs) {
		return static_cast<WriteMask>(static_cast<u16>(lhs) & static_cast<u16>(rhs));
	}

	inline WriteMask operator|(WriteMask lhs, WriteMask rhs) {
		return static_cast<WriteMask>(static_cast<u16>(lhs) | static_cast<u16>(rhs));
	}

	inline WriteMask& operator&=(WriteMask& lhs, WriteMask rhs) {
		lhs = lhs & rhs;
		return lhs;
	}

	inline WriteMask& operator|=(WriteMask& lhs, WriteMask rhs) {
		lhs = lhs | rhs;
		return lhs;
	}

	enum class StencilOp : u8 {
		Keep,
		Zero,
		Replace,
		Invert,
		IncrementClamp,
		IncrementWrap,
		DecrementClamp,
		DecrementWrap
	};

	enum class PolygonMode : u8 {
		Fill,
		Line,
		Point
	};

	enum class NativeWindowType : u8 {
		Headless,
		Win32,
		XCB,
		Wayland,
		Metal,
		Android,
		Vi
	};

	enum class ShaderStage : u8 {
		Compute,
		Mesh,
		Fragment
	};

	enum class Face : u8 {
		None,
		Front,
		Back,
		Both
	};

	enum class WindingOrder : u8 {
		CCW,
		CW
	};

	enum class IndexType : u8 {
		u8,
		u16,
		u32
	};

	enum class LoadOp : u8 {
		Load,
		Clear,
		None
	};

	enum class StoreOp : u8 {
		Store,
		Discard,
		None
	};

	enum class PipelineStage : u16 {
		None = 0,

		MeshRead = 1 << 0,
		MeshWrite = 1 << 1,

		FragmentRead = 1 << 2,
		FragmentWrite = 1 << 3,

		ComputeRead = 1 << 4,
		ComputeWrite = 1 << 5,

		CopyRead = 1 << 6,
		CopyWrite = 1 << 7,

		DepthRead = 1 << 8,
		DepthWrite = 1 << 9,

		ColorRead = 1 << 10,
		ColorWrite = 1 << 11,

		IndirectRead = 1 << 12,

		Present = ComputeRead,

		Mesh = MeshRead | MeshWrite,
		Fragment = FragmentRead | FragmentWrite,
		Compute = ComputeRead | ComputeWrite,
		Copy = CopyRead | CopyWrite,
		Depth = DepthRead | DepthWrite,
		Color = ColorRead | ColorWrite,

		RasterRead = MeshRead | FragmentRead,
		RasterWrite = MeshWrite | FragmentWrite,
		Raster = RasterRead | RasterWrite,

		ShaderRead = RasterRead | ComputeRead,
		ShaderWrite = RasterWrite | ComputeWrite,
		Shader = ShaderRead | ShaderWrite,

		RenderTargetRead = DepthRead | ColorRead,
		RenderTargetWrite = DepthWrite | ColorWrite,
		RenderTarget = RenderTargetRead | RenderTargetWrite,

		Read = ShaderRead | CopyRead | RenderTargetRead | IndirectRead,
		Write = ShaderWrite | CopyWrite | RenderTargetWrite,
		All = Read | Write
	};

	inline PipelineStage operator~(PipelineStage val) {
		return static_cast<PipelineStage>(~static_cast<u16>(val));
	}

	inline PipelineStage operator&(PipelineStage lhs, PipelineStage rhs) {
		return static_cast<PipelineStage>(static_cast<u16>(lhs) & static_cast<u16>(rhs));
	}

	inline PipelineStage operator|(PipelineStage lhs, PipelineStage rhs) {
		return static_cast<PipelineStage>(static_cast<u16>(lhs) | static_cast<u16>(rhs));
	}

	inline PipelineStage& operator&=(PipelineStage& lhs, PipelineStage rhs) {
		lhs = lhs & rhs;
		return lhs;
	}

	inline PipelineStage& operator|=(PipelineStage& lhs, PipelineStage rhs) {
		lhs = lhs | rhs;
		return lhs;
	}


	struct DeviceCreateInfo {
		// this will contain stuff some day
	};

	struct QueueCreateInfo {
		QueueType type;
	};

	struct SyncCreateInfo {
		u64 initialValue;
	};

	struct BufferCreateInfo {
		u64 size;
	};

	struct TextureCreateInfo {
		Format format;
		TextureType type = TextureType::Type2D;
		u32 width = 1;
		u32 height = 1;
		u32 depth = 1;
		u32 levels = 1;
		u32 layers = 1;
		u32 samples = 1;
	};

	struct TextureRegion {
		static constexpr u32 RemainingLevels = 0xFFFFFFFF;
		static constexpr u32 RemainingLayers = 0xFFFFFFFF;
		u32 baseLevel;
		u32 baseLayer;
		u32 numLevels = RemainingLevels;
		u32 numLayers = RemainingLayers;
		DepthStencilAspect aspect;
	};

	struct TextureView {
		Format format;
		TextureViewType type;
		TextureRegion region;
		Swizzle swizzleR;
		Swizzle swizzleG;
		Swizzle swizzleB;
		Swizzle swizzleA;
	};

	struct SamplerCreateInfo {
		Filter minFilter;
		Filter magFilter;
		Filter mipFilter;
		WrapMode wrapU;
		WrapMode wrapV;
		WrapMode wrapW;
		ReductionMode reductionMode;
		CompareOp compareOp;
		f32 minLod;
		f32 maxLod = 1000.0f;
		f32 lodBias;
		f32 anisotropy = 1.0f;
	};

	struct NativeWindow {
		NativeWindowType type;
		union {
			struct {
				HINSTANCE hinstance;
				HWND hwnd;
			} win32;

			struct {
				xcb_connection_t* connection;
				xcb_window_t window;
			} xcb;

			struct {
				wl_display* display;
				wl_surface* surface;
			} wayland;
			
			struct {
				CAMetalLayer* pLayer;
			} metal;

			struct {
				ANativeWindow* window;
			} android;

			struct {
				void* window;
			} vi;	
		};
	};

	struct SwapchainCreateInfo {
		NativeWindow nativeWindow;
		u32 width;
		u32 height;
		b8 vsync;
	};

	union ClearValue {
		struct {
			f32 r;
			f32 g;
			f32 b;
			f32 a;
		} fColor;

		struct {
			u32 r;
			u32 g;
			u32 b;
			u32 a;
		} uColor;

		struct {
			i32 r;
			i32 g;
			i32 b;
			i32 a;
		} iColor;

		struct {
			f32 depth;
			u8 stencil;
		} depthStencil;
	};

	struct ShaderCreateInfo {
		ShaderStage stage;
		const char* entryPoint = "main";
		View<const u32> code;
	};

	struct BufferRange {
		DeviceAddress address;
		u64 size;
	};

	struct ColorState {
		b8 logicOpEnable : 1;
		LogicOp logicOp : 4;
		f32 blendConstants[4];
	};

	struct BlendState {
		BlendFactor srcFactor : 5;
		BlendFactor dstFactor : 5;
		BlendOp blendOp : 3;
	};

	struct AttachmentColorState {
		u8 attachmentIndex : 4;
		b8 blendEnable : 1;
		BlendState colorBlend;
		BlendState alphaBlend;
		WriteMask writeMask : 4 = pl::WriteMask::RGBA;
	};

	struct StencilState {
		StencilOp failOp : 3;
		StencilOp depthFailOp : 3;
		StencilOp passOp : 3;
		CompareOp compareOp : 4;
		u32 compareMask;
		u32 writeMask;
		u32 reference;
	};

	struct DepthStencilState {
		b8 depthTestEnable : 1;
		b8 stencilTestEnable : 1;
		b8 depthBoundsTestEnable : 1;
		b8 depthWriteEnable : 1;
		CompareOp depthCompareOp : 4;
		StencilState front;
		StencilState back;
		f32 minDepthBounds;
		f32 maxDepthBounds;
	};

	struct MultisampleState {
		u8 sampleMask = 0xFF;
		u8 sampleCount : 4 = 1;
		b8 alphaToCoverageEnable : 1;
	};

	struct RasterizerState {
		b8 depthClampEnable : 1;
		b8 rasterizerDiscardEnable : 1;
		PolygonMode polygonMode : 2;
		Face cullMode : 2;
		WindingOrder windingOrder : 1;
		b8 depthBiasEnable : 1;
		f32 depthBiasConstantFactor;
		f32 depthBiasClamp;
		f32 depthBiasSlopeFactor;
		f32 lineWidth = 1.0f;
	};

	struct IndirectCommand {
		u32 groupsX;
		u32 groupsY;
		u32 groupsZ;
	};

	template <typename T>
	struct Rect2D {
		T x;
		T y;
		T width;
		T height;
	};

	template <typename T>
	struct Rect3D {
		T x;
		T y;
		T z;
		T width;
		T height;
		T depth;
	};

	template<typename T>
	struct vec2 {
		T x;
		T y;
	};

	template<typename T>
	struct vec3 {
		T x;
		T y;
		T z;
	};

	template<typename T>
	struct vec4 {
		T x;
		T y;
		T z;
		T w;
	};
}

