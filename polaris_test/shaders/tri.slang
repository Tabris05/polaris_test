#include "../shared/push_constants.h"

struct MSOut {
    f32vec4 position : SV_Position;
    f32vec2 uv;
};

struct ThreadId {
    u32 groupId : SV_GroupID;
    u32 threadId : SV_GroupThreadID;
};

static const u8 WorkGroupSize = Meshlet::MaxVertexCount;

[shader("mesh"), outputtopology("triangle"), NumThreads(WorkGroupSize)]
void msmain(uniform PushConstants pcs, ThreadId id, out vertices MSOut vertices[Meshlet::MaxVertexCount], out indices u32vec3 triangles[Meshlet::MaxTriangleCount]) {
    Meshlet meshlet = pcs.meshlets[id.groupId];
    SetMeshOutputCounts(meshlet.vertexCount, meshlet.triangleCount);

    if (id.threadId < meshlet.vertexCount) {
        vertices[id.threadId].position = mul(pcs.mvp, f32vec4(meshlet.vertices[id.threadId].position, 1.0f));
        vertices[id.threadId].uv = meshlet.vertices[id.threadId].uv;
    }

    for (u32 i = id.threadId; i < meshlet.triangleCount; i += WorkGroupSize) {
        triangles[i] = meshlet.triangles[i];
    }
}

[shader("fragment")]
f32vec4 fsmain(uniform PushConstants pcs, MSOut input) : SV_Target {
    return pow(pcs.texture.sample(input.uv), 1.0f / 2.2f);
}

