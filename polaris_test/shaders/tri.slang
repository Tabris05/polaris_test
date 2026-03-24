#include "../shared/push_constants.h"

struct MSOut {
    vec4f32 position : SV_Position;
    vec2f32 uv;
}

[shader("mesh"), outputtopology("triangle"), NumThreads(64)]
void msmain(uniform PushConstants pcs, u32 globalID: SV_DispatchThreadID, u32 localID: SV_GroupThreadID, out vertices MSOut verts[192], out indices uint3 tris[64]) {
    if (localID == 0) {
        u32 remainingVerts = pcs.vertexCount - globalID * 3;
        u32 vertsThisGroup = min(192, remainingVerts);
        SetMeshOutputCounts(vertsThisGroup, vertsThisGroup / 3);
    }

    if (globalID * 3 < pcs.vertexCount) {
        for (u32 i = 0; i < 3; i++) {
            u32 localIndex = localID * 3 + i;
            u32 globalIndex = globalID * 3 + i;

            verts[localIndex].position = mul(pcs.mvp, vec4f32(pcs.vertices[globalIndex].position, 1.0f));
            verts[localIndex].uv = pcs.vertices[globalIndex].uv;

            tris[localID][i] = localIndex;
        }
    }
}

[shader("fragment")]
vec4f32 fsmain(uniform PushConstants pcs, MSOut input) : SV_Target {
    return pcs.texture.sample(input.uv);
}

