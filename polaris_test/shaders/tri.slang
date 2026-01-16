#include "../shared/push_constants.h"


struct VSOut {
    vec4f32 position : SV_Position;
    vec2f32 uv;
}

[shader("vertex")]
VSOut vsmain(uniform PushConstants pcs, uint vtxIdx: SV_VertexID) {
    return VSOut(
        mul(pcs.mvp, vec4f32(pcs.vertices[vtxIdx].position, 1.0f)),
        pcs.vertices[vtxIdx].uv
    );
}

[shader("fragment")]
vec4f32 fsmain(uniform PushConstants pcs, VSOut input) : SV_Target {
    return pcs.texture.sample(input.uv);
}

