#include "util/profiling.h"
#include <stdlib.h>
#include <wlr/util/log.h>
#include "render/color.h"

struct wlr_color_transform *wlr_color_transform_init_linear_to_icc(
		const void *data, size_t size) {
	WLR_PROFILE_SCOPE();
	wlr_log(WLR_ERROR, "Cannot create color transform from ICC profile: "
		"LCMS2 is compile-time disabled");
	return NULL;
}

struct wlr_color_transform_lcms2 *color_transform_lcms2_from_base(
		struct wlr_color_transform *tr) {
	WLR_PROFILE_SCOPE();
	abort(); // unreachable
}

void color_transform_lcms2_finish(struct wlr_color_transform_lcms2 *tr) {
	WLR_PROFILE_SCOPE();
	abort(); // unreachable
}

void color_transform_lcms2_eval(struct wlr_color_transform_lcms2 *tr,
		float out[static 3], const float in[static 3]) {
	WLR_PROFILE_SCOPE();
	abort(); // unreachable
}
