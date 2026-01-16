#include "push_constants.h"

[vk::binding(0, 0)] RWTexture2D imageHeap[];
[vk::binding(0, 0)] Texture2D   textureHeap[];

[shader("compute"), numthreads(8, 8, 1)]
void csmain(uniform PushConstants constants, uint3 threadId: SV_DispatchThreadID) {
    imageHeap[constants.dstHandle][threadId.xy] = textureHeap[constants.srcHandle][threadId.xy];
}