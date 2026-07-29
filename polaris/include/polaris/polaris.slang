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

public typedef bool2 b8vec2;
public typedef bool3 b8vec3;
public typedef bool4 b8vec4;

public typedef int8_t2 i8vec2;
public typedef int8_t3 i8vec3;
public typedef int8_t4 i8vec4;

public typedef int16_t2 i16vec2;
public typedef int16_t3 i16vec3;
public typedef int16_t4 i16vec4;

public typedef int32_t2 i32vec2;
public typedef int32_t3 i32vec3;
public typedef int32_t4 i32vec4;

public typedef int64_t2 i64vec2;
public typedef int64_t3 i64vec3;
public typedef int64_t4 i64vec4;

public typedef uint8_t2 u8vec2;
public typedef uint8_t3 u8vec3;
public typedef uint8_t4 u8vec4;

public typedef uint16_t2 u16vec2;
public typedef uint16_t3 u16vec3;
public typedef uint16_t4 u16vec4;

public typedef uint32_t2 u32vec2;
public typedef uint32_t3 u32vec3;
public typedef uint32_t4 u32vec4;

public typedef uint64_t2 u64vec2;
public typedef uint64_t3 u64vec3;
public typedef uint64_t4 u64vec4;

public typedef float16_t2 f16vec2;
public typedef float16_t3 f16vec3;
public typedef float16_t4 f16vec4;

public typedef float32_t2 f32vec2;
public typedef float32_t3 f32vec3;
public typedef float32_t4 f32vec4;

public typedef float64_t2 f64vec2;
public typedef float64_t3 f64vec3;
public typedef float64_t4 f64vec4;

public typedef bool2x2 b8mat2;
public typedef bool3x3 b8mat3;
public typedef bool4x4 b8mat4;
public typedef bool2x3 b8mat2x3;
public typedef bool3x2 b8mat3x2;
public typedef bool2x4 b8mat2x4;
public typedef bool4x2 b8mat4x2;
public typedef bool3x4 b8mat3x4;
public typedef bool4x3 b8mat4x3;

public typedef int8_t2x2 i8mat2;
public typedef int8_t3x3 i8mat3;
public typedef int8_t4x4 i8mat4;
public typedef int8_t2x3 i8mat2x3;
public typedef int8_t3x2 i8mat3x2;
public typedef int8_t2x4 i8mat2x4;
public typedef int8_t4x2 i8mat4x2;
public typedef int8_t3x4 i8mat3x4;
public typedef int8_t4x3 i8mat4x3;

public typedef int16_t2x2 i16mat2;
public typedef int16_t3x3 i16mat3;
public typedef int16_t4x4 i16mat4;
public typedef int16_t2x3 i16mat2x3;
public typedef int16_t3x2 i16mat3x2;
public typedef int16_t2x4 i16mat2x4;
public typedef int16_t4x2 i16mat4x2;
public typedef int16_t3x4 i16mat3x4;
public typedef int16_t4x3 i16mat4x3;

public typedef int32_t2x2 i32mat2;
public typedef int32_t3x3 i32mat3;
public typedef int32_t4x4 i32mat4;
public typedef int32_t2x3 i32mat2x3;
public typedef int32_t3x2 i32mat3x2;
public typedef int32_t2x4 i32mat2x4;
public typedef int32_t4x2 i32mat4x2;
public typedef int32_t3x4 i32mat3x4;
public typedef int32_t4x3 i32mat4x3;

public typedef int64_t2x2 i64mat2;
public typedef int64_t3x3 i64mat3;
public typedef int64_t4x4 i64mat4;
public typedef int64_t2x3 i64mat2x3;
public typedef int64_t3x2 i64mat3x2;
public typedef int64_t2x4 i64mat2x4;
public typedef int64_t4x2 i64mat4x2;
public typedef int64_t3x4 i64mat3x4;
public typedef int64_t4x3 i64mat4x3;

public typedef uint8_t2x2 u8mat2;
public typedef uint8_t3x3 u8mat3;
public typedef uint8_t4x4 u8mat4;
public typedef uint8_t2x3 u8mat2x3;
public typedef uint8_t3x2 u8mat3x2;
public typedef uint8_t2x4 u8mat2x4;
public typedef uint8_t4x2 u8mat4x2;
public typedef uint8_t3x4 u8mat3x4;
public typedef uint8_t4x3 u8mat4x3;

public typedef uint16_t2x2 u16mat2;
public typedef uint16_t3x3 u16mat3;
public typedef uint16_t4x4 u16mat4;
public typedef uint16_t2x3 u16mat2x3;
public typedef uint16_t3x2 u16mat3x2;
public typedef uint16_t2x4 u16mat2x4;
public typedef uint16_t4x2 u16mat4x2;
public typedef uint16_t3x4 u16mat3x4;
public typedef uint16_t4x3 u16mat4x3;

public typedef uint32_t2x2 u32mat2;
public typedef uint32_t3x3 u32mat3;
public typedef uint32_t4x4 u32mat4;
public typedef uint32_t2x3 u32mat2x3;
public typedef uint32_t3x2 u32mat3x2;
public typedef uint32_t2x4 u32mat2x4;
public typedef uint32_t4x2 u32mat4x2;
public typedef uint32_t3x4 u32mat3x4;
public typedef uint32_t4x3 u32mat4x3;

public typedef uint64_t2x2 u64mat2;
public typedef uint64_t3x3 u64mat3;
public typedef uint64_t4x4 u64mat4;
public typedef uint64_t2x3 u64mat2x3;
public typedef uint64_t3x2 u64mat3x2;
public typedef uint64_t2x4 u64mat2x4;
public typedef uint64_t4x2 u64mat4x2;
public typedef uint64_t3x4 u64mat3x4;
public typedef uint64_t4x3 u64mat4x3;

public typedef float16_t2x2 f16mat2;
public typedef float16_t3x3 f16mat3;
public typedef float16_t4x4 f16mat4;
public typedef float16_t2x3 f16mat2x3;
public typedef float16_t3x2 f16mat3x2;
public typedef float16_t2x4 f16mat2x4;
public typedef float16_t4x2 f16mat4x2;
public typedef float16_t3x4 f16mat3x4;
public typedef float16_t4x3 f16mat4x3;

public typedef float32_t2x2 f32mat2;
public typedef float32_t3x3 f32mat3;
public typedef float32_t4x4 f32mat4;
public typedef float32_t2x3 f32mat2x3;
public typedef float32_t3x2 f32mat3x2;
public typedef float32_t2x4 f32mat2x4;
public typedef float32_t4x2 f32mat4x2;
public typedef float32_t3x4 f32mat3x4;
public typedef float32_t4x3 f32mat4x3;

public typedef float64_t2x2 f64mat2;
public typedef float64_t3x3 f64mat3;
public typedef float64_t4x4 f64mat4;
public typedef float64_t2x3 f64mat2x3;
public typedef float64_t3x2 f64mat3x2;
public typedef float64_t2x4 f64mat2x4;
public typedef float64_t4x2 f64mat4x2;
public typedef float64_t3x4 f64mat3x4;
public typedef float64_t4x3 f64mat4x3;

public struct TextureHandle<TexelType : ITexelElement, Shape : __ITextureShape, let isArray : int, let isMS : int, let access : int> {
    typealias TextureType = _Texture<TexelType, Shape, isArray, isMS, 0, access, 0, 0, 0>;

    const static u32 samplerOffset = 20;
    const static u32 textureMask = (1 << samplerOffset) - 1;
    const u32 handle = textureMask;

    TextureType getTexture() {
        return ResourceDescriptorHeap[NonUniformResourceIndex(handle & textureMask)];
    }

    SamplerState getSampler() {
        return SamplerDescriptorHeap[NonUniformResourceIndex(handle >> samplerOffset)];
    }

    SamplerComparisonState getComparisonSampler() {
        return SamplerDescriptorHeap[NonUniformResourceIndex(handle >> samplerOffset)];
    }

    // foo: this function is completely broken, it emits inefficient spirv and for reasons both known and unknown will only output black
    T unpackInvalidColor<T : ITexelElement = TexelType>() {
        vector<T.Element, T.elementCount> ret;
        for (i32 i = 0; i < T.elementCount; i++) {
            ret[i] = T.Element((handle >> uint(20 + i * 3)) & 0x7) / (T is IFloat ? T.Element(4) : T.Element(1));
        }

        return reinterpret<T>(ret);
    }

    TexelType invalidColorOr(IFunc<TexelType> fn) {
        if ((handle & textureMask) == textureMask) {
            return unpackInvalidColor();
        }

        return fn();
    }

    T allOnesOr<T : IInteger>(IFunc<T> fn) {
        if ((handle & textureMask) == textureMask) {
            return T(1);
        }

        return fn();
    }
}

__generic<TexelType : ITexelElement, Shape : __ITextureShape, let isArray : int>
public extension TextureHandle<TexelType, Shape, isArray, 0, 0> {
    public TexelType __subscript(vector<int, Shape.dimensions + isArray> location) {
        return invalidColorOr(() => getTexture()[location]);
    }

    public TexelType load(vector<int, Shape.dimensions + isArray + 1> location) {
        return invalidColorOr(() => getTexture().Load(location));
    }

    public TexelType sample(vector<float, Shape.dimensions + isArray> location) {
        return invalidColorOr(() => getTexture().Sample(getSampler(), location));
    }

    public TexelType sampleLevel(vector<float, Shape.dimensions + isArray> location, float level, constexpr vector<int, Shape.planeDimensions> offset = vector<int, Shape.planeDimensions>(0)) {
        if ((handle & textureMask) == textureMask) {
            return unpackInvalidColor();
        }
        return getTexture().SampleLevel(getSampler(), location, level, offset);
    }

    public float sampleAndCompare(vector<float, Shape.dimensions + isArray> location, float value) {
        if ((handle & textureMask) != textureMask) {
            return getTexture().SampleCmp(getComparisonSampler(), location, value);
        }
        return 0.0f;
    }
}

__generic<TexelType : ITexelElement, Shape : __ITextureShape, let isArray : int>
public extension TextureHandle<TexelType, Shape, isArray, 0, 1> {
    public __subscript(vector<uint, Shape.dimensions + isArray> location) -> TexelType
    {
        get {
            return invalidColorOr(() => getTexture()[location]);
        }

        set(TexelType newValue) {
            if ((handle & textureMask) != textureMask) {
                getTexture()[location] = newValue;
            }
        }
    }

    public TexelType load(vector<int, Shape.dimensions + isArray> location) {
        return invalidColorOr(() => getTexture().Load(location));
    }

    public void store(vector<uint, Shape.dimensions + isArray> location, TexelType texel) {
        if ((handle & textureMask) != textureMask) {
            getTexture().Store(location, texel);
        }
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 0, 0, 0> {
    public u16 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32 extent, numLevels;
            getTexture().GetDimensions(level, extent, numLevels);
            return u16(extent);
        });
    }

    public u8 levels() {
        return allOnesOr(() => {
            u32 extent, numLevels;
            getTexture().GetDimensions(0, extent, numLevels);
            return u8(numLevels);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 0, 0, 1> {
    public u16 extent() {
        return allOnesOr(() => {
            u32 extent;
            getTexture().GetDimensions(extent);
            return u16(extent);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 1, 0, 0> {
    public u16 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(level, extent.x, extent.y, extent.z);
            return u16(extent.x);
        });
    }

    public u8 levels() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(0, extent.x, extent.y, extent.z);
            return u8(extent.z);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec2 extent;
            getTexture().GetDimensions(extent.x, extent.y);
            return u16(extent.y);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape1D, 1, 0, 1> {
    public u16 extent() {
        return allOnesOr(() => {
            u32vec2 extent;
            getTexture().GetDimensions(extent.x, extent.y);
            return u16(extent.x);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec2 extent;
            getTexture().GetDimensions(extent.x, extent.y);
            return u16(extent.y);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 0, 0> {
    public u16vec2 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(level, extent.x, extent.y, extent.z);
            return u16vec2(extent.xy);
        });
    }

    public u8 levels() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(0, extent.x, extent.y, extent.z);
            return u8(extent.z);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 0, 1> {
    public u16vec2 extent() {
        return allOnesOr(() => {
            u32vec2 extent;
            getTexture().GetDimensions(extent.x, extent.y);
            return u16vec2(extent.xy);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 0, 0> {
    public u16vec2 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(level, extent.x, extent.y, extent.z, extent.w);
            return u16vec2(extent.xy);
        });
    }

    public u8 levels() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(0, extent.x, extent.y, extent.z, extent.w);
            return u8(extent.w);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16(extent.z);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 0, 1> {
    public u16vec2 extent() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16vec2(extent.xy);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16(extent.z);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 1, 0> {
    public u16vec2 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16vec2(extent.xy);
        });
    }

    public u8 samples() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u8(extent.z);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 0, 1, 1> {
    public u16vec2 extent() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16vec2(extent.xy);
        });
    }

    public u8 samples() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u8(extent.z);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 1, 0> {
    public u16vec2 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
            return u16vec2(extent.xy);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
            return u16(extent.z);
        });
    }

    public u8 samples() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
            return u8(extent.w);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape2D, 1, 1, 1> {
    public u16vec2 extent() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
            return u16vec2(extent.xy);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
            return u16(extent.z);
        });
    }

    public u8 samples() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z, extent.w);
            return u8(extent.w);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape3D, 0, 0, 0> {
    public u16vec3 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(level, extent.x, extent.y, extent.z, extent.w);
            return u16vec3(extent.xyz);
        });
    }

    public u8 levels() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(0, extent.x, extent.y, extent.z, extent.w);
            return u8(extent.w);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __Shape3D, 0, 0, 1> {
    public u16vec3 extent() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16vec3(extent.xyz);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 0, 0, 0> {
    public u16vec2 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(level, extent.x, extent.y, extent.z);
            return u16vec2(extent.xy);
        });
    }

    public u8 levels() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(0, extent.x, extent.y, extent.z);
            return u8(extent.z);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 0, 0, 1> {
    public u16vec2 extent() {
        return allOnesOr(() => {
            u32vec2 extent;
            getTexture().GetDimensions(extent.x, extent.y);
            return u16vec2(extent.xy);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 1, 0, 0> {
    public u16vec2 extent(u8 level = 0) {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(level, extent.x, extent.y, extent.z, extent.w);
            return u16vec2(extent.xy);
        });
    }

    public u8 levels() {
        return allOnesOr(() => {
            u32vec4 extent;
            getTexture().GetDimensions(0, extent.x, extent.y, extent.z, extent.w);
            return u8(extent.w);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16(extent.z);
        });
    }
}

__generic<TexelType : ITexelElement>
public extension TextureHandle<TexelType, __ShapeCube, 1, 0, 1> {
    public u16vec2 extent() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16vec2(extent.xy);
        });
    }

    public u16 layers() {
        return allOnesOr(() => {
            u32vec3 extent;
            getTexture().GetDimensions(extent.x, extent.y, extent.z);
            return u16(extent.z);
        });
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
