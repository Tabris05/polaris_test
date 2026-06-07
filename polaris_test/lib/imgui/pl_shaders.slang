#include <polaris/polaris.hpp>

struct Vertex {
    vec2f32 position;
    vec2f32 uv;
    vec4u8 color;
};

struct PushConstants {
    vec2f32 scale;
    vec2f32 translate;
    Vertex* vertexBuffer;
    u16* indexBuffer;
    u32 elementCount;
    Texture2D<vec4f32> texture;
};

struct MSOut {
    vec4f32 position : SV_Position;
    vec4f32 color;
    vec2f32 uv;
};

[shader("mesh"), outputtopology("triangle"), NumThreads(64)]
void msmain(uniform PushConstants pcs, u32 globalID: SV_DispatchThreadID, u32 localID: SV_GroupThreadID, out vertices MSOut verts[192], out indices vec3u32 tris[64]) {
    if (localID == 0) {
        u32 remainingVerts = pcs.elementCount - globalID * 3;
        u32 vertsThisGroup = min(192, remainingVerts);
        SetMeshOutputCounts(vertsThisGroup, vertsThisGroup / 3);
    }

    if (globalID * 3 < pcs.elementCount) {
        for (u32 i = 0; i < 3; i++) {
            u32 localIndex = localID * 3 + i;
            u32 globalIndex = globalID * 3 + i;

            Vertex v = pcs.vertexBuffer[pcs.indexBuffer[globalIndex]];

            verts[localIndex].position = vec4f32(v.position * pcs.scale + pcs.translate, 0.0f, 1.0f);
            verts[localIndex].color = vec4f32(v.color) / vec4f32(255.0f);
            verts[localIndex].uv = v.uv;

            tris[localID][i] = localIndex;
        }
    }
}

[shader("fragment")]
vec4f32 fsmain(uniform PushConstants pcs, MSOut input) : SV_Target {
    return input.color * pcs.texture.sample(input.uv);
}