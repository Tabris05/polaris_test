module polaris;

public typedef bool b8;

public typedef int8_t i8;
public typedef int16_t i16;
public typedef int32_t i32;
public typedef int64_t i64;

public typedef uint8_t u8;
public typedef uint16_t u16;
public typedef uint32_t u32;
public typedef uint64_t u64;

public typedef float16_t f16;
public typedef float32_t f32;
public typedef float64_t f64;

public typedef bool2 vec2b8;
public typedef bool3 vec3b8;
public typedef bool4 vec4b8;

public typedef int8_t2 vec2i8;
public typedef int8_t3 vec3i8;
public typedef int8_t4 vec4i8;

public typedef int16_t2 vec2i16;
public typedef int16_t3 vec3i16;
public typedef int16_t4 vec4i16;

public typedef int32_t2 vec2i32;
public typedef int32_t3 vec3i32;
public typedef int32_t4 vec4i32;

public typedef int64_t2 vec2i64;
public typedef int64_t3 vec3i64;
public typedef int64_t4 vec4i64;

public typedef uint8_t2 vec2u8;
public typedef uint8_t3 vec3u8;
public typedef uint8_t4 vec4u8;

public typedef uint16_t2 vec2u16;
public typedef uint16_t3 vec3u16;
public typedef uint16_t4 vec4u16;

public typedef uint32_t2 vec2u32;
public typedef uint32_t3 vec3u32;
public typedef uint32_t4 vec4u32;

public typedef uint64_t2 vec2u64;
public typedef uint64_t3 vec3u64;
public typedef uint64_t4 vec4u64;

public typedef float16_t2 vec2f16;
public typedef float16_t3 vec3f16;
public typedef float16_t4 vec4f16;

public typedef float32_t2 vec2f32;
public typedef float32_t3 vec3f32;
public typedef float32_t4 vec4f32;

public typedef float64_t2 vec2f64;
public typedef float64_t3 vec3f64;
public typedef float64_t4 vec4f64;

public typedef bool2x2 mat2b8;
public typedef bool3x3 mat3b8;
public typedef bool4x4 mat4b8;
public typedef bool2x3 mat2x3b8;
public typedef bool3x2 mat3x2b8;
public typedef bool2x4 mat2x4b8;
public typedef bool4x2 mat4x2b8;
public typedef bool3x4 mat3x4b8;
public typedef bool4x3 mat4x3b8;

public typedef int8_t2x2 mat2i8;
public typedef int8_t3x3 mat3i8;
public typedef int8_t4x4 mat4i8;
public typedef int8_t2x3 mat2x3i8;
public typedef int8_t3x2 mat3x2i8;
public typedef int8_t2x4 mat2x4i8;
public typedef int8_t4x2 mat4x2i8;
public typedef int8_t3x4 mat3x4i8;
public typedef int8_t4x3 mat4x3i8;

public typedef int16_t2x2 mat2i16;
public typedef int16_t3x3 mat3i16;
public typedef int16_t4x4 mat4i16;
public typedef int16_t2x3 mat2x3i16;
public typedef int16_t3x2 mat3x2i16;
public typedef int16_t2x4 mat2x4i16;
public typedef int16_t4x2 mat4x2i16;
public typedef int16_t3x4 mat3x4i16;
public typedef int16_t4x3 mat4x3i16;

public typedef int32_t2x2 mat2i32;
public typedef int32_t3x3 mat3i32;
public typedef int32_t4x4 mat4i32;
public typedef int32_t2x3 mat2x3i32;
public typedef int32_t3x2 mat3x2i32;
public typedef int32_t2x4 mat2x4i32;
public typedef int32_t4x2 mat4x2i32;
public typedef int32_t3x4 mat3x4i32;
public typedef int32_t4x3 mat4x3i32;

public typedef int64_t2x2 mat2i64;
public typedef int64_t3x3 mat3i64;
public typedef int64_t4x4 mat4i64;
public typedef int64_t2x3 mat2x3i64;
public typedef int64_t3x2 mat3x2i64;
public typedef int64_t2x4 mat2x4i64;
public typedef int64_t4x2 mat4x2i64;
public typedef int64_t3x4 mat3x4i64;
public typedef int64_t4x3 mat4x3i64;

public typedef uint8_t2x2 mat2u8;
public typedef uint8_t3x3 mat3u8;
public typedef uint8_t4x4 mat4u8;
public typedef uint8_t2x3 mat2x3u8;
public typedef uint8_t3x2 mat3x2u8;
public typedef uint8_t2x4 mat2x4u8;
public typedef uint8_t4x2 mat4x2u8;
public typedef uint8_t3x4 mat3x4u8;
public typedef uint8_t4x3 mat4x3u8;

public typedef uint16_t2x2 mat2u16;
public typedef uint16_t3x3 mat3u16;
public typedef uint16_t4x4 mat4u16;
public typedef uint16_t2x3 mat2x3u16;
public typedef uint16_t3x2 mat3x2u16;
public typedef uint16_t2x4 mat2x4u16;
public typedef uint16_t4x2 mat4x2u16;
public typedef uint16_t3x4 mat3x4u16;
public typedef uint16_t4x3 mat4x3u16;

public typedef uint32_t2x2 mat2u32;
public typedef uint32_t3x3 mat3u32;
public typedef uint32_t4x4 mat4u32;
public typedef uint32_t2x3 mat2x3u32;
public typedef uint32_t3x2 mat3x2u32;
public typedef uint32_t2x4 mat2x4u32;
public typedef uint32_t4x2 mat4x2u32;
public typedef uint32_t3x4 mat3x4u32;
public typedef uint32_t4x3 mat4x3u32;

public typedef int64_t2x2 mat2u64;
public typedef int64_t3x3 mat3u64;
public typedef int64_t4x4 mat4u64;
public typedef int64_t2x3 mat2x3u64;
public typedef int64_t3x2 mat3x2u64;
public typedef int64_t2x4 mat2x4u64;
public typedef int64_t4x2 mat4x2u64;
public typedef int64_t3x4 mat3x4u64;
public typedef int64_t4x3 mat4x3u64;

public typedef float16_t2x2 mat2f16;
public typedef float16_t3x3 mat3f16;
public typedef float16_t4x4 mat4f16;
public typedef float16_t2x3 mat2x3f16;
public typedef float16_t3x2 mat3x2f16;
public typedef float16_t2x4 mat2x4f16;
public typedef float16_t4x2 mat4x2f16;
public typedef float16_t3x4 mat3x4f16;
public typedef float16_t4x3 mat4x3f16;

public typedef float32_t2x2 mat2f32;
public typedef float32_t3x3 mat3f32;
public typedef float32_t4x4 mat4f32;
public typedef float32_t2x3 mat2x3f32;
public typedef float32_t3x2 mat3x2f32;
public typedef float32_t2x4 mat2x4f32;
public typedef float32_t4x2 mat4x2f32;
public typedef float32_t3x4 mat3x4f32;
public typedef float32_t4x3 mat4x3f32;

public typedef float64_t2x2 mat2f64;
public typedef float64_t3x3 mat3f64;
public typedef float64_t4x4 mat4f64;
public typedef float64_t2x3 mat2x3f64;
public typedef float64_t3x2 mat3x2f64;
public typedef float64_t2x4 mat2x4f64;
public typedef float64_t4x2 mat4x2f64;
public typedef float64_t3x4 mat3x4f64;
public typedef float64_t4x3 mat4x3f64;

[[vk::binding(0, 0)]] __DynamicResource<__DynamicResourceKind.General> textureHeap[];
[[vk::binding(1, 0)]] __DynamicResource<__DynamicResourceKind.Sampler> samplerHeap[];


public struct TextureHandle<TexelType : ITexelElement, Shape : __ITextureShape, let isArray : int, let isMS : int, let access : int> {
    typealias TextureType = _Texture<TexelType, Shape, isArray, isMS, 0, access, 0, 0, 0>;

    const static u32 samplerOffset = 20;
    const static u32 textureMask = (1 << samplerOffset) - 1;
    const u32 handle = textureMask;

    TextureType getTexture() {
        return textureHeap[NonUniformResourceIndex(handle & textureMask)].asOpaqueDescriptor<TextureType>();
    }

    SamplerState getSampler() {
        return samplerHeap[NonUniformResourceIndex(handle >> samplerOffset)].asOpaqueDescriptor<SamplerState>();
    }

    SamplerComparisonState getComparisonSampler() {
        return samplerHeap[NonUniformResourceIndex(handle >> samplerOffset)].asOpaqueDescriptor<SamplerComparisonState>();
    }

    // foo: this generates bad spirv (individual processing per element)
    // I'd love to be able to specialize per-number of components but Slang doesn't seem to have any notion of precedence when it comes to overloads, so that's fun
    // until they figure out their generic system a bit more I guess I'm stuck with this
    // this should be a rareish path anyways so it being a few extra ALU ops shouldn't be horrendous
    // (and maybe IHV optimizers can fix it up internally?)
    T unpackInvalidColor<T : ITexelElement>() where optional T : IFloat {
        vector<T.Element, T.elementCount> ret;
        T.Element normalizer = T.Element(1);
        if (T is IFloat) {
            normalizer = T.Element(7);
        }

        ret[0] = T.Element(((handle >> 20) & 0x7)) / normalizer;
        if (T.elementCount > 1) {
            ret[1] = T.Element(((handle >> 23) & 0x7)) / normalizer;
        }
        if (T.elementCount > 2) {
            ret[2] = T.Element(((handle >> 26) & 0x7)) / normalizer;
        }
        if (T.elementCount > 3) {
            ret[3] = T.Element(((handle >> 29) & 0x7)) / normalizer;
        }

        return reinterpret<T>(ret);
    }
}

// foo: would like to make a function that takes a lambda so the invalid check is only written once, but that crashes the compiler of this wonderful language
// also, I don't think I trust the compiler to not generate really bad spirv for that...
__generic<TexelType : ITexelElement, Shape : __ITextureShape, let isArray : int>
public extension TextureHandle<TexelType, Shape, isArray, 0, 0> {
    public TexelType __subscript(vector<int, Shape.dimensions + isArray> location) {
        if ((handle & textureMask) == textureMask) {
            return unpackInvalidColor<TexelType>();
        }
        return getTexture()[location];
    }

    public TexelType load(vector<int, Shape.dimensions + isArray + 1> location) {
        if ((handle & textureMask) == textureMask) {
            return unpackInvalidColor<TexelType>();
        }
        return getTexture().Load(location);
    }

    public TexelType sample(vector<float, Shape.dimensions + isArray> location) {
        if ((handle & textureMask) == textureMask) {
            return unpackInvalidColor<TexelType>();
        }
        return getTexture().Sample(getSampler(), location);
    }

    public TexelType sampleLevel(vector<float, Shape.dimensions + isArray> location, float level) {
        if ((handle & textureMask) == textureMask) {
            return unpackInvalidColor<TexelType>();
        }
        return getTexture().SampleLevel(getSampler(), location, level);
    }

    public float sampleAndCompare(vector<float, Shape.dimensions + isArray> location, float value) {
        if ((handle & textureMask) == textureMask) {
            return 0.0f;
        }
        return getTexture().SampleCmp(getComparisonSampler(), location, value);
    }
}

__generic<TexelType : ITexelElement, Shape : __ITextureShape, let isArray : int>
public extension TextureHandle<TexelType, Shape, isArray, 0, 1> {
    public __subscript(vector<uint, Shape.dimensions + isArray> location) -> TexelType
    {
        get {
            if ((handle & textureMask) == textureMask) {
                return unpackInvalidColor<TexelType>();
            }
            return getTexture()[location];
        }

        set(TexelType newValue) {
            if ((handle & textureMask) == textureMask) {
                return;
            }
            getTexture()[location] =  newValue;
        }
    }

    public TexelType load(vector<int, Shape.dimensions + isArray> location) {
        if ((handle & textureMask) == textureMask) {
            return unpackInvalidColor<TexelType>();
        }
        return getTexture().Load(location);
    }

    public void store(vector<uint, Shape.dimensions + isArray> location, TexelType texel) {
        if ((handle & textureMask) == textureMask) {
            return;
        }
        return getTexture().Store(location, texel);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 0, 0, 0> {
    public u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        u32 extent, numLevels;
        getTexture().GetDimensions(level, extent, numLevels);
        return u16(extent);
    }

    public u8 levels() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        u32 extent, numLevels;
        getTexture().GetDimensions(0, extent, numLevels);
        return u8(numLevels);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 0, 0, 1> {
    public u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        u32 extent;
        getTexture().GetDimensions(extent);
        return u16(extent);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 1, 0, 0> {
    public u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(level, extent.x, extent.y, extent.z);
        return u16(extent.x);
    }

    public u8 levels() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(0, extent.x, extent.y, extent.z);
        return u8(extent.z);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec2u32 extent;
        getTexture().GetDimensions(extent.x, extent.y);
        return u16(extent.y);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 1, 0, 1> {
    public u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec2u32 extent;
        getTexture().GetDimensions(extent.x, extent.y);
        return u16(extent.x);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec2u32 extent;
        getTexture().GetDimensions(extent.x, extent.y);
        return u16(extent.y);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 0, 0> {
    public vec2u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec3u32 extent;
        getTexture().GetDimensions(level, extent.x, extent.y, extent.z);
        return vec2u16(extent.xy);
    }

    public u8 levels() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(0, extent.x, extent.y, extent.z);
        return u8(extent.z);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 0, 1> {
    public vec2u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec2u32 extent;
        getTexture().GetDimensions(extent.x, extent.y);
        return vec2u16(extent.xy);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 0, 0> {
    public vec2u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec4u32 extent;
        getTexture().GetDimensions(level, extent.x, extent.y, extent.z, extent.w);
        return vec2u16(extent.xy);
    }

    public u8 levels() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec4u32 extent;
        getTexture().GetDimensions(0, extent.x, extent.y, extent.z, extent.w);
        return u8(extent.w);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return u16(extent.z);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 0, 1> {
    public vec2u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return vec2u16(extent.xy);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return u16(extent.z);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 1, 0> {
    public vec2u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return vec2u16(extent.xy);
    }

    public u8 samples() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return u8(extent.z);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 1, 1> {
    public vec2u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return vec2u16(extent.xy);
    }

    public u8 samples() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return u8(extent.z);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 1, 0> {
    public vec2u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec4u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
        return vec2u16(extent.xy);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec4u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
        return u16(extent.z);
    }

    public u8 samples() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec4u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
        return u8(extent.w);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 1, 1> {
    public vec2u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec4u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
        return vec2u16(extent.xy);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec4u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
        return u16(extent.z);
    }

    public u8 samples() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec4u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
        return u8(extent.w);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape3D, 0, 0, 0> {
    public vec3u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return vec3u16(1, 1, 1);
        }
        vec4u32 extent;
        getTexture().GetDimensions(level, extent.x, extent.y, extent.z, extent.w);
        return vec3u16(extent.xyz);
    }

    public u8 levels() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec4u32 extent;
        getTexture().GetDimensions(0, extent.x, extent.y, extent.z, extent.w);
        return u8(extent.w);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape3D, 0, 0, 1> {
    public vec3u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return vec3u16(1, 1, 1);
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return vec3u16(extent.xyz);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 0, 0, 0> {
    public vec2u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec3u32 extent;
        getTexture().GetDimensions(level, extent.x, extent.y, extent.z);
        return vec2u16(extent.xy);
    }

    public u8 levels() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(0, extent.x, extent.y, extent.z);
        return u8(extent.z);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 0, 0, 1> {
    public vec2u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec2u32 extent;
        getTexture().GetDimensions(extent.x, extent.y);
        return vec2u16(extent.xy);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 1, 0, 0> {
    public vec2u16 extent(u8 level = 0) {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec4u32 extent;
        getTexture().GetDimensions(level, extent.x, extent.y, extent.z, extent.w);
        return vec2u16(extent.xy);
    }

    public u8 levels() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec4u32 extent;
        getTexture().GetDimensions(0, extent.x, extent.y, extent.z, extent.w);
        return u8(extent.w);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return u16(extent.z);
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 1, 0, 1> {
    public vec2u16 extent() {
        if ((handle & textureMask) == textureMask) {
            return vec2u16(1, 1);
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return vec2u16(extent.xy);
    }

    public u16 layers() {
        if ((handle & textureMask) == textureMask) {
            return 1;
        }
        vec3u32 extent;
        getTexture().GetDimensions(extent.x, extent.y, extent.z);
        return u16(extent.z);
    }
}

public typealias Texture1D<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape1D, 0, 0, 0>;
public typealias Texture1DArray<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape1D, 1, 0, 0>;
public typealias Texture2D<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 0, 0, 0>;
public typealias Texture2DArray<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 1, 0, 0>;
public typealias Texture2DMS<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 0, 1, 0>;
public typealias Texture2DMSArray<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 1, 1, 0>;
public typealias TextureCube<TexelType : ITexelElement> = TextureHandle<TexelType, __ShapeCube, 0, 0, 0>;
public typealias TextureCubeArray<TexelType : ITexelElement> = TextureHandle<TexelType, __ShapeCube, 1, 0, 0>;
public typealias Texture3D<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape3D, 0, 0, 0>;

public typealias Image1D<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape1D, 0, 0, 1>;
public typealias Image1DArray<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape1D, 1, 0, 1>;
public typealias Image2D<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 0, 0, 1>;
public typealias Image2DArray<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 1, 0, 1>;
public typealias Image2DMS<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 0, 1, 1>;
public typealias Image2DMSArray<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape2D, 1, 1, 1>;
public typealias ImageCube<TexelType : ITexelElement> = TextureHandle<TexelType, __ShapeCube, 0, 0, 1>;
public typealias ImageCubeArray<TexelType : ITexelElement> = TextureHandle<TexelType, __ShapeCube, 1, 0, 1>;
public typealias Image3D<TexelType : ITexelElement> = TextureHandle<TexelType, __Shape3D, 0, 0, 1>;