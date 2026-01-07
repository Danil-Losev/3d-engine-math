#include "mat4x4.hpp"

#include <immintrin.h>

#include <cstring>

#define NO_SIMD

math::mat4x4::mat4x4() { std::memset(m_matrix, 0, sizeof(m_matrix)); }

math::mat4x4::mat4x4(const float (&elements)[4][4]) {
    std::memcpy(m_matrix, elements, sizeof(m_matrix));
}

math::mat4x4::mat4x4(const mat4x4 &other) {
    std::memcpy(m_matrix, other.m_matrix, sizeof(m_matrix));
}

math::mat4x4 &math::mat4x4::operator=(const mat4x4 &other) {
    if (this != &other) {
        std::memcpy(m_matrix, other.m_matrix, sizeof(m_matrix));
    }
    return *this;
}

math::mat4x4 math::mat4x4::operator+(const mat4x4 &other) const {
    mat4x4 result;
#ifndef NO_SIMD
    __m256 vec_a1 = _mm256_load_ps(&m_matrix[0][0]);
    __m256 vec_b1 = _mm256_load_ps(&other.m_matrix[0][0]);
    __m256 vec_result1 = _mm256_add_ps(vec_a1, vec_b1);
    _mm256_store_ps(&result.m_matrix[0][0], vec_result1);

    __m256 vec_a2 = _mm256_load_ps(&m_matrix[2][0]);
    __m256 vec_b2 = _mm256_load_ps(&other.m_matrix[2][0]);
    __m256 vec_result2 = _mm256_add_ps(vec_a2, vec_b2);
    _mm256_store_ps(&result.m_matrix[2][0], vec_result2);
#else
    result.m_matrix[0][0] = m_matrix[0][0] + other.m_matrix[0][0];
    result.m_matrix[0][1] = m_matrix[0][1] + other.m_matrix[0][1];
    result.m_matrix[0][2] = m_matrix[0][2] + other.m_matrix[0][2];
    result.m_matrix[0][3] = m_matrix[0][3] + other.m_matrix[0][3];

    result.m_matrix[1][0] = m_matrix[1][0] + other.m_matrix[1][0];
    result.m_matrix[1][1] = m_matrix[1][1] + other.m_matrix[1][1];
    result.m_matrix[1][2] = m_matrix[1][2] + other.m_matrix[1][2];
    result.m_matrix[1][3] = m_matrix[1][3] + other.m_matrix[1][3];

    result.m_matrix[2][0] = m_matrix[2][0] + other.m_matrix[2][0];
    result.m_matrix[2][1] = m_matrix[2][1] + other.m_matrix[2][1];
    result.m_matrix[2][2] = m_matrix[2][2] + other.m_matrix[2][2];
    result.m_matrix[2][3] = m_matrix[2][3] + other.m_matrix[2][3];

    result.m_matrix[3][0] = m_matrix[3][0] + other.m_matrix[3][0];
    result.m_matrix[3][1] = m_matrix[3][1] + other.m_matrix[3][1];
    result.m_matrix[3][2] = m_matrix[3][2] + other.m_matrix[3][2];
    result.m_matrix[3][3] = m_matrix[3][3] + other.m_matrix[3][3];
#endif
    return result;
}

math::mat4x4 math::mat4x4::operator-(const mat4x4 &other) const {
    mat4x4 result;
#ifndef NO_SIMD
    __m256 vec_a1 = _mm256_load_ps(&m_matrix[0][0]);
    __m256 vec_b1 = _mm256_load_ps(&other.m_matrix[0][0]);
    __m256 vec_result1 = _mm256_sub_ps(vec_a1, vec_b1);
    _mm256_store_ps(&result.m_matrix[0][0], vec_result1);

    __m256 vec_a2 = _mm256_load_ps(&m_matrix[2][0]);
    __m256 vec_b2 = _mm256_load_ps(&other.m_matrix[2][0]);
    __m256 vec_result2 = _mm256_sub_ps(vec_a2, vec_b2);
    _mm256_store_ps(&result.m_matrix[2][0], vec_result2);
#else
    result.m_matrix[0][0] = m_matrix[0][0] - other.m_matrix[0][0];
    result.m_matrix[0][1] = m_matrix[0][1] - other.m_matrix[0][1];
    result.m_matrix[0][2] = m_matrix[0][2] - other.m_matrix[0][2];
    result.m_matrix[0][3] = m_matrix[0][3] - other.m_matrix[0][3];

    result.m_matrix[1][0] = m_matrix[1][0] - other.m_matrix[1][0];
    result.m_matrix[1][1] = m_matrix[1][1] - other.m_matrix[1][1];
    result.m_matrix[1][2] = m_matrix[1][2] - other.m_matrix[1][2];
    result.m_matrix[1][3] = m_matrix[1][3] - other.m_matrix[1][3];

    result.m_matrix[2][0] = m_matrix[2][0] - other.m_matrix[2][0];
    result.m_matrix[2][1] = m_matrix[2][1] - other.m_matrix[2][1];
    result.m_matrix[2][2] = m_matrix[2][2] - other.m_matrix[2][2];
    result.m_matrix[2][3] = m_matrix[2][3] - other.m_matrix[2][3];

    result.m_matrix[3][0] = m_matrix[3][0] - other.m_matrix[3][0];
    result.m_matrix[3][1] = m_matrix[3][1] - other.m_matrix[3][1];
    result.m_matrix[3][2] = m_matrix[3][2] - other.m_matrix[3][2];
    result.m_matrix[3][3] = m_matrix[3][3] - other.m_matrix[3][3];
#endif
    return result;
}

math::mat4x4 math::mat4x4::operator*(const mat4x4 &other) const {
    mat4x4 result;
#ifndef NO_SIMD

    const __m128 o0 = _mm_load_ps(&other.m_matrix[0][0]);
    const __m128 o1 = _mm_load_ps(&other.m_matrix[1][0]);
    const __m128 o2 = _mm_load_ps(&other.m_matrix[2][0]);
    const __m128 o3 = _mm_load_ps(&other.m_matrix[3][0]);

    const __m128 m0 = _mm_load_ps(&m_matrix[0][0]);
    const __m128 m1 = _mm_load_ps(&m_matrix[1][0]);
    const __m128 m2 = _mm_load_ps(&m_matrix[2][0]);
    const __m128 m3 = _mm_load_ps(&m_matrix[3][0]);

    __m128 m0_x = _mm_shuffle_ps(m0, m0, 0x00);
    __m128 m0_y = _mm_shuffle_ps(m0, m0, 0x55);
    __m128 m0_z = _mm_shuffle_ps(m0, m0, 0xAA);
    __m128 m0_w = _mm_shuffle_ps(m0, m0, 0xFF);

    __m128 m1_x = _mm_shuffle_ps(m1, m1, 0x00);
    __m128 m1_y = _mm_shuffle_ps(m1, m1, 0x55);
    __m128 m1_z = _mm_shuffle_ps(m1, m1, 0xAA);
    __m128 m1_w = _mm_shuffle_ps(m1, m1, 0xFF);

    __m128 m2_x = _mm_shuffle_ps(m2, m2, 0x00);
    __m128 m2_y = _mm_shuffle_ps(m2, m2, 0x55);
    __m128 m2_z = _mm_shuffle_ps(m2, m2, 0xAA);
    __m128 m2_w = _mm_shuffle_ps(m2, m2, 0xFF);

    __m128 m3_x = _mm_shuffle_ps(m3, m3, 0x00);
    __m128 m3_y = _mm_shuffle_ps(m3, m3, 0x55);
    __m128 m3_z = _mm_shuffle_ps(m3, m3, 0xAA);
    __m128 m3_w = _mm_shuffle_ps(m3, m3, 0xFF);

    __m128 r0 = _mm_mul_ps(m0_x, o0);
    __m128 r1 = _mm_mul_ps(m1_x, o0);
    __m128 r2 = _mm_mul_ps(m2_x, o0);
    __m128 r3 = _mm_mul_ps(m3_x, o0);

    r0 = _mm_fmadd_ps(m0_y, o1, r0);
    r1 = _mm_fmadd_ps(m1_y, o1, r1);
    r2 = _mm_fmadd_ps(m2_y, o1, r2);
    r3 = _mm_fmadd_ps(m3_y, o1, r3);

    r0 = _mm_fmadd_ps(m0_z, o2, r0);
    r1 = _mm_fmadd_ps(m1_z, o2, r1);
    r2 = _mm_fmadd_ps(m2_z, o2, r2);
    r3 = _mm_fmadd_ps(m3_z, o2, r3);

    r0 = _mm_fmadd_ps(m0_w, o3, r0);
    r1 = _mm_fmadd_ps(m1_w, o3, r1);
    r2 = _mm_fmadd_ps(m2_w, o3, r2);
    r3 = _mm_fmadd_ps(m3_w, o3, r3);

    _mm_store_ps(&result.m_matrix[0][0], r0);
    _mm_store_ps(&result.m_matrix[1][0], r1);
    _mm_store_ps(&result.m_matrix[2][0], r2);
    _mm_store_ps(&result.m_matrix[3][0], r3);

#else
    result.m_matrix[0][0] =
        m_matrix[0][0] * other.m_matrix[0][0] + m_matrix[0][1] * other.m_matrix[1][0] +
        m_matrix[0][2] * other.m_matrix[2][0] + m_matrix[0][3] * other.m_matrix[3][0];
    result.m_matrix[0][1] =
        m_matrix[0][0] * other.m_matrix[0][1] + m_matrix[0][1] * other.m_matrix[1][1] +
        m_matrix[0][2] * other.m_matrix[2][1] + m_matrix[0][3] * other.m_matrix[3][1];
    result.m_matrix[0][2] =
        m_matrix[0][0] * other.m_matrix[0][2] + m_matrix[0][1] * other.m_matrix[1][2] +
        m_matrix[0][2] * other.m_matrix[2][2] + m_matrix[0][3] * other.m_matrix[3][2];
    result.m_matrix[0][3] =
        m_matrix[0][0] * other.m_matrix[0][3] + m_matrix[0][1] * other.m_matrix[1][3] +
        m_matrix[0][2] * other.m_matrix[2][3] + m_matrix[0][3] * other.m_matrix[3][3];
    result.m_matrix[1][0] =
        m_matrix[1][0] * other.m_matrix[0][0] + m_matrix[1][1] * other.m_matrix[1][0] +
        m_matrix[1][2] * other.m_matrix[2][0] + m_matrix[1][3] * other.m_matrix[3][0];
    result.m_matrix[1][1] =
        m_matrix[1][0] * other.m_matrix[0][1] + m_matrix[1][1] * other.m_matrix[1][1] +
        m_matrix[1][2] * other.m_matrix[2][1] + m_matrix[1][3] * other.m_matrix[3][1];
    result.m_matrix[1][2] =
        m_matrix[1][0] * other.m_matrix[0][2] + m_matrix[1][1] * other.m_matrix[1][2] +
        m_matrix[1][2] * other.m_matrix[2][2] + m_matrix[1][3] * other.m_matrix[3][2];
    result.m_matrix[1][3] =
        m_matrix[1][0] * other.m_matrix[0][3] + m_matrix[1][1] * other.m_matrix[1][3] +
        m_matrix[1][2] * other.m_matrix[2][3] + m_matrix[1][3] * other.m_matrix[3][3];
    result.m_matrix[2][0] =
        m_matrix[2][0] * other.m_matrix[0][0] + m_matrix[2][1] * other.m_matrix[1][0] +
        m_matrix[2][2] * other.m_matrix[2][0] + m_matrix[2][3] * other.m_matrix[3][0];
    result.m_matrix[2][1] =
        m_matrix[2][0] * other.m_matrix[0][1] + m_matrix[2][1] * other.m_matrix[1][1] +
        m_matrix[2][2] * other.m_matrix[2][1] + m_matrix[2][3] * other.m_matrix[3][1];
    result.m_matrix[2][2] =
        m_matrix[2][0] * other.m_matrix[0][2] + m_matrix[2][1] * other.m_matrix[1][2] +
        m_matrix[2][2] * other.m_matrix[2][2] + m_matrix[2][3] * other.m_matrix[3][2];
    result.m_matrix[2][3] =
        m_matrix[2][0] * other.m_matrix[0][3] + m_matrix[2][1] * other.m_matrix[1][3] +
        m_matrix[2][2] * other.m_matrix[2][3] + m_matrix[2][3] * other.m_matrix[3][3];
    result.m_matrix[3][0] =
        m_matrix[3][0] * other.m_matrix[0][0] + m_matrix[3][1] * other.m_matrix[1][0] +
        m_matrix[3][2] * other.m_matrix[2][0] + m_matrix[3][3] * other.m_matrix[3][0];
    result.m_matrix[3][1] =
        m_matrix[3][0] * other.m_matrix[0][1] + m_matrix[3][1] * other.m_matrix[1][1] +
        m_matrix[3][2] * other.m_matrix[2][1] + m_matrix[3][3] * other.m_matrix[3][1];
    result.m_matrix[3][2] =
        m_matrix[3][0] * other.m_matrix[0][2] + m_matrix[3][1] * other.m_matrix[1][2] +
        m_matrix[3][2] * other.m_matrix[2][2] + m_matrix[3][3] * other.m_matrix[3][2];
    result.m_matrix[3][3] =
        m_matrix[3][0] * other.m_matrix[0][3] + m_matrix[3][1] * other.m_matrix[1][3] +
        m_matrix[3][2] * other.m_matrix[2][3] + m_matrix[3][3] * other.m_matrix[3][3];
#endif
    return result;
}

math::mat4x4 &math::mat4x4::operator+=(const mat4x4 &other) {
#ifndef NO_SIMD
    __m256 vec_a1 = _mm256_load_ps(&m_matrix[0][0]);
    __m256 vec_b1 = _mm256_load_ps(&other.m_matrix[0][0]);
    __m256 vec_result1 = _mm256_add_ps(vec_a1, vec_b1);
    _mm256_store_ps(&m_matrix[0][0], vec_result1);

    __m256 vec_a2 = _mm256_load_ps(&m_matrix[2][0]);
    __m256 vec_b2 = _mm256_load_ps(&other.m_matrix[2][0]);
    __m256 vec_result2 = _mm256_add_ps(vec_a2, vec_b2);
    _mm256_store_ps(&m_matrix[2][0], vec_result2);
#else
    m_matrix[0][0] += other.m_matrix[0][0];
    m_matrix[0][1] += other.m_matrix[0][1];
    m_matrix[0][2] += other.m_matrix[0][2];
    m_matrix[0][3] += other.m_matrix[0][3];
    m_matrix[1][0] += other.m_matrix[1][0];
    m_matrix[1][1] += other.m_matrix[1][1];
    m_matrix[1][2] += other.m_matrix[1][2];
    m_matrix[1][3] += other.m_matrix[1][3];
    m_matrix[2][0] += other.m_matrix[2][0];
    m_matrix[2][1] += other.m_matrix[2][1];
    m_matrix[2][2] += other.m_matrix[2][2];
    m_matrix[2][3] += other.m_matrix[2][3];
    m_matrix[3][0] += other.m_matrix[3][0];
    m_matrix[3][1] += other.m_matrix[3][1];
    m_matrix[3][2] += other.m_matrix[3][2];
    m_matrix[3][3] += other.m_matrix[3][3];
#endif
    return *this;
}

math::mat4x4 &math::mat4x4::operator-=(const mat4x4 &other) {
#ifndef NO_SIMD
    __m256 vec_a1 = _mm256_load_ps(&m_matrix[0][0]);
    __m256 vec_b1 = _mm256_load_ps(&other.m_matrix[0][0]);
    __m256 vec_result1 = _mm256_sub_ps(vec_a1, vec_b1);
    _mm256_store_ps(&m_matrix[0][0], vec_result1);

    __m256 vec_a2 = _mm256_load_ps(&m_matrix[2][0]);
    __m256 vec_b2 = _mm256_load_ps(&other.m_matrix[2][0]);
    __m256 vec_result2 = _mm256_sub_ps(vec_a2, vec_b2);
    _mm256_store_ps(&m_matrix[2][0], vec_result2);
#else
    m_matrix[0][0] -= other.m_matrix[0][0];
    m_matrix[0][1] -= other.m_matrix[0][1];
    m_matrix[0][2] -= other.m_matrix[0][2];
    m_matrix[0][3] -= other.m_matrix[0][3];
    m_matrix[1][0] -= other.m_matrix[1][0];
    m_matrix[1][1] -= other.m_matrix[1][1];
    m_matrix[1][2] -= other.m_matrix[1][2];
    m_matrix[1][3] -= other.m_matrix[1][3];
    m_matrix[2][0] -= other.m_matrix[2][0];
    m_matrix[2][1] -= other.m_matrix[2][1];
    m_matrix[2][2] -= other.m_matrix[2][2];
    m_matrix[2][3] -= other.m_matrix[2][3];
    m_matrix[3][0] -= other.m_matrix[3][0];
    m_matrix[3][1] -= other.m_matrix[3][1];
    m_matrix[3][2] -= other.m_matrix[3][2];
    m_matrix[3][3] -= other.m_matrix[3][3];
#endif
    return *this;
}

math::mat4x4 &math::mat4x4::operator*=(const mat4x4 &other) {
#ifndef NO_SIMD
    const __m128 b0 = _mm_load_ps(&other.m_matrix[0][0]);
    const __m128 b1 = _mm_load_ps(&other.m_matrix[1][0]);
    const __m128 b2 = _mm_load_ps(&other.m_matrix[2][0]);
    const __m128 b3 = _mm_load_ps(&other.m_matrix[3][0]);

    const __m128 a0 = _mm_load_ps(&m_matrix[0][0]);
    const __m128 a1 = _mm_load_ps(&m_matrix[1][0]);
    const __m128 a2 = _mm_load_ps(&m_matrix[2][0]);
    const __m128 a3 = _mm_load_ps(&m_matrix[3][0]);

    __m128 a0_x = _mm_shuffle_ps(a0, a0, 0x00);
    __m128 a0_y = _mm_shuffle_ps(a0, a0, 0x55);
    __m128 a0_z = _mm_shuffle_ps(a0, a0, 0xAA);
    __m128 a0_w = _mm_shuffle_ps(a0, a0, 0xFF);

    __m128 a1_x = _mm_shuffle_ps(a1, a1, 0x00);
    __m128 a1_y = _mm_shuffle_ps(a1, a1, 0x55);
    __m128 a1_z = _mm_shuffle_ps(a1, a1, 0xAA);
    __m128 a1_w = _mm_shuffle_ps(a1, a1, 0xFF);

    __m128 a2_x = _mm_shuffle_ps(a2, a2, 0x00);
    __m128 a2_y = _mm_shuffle_ps(a2, a2, 0x55);
    __m128 a2_z = _mm_shuffle_ps(a2, a2, 0xAA);
    __m128 a2_w = _mm_shuffle_ps(a2, a2, 0xFF);

    __m128 a3_x = _mm_shuffle_ps(a3, a3, 0x00);
    __m128 a3_y = _mm_shuffle_ps(a3, a3, 0x55);
    __m128 a3_z = _mm_shuffle_ps(a3, a3, 0xAA);
    __m128 a3_w = _mm_shuffle_ps(a3, a3, 0xFF);

    __m128 r0 = _mm_mul_ps(a0_x, b0);
    __m128 r1 = _mm_mul_ps(a1_x, b0);
    __m128 r2 = _mm_mul_ps(a2_x, b0);
    __m128 r3 = _mm_mul_ps(a3_x, b0);

    r0 = _mm_fmadd_ps(a0_y, b1, r0);
    r1 = _mm_fmadd_ps(a1_y, b1, r1);
    r2 = _mm_fmadd_ps(a2_y, b1, r2);
    r3 = _mm_fmadd_ps(a3_y, b1, r3);

    r0 = _mm_fmadd_ps(a0_z, b2, r0);
    r1 = _mm_fmadd_ps(a1_z, b2, r1);
    r2 = _mm_fmadd_ps(a2_z, b2, r2);
    r3 = _mm_fmadd_ps(a3_z, b2, r3);

    r0 = _mm_fmadd_ps(a0_w, b3, r0);
    r1 = _mm_fmadd_ps(a1_w, b3, r1);
    r2 = _mm_fmadd_ps(a2_w, b3, r2);
    r3 = _mm_fmadd_ps(a3_w, b3, r3);

    _mm_store_ps(&m_matrix[0][0], r0);
    _mm_store_ps(&m_matrix[1][0], r1);
    _mm_store_ps(&m_matrix[2][0], r2);
    _mm_store_ps(&m_matrix[3][0], r3);
#else
    mat4x4 temp = *this;

    m_matrix[0][0] =
        temp.m_matrix[0][0] * other.m_matrix[0][0] + temp.m_matrix[0][1] * other.m_matrix[1][0] +
        temp.m_matrix[0][2] * other.m_matrix[2][0] + temp.m_matrix[0][3] * other.m_matrix[3][0];
    m_matrix[0][1] =
        temp.m_matrix[0][0] * other.m_matrix[0][1] + temp.m_matrix[0][1] * other.m_matrix[1][1] +
        temp.m_matrix[0][2] * other.m_matrix[2][1] + temp.m_matrix[0][3] * other.m_matrix[3][1];
    m_matrix[0][2] =
        temp.m_matrix[0][0] * other.m_matrix[0][2] + temp.m_matrix[0][1] * other.m_matrix[1][2] +
        temp.m_matrix[0][2] * other.m_matrix[2][2] + temp.m_matrix[0][3] * other.m_matrix[3][2];
    m_matrix[0][3] =
        temp.m_matrix[0][0] * other.m_matrix[0][3] + temp.m_matrix[0][1] * other.m_matrix[1][3] +
        temp.m_matrix[0][2] * other.m_matrix[2][3] + temp.m_matrix[0][3] * other.m_matrix[3][3];

    m_matrix[1][0] =
        temp.m_matrix[1][0] * other.m_matrix[0][0] + temp.m_matrix[1][1] * other.m_matrix[1][0] +
        temp.m_matrix[1][2] * other.m_matrix[2][0] + temp.m_matrix[1][3] * other.m_matrix[3][0];
    m_matrix[1][1] =
        temp.m_matrix[1][0] * other.m_matrix[0][1] + temp.m_matrix[1][1] * other.m_matrix[1][1] +
        temp.m_matrix[1][2] * other.m_matrix[2][1] + temp.m_matrix[1][3] * other.m_matrix[3][1];
    m_matrix[1][2] =
        temp.m_matrix[1][0] * other.m_matrix[0][2] + temp.m_matrix[1][1] * other.m_matrix[1][2] +
        temp.m_matrix[1][2] * other.m_matrix[2][2] + temp.m_matrix[1][3] * other.m_matrix[3][2];
    m_matrix[1][3] =
        temp.m_matrix[1][0] * other.m_matrix[0][3] + temp.m_matrix[1][1] * other.m_matrix[1][3] +
        temp.m_matrix[1][2] * other.m_matrix[2][3] + temp.m_matrix[1][3] * other.m_matrix[3][3];

    m_matrix[2][0] =
        temp.m_matrix[2][0] * other.m_matrix[0][0] + temp.m_matrix[2][1] * other.m_matrix[1][0] +
        temp.m_matrix[2][2] * other.m_matrix[2][0] + temp.m_matrix[2][3] * other.m_matrix[3][0];
    m_matrix[2][1] =
        temp.m_matrix[2][0] * other.m_matrix[0][1] + temp.m_matrix[2][1] * other.m_matrix[1][1] +
        temp.m_matrix[2][2] * other.m_matrix[2][1] + temp.m_matrix[2][3] * other.m_matrix[3][1];
    m_matrix[2][2] =
        temp.m_matrix[2][0] * other.m_matrix[0][2] + temp.m_matrix[2][1] * other.m_matrix[1][2] +
        temp.m_matrix[2][2] * other.m_matrix[2][2] + temp.m_matrix[2][3] * other.m_matrix[3][2];
    m_matrix[2][3] =
        temp.m_matrix[2][0] * other.m_matrix[0][3] + temp.m_matrix[2][1] * other.m_matrix[1][3] +
        temp.m_matrix[2][2] * other.m_matrix[2][3] + temp.m_matrix[2][3] * other.m_matrix[3][3];

    m_matrix[3][0] =
        temp.m_matrix[3][0] * other.m_matrix[0][0] + temp.m_matrix[3][1] * other.m_matrix[1][0] +
        temp.m_matrix[3][2] * other.m_matrix[2][0] + temp.m_matrix[3][3] * other.m_matrix[3][0];
    m_matrix[3][1] =
        temp.m_matrix[3][0] * other.m_matrix[0][1] + temp.m_matrix[3][1] * other.m_matrix[1][1] +
        temp.m_matrix[3][2] * other.m_matrix[2][1] + temp.m_matrix[3][3] * other.m_matrix[3][1];
    m_matrix[3][2] =
        temp.m_matrix[3][0] * other.m_matrix[0][2] + temp.m_matrix[3][1] * other.m_matrix[1][2] +
        temp.m_matrix[3][2] * other.m_matrix[2][2] + temp.m_matrix[3][3] * other.m_matrix[3][2];
    m_matrix[3][3] =
        temp.m_matrix[3][0] * other.m_matrix[0][3] + temp.m_matrix[3][1] * other.m_matrix[1][3] +
        temp.m_matrix[3][2] * other.m_matrix[2][3] + temp.m_matrix[3][3] * other.m_matrix[3][3];
#endif
    return *this;
}

math::mat4x4 math::mat4x4::operator+(const float scalar) const {
    mat4x4 result;
#ifndef NO_SIMD
    __m256 vec_scalar = _mm256_set1_ps(scalar);
    __m256 vec_a1 = _mm256_load_ps(&m_matrix[0][0]);
    __m256 vec_result1 = _mm256_add_ps(vec_a1, vec_scalar);
    _mm256_store_ps(&result.m_matrix[0][0], vec_result1);
    __m256 vec_a2 = _mm256_load_ps(&m_matrix[2][0]);
    __m256 vec_result2 = _mm256_add_ps(vec_a2, vec_scalar);
    _mm256_store_ps(&result.m_matrix[2][0], vec_result2);
#else
    result.m_matrix[0][0] = m_matrix[0][0] + scalar;
    result.m_matrix[0][1] = m_matrix[0][1] + scalar;
    result.m_matrix[0][2] = m_matrix[0][2] + scalar;
    result.m_matrix[0][3] = m_matrix[0][3] + scalar;
    result.m_matrix[1][0] = m_matrix[1][0] + scalar;
    result.m_matrix[1][1] = m_matrix[1][1] + scalar;
    result.m_matrix[1][2] = m_matrix[1][2] + scalar;
    result.m_matrix[1][3] = m_matrix[1][3] + scalar;
    result.m_matrix[2][0] = m_matrix[2][0] + scalar;
    result.m_matrix[2][1] = m_matrix[2][1] + scalar;
    result.m_matrix[2][2] = m_matrix[2][2] + scalar;
    result.m_matrix[2][3] = m_matrix[2][3] + scalar;
    result.m_matrix[3][0] = m_matrix[3][0] + scalar;
    result.m_matrix[3][1] = m_matrix[3][1] + scalar;
    result.m_matrix[3][2] = m_matrix[3][2] + scalar;
    result.m_matrix[3][3] = m_matrix[3][3] + scalar;
#endif
    return result;
}

math::mat4x4 math::mat4x4::operator-(const float scalar) const {
    mat4x4 result;
#ifndef NO_SIMD
    __m256 vec_scalar = _mm256_set1_ps(scalar);
    __m256 vec_a1 = _mm256_load_ps(&m_matrix[0][0]);
    __m256 vec_result1 = _mm256_sub_ps(vec_a1, vec_scalar);
    _mm256_store_ps(&result.m_matrix[0][0], vec_result1);
    __m256 vec_a2 = _mm256_load_ps(&m_matrix[2][0]);
    __m256 vec_result2 = _mm256_sub_ps(vec_a2, vec_scalar);
    _mm256_store_ps(&result.m_matrix[2][0], vec_result2);
#else
    result.m_matrix[0][0] = m_matrix[0][0] - scalar;
    result.m_matrix[0][1] = m_matrix[0][1] - scalar;
    result.m_matrix[0][2] = m_matrix[0][2] - scalar;
    result.m_matrix[0][3] = m_matrix[0][3] - scalar;
    result.m_matrix[1][0] = m_matrix[1][0] - scalar;
    result.m_matrix[1][1] = m_matrix[1][1] - scalar;
    result.m_matrix[1][2] = m_matrix[1][2] - scalar;
    result.m_matrix[1][3] = m_matrix[1][3] - scalar;
    result.m_matrix[2][0] = m_matrix[2][0] - scalar;
    result.m_matrix[2][1] = m_matrix[2][1] - scalar;
    result.m_matrix[2][2] = m_matrix[2][2] - scalar;
    result.m_matrix[2][3] = m_matrix[2][3] - scalar;
    result.m_matrix[3][0] = m_matrix[3][0] - scalar;
    result.m_matrix[3][1] = m_matrix[3][1] - scalar;
    result.m_matrix[3][2] = m_matrix[3][2] - scalar;
    result.m_matrix[3][3] = m_matrix[3][3] - scalar;
#endif
    return result;
}

math::mat4x4 math::mat4x4::operator*(const float scalar) const {
    mat4x4 result;
#ifndef NO_SIMD
    __m256 vec_scalar = _mm256_set1_ps(scalar);
    __m256 vec_a1 = _mm256_load_ps(&m_matrix[0][0]);
    __m256 vec_result1 = _mm256_mul_ps(vec_a1, vec_scalar);
    _mm256_store_ps(&result.m_matrix[0][0], vec_result1);
    __m256 vec_a2 = _mm256_load_ps(&m_matrix[2][0]);
    __m256 vec_result2 = _mm256_mul_ps(vec_a2, vec_scalar);
    _mm256_store_ps(&result.m_matrix[2][0], vec_result2);
#else
    result.m_matrix[0][0] = m_matrix[0][0] * scalar;
    result.m_matrix[0][1] = m_matrix[0][1] * scalar;
    result.m_matrix[0][2] = m_matrix[0][2] * scalar;
    result.m_matrix[0][3] = m_matrix[0][3] * scalar;
    result.m_matrix[1][0] = m_matrix[1][0] * scalar;
    result.m_matrix[1][1] = m_matrix[1][1] * scalar;
    result.m_matrix[1][2] = m_matrix[1][2] * scalar;
    result.m_matrix[1][3] = m_matrix[1][3] * scalar;
    result.m_matrix[2][0] = m_matrix[2][0] * scalar;
    result.m_matrix[2][1] = m_matrix[2][1] * scalar;
    result.m_matrix[2][2] = m_matrix[2][2] * scalar;
    result.m_matrix[2][3] = m_matrix[2][3] * scalar;
    result.m_matrix[3][0] = m_matrix[3][0] * scalar;
    result.m_matrix[3][1] = m_matrix[3][1] * scalar;
    result.m_matrix[3][2] = m_matrix[3][2] * scalar;
    result.m_matrix[3][3] = m_matrix[3][3] * scalar;
#endif
    return result;
}

math::mat4x4 &math::mat4x4::operator+=(const float scalar) {
    *this = *this + scalar;
    return *this;
}

math::mat4x4 &math::mat4x4::operator-=(const float scalar) {
    *this = *this - scalar;
    return *this;
}

math::mat4x4 &math::mat4x4::operator*=(const float scalar) {
    *this = *this * scalar;
    return *this;
}

float &math::mat4x4::at(int row, int col) { return m_matrix[row][col]; }

const float &math::mat4x4::at(int row, int col) const {
    if (row < 0 || row >= 4) {
        throw std::out_of_range("Row index out of range");
    }
    if (col < 0 || col >= 4) {
        throw std::out_of_range("Column index out of range");
    }
    return m_matrix[row][col];
}

math::mat4x4 math::mat4x4::transpose() const {
    mat4x4 result;
#ifndef NO_SIMD
    __m128 row0 = _mm_load_ps(&m_matrix[0][0]);
    __m128 row1 = _mm_load_ps(&m_matrix[1][0]);
    __m128 row2 = _mm_load_ps(&m_matrix[2][0]);
    __m128 row3 = _mm_load_ps(&m_matrix[3][0]);

    _MM_TRANSPOSE4_PS(row0, row1, row2, row3);

    _mm_store_ps(&result.m_matrix[0][0], row0);
    _mm_store_ps(&result.m_matrix[1][0], row1);
    _mm_store_ps(&result.m_matrix[2][0], row2);
    _mm_store_ps(&result.m_matrix[3][0], row3);
#else
    result.m_matrix[0][0] = m_matrix[0][0];
    result.m_matrix[0][1] = m_matrix[1][0];
    result.m_matrix[0][2] = m_matrix[2][0];
    result.m_matrix[0][3] = m_matrix[3][0];
    result.m_matrix[1][0] = m_matrix[0][1];
    result.m_matrix[1][1] = m_matrix[1][1];
    result.m_matrix[1][2] = m_matrix[2][1];
    result.m_matrix[1][3] = m_matrix[3][1];
    result.m_matrix[2][0] = m_matrix[0][2];
    result.m_matrix[2][1] = m_matrix[1][2];
    result.m_matrix[2][2] = m_matrix[2][2];
    result.m_matrix[2][3] = m_matrix[3][2];
    result.m_matrix[3][0] = m_matrix[0][3];
    result.m_matrix[3][1] = m_matrix[1][3];
    result.m_matrix[3][2] = m_matrix[2][3];
    result.m_matrix[3][3] = m_matrix[3][3];
#endif
    return result;
}

math::mat4x4 math::mat4x4::inverse() const {
    float m00 = m_matrix[0][0], m01 = m_matrix[0][1], m02 = m_matrix[0][2], m03 = m_matrix[0][3];
    float m10 = m_matrix[1][0], m11 = m_matrix[1][1], m12 = m_matrix[1][2], m13 = m_matrix[1][3];
    float m20 = m_matrix[2][0], m21 = m_matrix[2][1], m22 = m_matrix[2][2], m23 = m_matrix[2][3];
    float m30 = m_matrix[3][0], m31 = m_matrix[3][1], m32 = m_matrix[3][2], m33 = m_matrix[3][3];

    float coef00 = m22 * m33 - m32 * m23;
    float coef02 = m12 * m33 - m32 * m13;
    float coef03 = m12 * m23 - m22 * m13;

    float coef04 = m21 * m33 - m31 * m23;
    float coef06 = m11 * m33 - m31 * m13;
    float coef07 = m11 * m23 - m21 * m13;

    float coef08 = m21 * m32 - m31 * m22;
    float coef10 = m11 * m32 - m31 * m12;
    float coef11 = m11 * m22 - m21 * m12;

    float coef12 = m20 * m33 - m30 * m23;
    float coef14 = m10 * m33 - m30 * m13;
    float coef15 = m10 * m23 - m20 * m13;

    float coef16 = m20 * m32 - m30 * m22;
    float coef18 = m10 * m32 - m30 * m12;
    float coef19 = m10 * m22 - m20 * m12;

    float coef20 = m20 * m31 - m30 * m21;
    float coef22 = m10 * m31 - m30 * m11;
    float coef23 = m10 * m21 - m20 * m11;

    float fac0 = m11 * coef00 - m12 * coef04 + m13 * coef08;
    float fac1 = m10 * coef00 - m12 * coef12 + m13 * coef16;
    float fac2 = m10 * coef04 - m11 * coef12 + m13 * coef20;
    float fac3 = m10 * coef08 - m11 * coef16 + m12 * coef20;

    float det = m00 * fac0 - m01 * fac1 + m02 * fac2 - m03 * fac3;

    if (std::abs(det) < 1e-8f) {
        throw std::runtime_error("Matrix is not invertible");
    }

    float invDet = 1.0f / det;

    mat4x4 result;

    result.m_matrix[0][0] = +(m11 * coef00 - m12 * coef04 + m13 * coef08) * invDet;
    result.m_matrix[0][1] = -(m01 * coef00 - m02 * coef04 + m03 * coef08) * invDet;
    result.m_matrix[0][2] = +(m01 * coef02 - m02 * coef06 + m03 * coef10) * invDet;
    result.m_matrix[0][3] = -(m01 * coef03 - m02 * coef07 + m03 * coef11) * invDet;

    result.m_matrix[1][0] = -(m10 * coef00 - m12 * coef12 + m13 * coef16) * invDet;
    result.m_matrix[1][1] = +(m00 * coef00 - m02 * coef12 + m03 * coef16) * invDet;
    result.m_matrix[1][2] = -(m00 * coef02 - m02 * coef14 + m03 * coef18) * invDet;
    result.m_matrix[1][3] = +(m00 * coef03 - m02 * coef15 + m03 * coef19) * invDet;

    result.m_matrix[2][0] = +(m10 * coef04 - m11 * coef12 + m13 * coef20) * invDet;
    result.m_matrix[2][1] = -(m00 * coef04 - m01 * coef12 + m03 * coef20) * invDet;
    result.m_matrix[2][2] = +(m00 * coef06 - m01 * coef14 + m03 * coef22) * invDet;
    result.m_matrix[2][3] = -(m00 * coef07 - m01 * coef15 + m03 * coef23) * invDet;

    result.m_matrix[3][0] = -(m10 * coef08 - m11 * coef16 + m12 * coef20) * invDet;
    result.m_matrix[3][1] = +(m00 * coef08 - m01 * coef16 + m02 * coef20) * invDet;
    result.m_matrix[3][2] = -(m00 * coef10 - m01 * coef18 + m02 * coef22) * invDet;
    result.m_matrix[3][3] = +(m00 * coef11 - m01 * coef19 + m02 * coef23) * invDet;

    return result;
}

double math::mat4x4::determinant() const {
    const float *m = &m_matrix[0][0];
    return m[0] * (m[5] * (m[10] * m[15] - m[11] * m[14]) - m[6] * (m[9] * m[15] - m[11] * m[13]) +
                   m[7] * (m[9] * m[14] - m[10] * m[13])) -
           m[1] * (m[4] * (m[10] * m[15] - m[11] * m[14]) - m[6] * (m[8] * m[15] - m[11] * m[12]) +
                   m[7] * (m[8] * m[14] - m[10] * m[12])) +
           m[2] * (m[4] * (m[9] * m[15] - m[11] * m[13]) - m[5] * (m[8] * m[15] - m[11] * m[12]) +
                   m[7] * (m[8] * m[13] - m[9] * m[12])) -
           m[3] * (m[4] * (m[9] * m[14] - m[10] * m[13]) - m[5] * (m[8] * m[14] - m[10] * m[12]) +
                   m[6] * (m[8] * m[13] - m[9] * m[12]));
}

math::mat4x4 math::mat4x4::identity() {
    return {{{1.0f, 0.0f, 0.0f, 0.0f},
             {0.0f, 1.0f, 0.0f, 0.0f},
             {0.0f, 0.0f, 1.0f, 0.0f},
             {0.0f, 0.0f, 0.0f, 1.0f}}};
}

math::mat4x4 math::mat4x4::zero() {
    return {{{0.0f, 0.0f, 0.0f, 0.0f},
             {0.0f, 0.0f, 0.0f, 0.0f},
             {0.0f, 0.0f, 0.0f, 0.0f},
             {0.0f, 0.0f, 0.0f, 0.0f}}};
}

std::ostream &math::operator<<(std::ostream &os, const math::mat4x4 &matrix) {
    os << "[" << matrix.at(0, 0) << ", " << matrix.at(0, 1) << ", " << matrix.at(0, 2) << ", "
       << matrix.at(0, 3) << "]\n"
       << "[" << matrix.at(1, 0) << ", " << matrix.at(1, 1) << ", " << matrix.at(1, 2) << ", "
       << matrix.at(1, 3) << "]\n"
       << "[" << matrix.at(2, 0) << ", " << matrix.at(2, 1) << ", " << matrix.at(2, 2) << ", "
       << matrix.at(2, 3) << "]\n"
       << "[" << matrix.at(3, 0) << ", " << matrix.at(3, 1) << ", " << matrix.at(3, 2) << ", "
       << matrix.at(3, 3) << "]\n";

    return os;
}