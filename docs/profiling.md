# Profiling dwl and wlroots

From the wlroots checkout, run:

```sh
./dwl/build-profiling.sh
./dwl/build-profiling/dwl
```

The `dwl/` directory contains the compositor source directly. The script builds
wlroots in `build-profiling`, then dwl in `dwl/build-profiling`. It uses this
checkout's headers and libraries, not the system wlroots. Keep both build
directories together: the binary's runtime search path points to the wlroots
build directory. Nothing is installed system-wide.

For GUI inspection, use a matching Tracy profiler and connect to `localhost:8086`
(or the compositor machine's address). The automated Vortex comparison uses
`../../scripts/profile-compare.sh` and GUI-free capture/export tools instead.
One shared Tracy client serves both
libraries and the compositor. It uses Vortex's Tracy 0.13.1 source, protocol 76,
and the same default `TRACY_ENABLE` CPU profiling configuration. Start dwl in
its intended session; the build script does not launch it.

Scopes include runtime dwl callbacks and inline client/IME helpers, wlroots
protocol and scene operations, DRM and nested Wayland/X11 backends, backend
aggregation and session events, GLES2/EGL and Vulkan CPU work, allocators, and
utilities. Dynamic creation/destruction, hotplug, input, commits, rendering,
synchronization, and presentation remain visible. One-time setup, global
registration, device discovery helpers, shader compilation and renderer
construction are excluded. No GPU Tracy instrumentation is added. Per-frame
resource initialization and pipeline cache misses are included because they
can affect frame time. A scope's cleanup attribute emits its end on every
normal return, including early returns.

`python3 scripts/check-profiling.py` compares every function in the selected
source files with `scripts/profiling-coverage.json`, including explicit excluded
functions. Review that inventory when adding functions. Disabled profiling
expands all scopes to nothing and does not require Tracy or a C++ compiler.

The build needs Meson, Ninja, C/C++ compilers, wayland-scanner and development
dependencies for wlroots DRM, libinput, nested X11/Wayland, GLES2 and Vulkan.
It uses Vortex's `../../subprojects/tracy` source, or the path in `TRACY_SOURCE`,
and never downloads dependencies itself.
Optional Xwayland and ICC support are disabled. `build-profiling/config.h` inside
the dwl checkout is copied from `config.def.h` only if absent, so edits survive
rebuilds. Remove or replace that file to reset the configuration. The normal
Makefile build remains available separately. The dwl source includes the three
API changes needed for this wlroots 0.21 checkout.
