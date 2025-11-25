struct VSOut {
    float4 position : SV_Position;
    float4 color;
}

[shader("vertex")]
VSOut vsmain(uint vtxIdx: SV_VertexID) {
    float3 positions[3] = {
        float3( 0.0f, -0.5f, 0.0f),
        float3( 0.5f,  0.5f, 0.0f),
        float3(-0.5f,  0.5f, 0.0f)
    };

    float4 colors[3] = {
        float4(1.0f, 0.0f, 0.0f, .0f),
        float4(0.0f, 1.0f, 0.0f, .0f),
        float4(0.0f, 0.0f, 1.0f, .0f)
    };

    VSOut out = {
        float4(positions[vtxIdx], 1.0),
        colors[vtxIdx]
    };

    return out;
}

[shader("fragment")]
float4 fsmain(VSOut input) : SV_Target {
    return input.color;
}

