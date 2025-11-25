#ifdef __cplusplus
#pragma once
#include <tabris/types.hpp>
#else
typedef uint u32;
#endif

struct PushConstants {
	u32 srcHandle;
	u32 dstHandle;
};
