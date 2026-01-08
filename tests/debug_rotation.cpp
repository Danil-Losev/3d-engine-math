#include "../mat4x4/mat4x4.hpp"
#include "../vec4/vec4.hpp"
#include <iostream>
#include <cmath>

int main() {
    using namespace math;
    
    // Простой тест: вращение вокруг Z на 90 градусов
    vec4 v(1.0f, 0.0f, 0.0f, 1.0f);
    mat4x4 rz = mat4x4::rotation_z(1.5708f);
    vec4 result = rz * v;
    
    std::cout << "Test Rz(90°) × (1,0,0,1):\n";
    std::cout << "Expected: (0, 1, 0, 1)\n";
    std::cout << "Got: (" << result.x() << ", " << result.y() << ", " << result.z() << ", " << result.w() << ")\n\n";
    
    // Тест композиции вручную
    mat4x4 rx = mat4x4::rotation_x(1.5708f);
    mat4x4 ry = mat4x4::rotation_y(1.5708f);
    
    mat4x4 r_manual = rx * ry * rz;
    vec4 v2(2.0f, 3.0f, 1.0f, 1.0f);
    vec4 result_manual = r_manual * v2;
    
    std::cout << "Manual R = Rx × Ry × Rz:\n";
    std::cout << "Result: (" << result_manual.x() << ", " << result_manual.y() << ", " << result_manual.z() << ", " << result_manual.w() << ")\n\n";
    
    // Тест через rotation_axis_angle
    mat4x4 r_auto = mat4x4::rotation_axis_angle({1.5708f}, {1.5708f}, {1.5708f});
    vec4 result_auto = r_auto * v2;
    
    std::cout << "rotation_axis_angle({90}, {90}, {90}):\n";
    std::cout << "Result: (" << result_auto.x() << ", " << result_auto.y() << ", " << result_auto.z() << ", " << result_auto.w() << ")\n\n";
    
    std::cout << "Are they equal? " << (abs(result_manual.x() - result_auto.x()) < 0.001f ? "YES" : "NO") << "\n";
    
    return 0;
}
