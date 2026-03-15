#include "push_constants.h"

[shader("compute"), numthreads(8, 8, 1)]
void csmain(uniform PushConstants constants, uint3 threadId: SV_DispatchThreadID) {
    DescriptorHandle<RWTexture2D>(constants.dstHandle)[threadId.xy] = DescriptorHandle<Texture2D>(constants.srcHandle)[threadId.xy];
}