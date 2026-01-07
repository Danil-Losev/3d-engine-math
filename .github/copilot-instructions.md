# Copilot Usage Guide for 3d-engine-math

- Purpose: lightweight float math primitives for 3D graphics. Core types live in [vec2/vec2.hpp](vec2/vec2.hpp#L1-L82), [vec3/vec3.hpp](vec3/vec3.hpp#L1-L83), [vec4/vec4.hpp](vec4/vec4.hpp#L1-L88), and [mat4x4/mat4x4.hpp](mat4x4/mat4x4.hpp#L1-L70).
- Build locally with standalone g++ (no CMake):
  - Vectors: `g++ -std=c++20 -O3 -march=native vec2/vec2.cpp vec3/vec3.cpp vec4/vec4.cpp vec3/main.cpp -o vec_demo` (main uses vec3 tests; see caveat below).
  - Matrices: `g++ -std=c++20 -O3 -march=native -mavx2 -mfma mat4x4/mat4x4.cpp mat4x4/main.cpp -o mat_demo`.
- mat4x4 SIMD toggle: [mat4x4/mat4x4.cpp](mat4x4/mat4x4.cpp#L1-L340) defines `NO_SIMD`, so AVX/FMA paths are disabled by default. Remove the define to enable intrinsics; requires AVX2 + FMA and the alignment in [mat4x4/mat4x4.hpp](mat4x4/mat4x4.hpp#L62) (`alignas(32) float m_matrix[4][4];`).
- mat4x4 API expectations: `inverse()` throws on non-invertible matrices; `at(row,col)` checks bounds only in the `const` overload. Determinant uses the contiguous `m_matrix` layout (row-major) assumptions in [mat4x4/mat4x4.cpp](mat4x4/mat4x4.cpp#L300-L360).
- mat4x4 benchmarks: [mat4x4/main.cpp](mat4x4/main.cpp#L1-L186) runs 1e8 iterations per op; this is long-running—lower counts when iterating locally.
- vec3 dependencies: includes vec2 (constructor + projections). Static and instance utilities are duplicated (dot, cross, angle); keep implementations consistent and avoid extra sqrt where possible.
- vec3 numeric conventions: treat near-zero vectors via epsilon `1e-8f` (see normalize, projection, reflection in [vec3/vec3.cpp](vec3/vec3.cpp#L69-L196)). [[unlikely]] hints mark rare branches; preserve them when modifying edge checks.
- vec3 optimization notes: two reports ([vec3/OPTIMIZATION_REPORT.md](vec3/OPTIMIZATION_REPORT.md#L1-L220), [vec3/PERFORMANCE_OPTIMIZATION_REPORT.md](vec3/PERFORMANCE_OPTIMIZATION_REPORT.md#L1-L220)) capture preferred patterns—favor single `sqrt` with inline arithmetic, avoid temporary vec3 objects, prefer initialization lists, consider `constexpr/inline` for short methods, and add `length_squared`/`distance_squared` style helpers when comparing magnitudes.
- vec3 main caveat: [vec3/main.cpp](vec3/main.cpp#L1-L210) is a test/benchmark harness but currently calls non-existent APIs (`normalize()` returning value, `to_normalized()`) and uses 1e9-iteration benchmarks. Expect to fix those calls or avoid this harness when compiling the library.
- vec4 mirrors vec3 semantics (dot/angle/projection) and provides vec3 projections; keep API parity when adding features (see [vec4/vec4.cpp](vec4/vec4.cpp#L1-L170)).
- Error handling: most math functions assume valid inputs; only mat4x4 inverse throws. Validate inputs at call sites if adding public-facing code.
- Performance defaults: prefer pass-by-const-ref for vectors/matrices; avoid redundant temporaries; when adding new heavy math, consider intrinsics guarded by `#ifndef NO_SIMD` like existing mat4x4 code paths.
- Style: ASCII-only, namespace `math`, headers expose class interfaces; keep short, inline-friendly bodies in headers only when necessary. Maintain branch hints and epsilon checks already used for stability.
- If adding transforms to mat4x4, match row-major layout and extend static constructors near the commented stubs in [mat4x4/mat4x4.hpp](mat4x4/mat4x4.hpp#L39-L59) to keep API grouped.
