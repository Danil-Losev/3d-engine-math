#include "../mat4x4/mat4x4.hpp"
#include "../vec4/vec4.hpp"
#include "../vec3/vec3.hpp"
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>

// Test utilities
namespace test {
    int passed = 0;
    int failed = 0;
    
    constexpr float EPSILON = 0.001f;
    
    bool approx_equal(float a, float b, float epsilon = EPSILON) {
        return std::abs(a - b) < epsilon;
    }
    
    bool vec4_equal(const math::vec4& a, const math::vec4& b, float epsilon = EPSILON) {
        return approx_equal(a.x(), b.x(), epsilon) &&
               approx_equal(a.y(), b.y(), epsilon) &&
               approx_equal(a.z(), b.z(), epsilon) &&
               approx_equal(a.w(), b.w(), epsilon);
    }
    
    void assert_vec4(const std::string& name, const math::vec4& result, 
                     float ex, float ey, float ez, float ew) {
        if (approx_equal(result.x(), ex) && approx_equal(result.y(), ey) &&
            approx_equal(result.z(), ez) && approx_equal(result.w(), ew)) {
            std::cout << "+ " << name << " PASSED\n";
            passed++;
        } else {
            std::cout << "- " << name << " FAILED\n";
            std::cout << "  Expected: (" << ex << ", " << ey << ", " << ez << ", " << ew << ")\n";
            std::cout << "  Got:      (" << result.x() << ", " << result.y() 
                      << ", " << result.z() << ", " << result.w() << ")\n";
            failed++;
        }
    }
    
    void print_summary() {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "TEST SUMMARY\n";
        std::cout << std::string(50, '=') << "\n";
        std::cout << "Passed: " << passed << "\n";
        std::cout << "Failed: " << failed << "\n";
        std::cout << "Total:  " << (passed + failed) << "\n";
        if (failed == 0) {
            std::cout << "\n🎉 ALL TESTS PASSED! 🎉\n";
        } else {
            std::cout << "\n❌ SOME TESTS FAILED ❌\n";
        }
        std::cout << std::string(50, '=') << "\n";
    }
}

// Test identity matrix
void test_identity() {
    std::cout << "\n=== Testing Identity Matrix ===\n";
    
    math::mat4x4 identity = math::mat4x4::identity();
    math::vec4 v(5.0f, 10.0f, 15.0f, 1.0f);
    math::vec4 result = identity * v;
    
    test::assert_vec4("Identity × vector", result, 5.0f, 10.0f, 15.0f, 1.0f);
}

// Test translation
void test_translation() {
    std::cout << "\n=== Testing Translation ===\n";
    
    math::mat4x4 t = math::mat4x4::translation(5.0f, 10.0f, 15.0f);
    
    // Test 1: Translate origin
    math::vec4 v1(0.0f, 0.0f, 0.0f, 1.0f);
    math::vec4 r1 = t * v1;
    test::assert_vec4("Translate origin", r1, 5.0f, 10.0f, 15.0f, 1.0f);
    
    // Test 2: Translate point
    math::vec4 v2(1.0f, 2.0f, 3.0f, 1.0f);
    math::vec4 r2 = t * v2;
    test::assert_vec4("Translate (1,2,3)", r2, 6.0f, 12.0f, 18.0f, 1.0f);
    
    // Test 3: Vector (w=0) should not translate
    math::vec4 v3(1.0f, 2.0f, 3.0f, 0.0f);
    math::vec4 r3 = t * v3;
    test::assert_vec4("Translate vector (w=0)", r3, 1.0f, 2.0f, 3.0f, 0.0f);
}

// Test scaling
void test_scaling() {
    std::cout << "\n=== Testing Scaling ===\n";
    
    math::mat4x4 s = math::mat4x4::scaling(2.0f, 3.0f, 4.0f);
    
    // Test 1: Scale uniform point
    math::vec4 v1(1.0f, 1.0f, 1.0f, 1.0f);
    math::vec4 r1 = s * v1;
    test::assert_vec4("Scale (1,1,1)", r1, 2.0f, 3.0f, 4.0f, 1.0f);
    
    // Test 2: Scale arbitrary point
    math::vec4 v2(2.0f, 3.0f, 4.0f, 1.0f);
    math::vec4 r2 = s * v2;
    test::assert_vec4("Scale (2,3,4)", r2, 4.0f, 9.0f, 16.0f, 1.0f);
    
    // Test 3: Uniform scaling
    math::mat4x4 s_uniform = math::mat4x4::scaling(2.0f, 2.0f, 2.0f);
    math::vec4 v3(1.0f, 2.0f, 3.0f, 1.0f);
    math::vec4 r3 = s_uniform * v3;
    test::assert_vec4("Uniform scale 2x", r3, 2.0f, 4.0f, 6.0f, 1.0f);
}

// Test rotation X
void test_rotation_x() {
    std::cout << "\n=== Testing Rotation X ===\n";
    
    // 90 degrees
    math::mat4x4 rx90 = math::mat4x4::rotation_x(1.5708f);
    
    // Test 1: Rotate (0,1,0) 90° around X → (0,0,1)
    math::vec4 v1(0.0f, 1.0f, 0.0f, 1.0f);
    math::vec4 r1 = rx90 * v1;
    test::assert_vec4("Rx(90°) × (0,1,0)", r1, 0.0f, 0.0f, 1.0f, 1.0f);
    
    // Test 2: Rotate (0,0,1) 90° around X → (0,-1,0)
    math::vec4 v2(0.0f, 0.0f, 1.0f, 1.0f);
    math::vec4 r2 = rx90 * v2;
    test::assert_vec4("Rx(90°) × (0,0,1)", r2, 0.0f, -1.0f, 0.0f, 1.0f);
    
    // Test 3: X-axis unchanged
    math::vec4 v3(1.0f, 0.0f, 0.0f, 1.0f);
    math::vec4 r3 = rx90 * v3;
    test::assert_vec4("Rx(90°) × (1,0,0)", r3, 1.0f, 0.0f, 0.0f, 1.0f);
}

// Test rotation Y
void test_rotation_y() {
    std::cout << "\n=== Testing Rotation Y ===\n";
    
    // 90 degrees
    math::mat4x4 ry90 = math::mat4x4::rotation_y(1.5708f);
    
    // Test 1: Rotate (1,0,0) 90° around Y → (0,0,-1)
    math::vec4 v1(1.0f, 0.0f, 0.0f, 1.0f);
    math::vec4 r1 = ry90 * v1;
    test::assert_vec4("Ry(90°) × (1,0,0)", r1, 0.0f, 0.0f, -1.0f, 1.0f);
    
    // Test 2: Rotate (0,0,1) 90° around Y → (1,0,0)
    math::vec4 v2(0.0f, 0.0f, 1.0f, 1.0f);
    math::vec4 r2 = ry90 * v2;
    test::assert_vec4("Ry(90°) × (0,0,1)", r2, 1.0f, 0.0f, 0.0f, 1.0f);
    
    // Test 3: Y-axis unchanged
    math::vec4 v3(0.0f, 1.0f, 0.0f, 1.0f);
    math::vec4 r3 = ry90 * v3;
    test::assert_vec4("Ry(90°) × (0,1,0)", r3, 0.0f, 1.0f, 0.0f, 1.0f);
}

// Test rotation Z
void test_rotation_z() {
    std::cout << "\n=== Testing Rotation Z ===\n";
    
    // 90 degrees
    math::mat4x4 rz90 = math::mat4x4::rotation_z(1.5708f);
    
    // Test 1: Rotate (1,0,0) 90° around Z → (0,1,0)
    math::vec4 v1(1.0f, 0.0f, 0.0f, 1.0f);
    math::vec4 r1 = rz90 * v1;
    test::assert_vec4("Rz(90°) × (1,0,0)", r1, 0.0f, 1.0f, 0.0f, 1.0f);
    
    // Test 2: Rotate (0,1,0) 90° around Z → (-1,0,0)
    math::vec4 v2(0.0f, 1.0f, 0.0f, 1.0f);
    math::vec4 r2 = rz90 * v2;
    test::assert_vec4("Rz(90°) × (0,1,0)", r2, -1.0f, 0.0f, 0.0f, 1.0f);
    
    // Test 3: Z-axis unchanged
    math::vec4 v3(0.0f, 0.0f, 1.0f, 1.0f);
    math::vec4 r3 = rz90 * v3;
    test::assert_vec4("Rz(90°) × (0,0,1)", r3, 0.0f, 0.0f, 1.0f, 1.0f);
}

// Test rotation combinations
void test_rotation_combinations() {
    std::cout << "\n=== Testing Rotation Combinations ===\n";
    
    float angle = 1.5708f; // 90 degrees
    
    // Test 1: Manual composition vs rotation_axis_angle
    math::mat4x4 rx = math::mat4x4::rotation_x(angle);
    math::mat4x4 ry = math::mat4x4::rotation_y(angle);
    math::mat4x4 rz = math::mat4x4::rotation_z(angle);
    
    math::mat4x4 r_manual = rz * ry * rx;
    math::mat4x4 r_auto = math::mat4x4::rotation_axis_angle({angle}, {angle}, {angle});
    
    math::vec4 v(2.0f, 3.0f, 1.0f, 1.0f);
    math::vec4 result_manual = r_manual * v;
    math::vec4 result_auto = r_auto * v;
    
    test::assert_vec4("Manual Rz×Ry×Rx", result_manual, 6.0f, 13.0f, -2.0f, 1.0f);
    test::assert_vec4("rotation_axis_angle", result_auto, 6.0f, 13.0f, -2.0f, 1.0f);
    
    // Test 2: Only X rotation
    math::mat4x4 r_only_x = math::mat4x4::rotation_axis_angle({angle}, std::nullopt, std::nullopt);
    math::vec4 v2(0.0f, 1.0f, 0.0f, 1.0f);
    math::vec4 result_only_x = r_only_x * v2;
    test::assert_vec4("Only X rotation", result_only_x, 0.0f, 0.0f, 1.0f, 1.0f);
    
    // Test 3: Only Y rotation
    math::mat4x4 r_only_y = math::mat4x4::rotation_axis_angle(std::nullopt, {angle}, std::nullopt);
    math::vec4 v3(1.0f, 0.0f, 0.0f, 1.0f);
    math::vec4 result_only_y = r_only_y * v3;
    test::assert_vec4("Only Y rotation", result_only_y, 0.0f, 0.0f, -1.0f, 1.0f);
    
    // Test 4: Only Z rotation
    math::mat4x4 r_only_z = math::mat4x4::rotation_axis_angle(std::nullopt, std::nullopt, {angle});
    math::vec4 v4(1.0f, 0.0f, 0.0f, 1.0f);
    math::vec4 result_only_z = r_only_z * v4;
    test::assert_vec4("Only Z rotation", result_only_z, 0.0f, 1.0f, 0.0f, 1.0f);
    
    // Test 5: XY rotation
    math::mat4x4 r_xy = math::mat4x4::rotation_axis_angle({angle}, {angle}, std::nullopt);
    math::vec4 v5(1.0f, 1.0f, 1.0f, 1.0f);
    math::vec4 expected_xy = ry * rx * v5;
    math::vec4 result_xy = r_xy * v5;
    if (test::vec4_equal(result_xy, expected_xy)) {
        test::passed++;
        std::cout << "+ XY rotation PASSED\n";
    } else {
        test::failed++;
        std::cout << "- XY rotation FAILED\n";
    }
    
    // Test 6: YZ rotation
    math::mat4x4 r_yz = math::mat4x4::rotation_axis_angle(std::nullopt, {angle}, {angle});
    math::vec4 v6(1.0f, 1.0f, 1.0f, 1.0f);
    math::vec4 expected_yz = rz * ry * v6;
    math::vec4 result_yz = r_yz * v6;
    if (test::vec4_equal(result_yz, expected_yz)) {
        test::passed++;
        std::cout << "+ YZ rotation PASSED\n";
    } else {
        test::failed++;
        std::cout << "- YZ rotation FAILED\n";
    }
}

// Test composite transformations
void test_composite_transformations() {
    std::cout << "\n=== Testing Composite Transformations ===\n";
    
    // Test 1: Scale then translate
    math::mat4x4 s = math::mat4x4::scaling(2.0f, 2.0f, 2.0f);
    math::mat4x4 t = math::mat4x4::translation(5.0f, 10.0f, 15.0f);
    math::mat4x4 ts = t * s;
    
    math::vec4 v1(1.0f, 1.0f, 1.0f, 1.0f);
    math::vec4 r1 = ts * v1;
    // Scale first: (1,1,1) → (2,2,2), then translate: (2,2,2) → (7,12,17)
    test::assert_vec4("Translate × Scale", r1, 7.0f, 12.0f, 17.0f, 1.0f);
    
    // Test 2: Rotate then translate
    math::mat4x4 rz = math::mat4x4::rotation_z(1.5708f);
    math::mat4x4 t2 = math::mat4x4::translation(5.0f, 0.0f, 0.0f);
    math::mat4x4 tr = t2 * rz;
    
    math::vec4 v2(1.0f, 0.0f, 0.0f, 1.0f);
    math::vec4 r2 = tr * v2;
    // Rotate: (1,0,0) → (0,1,0), then translate: (0,1,0) → (5,1,0)
    test::assert_vec4("Translate × Rotate", r2, 5.0f, 1.0f, 0.0f, 1.0f);
    
    // Test 3: Full model matrix (T × R × S)
    math::mat4x4 model = math::mat4x4::make_model_matrix(
        math::mat4x4::translation(5.0f, 10.0f, 0.0f),
        math::mat4x4::rotation_axis_angle({1.5708f}, {1.5708f}, {1.5708f}),
        math::mat4x4::scaling(2.0f, 3.0f, 1.0f)
    );
    
    math::vec4 v3(1.0f, 1.0f, 1.0f, 1.0f);
    math::vec4 r3 = model * v3;
    test::assert_vec4("Full model matrix", r3, 6.0f, 13.0f, -2.0f, 1.0f);
}

// Test matrix operations
void test_matrix_operations() {
    std::cout << "\n=== Testing Matrix Operations ===\n";
    
    // Test 1: Transpose
    math::mat4x4 m = math::mat4x4::translation(1.0f, 2.0f, 3.0f);
    math::mat4x4 mt = m.transpose();
    
    // For translation matrix, transposing moves translation to last row → last column
    bool transpose_correct = 
        test::approx_equal(mt.at(0, 0), 1.0f) &&
        test::approx_equal(mt.at(1, 1), 1.0f) &&
        test::approx_equal(mt.at(2, 2), 1.0f) &&
        test::approx_equal(mt.at(3, 3), 1.0f);
    
    if (transpose_correct) {
        test::passed++;
        std::cout << "+ Transpose PASSED\n";
    } else {
        test::failed++;
        std::cout << "- Transpose FAILED\n";
    }
    
    // Test 2: Determinant of identity
    math::mat4x4 identity = math::mat4x4::identity();
    double det = identity.determinant();
    if (test::approx_equal(det, 1.0, 0.001)) {
        test::passed++;
        std::cout << "+ Determinant of identity PASSED\n";
    } else {
        test::failed++;
        std::cout << "- Determinant of identity FAILED (got " << det << ")\n";
    }
    
    // Test 3: Inverse
    math::mat4x4 t = math::mat4x4::translation(5.0f, 10.0f, 15.0f);
    math::mat4x4 t_inv = t.inverse();
    math::mat4x4 should_be_identity = t * t_inv;
    
    math::vec4 v(1.0f, 2.0f, 3.0f, 1.0f);
    math::vec4 identity_result = should_be_identity * v;
    
    if (test::vec4_equal(identity_result, v, 0.01f)) {
        test::passed++;
        std::cout << "+ Matrix inverse PASSED\n";
    } else {
        test::failed++;
        std::cout << "- Matrix inverse FAILED\n";
    }
}

// Test edge cases
void test_edge_cases() {
    std::cout << "\n=== Testing Edge Cases ===\n";
    
    // Test 1: Zero scaling (should work but might be degenerate)
    math::mat4x4 s_zero = math::mat4x4::scaling(0.0f, 0.0f, 0.0f);
    math::vec4 v1(1.0f, 2.0f, 3.0f, 1.0f);
    math::vec4 r1 = s_zero * v1;
    test::assert_vec4("Zero scaling", r1, 0.0f, 0.0f, 0.0f, 1.0f);
    
    // Test 2: Negative scaling (mirroring)
    math::mat4x4 s_neg = math::mat4x4::scaling(-1.0f, 1.0f, 1.0f);
    math::vec4 v2(1.0f, 2.0f, 3.0f, 1.0f);
    math::vec4 r2 = s_neg * v2;
    test::assert_vec4("Negative scaling (mirror X)", r2, -1.0f, 2.0f, 3.0f, 1.0f);
    
    // Test 3: 360 degree rotation (should return to original)
    math::mat4x4 r360 = math::mat4x4::rotation_z(6.2832f); // 2π
    math::vec4 v3(1.0f, 2.0f, 3.0f, 1.0f);
    math::vec4 r3 = r360 * v3;
    test::assert_vec4("360° rotation", r3, 1.0f, 2.0f, 3.0f, 1.0f);
    
    // Test 4: Zero translation
    math::mat4x4 t_zero = math::mat4x4::translation(0.0f, 0.0f, 0.0f);
    math::vec4 v4(5.0f, 10.0f, 15.0f, 1.0f);
    math::vec4 r4 = t_zero * v4;
    test::assert_vec4("Zero translation", r4, 5.0f, 10.0f, 15.0f, 1.0f);
    
    // Test 5: Small angles
    math::mat4x4 r_small = math::mat4x4::rotation_x(0.0001f);
    math::vec4 v5(1.0f, 1.0f, 1.0f, 1.0f);
    math::vec4 r5 = r_small * v5;
    // Should be very close to original
    if (test::approx_equal(r5.x(), 1.0f, 0.01f) && 
        test::approx_equal(r5.y(), 1.0f, 0.01f) &&
        test::approx_equal(r5.z(), 1.0f, 0.01f)) {
        test::passed++;
        std::cout << "+ Small angle rotation PASSED\n";
    } else {
        test::failed++;
        std::cout << "- Small angle rotation FAILED\n";
    }
}

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║   MAT4X4 TRANSFORMATION TEST SUITE            ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    
    test_identity();
    test_translation();
    test_scaling();
    test_rotation_x();
    test_rotation_y();
    test_rotation_z();
    test_rotation_combinations();
    test_composite_transformations();
    test_matrix_operations();
    test_edge_cases();
    
    test::print_summary();
    
    return test::failed > 0 ? 1 : 0;
}
