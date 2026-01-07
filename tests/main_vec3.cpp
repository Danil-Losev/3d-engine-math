#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "../vec3/vec3.hpp"

void print_test(const char* name, bool passed)
{
    std::cout << "[" << (passed ? "PASS" : "FAIL") << "] " << name << std::endl;
}

bool approx_equal(float a, float b, float epsilon = 0.0001f)
{
    return std::abs(a - b) < epsilon;
}

bool vec_approx_equal(const math::vec3& a, const math::vec3& b, float epsilon = 0.0001f)
{
    return approx_equal(a.x(), b.x(), epsilon) && approx_equal(a.y(), b.y(), epsilon) && approx_equal(a.z(), b.z(), epsilon);
}

void run_tests()
{
    std::cout << "=== TESTING vec3 CLASS ===" << std::endl
              << std::endl;

    std::cout << "--- Constructors ---" << std::endl;
    math::vec3 v1;
    print_test("Default constructor", v1.x() == 0.0f && v1.y() == 0.0f && v1.z() == 0.0f);

    math::vec3 v2(1.0f, 2.0f, 3.0f);
    print_test("Constructor with values", v2.x() == 1.0f && v2.y() == 2.0f && v2.z() == 3.0f);

    math::vec3 v3(3.0f, 4.0f, 0.0f, true);
    print_test("Constructor with normalization", approx_equal(v3.length(), 1.0f));

    std::cout << std::endl
              << "--- Static Factory Methods ---" << std::endl;
    math::vec3 zero = math::vec3::zero();
    print_test("zero()", vec_approx_equal(zero, math::vec3(0, 0, 0)));

    math::vec3 one = math::vec3::one();
    print_test("one()", vec_approx_equal(one, math::vec3(1, 1, 1)));

    math::vec3 i = math::vec3::basis_i();
    print_test("basis_i()", vec_approx_equal(i, math::vec3(1, 0, 0)));

    math::vec3 j = math::vec3::basis_j();
    print_test("basis_j()", vec_approx_equal(j, math::vec3(0, 1, 0)));

    math::vec3 k = math::vec3::basis_k();
    print_test("basis_k()", vec_approx_equal(k, math::vec3(0, 0, 1)));

    std::cout << std::endl
              << "--- Arithmetic Operators ---" << std::endl;
    math::vec3 a(1.0f, 2.0f, 3.0f);
    math::vec3 b(4.0f, 5.0f, 6.0f);

    math::vec3 sum = a + b;
    print_test("operator+", vec_approx_equal(sum, math::vec3(5, 7, 9)));

    math::vec3 diff = b - a;
    print_test("operator-", vec_approx_equal(diff, math::vec3(3, 3, 3)));

    math::vec3 scaled = a * 2.0f;
    print_test("operator*", vec_approx_equal(scaled, math::vec3(2, 4, 6)));

    math::vec3 divided = a / 2.0f;
    print_test("operator/", vec_approx_equal(divided, math::vec3(0.5f, 1.0f, 1.5f)));

    math::vec3 c = a;
    c += b;
    print_test("operator+=", vec_approx_equal(c, math::vec3(5, 7, 9)));

    c = b;
    c -= a;
    print_test("operator-=", vec_approx_equal(c, math::vec3(3, 3, 3)));

    c = a;
    c *= 2.0f;
    print_test("operator*=", vec_approx_equal(c, math::vec3(2, 4, 6)));

    c = a;
    c /= 2.0f;
    print_test("operator/=", vec_approx_equal(c, math::vec3(0.5f, 1.0f, 1.5f)));

    std::cout << std::endl
              << "--- Vector Operations ---" << std::endl;
    float dot = a.dot_production(b);
    print_test("dot_production", approx_equal(dot, 32.0f));

    float static_dot = math::vec3::dot_production(a, b);
    print_test("static dot_production", approx_equal(static_dot, 32.0f));

    math::vec3 cross = math::vec3::cross_production(math::vec3(1, 0, 0), math::vec3(0, 1, 0));
    print_test("cross_production", vec_approx_equal(cross, math::vec3(0, 0, 1)));

    float mixed = math::vec3::mixed_production(math::vec3(1, 0, 0), math::vec3(0, 1, 0), math::vec3(0, 0, 1));
    print_test("mixed_production", approx_equal(mixed, 1.0f));

    std::cout << std::endl
              << "--- Length and Distance ---" << std::endl;
    math::vec3 v4(3.0f, 4.0f, 0.0f);
    print_test("length()", approx_equal(v4.length(), 5.0f));

    float dist = v4.distance_to(math::vec3::zero());
    print_test("distance_to()", approx_equal(dist, 5.0f));

    std::cout << std::endl
              << "--- Normalization ---" << std::endl;
    math::vec3 v5(3.0f, 4.0f, 0.0f);
    math::vec3 normalized = v5.normalized();
    print_test("normalized()", approx_equal(normalized.length(), 1.0f));

    v5.normalize();
    print_test("normalize()", approx_equal(v5.length(), 1.0f));

    std::cout << std::endl
              << "--- Angles ---" << std::endl;
    math::vec3 vx(1.0f, 0.0f, 0.0f);
    math::vec3 vy(0.0f, 1.0f, 0.0f);
    float angle = vx.angle_between(vy);
    print_test("angle_between", approx_equal(angle, 3.14159f / 2.0f));

    float static_angle = math::vec3::angle_between(vx, vy);
    print_test("static angle_between", approx_equal(static_angle, 3.14159f / 2.0f));

    float x_angle = vx.x_axis_angle();
    print_test("x_axis_angle", approx_equal(x_angle, 0.0f));

    float y_angle = vy.y_axis_angle();
    print_test("y_axis_angle", approx_equal(y_angle, 0.0f));

    math::vec3 vz(0.0f, 0.0f, 1.0f);
    float z_angle = vz.z_axis_angle();
    print_test("z_axis_angle", approx_equal(z_angle, 0.0f));

    std::cout << std::endl
              << "--- Projection and Reflection ---" << std::endl;
    math::vec3 v6(1.0f, 1.0f, 0.0f);
    math::vec3 v7(1.0f, 0.0f, 0.0f);
    math::vec3 proj = v6.project_on_vector(v7);
    print_test("project_on_vector", vec_approx_equal(proj, math::vec3(1, 0, 0)));

    math::vec3 static_proj = math::vec3::project_vector(v6, v7);
    print_test("static project_vector", vec_approx_equal(static_proj, math::vec3(1, 0, 0)));

    math::vec3 v8(1.0f, -1.0f, 0.0f);
    math::vec3 normal(0.0f, 1.0f, 0.0f);
    math::vec3 reflected = v8.reflect(normal);
    print_test("reflect", vec_approx_equal(reflected, math::vec3(1, 1, 0)));

    math::vec3 static_reflected = math::vec3::reflect_vector(v8, normal);
    print_test("static reflect_vector", vec_approx_equal(static_reflected, math::vec3(1, 1, 0)));

    std::cout << std::endl
              << "--- Vector Relations ---" << std::endl;
    math::vec3 v9(2.0f, 4.0f, 6.0f);
    math::vec3 v10(1.0f, 2.0f, 3.0f);
    float coeff;
    bool collinear = math::vec3::is_collinear(v9, v10, coeff);
    print_test("is_collinear", collinear && approx_equal(coeff, 2.0f));

    bool parallel = math::vec3::is_parallel(v9, v10);
    print_test("is_parallel", parallel);

    bool orthogonal = math::vec3::is_orthogonal(vx, vy);
    print_test("is_orthogonal", orthogonal);

    math::vec3 v11(1.0f, 0.0f, 0.0f);
    math::vec3 v12(0.0f, 1.0f, 0.0f);
    math::vec3 v13(1.0f, 1.0f, 0.0f);
    bool coplanar = math::vec3::is_coplanar(v11, v12, v13);
    print_test("is_coplanar", coplanar);

    std::cout << std::endl;
}

void run_speed_tests()
{
    std::cout << "=== SPEED TESTS ===" << std::endl
              << std::endl;

    const long long iterations = 1000000000;
    math::vec3 a(1.0f, 2.0f, 3.0f);
    math::vec3 b(4.0f, 5.0f, 6.0f);
    volatile float result = 0.0f;
    math::vec3 vec_result;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    std::chrono::duration<double, std::milli> duration;

    // Vector addition
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        vec_result = a + b;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Vector addition:      " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Vector subtraction
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        vec_result = b - a;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Vector subtraction:   " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Scalar multiplication
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        vec_result = a * 2.5f;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Scalar multiply:      " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Dot product
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        result = a.dot_production(b);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Dot product:          " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Cross product
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        vec_result = a.cross_production(b);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Cross product:        " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Length calculation
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        result = a.length();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Length:               " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Normalization
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        vec_result = a.normalized();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Normalized:            " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Distance calculation
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        result = a.distance_to(b);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Distance:             " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Projection
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        vec_result = a.project_on_vector(b);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Projection:           " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    // Reflection
    start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < iterations; i++) {
        vec_result = a.reflect(b);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Reflection:           " << std::setw(10) << duration.count() << " ms ("
              << std::setw(8) << (iterations / duration.count() / 1000.0) << " M ops/s)" << std::endl;

    std::cout << std::endl;
}

int main(int argc, const char** argv)
{
    run_tests();
    run_speed_tests();
    return 0;
}