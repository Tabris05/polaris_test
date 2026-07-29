#include <polaris/polaris.hpp>

struct Vertex {
    f32vec3 position;
    f32vec2 uv;
};

struct Meshlet {
    static const u8 MaxVertexCount = 64;
    static const u8 MaxTriangleCount = 126;
    Vertex* vertices;
    u8vec3* triangles;
    u8 vertexCount;
    u8 triangleCount;
};

struct PushConstants {
    Meshlet* meshlets;
    f32mat4 mvp;
    Texture2D<f32vec4> texture;
};