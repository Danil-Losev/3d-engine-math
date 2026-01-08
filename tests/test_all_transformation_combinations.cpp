#include "../mat4x4/mat4x4.hpp"
#include "../vec4/vec4.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace test {
int passed = 0;
int failed = 0;
constexpr float EPSILON = 0.001f;
constexpr float PI = 3.14159265359f;

bool approx_equal(float a, float b, float epsilon = EPSILON)
{
    return std::abs(a - b) < epsilon;
}

bool vec4_equal(const math::vec4& a, const math::vec4& b, float epsilon = EPSILON)
{
    return approx_equal(a.x(), b.x(), epsilon) && approx_equal(a.y(), b.y(), epsilon) && approx_equal(a.z(), b.z(), epsilon) && approx_equal(a.w(), b.w(), epsilon);
}

bool mat4_equal(const math::mat4x4& a, const math::mat4x4& b, float epsilon = EPSILON)
{
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (!approx_equal(a.at(r, c), b.at(r, c), epsilon)) {
                return false;
            }
        }
    }
    return true;
}

void assert_test(const std::string& name, bool condition, const std::string& msg = "")
{
    if (condition) {
        std::cout << "  + " << name << "\n";
        passed++;
    } else {
        std::cout << "  - " << name << " FAILED";
        if (!msg.empty())
            std::cout << " - " << msg;
        std::cout << "\n";
        failed++;
    }
}

void assert_vec4(const std::string& name, const math::vec4& result,
    float ex, float ey, float ez, float ew)
{
    bool ok = approx_equal(result.x(), ex) && approx_equal(result.y(), ey) && approx_equal(result.z(), ez) && approx_equal(result.w(), ew);
    if (ok) {
        std::cout << "  + " << name << "\n";
        passed++;
    } else {
        std::cout << "  - " << name << " FAILED\n";
        std::cout << "    Expected: (" << ex << ", " << ey << ", " << ez << ", " << ew << ")\n";
        std::cout << "    Got:      (" << result.x() << ", " << result.y()
                  << ", " << result.z() << ", " << result.w() << ")\n";
        failed++;
    }
}

void print_summary()
{
    std::cout << "\n"
              << std::string(60, '=') << "\n";
    std::cout << "TEST SUMMARY\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << (passed + failed) << "\n";
    if (failed == 0) {
        std::cout << "\n🎉 ALL TESTS PASSED! 🎉\n";
    } else {
        std::cout << "\n❌ " << failed << " TESTS FAILED ❌\n";
    }
    std::cout << std::string(60, '=') << "\n";
}
}

// ============================================================================
// 1. BASIC TRANSFORMATIONS
// ============================================================================

void test_translation_all_cases()
{
    std::cout << "\n[1] TRANSLATION - All Cases\n";

    // Positive translation
    math::mat4x4 t1 = math::mat4x4::translation(5.0f, 10.0f, 15.0f);
    math::vec4 v1(0, 0, 0, 1);
    test::assert_vec4("Positive translation", t1 * v1, 5, 10, 15, 1);

    // Negative translation
    math::mat4x4 t2 = math::mat4x4::translation(-5.0f, -10.0f, -15.0f);
    test::assert_vec4("Negative translation", t2 * v1, -5, -10, -15, 1);

    // Zero translation (identity-like)
    math::mat4x4 t3 = math::mat4x4::translation(0, 0, 0);
    math::vec4 v2(3, 4, 5, 1);
    test::assert_vec4("Zero translation", t3 * v2, 3, 4, 5, 1);

    // Translation doesn't affect vectors (w=0)
    math::vec4 dir(1, 1, 1, 0);
    test::assert_vec4("Translation w=0 (direction)", t1 * dir, 1, 1, 1, 0);

    // Chained translations
    math::mat4x4 ta = math::mat4x4::translation(1, 2, 3);
    math::mat4x4 tb = math::mat4x4::translation(4, 5, 6);
    math::mat4x4 tc = tb * ta;
    test::assert_vec4("Chained translations", tc * v1, 5, 7, 9, 1);
}

void test_scaling_all_cases()
{
    std::cout << "\n[2] SCALING - All Cases\n";

    math::vec4 v(2, 3, 4, 1);

    // Uniform scaling
    math::mat4x4 s1 = math::mat4x4::scaling(2, 2, 2);
    test::assert_vec4("Uniform scale", s1 * v, 4, 6, 8, 1);

    // Non-uniform scaling
    math::mat4x4 s2 = math::mat4x4::scaling(2, 3, 4);
    test::assert_vec4("Non-uniform scale", s2 * v, 4, 9, 16, 1);

    // Negative scaling (mirroring)
    math::mat4x4 s3 = math::mat4x4::scaling(-1, 1, 1);
    test::assert_vec4("Mirror X", s3 * v, -2, 3, 4, 1);

    math::mat4x4 s4 = math::mat4x4::scaling(1, -1, 1);
    test::assert_vec4("Mirror Y", s4 * v, 2, -3, 4, 1);

    math::mat4x4 s5 = math::mat4x4::scaling(1, 1, -1);
    test::assert_vec4("Mirror Z", s5 * v, 2, 3, -4, 1);

    // Zero scaling
    math::mat4x4 s6 = math::mat4x4::scaling(0, 0, 0);
    test::assert_vec4("Zero scale", s6 * v, 0, 0, 0, 1);

    // Fractional scaling
    math::mat4x4 s7 = math::mat4x4::scaling(0.5f, 0.5f, 0.5f);
    test::assert_vec4("Half scale", s7 * v, 1, 1.5f, 2, 1);

    // Chained scaling
    math::mat4x4 sa = math::mat4x4::scaling(2, 2, 2);
    math::mat4x4 sb = math::mat4x4::scaling(3, 3, 3);
    math::mat4x4 sc = sb * sa;
    math::vec4 vunit(1, 1, 1, 1);
    test::assert_vec4("Chained scales (2x then 3x = 6x)", sc * vunit, 6, 6, 6, 1);
}

void test_rotation_x_all_cases()
{
    std::cout << "\n[3] ROTATION X - All Cases\n";

    // 90 degrees
    math::mat4x4 rx90 = math::mat4x4::rotation_x(test::PI / 2);
    test::assert_vec4("Rx(90`) Y->Z", rx90 * math::vec4(0, 1, 0, 1), 0, 0, 1, 1);
    test::assert_vec4("Rx(90`) Z->Y", rx90 * math::vec4(0, 0, 1, 1), 0, -1, 0, 1);
    test::assert_vec4("Rx(90`) X unchanged", rx90 * math::vec4(1, 0, 0, 1), 1, 0, 0, 1);

    // 180 degrees
    math::mat4x4 rx180 = math::mat4x4::rotation_x(test::PI);
    test::assert_vec4("Rx(180`) Y->Y", rx180 * math::vec4(0, 1, 0, 1), 0, -1, 0, 1);
    test::assert_vec4("Rx(180`) Z->Z", rx180 * math::vec4(0, 0, 1, 1), 0, 0, -1, 1);

    // 270 degrees
    math::mat4x4 rx270 = math::mat4x4::rotation_x(3 * test::PI / 2);
    test::assert_vec4("Rx(270`) Y->Z", rx270 * math::vec4(0, 1, 0, 1), 0, 0, -1, 1);

    // 360 degrees (full rotation)
    math::mat4x4 rx360 = math::mat4x4::rotation_x(2 * test::PI);
    test::assert_vec4("Rx(360`) identity", rx360 * math::vec4(1, 2, 3, 1), 1, 2, 3, 1);

    // Negative angle
    math::mat4x4 rx_neg90 = math::mat4x4::rotation_x(-test::PI / 2);
    test::assert_vec4("Rx(-90`) Y->Z", rx_neg90 * math::vec4(0, 1, 0, 1), 0, 0, -1, 1);
}

void test_rotation_y_all_cases()
{
    std::cout << "\n[4] ROTATION Y - All Cases\n";

    // 90 degrees
    math::mat4x4 ry90 = math::mat4x4::rotation_y(test::PI / 2);
    test::assert_vec4("Ry(90`) X->Z", ry90 * math::vec4(1, 0, 0, 1), 0, 0, -1, 1);
    test::assert_vec4("Ry(90`) Z->X", ry90 * math::vec4(0, 0, 1, 1), 1, 0, 0, 1);
    test::assert_vec4("Ry(90`) Y unchanged", ry90 * math::vec4(0, 1, 0, 1), 0, 1, 0, 1);

    // 180 degrees
    math::mat4x4 ry180 = math::mat4x4::rotation_y(test::PI);
    test::assert_vec4("Ry(180`) X->X", ry180 * math::vec4(1, 0, 0, 1), -1, 0, 0, 1);
    test::assert_vec4("Ry(180`) Z->Z", ry180 * math::vec4(0, 0, 1, 1), 0, 0, -1, 1);

    // 270 degrees
    math::mat4x4 ry270 = math::mat4x4::rotation_y(3 * test::PI / 2);
    test::assert_vec4("Ry(270`) X->Z", ry270 * math::vec4(1, 0, 0, 1), 0, 0, 1, 1);

    // 360 degrees
    math::mat4x4 ry360 = math::mat4x4::rotation_y(2 * test::PI);
    test::assert_vec4("Ry(360`) identity", ry360 * math::vec4(1, 2, 3, 1), 1, 2, 3, 1);

    // Negative angle
    math::mat4x4 ry_neg90 = math::mat4x4::rotation_y(-test::PI / 2);
    test::assert_vec4("Ry(-90`) X->Z", ry_neg90 * math::vec4(1, 0, 0, 1), 0, 0, 1, 1);
}

void test_rotation_z_all_cases()
{
    std::cout << "\n[5] ROTATION Z - All Cases\n";

    // 90 degrees
    math::mat4x4 rz90 = math::mat4x4::rotation_z(test::PI / 2);
    test::assert_vec4("Rz(90`) X->Y", rz90 * math::vec4(1, 0, 0, 1), 0, 1, 0, 1);
    test::assert_vec4("Rz(90`) Y->X", rz90 * math::vec4(0, 1, 0, 1), -1, 0, 0, 1);
    test::assert_vec4("Rz(90`) Z unchanged", rz90 * math::vec4(0, 0, 1, 1), 0, 0, 1, 1);

    // 180 degrees
    math::mat4x4 rz180 = math::mat4x4::rotation_z(test::PI);
    test::assert_vec4("Rz(180`) X->X", rz180 * math::vec4(1, 0, 0, 1), -1, 0, 0, 1);
    test::assert_vec4("Rz(180`) Y->Y", rz180 * math::vec4(0, 1, 0, 1), 0, -1, 0, 1);

    // 270 degrees
    math::mat4x4 rz270 = math::mat4x4::rotation_z(3 * test::PI / 2);
    test::assert_vec4("Rz(270`) X->Y", rz270 * math::vec4(1, 0, 0, 1), 0, -1, 0, 1);

    // 360 degrees
    math::mat4x4 rz360 = math::mat4x4::rotation_z(2 * test::PI);
    test::assert_vec4("Rz(360`) identity", rz360 * math::vec4(1, 2, 3, 1), 1, 2, 3, 1);

    // Negative angle
    math::mat4x4 rz_neg90 = math::mat4x4::rotation_z(-test::PI / 2);
    test::assert_vec4("Rz(-90`) X->Y", rz_neg90 * math::vec4(1, 0, 0, 1), 0, -1, 0, 1);
}

// ============================================================================
// 2. ROTATION AXIS COMBINATIONS (all 7 non-trivial combinations)
// ============================================================================

void test_rotation_axis_angle_combinations()
{
    std::cout << "\n[6] ROTATION AXIS-ANGLE - All Combinations\n";

    float angle = test::PI / 2;

    // Single axis (X, Y, Z)
    math::mat4x4 r_x = math::mat4x4::rotation_axis_angle_intrinsic({ angle }, std::nullopt, std::nullopt);
    test::assert_vec4("Axis-angle X only", r_x * math::vec4(0, 1, 0, 1), 0, 0, 1, 1);

    math::mat4x4 r_y = math::mat4x4::rotation_axis_angle_intrinsic(std::nullopt, { angle }, std::nullopt);
    test::assert_vec4("Axis-angle Y only", r_y * math::vec4(1, 0, 0, 1), 0, 0, -1, 1);

    math::mat4x4 r_z = math::mat4x4::rotation_axis_angle_intrinsic(std::nullopt, std::nullopt, { angle });
    test::assert_vec4("Axis-angle Z only", r_z * math::vec4(1, 0, 0, 1), 0, 1, 0, 1);

    // Double axis combinations (XY, XZ, YZ)
    math::mat4x4 r_xy = math::mat4x4::rotation_axis_angle_intrinsic({ angle }, { angle }, std::nullopt);
    math::vec4 v_xy(1, 1, 0, 1);
    math::vec4 result_xy = r_xy * v_xy;
    test::assert_test("Axis-angle XY", result_xy.w() == 1.0f);

    math::mat4x4 r_xz = math::mat4x4::rotation_axis_angle_intrinsic({ angle }, std::nullopt, { angle });
    math::vec4 result_xz = r_xz * v_xy;
    test::assert_test("Axis-angle XZ", result_xz.w() == 1.0f);

    math::mat4x4 r_yz = math::mat4x4::rotation_axis_angle_intrinsic(std::nullopt, { angle }, { angle });
    math::vec4 result_yz = r_yz * v_xy;
    test::assert_test("Axis-angle YZ", result_yz.w() == 1.0f);

    // Triple axis (XYZ)
    math::mat4x4 r_xyz = math::mat4x4::rotation_axis_angle_intrinsic({ angle }, { angle }, { angle });
    math::vec4 result_xyz = r_xyz * math::vec4(1, 1, 1, 1);
    test::assert_test("Axis-angle XYZ", result_xyz.w() == 1.0f);

    // None (should be identity)
    math::mat4x4 r_none = math::mat4x4::rotation_axis_angle_intrinsic(std::nullopt, std::nullopt, std::nullopt);
    test::assert_vec4("Axis-angle none (identity)", r_none * math::vec4(5, 7, 9, 1), 5, 7, 9, 1);
}

// ============================================================================
// 3. TWO-TRANSFORM COMBINATIONS (T, R, S)
// ============================================================================

void test_two_transform_combinations()
{
    std::cout << "\n[7] TWO-TRANSFORM COMBINATIONS\n";

    math::vec4 v(1, 0, 0, 1);
    float angle = test::PI / 2;

    // T * S
    math::mat4x4 t = math::mat4x4::translation(5, 0, 0);
    math::mat4x4 s = math::mat4x4::scaling(2, 2, 2);
    test::assert_vec4("T*S: scale then translate", (t * s) * v, 7, 0, 0, 1);
    test::assert_vec4("S*T: translate then scale", (s * t) * v, 12, 0, 0, 1);

    // T * Rx
    math::mat4x4 rx = math::mat4x4::rotation_x(angle);
    math::vec4 v2(0, 1, 0, 1);
    test::assert_vec4("T*Rx", (t * rx) * v2, 5, 0, 1, 1);
    test::assert_vec4("Rx*T", (rx * t) * v2, 0, 0, 1, 1);

    // T * Ry
    math::mat4x4 ry = math::mat4x4::rotation_y(angle);
    test::assert_vec4("T*Ry", (t * ry) * v, 5, 0, -1, 1);
    test::assert_vec4("Ry*T", (ry * t) * v, 0, 0, -6, 1);

    // T * Rz
    math::mat4x4 rz = math::mat4x4::rotation_z(angle);
    test::assert_vec4("T*Rz", (t * rz) * v, 5, 1, 0, 1);
    test::assert_vec4("Rz*T", (rz * t) * v, -1, 6, 0, 1);

    // S * Rx
    test::assert_vec4("S*Rx", (s * rx) * v2, 0, 0, 2, 1);
    test::assert_vec4("Rx*S", (rx * s) * v2, 0, 0, 2, 1);

    // S * Ry
    test::assert_vec4("S*Ry", (s * ry) * v, 0, 0, -2, 1);
    test::assert_vec4("Ry*S", (ry * s) * v, 0, 0, -2, 1);

    // S * Rz
    test::assert_vec4("S*Rz", (s * rz) * v, 0, 2, 0, 1);
    test::assert_vec4("Rz*S", (rz * s) * v, 0, 2, 0, 1);

    // Rx * Ry
    test::assert_vec4("Rx*Ry", (rx * ry) * v, 0, 0, -1, 1);
    test::assert_vec4("Ry*Rx", (ry * rx) * v, 0, 1, -1, 1);

    // Rx * Rz
    test::assert_vec4("Rx*Rz", (rx * rz) * v, 0, 1, 0, 1);
    test::assert_vec4("Rz*Rx", (rz * rx) * v2, -1, 0, 1, 1);

    // Ry * Rz
    test::assert_vec4("Ry*Rz", (ry * rz) * v, 0, 1, -1, 1);
    test::assert_vec4("Rz*Ry", (rz * ry) * v, 0, 0, -1, 1);
}

// ============================================================================
// 4. THREE-TRANSFORM COMBINATIONS
// ============================================================================

void test_three_transform_combinations()
{
    std::cout << "\n[8] THREE-TRANSFORM COMBINATIONS (T*R*S)\n";

    math::vec4 v(1, 1, 1, 1);
    float angle = test::PI / 2;

    math::mat4x4 t = math::mat4x4::translation(10, 20, 30);
    math::mat4x4 s = math::mat4x4::scaling(2, 3, 4);
    math::mat4x4 rx = math::mat4x4::rotation_x(angle);
    math::mat4x4 ry = math::mat4x4::rotation_y(angle);
    math::mat4x4 rz = math::mat4x4::rotation_z(angle);

    // All 6 permutations of T, Rx, S
    test::assert_vec4("T*Rx*S", (t * rx * s) * v, 12, 20, 33, 1);
    test::assert_vec4("T*S*Rx", (t * s * rx) * v, 12, 17, 34, 1);
    test::assert_vec4("Rx*T*S", (rx * t * s) * v, 2, -30, 23, 1);
    test::assert_vec4("Rx*S*T", (rx * s * t) * v, 12, -30, 23, 1);
    test::assert_vec4("S*T*Rx", (s * t * rx) * v, 22, -30, 63, 1);
    test::assert_vec4("S*Rx*T", (s * rx * t) * v, 2, -30, 23, 1);

    // Standard model matrix: T * R * S
    math::mat4x4 model_x = t * rx * s;
    test::assert_vec4("Model T*Rx*S", model_x * v, 12, 20, 33, 1);

    math::mat4x4 model_y = t * ry * s;
    test::assert_vec4("Model T*Ry*S", model_y * v, 14, 23, 28, 1);

    math::mat4x4 model_z = t * rz * s;
    test::assert_vec4("Model T*Rz*S", model_z * v, 7, 22, 34, 1);

    // All three rotations
    math::mat4x4 r_all = rz * ry * rx;
    test::assert_vec4("Rz*Ry*Rx", r_all * v, 1, -1, -1, 1);

    // Full transform with all rotations
    math::mat4x4 full = t * rz * ry * rx * s;
    test::assert_vec4("T*Rz*Ry*Rx*S", full * v, 12, 17, 26, 1);
}

// ============================================================================
// 5. MAKE_MODEL_MATRIX FUNCTION
// ============================================================================

void test_make_model_matrix()
{
    std::cout << "\n[9] MAKE_MODEL_MATRIX Function\n";

    float angle = test::PI / 4; // 45 degrees
    math::vec4 v(1, 0, 0, 1);

    // Simple T*R*S
    math::mat4x4 t = math::mat4x4::translation(5, 10, 15);
    math::mat4x4 r = math::mat4x4::rotation_z(test::PI / 2);
    math::mat4x4 s = math::mat4x4::scaling(2, 2, 2);

    math::mat4x4 model = math::mat4x4::make_model_matrix(t, r, s);
    math::mat4x4 manual = t * r * s;

    test::assert_test("make_model_matrix matches T*R*S", test::mat4_equal(model, manual));

    // With complex rotation
    math::mat4x4 r_complex = math::mat4x4::rotation_axis_angle_intrinsic({ angle }, { angle }, { angle });
    math::mat4x4 model2 = math::mat4x4::make_model_matrix(t, r_complex, s);
    math::mat4x4 manual2 = t * r_complex * s;

    test::assert_test("make_model_matrix with axis-angle", test::mat4_equal(model2, manual2));

    // Identity components
    math::mat4x4 id = math::mat4x4::identity();
    math::mat4x4 model_id = math::mat4x4::make_model_matrix(id, id, id);
    test::assert_vec4("make_model_matrix all identity", model_id * v, 1, 0, 0, 1);
}

// ============================================================================
// 6. INVERSE AND TRANSPOSE
// ============================================================================

void test_inverse_transpose()
{
    std::cout << "\n[10] INVERSE AND TRANSPOSE\n";

    float angle = test::PI / 3;

    // Translation inverse
    math::mat4x4 t = math::mat4x4::translation(5, 10, 15);
    math::mat4x4 t_inv = t.inverse();
    math::mat4x4 should_be_id = t * t_inv;
    math::vec4 v(2, 3, 4, 1);
    math::vec4 result = should_be_id * v;
    test::assert_test("Translation inverse", test::vec4_equal(result, v, 0.01f));

    // Scaling inverse
    math::mat4x4 s = math::mat4x4::scaling(2, 3, 4);
    math::mat4x4 s_inv = s.inverse();
    result = (s * s_inv) * v;
    test::assert_test("Scaling inverse", test::vec4_equal(result, v, 0.01f));

    // Rotation inverse
    math::mat4x4 rx = math::mat4x4::rotation_x(angle);
    math::mat4x4 rx_inv = rx.inverse();
    result = (rx * rx_inv) * v;
    test::assert_test("Rotation X inverse", test::vec4_equal(result, v, 0.01f));

    math::mat4x4 ry = math::mat4x4::rotation_y(angle);
    math::mat4x4 ry_inv = ry.inverse();
    result = (ry * ry_inv) * v;
    test::assert_test("Rotation Y inverse", test::vec4_equal(result, v, 0.01f));

    math::mat4x4 rz = math::mat4x4::rotation_z(angle);
    math::mat4x4 rz_inv = rz.inverse();
    result = (rz * rz_inv) * v;
    test::assert_test("Rotation Z inverse", test::vec4_equal(result, v, 0.01f));

    // Model matrix inverse
    math::mat4x4 model = math::mat4x4::make_model_matrix(t, rx, s);
    math::mat4x4 model_inv = model.inverse();
    result = (model * model_inv) * v;
    test::assert_test("Model matrix inverse", test::vec4_equal(result, v, 0.01f));

    // Transpose
    math::mat4x4 m_t = t.transpose();
    test::assert_test("Transpose preserves diagonal",
        test::approx_equal(m_t.at(0, 0), 1.0f) && test::approx_equal(m_t.at(1, 1), 1.0f) && test::approx_equal(m_t.at(2, 2), 1.0f) && test::approx_equal(m_t.at(3, 3), 1.0f));

    // Rotation transpose = rotation inverse
    math::mat4x4 rx_t = rx.transpose();
    test::assert_test("Rotation transpose = inverse", test::mat4_equal(rx_t, rx_inv, 0.01f));
}

// ============================================================================
// 7. EDGE CASES AND STRESS TESTS
// ============================================================================

void test_edge_cases()
{
    std::cout << "\n[11] EDGE CASES\n";

    math::vec4 v(1, 2, 3, 1);

    // Very small transformations
    math::mat4x4 t_tiny = math::mat4x4::translation(0.0001f, 0.0001f, 0.0001f);
    test::assert_test("Tiny translation", test::vec4_equal((t_tiny * v), v, 0.001f));

    math::mat4x4 s_tiny = math::mat4x4::scaling(1.0001f, 1.0001f, 1.0001f);
    test::assert_test("Tiny scale", test::vec4_equal((s_tiny * v), v, 0.01f));

    math::mat4x4 r_tiny = math::mat4x4::rotation_x(0.0001f);
    test::assert_test("Tiny rotation", test::vec4_equal((r_tiny * v), v, 0.01f));

    // Very large transformations
    math::mat4x4 t_large = math::mat4x4::translation(1000, 2000, 3000);
    test::assert_vec4("Large translation", t_large * v, 1001, 2002, 3003, 1);

    math::mat4x4 s_large = math::mat4x4::scaling(100, 100, 100);
    test::assert_vec4("Large scale", s_large * v, 100, 200, 300, 1);

    // Multiple full rotations
    math::mat4x4 r_multi = math::mat4x4::rotation_x(4 * test::PI); // 720 degrees
    test::assert_test("Multiple 360` rotations", test::vec4_equal((r_multi * v), v, 0.01f));

    // Chaining many transforms
    math::mat4x4 chain = math::mat4x4::identity();
    for (int i = 0; i < 10; ++i) {
        chain = chain * math::mat4x4::translation(1, 0, 0);
    }
    test::assert_vec4("Chain 10 translations", chain * math::vec4(0, 0, 0, 1), 10, 0, 0, 1);
}

// ============================================================================
// 8. DETERMINANT TESTS
// ============================================================================

void test_determinant()
{
    std::cout << "\n[12] DETERMINANT\n";

    // Identity
    test::assert_test("det(Identity) = 1",
        test::approx_equal(math::mat4x4::identity().determinant(), 1.0, 0.001));

    // Translation (det = 1)
    test::assert_test("det(Translation) = 1",
        test::approx_equal(math::mat4x4::translation(5, 10, 15).determinant(), 1.0, 0.001));

    // Rotation (det = 1)
    test::assert_test("det(Rotation) = 1",
        test::approx_equal(math::mat4x4::rotation_x(test::PI / 4).determinant(), 1.0, 0.001));

    // Uniform scale (det = s³)
    test::assert_test("det(Scale 2*2*2) = 8",
        test::approx_equal(math::mat4x4::scaling(2, 2, 2).determinant(), 8.0, 0.001));

    // Non-uniform scale
    test::assert_test("det(Scale 2*3*4) = 24",
        test::approx_equal(math::mat4x4::scaling(2, 3, 4).determinant(), 24.0, 0.001));

    // Zero scale (det = 0)
    test::assert_test("det(Scale 0) = 0",
        test::approx_equal(math::mat4x4::scaling(0, 1, 1).determinant(), 0.0, 0.001));

    // Negative scale (mirroring)
    test::assert_test("det(Mirror) < 0",
        math::mat4x4::scaling(-1, 1, 1).determinant() < 0);
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    std::cout << "\n";

    test_translation_all_cases();
    test_scaling_all_cases();
    test_rotation_x_all_cases();
    test_rotation_y_all_cases();
    test_rotation_z_all_cases();
    test_rotation_axis_angle_combinations();
    test_two_transform_combinations();
    test_three_transform_combinations();
    test_make_model_matrix();
    test_inverse_transpose();
    test_edge_cases();
    test_determinant();

    test::print_summary();

    return test::failed > 0 ? 1 : 0;
}
