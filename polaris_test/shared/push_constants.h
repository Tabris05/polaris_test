#include <polaris/polaris.hpp>

struct Vertex {
    vec3f32 position;
    vec2f32 uv;
};

struct PushConstants {
    Vertex* vertices;
    mat4f32 mvp;
    Texture2D<vec4f32> texture;
};