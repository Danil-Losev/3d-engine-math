#include "../mat4x4/mat4x4.hpp"
#include "../vec4/vec4.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

int main()
{
    using namespace math;
    std::cout << std::fixed << std::setprecision(2);
    // Тест 1: Простое вращение Z
    std::cout << "=== TEST 1: Rotation Z (90 degrees) ===\n";
    mat4x4 rz = mat4x4::rotation_z(1.5708f);
    std::cout << "Rz matrix:\n"
              << rz << "\n";

    vec4 v1(1.0f, 0.0f, 0.0f, 1.0f);
    std::cout << "Vector: (1, 0, 0, 1)\n";
    vec4 r1 = rz * v1;
    std::cout << "Result: (" << r1.x() << ", " << r1.y() << ", " << r1.z() << ", " << r1.w() << ")\n";
    std::cout << "Expected: (0, 1, 0, 1)\n\n";

    // Тест 2: Пошаговые вращения
    std::cout << "=== TEST 2: Step-by-step rotations ===\n";
    mat4x4 rx = mat4x4::rotation_x(1.5708f);
    std::cout << "Rx matrix:\n"
              << rx << "\n";

    mat4x4 ry = mat4x4::rotation_y(1.5708f);
    std::cout << "Ry matrix:\n"
              << ry << "\n";

    // Применим к (2, 3, 1, 1)
    vec4 v2(2.0f, 3.0f, 1.0f, 1.0f);
    std::cout << "Original vector: (2, 3, 1, 1)\n\n";

    vec4 after_rz = rz * v2;
    std::cout << "After Rz: (" << after_rz.x() << ", " << after_rz.y() << ", " << after_rz.z() << ", " << after_rz.w() << ")\n";

    vec4 after_ry = ry * after_rz;
    std::cout << "After Ry: (" << after_ry.x() << ", " << after_ry.y() << ", " << after_ry.z() << ", " << after_ry.w() << ")\n";

    vec4 after_rx = rx * after_ry;
    std::cout << "After Rx: (" << after_rx.x() << ", " << after_rx.y() << ", " << after_rx.z() << ", " << after_rx.w() << ")\n\n";

    // Тест 3: Композитная матрица
    std::cout << "=== TEST 3: Composite matrix ===\n";
    mat4x4 composite = rx * ry * rz;
    std::cout << "R = Rx × Ry × Rz:\n"
              << composite << "\n";

    vec4 result_composite = composite * v2;
    std::cout << "R × (2,3,1,1) = (" << result_composite.x() << ", " << result_composite.y()
              << ", " << result_composite.z() << ", " << result_composite.w() << ")\n\n";

    // Тест 4: Проверка умножения матрицы на вектор вручную
    std::cout << "=== TEST 4: Manual calculation ===\n";
    std::cout << "Let's verify Rz × (1, 0, 0, 1) manually:\n";
    std::cout << "row 0: " << rz.at(0, 0) << "*1 + " << rz.at(0, 1) << "*0 + " << rz.at(0, 2) << "*0 + " << rz.at(0, 3) << "*1 = ";
    float manual_x = rz.at(0, 0) * 1 + rz.at(0, 1) * 0 + rz.at(0, 2) * 0 + rz.at(0, 3) * 1;
    std::cout << manual_x << "\n";

    std::cout << "row 1: " << rz.at(1, 0) << "*1 + " << rz.at(1, 1) << "*0 + " << rz.at(1, 2) << "*0 + " << rz.at(1, 3) << "*1 = ";
    float manual_y = rz.at(1, 0) * 1 + rz.at(1, 1) * 0 + rz.at(1, 2) * 0 + rz.at(1, 3) * 1;
    std::cout << manual_y << "\n";

    return 0;
}
