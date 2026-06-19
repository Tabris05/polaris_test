[shader("compute"), numthreads(8, 8, 1)]
void csmain(uniform uint srcHandle, uniform uint dstHandle, uint3 threadId: SV_DispatchThreadID) {
    DescriptorHandle<RWTexture2D>(dstHandle)[threadId.xy] = DescriptorHandle<Texture2D>(srcHandle)[threadId.xy];
}