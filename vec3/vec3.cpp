#include "vec3.hpp"
#include <cmath>

namespace math {

vec3::vec3() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}

vec3::vec3(float x, float y, float z, bool normalize) {
    this->m_x = x;
    this->m_y = y;
    this->m_z = z;
    if (normalize) [[unlikely]] {
        this->normalize();
    }
}

vec3::vec3(const point3& point)
{
    m_x = point.x;
    m_y = point.y;
    m_z = point.z;
}

vec3::vec3(const point3& start, const point3& end)
{
    m_x = end.x - start.x;
    m_y = end.y - start.y;
    m_z = end.z - start.z;
}

vec3::vec3(const vec2 &xy, float z, bool normalize) {
    this->m_x = xy.x();
    this->m_y = xy.y();
    this->m_z = z;
    if (normalize) [[unlikely]] {
        this->normalize();
    }
}

vec3::vec3(const point2& point, float z, bool normalize)
{
    m_x = point.x;
    m_y = point.y;
    m_z = z;
    if (normalize) [[unlikely]] {
        this->normalize();
    }
}

vec3::vec3(const vec3& other)
{
    m_x = other.m_x;
    m_y = other.m_y;
    m_z = other.m_z;
}

float vec3::x() const { return m_x; }

float vec3::y() const { return m_y; }

float vec3::z() const { return m_z; }

float vec3::x(float x) {
    m_x = x;
    return m_x;
}

float vec3::y(float y) {
    m_y = y;
    return m_y;
}

float vec3::z(float z) {
    m_z = z;
    return m_z;
}

vec3 &math::vec3::operator=(const vec3 &other) {
    if (this != &other) [[likely]] {
        this->m_x = other.m_x;
        this->m_y = other.m_y;
        this->m_z = other.m_z;
    }
    return *this;
}

vec3 &vec3::operator+=(const vec3 &other) {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
}

vec3 &vec3::operator-=(const vec3 &other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;
    return *this;
}

vec3 vec3::operator+(const vec3 &other) const {
    vec3 result;
    result.m_x = m_x + other.m_x;
    result.m_y = m_y + other.m_y;
    result.m_z = m_z + other.m_z;
    return result;
}

vec3 vec3::operator-(const vec3 &other) const {
    vec3 result;
    result.m_x = m_x - other.m_x;
    result.m_y = m_y - other.m_y;
    result.m_z = m_z - other.m_z;
    return result;
}

vec3 &vec3::operator*=(float scalar) {
    m_x *= scalar;
    m_y *= scalar;
    m_z *= scalar;
    return *this;
}

vec3 &vec3::operator/=(float scalar) {
    m_x /= scalar;
    m_y /= scalar;
    m_z /= scalar;
    return *this;
}

vec3 vec3::operator*(float scalar) const {
    vec3 result;
    result.m_x = m_x * scalar;
    result.m_y = m_y * scalar;
    result.m_z = m_z * scalar;
    return result;
}

vec3 vec3::operator/(float scalar) const { return vec3(m_x / scalar, m_y / scalar, m_z / scalar); }

float vec3::dot_production(const vec3 &other) const {
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

vec3 vec3::cross_production(const vec3 &other) const {
    vec3 result;
    result.m_x = m_y * other.m_z - m_z * other.m_y;
    result.m_y = m_z * other.m_x - m_x * other.m_z;
    result.m_z = m_x * other.m_y - m_y * other.m_x;
    return result;
}

float vec3::distance_to(const vec3 &other) const {
    float dx = m_x - other.m_x;
    float dy = m_y - other.m_y;
    float dz = m_z - other.m_z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float vec3::length() const { return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z); }

vec3 vec3::normalized() const {
    float len = m_x * m_x + m_y * m_y + m_z * m_z;
    if (len < 1e-8f) [[unlikely]] {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float inv_len = 1.0f / std::sqrt(len);
    return vec3(m_x * inv_len, m_y * inv_len, m_z * inv_len);
}

void vec3::normalize() {
    float len = m_x * m_x + m_y * m_y + m_z * m_z;
    if (len < 1e-8f) [[unlikely]] {
        m_x = 0.0f;
        m_y = 0.0f;
        m_z = 0.0f;
        return;
    }
    float inv_len = 1.0f / std::sqrt(len);
    m_x *= inv_len;
    m_y *= inv_len;
    m_z *= inv_len;
}

vec3 vec3::reflect(const vec3 &normal) const {
    float len_sq = normal.m_x * normal.m_x + normal.m_y * normal.m_y + normal.m_z * normal.m_z;
    if (len_sq < 1e-8f) [[unlikely]] {
        return *this;
    }
    float inv_len = 1.0f / std::sqrt(len_sq);
    float nx = normal.m_x * inv_len;
    float ny = normal.m_y * inv_len;
    float nz = normal.m_z * inv_len;

    float dot = m_x * nx + m_y * ny + m_z * nz;
    float factor = 2.0f * dot;

    return vec3(m_x - nx * factor, m_y - ny * factor, m_z - nz * factor);
}

float vec3::angle_between(const vec3 &other) const {
    float dot_p = m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
    float len_sq_a = m_x * m_x + m_y * m_y + m_z * m_z;
    float len_sq_b = other.m_x * other.m_x + other.m_y * other.m_y + other.m_z * other.m_z;
    float len_product = std::sqrt(len_sq_a * len_sq_b);
    if (len_product < 1e-8f) [[unlikely]] {
        return 0.0f;
    }
    return std::acos(dot_p / len_product);
}

float vec3::x_axis_angle() const { return std::acos(m_x / this->length()); }

float vec3::y_axis_angle() const { return std::acos(m_y / this->length()); }

float vec3::z_axis_angle() const { return std::acos(m_z / this->length()); }

vec3 vec3::project_on_vector(const vec3 &other) const {
    float dot_p = m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
    float other_len_sq = other.m_x * other.m_x + other.m_y * other.m_y + other.m_z * other.m_z;
    if (other_len_sq < 1e-8f) [[unlikely]] {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float scalar = dot_p / other_len_sq;
    return vec3(other.m_x * scalar, other.m_y * scalar, other.m_z * scalar);
}

vec3 vec3::rotated_around_axis(const vec3 &axis, float angle_rad) const {
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    vec3 norm_axis = axis.normalized();

    float ux = norm_axis.m_x;
    float uy = norm_axis.m_y;
    float uz = norm_axis.m_z;

    vec3 result;
    result.m_x = (cos_a + (1 - cos_a) * ux * ux) * m_x +
                 ((1 - cos_a) * ux * uy - sin_a * uz) * m_y +
                 ((1 - cos_a) * ux * uz + sin_a * uy) * m_z;

    result.m_y = ((1 - cos_a) * uy * ux + sin_a * uz) * m_x +
                 (cos_a + (1 - cos_a) * uy * uy) * m_y + ((1 - cos_a) * uy * uz - sin_a * ux) * m_z;

    result.m_z = ((1 - cos_a) * uz * ux - sin_a * uy) * m_x +
                 ((1 - cos_a) * uz * uy + sin_a * ux) * m_y + (cos_a + (1 - cos_a) * uz * uz) * m_z;

    return result;
}

void vec3::rotate_around_axis(const vec3 &axis, float angle_rad) {
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    vec3 norm_axis = axis.normalized();
    float ux = norm_axis.m_x;
    float uy = norm_axis.m_y;
    float uz = norm_axis.m_z;
    float new_x = (cos_a + (1 - cos_a) * ux * ux) * m_x +
                  ((1 - cos_a) * ux * uy - sin_a * uz) * m_y +
                  ((1 - cos_a) * ux * uz + sin_a * uy) * m_z;
    float new_y = ((1 - cos_a) * uy * ux + sin_a * uz) * m_x +
                  (cos_a + (1 - cos_a) * uy * uy) * m_y +
                  ((1 - cos_a) * uy * uz - sin_a * ux) * m_z;
    float new_z = ((1 - cos_a) * uz * ux - sin_a * uy) * m_x +
                  ((1 - cos_a) * uz * uy + sin_a * ux) * m_y +
                  (cos_a + (1 - cos_a) * uz * uz) * m_z;
    m_x = new_x;
    m_y = new_y;
    m_z = new_z;
}

vec2 vec3::to_vec2_orthographic() const { return vec2(m_x, m_y); }

vec2 vec3::to_vec2_perspective(float focal_length) const {
    if (m_z == 0.0f) [[unlikely]] {
        return vec2(0.0f, 0.0f);
    }
    float inv_z = focal_length / m_z;
    return vec2(m_x * inv_z, m_y * inv_z);
}

vec3 vec3::zero() { return vec3(0.0f, 0.0f, 0.0f); }

vec3 vec3::one() { return vec3(1.0f, 1.0f, 1.0f); }

vec3 vec3::basis_i() { return vec3(1.0f, 0.0f, 0.0f); }

vec3 vec3::basis_j() { return vec3(0.0f, 1.0f, 0.0f); }

vec3 vec3::basis_k() { return vec3(0.0f, 0.0f, 1.0f); }

bool vec3::is_collinear(const vec3 &a, const vec3 &b, float &proportionality_coefficient) {
    proportionality_coefficient = a.m_x / b.m_x;
    if (a.m_y == proportionality_coefficient * b.m_y &&
        a.m_z == proportionality_coefficient * b.m_z) [[likely]] {
        return true;
    }
    proportionality_coefficient = 0.0f;
    return false;
}

bool vec3::is_coplanar(const vec3 &a, const vec3 &b, const vec3 &c) {
    float mixed_prod = a.m_x * (b.m_y * c.m_z - b.m_z * c.m_y) -
                       a.m_y * (b.m_x * c.m_z - b.m_z * c.m_x) +
                       a.m_z * (b.m_x * c.m_y - b.m_y * c.m_x);
    return std::abs(mixed_prod) < 1e-8f;
}

bool vec3::is_orthogonal(const vec3 &a, const vec3 &b) {
    float dot_p = a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
    return std::abs(dot_p) < 1e-8f;
}

bool vec3::is_parallel(const vec3 &a, const vec3 &b) {
    float proportionality_coefficient;
    return is_collinear(a, b, proportionality_coefficient);
}

vec3 vec3::lerp(const vec3 &a, const vec3 &b, float t) {
    return vec3(a.m_x + (b.m_x - a.m_x) * t, a.m_y + (b.m_y - a.m_y) * t,
                a.m_z + (b.m_z - a.m_z) * t);
}

float vec3::angle_between(const vec3 &a, const vec3 &b) {
    float dot_p = a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
    float len_sq_a = a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z;
    float len_sq_b = b.m_x * b.m_x + b.m_y * b.m_y + b.m_z * b.m_z;
    float len_product = std::sqrt(len_sq_a * len_sq_b);
    if (len_product < 1e-8f) [[unlikely]] {
        return 0.0f;
    }
    return std::acos(dot_p / len_product);
}

vec3 vec3::cross_production(const vec3 &a, const vec3 &b) {
    vec3 result;
    result.m_x = a.m_y * b.m_z - a.m_z * b.m_y;
    result.m_y = a.m_z * b.m_x - a.m_x * b.m_z;
    result.m_z = a.m_x * b.m_y - a.m_y * b.m_x;
    return result;
}

float vec3::dot_production(const vec3 &a, const vec3 &b) {
    return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
}

float vec3::mixed_production(const vec3 &a, const vec3 &b, const vec3 &c) {
    return a.m_x * (b.m_y * c.m_z - b.m_z * c.m_y) - a.m_y * (b.m_x * c.m_z - b.m_z * c.m_x) +
           a.m_z * (b.m_x * c.m_y - b.m_y * c.m_x);
}

vec3 vec3::project_vector(const vec3 &source, const vec3 &on_target) {
    float dot_p =
        source.m_x * on_target.m_x + source.m_y * on_target.m_y + source.m_z * on_target.m_z;
    float on_target_len_sq = on_target.m_x * on_target.m_x + on_target.m_y * on_target.m_y +
                             on_target.m_z * on_target.m_z;
    if (on_target_len_sq < 1e-8f) [[unlikely]] {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float scalar = dot_p / on_target_len_sq;
    return vec3(on_target.m_x * scalar, on_target.m_y * scalar, on_target.m_z * scalar);
}

vec3 vec3::reflect_vector(const vec3 &falling, const vec3 &normal) {
    float len_sq = normal.m_x * normal.m_x + normal.m_y * normal.m_y + normal.m_z * normal.m_z;
    if (len_sq < 1e-8f) [[unlikely]] {
        return falling;
    }
    float inv_len = 1.0f / std::sqrt(len_sq);
    float nx = normal.m_x * inv_len;
    float ny = normal.m_y * inv_len;
    float nz = normal.m_z * inv_len;

    float dot = falling.m_x * nx + falling.m_y * ny + falling.m_z * nz;
    float factor = 2.0f * dot;

    return vec3(falling.m_x - nx * factor, falling.m_y - ny * factor, falling.m_z - nz * factor);
}

} // namespace math