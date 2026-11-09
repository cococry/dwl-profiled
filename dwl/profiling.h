#pragma once

#ifdef TRACY_ENABLE
#include <tracy/TracyC.h>
#include <time.h>

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

#ifndef WLR_PROFILE_COMPARISON
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

#ifdef WLR_PROFILE_COMPARISON
/* Align coarse zones with the external workload's CLOCK_MONOTONIC window.
 * The same timestamp event is emitted by both compositors. */
#define WLR_PROFILE_COMPARE_IMPL(id, name)                              \
  WLR_PROFILE_SCOPE_IMPL(id, name);                                     \
  struct timespec WLR_PROFILE_CONCAT(_wlr_profile_clock_, id);                 \
  clock_gettime(CLOCK_MONOTONIC, &WLR_PROFILE_CONCAT(_wlr_profile_clock_, id)); \
  TracyCZoneValue(WLR_PROFILE_CONCAT(_wlr_profile_zone_, id),                  \
      (uint64_t)WLR_PROFILE_CONCAT(_wlr_profile_clock_, id).tv_sec *           \
          UINT64_C(1000000000) +                                            \
      (uint64_t)WLR_PROFILE_CONCAT(_wlr_profile_clock_, id).tv_nsec)
#define WLR_PROFILE_COMPARE(name) WLR_PROFILE_COMPARE_IMPL(__COUNTER__, name)
#else
#define WLR_PROFILE_COMPARE(name)
#endif

#else

#define WLR_PROFILE_SCOPE()
#define WLR_PROFILE_SCOPE_NAMED(name)
#define WLR_PROFILE_BEGIN(zone)
#define WLR_PROFILE_END(zone)

#define WLR_PROFILE_COMPARE(name)

#endif
