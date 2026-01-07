#include "../mat4x4/mat4x4.hpp"
#include <cassert>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

void test_addition()
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 16, 15, 14, 13 }, { 12, 11, 10, 9 }, { 8, 7, 6, 5 }, { 4, 3, 2, 1 } } };
    math::mat4x4 c = a + b;
    assert(c.at(0, 0) == 17.0f);
    assert(c.at(3, 3) == 17.0f);
    std::cout << " Addition test passed\n";
}

void test_subtraction()
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 16, 15, 14, 13 }, { 12, 11, 10, 9 }, { 8, 7, 6, 5 }, { 4, 3, 2, 1 } } };
    math::mat4x4 c = a - b;
    assert(c.at(0, 0) == -15.0f);
    assert(c.at(3, 3) == 15.0f);
    std::cout << " Subtraction test passed\n";
}

void test_multiplication()
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } };
    math::mat4x4 c = a * b;
    assert(std::abs(c.at(0, 0) - 1.0f) < 1e-6f);
    assert(std::abs(c.at(3, 3) - 16.0f) < 1e-6f);
    std::cout << " Multiplication test passed\n";
}

void test_add_assign()
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } } };
    a += b;
    assert(a.at(0, 0) == 2.0f);
    assert(a.at(3, 3) == 17.0f);
    std::cout << " Add-assign test passed\n";
}

void test_subtract_assign()
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } } };
    a -= b;
    assert(a.at(0, 0) == 0.0f);
    assert(a.at(3, 3) == 15.0f);
    std::cout << " Subtract-assign test passed\n";
}

void test_multiply_assign()
{
    math::mat4x4 a = { { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } };
    math::mat4x4 b = { { { 2, 0, 0, 0 }, { 0, 2, 0, 0 }, { 0, 0, 2, 0 }, { 0, 0, 0, 2 } } };
    a *= b;
    assert(std::abs(a.at(0, 0) - 2.0f) < 1e-6f);
    assert(std::abs(a.at(3, 3) - 2.0f) < 1e-6f);
    std::cout << " Multiply-assign test passed\n";
}

void test_scalar_addition()
{
    math::mat4x4 a = { { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } } };
    math::mat4x4 c = a + 2.0f;
    assert(c.at(0, 0) == 3.0f);
    assert(c.at(3, 3) == 3.0f);
    std::cout << " Scalar addition test passed\n";
}

void test_scalar_subtraction()
{
    math::mat4x4 a = { { { 5, 5, 5, 5 }, { 5, 5, 5, 5 }, { 5, 5, 5, 5 }, { 5, 5, 5, 5 } } };
    math::mat4x4 c = a - 2.0f;
    assert(c.at(0, 0) == 3.0f);
    assert(c.at(3, 3) == 3.0f);
    std::cout << " Scalar subtraction test passed\n";
}

void test_scalar_multiplication()
{
    math::mat4x4 a = { { { 2, 2, 2, 2 }, { 2, 2, 2, 2 }, { 2, 2, 2, 2 }, { 2, 2, 2, 2 } } };
    math::mat4x4 c = a * 3.0f;
    assert(c.at(0, 0) == 6.0f);
    assert(c.at(3, 3) == 6.0f);
    std::cout << " Scalar multiplication test passed\n";
}

void test_scalar_assign_add()
{
    math::mat4x4 a = { { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } } };
    a += 2.0f;
    assert(a.at(0, 0) == 3.0f);
    std::cout << " Scalar add-assign test passed\n";
}

void test_scalar_assign_subtract()
{
    math::mat4x4 a = { { { 5, 5, 5, 5 }, { 5, 5, 5, 5 }, { 5, 5, 5, 5 }, { 5, 5, 5, 5 } } };
    a -= 2.0f;
    assert(a.at(0, 0) == 3.0f);
    std::cout << " Scalar subtract-assign test passed\n";
}

void test_scalar_assign_multiply()
{
    math::mat4x4 a = { { { 2, 2, 2, 2 }, { 2, 2, 2, 2 }, { 2, 2, 2, 2 }, { 2, 2, 2, 2 } } };
    a *= 3.0f;
    assert(a.at(0, 0) == 6.0f);
    std::cout << " Scalar multiply-assign test passed\n";
}

void test_determinant()
{
    math::mat4x4 a = { { { 1, 1, 1, 1 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } };
    double det = a.determinant();
    assert(std::abs(det - 1.0) < 1e-6);
    std::cout << " Determinant test passed\n";
}

void test_transpose()
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 t = a.transpose();
    assert(t.at(0, 1) == 5.0f);
    assert(t.at(2, 3) == 15.0f);

    std::cout << " Transpose test passed\n";
}

void test_inverse()
{
    math::mat4x4 a = { { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } };
    math::mat4x4 inv = a.inverse();
    assert(std::abs(inv.at(0, 0) - 1.0f) < 1e-6f);
    assert(std::abs(inv.at(3, 3) - 1.0f) < 1e-6f);
    std::cout << " Inverse test passed\n";
}

void test_identity()
{
    math::mat4x4 a = math::mat4x4::identity();
    assert(a.at(0, 0) == 1.0f);
    assert(a.at(1, 1) == 1.0f);
    assert(a.at(0, 1) == 0.0f);
    std::cout << " Identity test passed\n";
}

void test_zero()
{
    math::mat4x4 a = math::mat4x4::zero();
    assert(a.at(0, 0) == 0.0f);
    assert(a.at(3, 3) == 0.0f);
    std::cout << " Zero test passed\n";
}

void benchmark_addition(size_t iterations)
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 16, 15, 14, 13 }, { 12, 11, 10, 9 }, { 8, 7, 6, 5 }, { 4, 3, 2, 1 } } };
    math::mat4x4 c;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        c = a + b;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long double> duration = end - start;
    std::cout << "Addition benchmark (" << iterations << " iterations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(20) << duration.count()
              << " seconds\n";
    std::cout << "  Average time per operation: " << (duration.count() / iterations)
              << " seconds\n";
}

void benchmark_subtraction(size_t iterations)
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 16, 15, 14, 13 }, { 12, 11, 10, 9 }, { 8, 7, 6, 5 }, { 4, 3, 2, 1 } } };
    math::mat4x4 c;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        c = a - b;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long double> duration = end - start;
    std::cout << "Subtraction benchmark (" << iterations << " iterations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(20) << duration.count()
              << " seconds\n";
    std::cout << "  Average time per operation: " << (duration.count() / iterations)
              << " seconds\n";
}

void benchmark_multiplication(size_t iterations)
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 b = { { { 16, 15, 14, 13 }, { 12, 11, 10, 9 }, { 8, 7, 6, 5 }, { 4, 3, 2, 1 } } };
    math::mat4x4 c;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        c = a * b;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long double> duration = end - start;
    std::cout << "Multiplication benchmark (" << iterations << " iterations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(20) << duration.count()
              << " seconds\n";
    std::cout << "  Average time per operation: " << (duration.count() / iterations)
              << " seconds\n";
}

void benchmark_scalar_addition(size_t iterations)
{
    math::mat4x4 a = { { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } } };
    math::mat4x4 c;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        c = a + 2.0f;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long double> duration = end - start;
    std::cout << "Scalar addition benchmark (" << iterations << " iterations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(20) << duration.count()
              << " seconds\n";
    std::cout << "  Average time per operation: " << (duration.count() / iterations)
              << " seconds\n";
}

void benchmark_scalar_multiplication(size_t iterations)
{
    math::mat4x4 a = { { { 2, 2, 2, 2 }, { 2, 2, 2, 2 }, { 2, 2, 2, 2 }, { 2, 2, 2, 2 } } };
    math::mat4x4 c;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        c = a * 3.0f;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long double> duration = end - start;
    std::cout << "Scalar multiplication benchmark (" << iterations << " iterations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(20) << duration.count()
              << " seconds\n";
    std::cout << "  Average time per operation: " << (duration.count() / iterations)
              << " seconds\n";
}

void benchmark_transpose(size_t iterations)
{
    math::mat4x4 a = { { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } };
    math::mat4x4 c;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        c = a.transpose();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long double> duration = end - start;
    std::cout << "Transpose benchmark (" << iterations << " iterations):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(20) << duration.count()
              << " seconds\n";
    std::cout << "  Average time per operation: " << (duration.count() / iterations)
              << " seconds\n";
}

int main(int argc, const char** argv)
{
    std::cout << "=== Running Unit Tests ===\n\n";
    test_addition();
    test_subtraction();
    test_multiplication();
    test_add_assign();
    test_subtract_assign();
    test_multiply_assign();
    test_scalar_addition();
    test_scalar_subtraction();
    test_scalar_multiplication();
    test_scalar_assign_add();
    test_scalar_assign_subtract();
    test_scalar_assign_multiply();
    test_determinant();
    test_transpose();
    test_inverse();
    test_identity();
    test_zero();

    std::cout << "\n=== Running Performance Benchmarks ===\n\n";
    const size_t iterations = 100000000;
    benchmark_addition(iterations);
    std::cout << "\n";
    benchmark_subtraction(iterations);
    std::cout << "\n";
    benchmark_multiplication(iterations);
    std::cout << "\n";
    benchmark_scalar_addition(iterations);
    std::cout << "\n";
    benchmark_scalar_multiplication(iterations);
    std::cout << "\n";
    benchmark_transpose(iterations);

    std::cout << "\n✓ All tests passed!\n";
    return 0;
}