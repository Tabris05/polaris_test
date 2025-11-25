#include "push_constants.h"

[vk::binding(0, 0)] Texture2D sampledHeap[];
[vk::binding(0, 0)] RWTexture2D rwHeap[];

[shader("compute"), numthreads(8, 8, 1)]
void csmain(uniform PushConstants constants, uint3 threadId: SV_DispatchThreadID) {
    rwHeap[constants.dstHandle][threadId.xy] = sampledHeap[constants.srcHandle][threadId.xy];
}