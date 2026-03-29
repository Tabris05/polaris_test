#include <polaris/polaris.hpp>

struct Vertex {
    vec3f32 position;
    vec2f32 uv;
};

struct Meshlet {
    static const u8 MaxVertexCount = 64;
    static const u8 MaxTriangleCount = 126;
    Vertex* vertices;
    vec3u8* triangles;
    u8 vertexCount;
    u8 triangleCount;
};

struct PushConstants {
    Meshlet* meshlets;
    mat4f32 mvp;
    Texture2D<vec4f32> texture;
};