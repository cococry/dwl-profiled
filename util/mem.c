#include "util/profiling.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "util/mem.h"

bool memdup(void *out, const void *src, size_t size) {
	WLR_PROFILE_SCOPE();
	void *dst = malloc(size);
	if (dst == NULL) {
		return false;
	}
	memcpy(dst, src, size);
	void **dst_ptr = out;
	*dst_ptr = dst;
	return true;
}
