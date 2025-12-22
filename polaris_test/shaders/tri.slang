struct Vertex {
    float3 position;
    float2 uv;
}

struct VSOut {
    float4 position : SV_Position;
    float4 color;
}

struct Texture2D<T> {
    void foo() {

    }
};

[shader("vertex")]
VSOut vsmain(Vertex *vertices, uint vtxIdx: SV_VertexID) {
    Texture2D<float4> var;
    var.foo();
    return VSOut(
        float4(vertices[vtxIdx].position, 1.0f),
        float4(vertices[vtxIdx].uv, 0.0f, 1.0f)
    );
}

[shader("fragment")]
float4 fsmain(VSOut input) : SV_Target {
    return input.color;
}

