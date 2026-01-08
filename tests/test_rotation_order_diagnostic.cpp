#include "../mat4x4/mat4x4.hpp"
#include "../vec4/vec4.hpp"
#include <iostream>
#include <cmath>

int main() {
    const float PI = 3.14159265359f;
    float angle = PI / 2; // 90 degrees
    
    std::cout << "\n=== ROTATION ORDER DIAGNOSTIC ===\n\n";
    
    // Test vector
    math::vec4 v(1, 0, 0, 1);
    
    std::cout << "Initial vector: (" << v.x() << ", " << v.y() << ", " << v.z() << ")\n\n";
    
    // Individual rotations
    math::mat4x4 rx = math::mat4x4::rotation_x(angle);
    math::mat4x4 ry = math::mat4x4::rotation_y(angle);
    math::mat4x4 rz = math::mat4x4::rotation_z(angle);
    
    std::cout << "--- Individual Rotations (90°) ---\n";
    
    math::vec4 v_rx = rx * v;
    std::cout << "Rx × (1,0,0) = (" << v_rx.x() << ", " << v_rx.y() << ", " << v_rx.z() << ")\n";
    
    math::vec4 v_ry = ry * v;
    std::cout << "Ry × (1,0,0) = (" << v_ry.x() << ", " << v_ry.y() << ", " << v_ry.z() << ")\n";
    
    math::vec4 v_rz = rz * v;
    std::cout << "Rz × (1,0,0) = (" << v_rz.x() << ", " << v_rz.y() << ", " << v_rz.z() << ")\n";
    
    std::cout << "\n--- Manual Composition Test ---\n";
    
    // Manual: Rx × Ry × Rz
    math::mat4x4 manual_xyz = rx * ry * rz;
    math::vec4 result_manual = manual_xyz * v;
    std::cout << "(Rx × Ry × Rz) × v = (" << result_manual.x() << ", " 
              << result_manual.y() << ", " << result_manual.z() << ")\n";
    std::cout << "  Order applied: Rz → Ry → Rx (right-to-left)\n";
    
    // Manual: Rz × Ry × Rx  
    math::mat4x4 manual_zyx = rz * ry * rx;
    math::vec4 result_zyx = manual_zyx * v;
    std::cout << "(Rz × Ry × Rx) × v = (" << result_zyx.x() << ", " 
              << result_zyx.y() << ", " << result_zyx.z() << ")\n";
    std::cout << "  Order applied: Rx → Ry → Rz (right-to-left)\n";
    
    std::cout << "\n--- rotation_axis_angle Test ---\n";
    
    math::mat4x4 r_all = math::mat4x4::rotation_axis_angle({angle}, {angle}, {angle});
    math::vec4 result_all = r_all * v;
    std::cout << "rotation_axis_angle(x, y, z) × v = (" << result_all.x() << ", " 
              << result_all.y() << ", " << result_all.z() << ")\n";
    
    // Check which one matches
    auto approx_equal = [](float a, float b) { return std::abs(a - b) < 0.001f; };
    auto vec_equal = [&](const math::vec4& a, const math::vec4& b) {
        return approx_equal(a.x(), b.x()) && 
               approx_equal(a.y(), b.y()) && 
               approx_equal(a.z(), b.z());
    };
    
    if (vec_equal(result_all, result_manual)) {
        std::cout << "\n✓ Matches (Rx × Ry × Rz) - Z→Y→X order\n";
    } else if (vec_equal(result_all, result_zyx)) {
        std::cout << "\n✓ Matches (Rz × Ry × Rx) - X→Y→Z order\n";
    } else {
        std::cout << "\n✗ Matches neither!\n";
    }
    
    std::cout << "\n--- Step-by-step rotation_axis_angle internals ---\n";
    std::cout << "Based on code: rotation = I × Rz × Ry × Rx\n";
    std::cout << "Multiplication order (left-to-right): I, Rz, Ry, Rx\n";
    std::cout << "Application order (right-to-left): Rx → Ry → Rz\n";
    
    // Simulate internal order
    math::mat4x4 step1 = math::mat4x4::identity() * rz;
    math::mat4x4 step2 = step1 * ry;
    math::mat4x4 step3 = step2 * rx;
    math::vec4 result_simulated = step3 * v;
    std::cout << "Simulated result: (" << result_simulated.x() << ", " 
              << result_simulated.y() << ", " << result_simulated.z() << ")\n";
    
    if (vec_equal(result_simulated, result_all)) {
        std::cout << "✓ Simulation matches rotation_axis_angle\n";
    } else {
        std::cout << "✗ Simulation does NOT match!\n";
    }
    
    std::cout << "\n--- Testing Ry × Rx combination ---\n";
    math::mat4x4 ry_rx = ry * rx;
    math::vec4 test_v(1, 0, 0, 1);
    math::vec4 result_ry_rx = ry_rx * test_v;
    std::cout << "(Ry × Rx) × (1,0,0) = (" << result_ry_rx.x() << ", " 
              << result_ry_rx.y() << ", " << result_ry_rx.z() << ")\n";
    std::cout << "Expected if Rx first: (1,0,0) stays, then Ry: (0,0,-1)\n";
    std::cout << "Expected if Ry first: (0,0,-1), then Rx: (0,1,-1)? No... let me recalculate\n";
    
    // Step by step
    math::vec4 after_rx = rx * test_v;
    std::cout << "\nStep 1 - Apply Rx: (1,0,0) → (" << after_rx.x() << ", " 
              << after_rx.y() << ", " << after_rx.z() << ")\n";
    math::vec4 after_ry = ry * after_rx;
    std::cout << "Step 2 - Apply Ry: → (" << after_ry.x() << ", " 
              << after_ry.y() << ", " << after_ry.z() << ")\n";
    
    std::cout << "\n================================\n";
    
    return 0;
}
