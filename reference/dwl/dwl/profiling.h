#pragma once

#ifdef TRACY_ENABLE
#include <tracy/TracyC.h>

static inline void _wlr_profile_scope_end(TracyCZoneCtx *zone) {
  TracyCZoneEnd(*zone);
}

#define WLR_PROFILE_CONCAT_IMPL(a, b) a##b
#define WLR_PROFILE_CONCAT(a, b) WLR_PROFILE_CONCAT_IMPL(a, b)
#define WLR_PROFILE_SCOPE_IMPL(id, name)                                      \
  static const struct ___tracy_source_location_data                         \
      WLR_PROFILE_CONCAT(_wlr_profile_location_, id) = {                       \
          name, __func__, __FILE__, __LINE__, 0};                           \
  TracyCZoneCtx WLR_PROFILE_CONCAT(_wlr_profile_zone_, id)                       \
      __attribute__((cleanup(_wlr_profile_scope_end), unused)) =              \
          ___tracy_emit_zone_begin(                                        \
              &WLR_PROFILE_CONCAT(_wlr_profile_location_, id), 1)

#define WLR_PROFILE_SCOPE() WLR_PROFILE_SCOPE_IMPL(__COUNTER__, NULL)
#define WLR_PROFILE_SCOPE_NAMED(name) WLR_PROFILE_SCOPE_IMPL(__COUNTER__, name)

#define WLR_PROFILE_BEGIN(zone) TracyCZone(zone, 1)
#define WLR_PROFILE_END(zone) TracyCZoneEnd(zone)

#else

#define WLR_PROFILE_SCOPE()
#define WLR_PROFILE_SCOPE_NAMED(name)
#define WLR_PROFILE_BEGIN(zone)
#define WLR_PROFILE_END(zone)

#endif
