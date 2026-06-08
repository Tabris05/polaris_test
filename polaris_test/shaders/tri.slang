#include "../shared/push_constants.h"

struct MSOut {
    vec4f32 position : SV_Position;
    vec2f32 uv;
};

struct ThreadId {
    u32 groupId : SV_GroupID;
    u32 threadId : SV_GroupThreadID;
};

static const u8 WorkGroupSize = Meshlet::MaxVertexCount;

[shader("mesh"), outputtopology("triangle"), NumThreads(WorkGroupSize)]
void msmain(uniform PushConstants pcs, ThreadId id, out vertices MSOut vertices[Meshlet::MaxVertexCount], out indices vec3u32 triangles[Meshlet::MaxTriangleCount]) {
    Meshlet meshlet = pcs.meshlets[id.groupId];
    SetMeshOutputCounts(meshlet.vertexCount, meshlet.triangleCount);

    if (id.threadId < meshlet.vertexCount) {
        vertices[id.threadId].position = mul(pcs.mvp, vec4f32(meshlet.vertices[id.threadId].position, 1.0f));
        vertices[id.threadId].uv = meshlet.vertices[id.threadId].uv;
    }

    for (u8 i = id.threadId; i < meshlet.triangleCount; i += WorkGroupSize) {
        triangles[i] = meshlet.triangles[i];
    }
}

[shader("fragment")]
vec4f32 fsmain(uniform PushConstants pcs, MSOut input) : SV_Target {
    return pow(pcs.texture.sample(input.uv), 1.0f / 2.2f);
}

