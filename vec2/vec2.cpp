#include "vec2.hpp"
#include <cmath>

math::vec2::vec2() {
    m_x = 0.0f;
    m_y = 0.0f;
}

math::vec2::vec2(float x, float y) {
    m_x = x;
    m_y = y;
}

math::vec2::vec2(const point2& point)
{
    m_x = point.x;
    m_y = point.y;
}

math::vec2::vec2(const point2& start, const point2& end)
{
    m_x = end.x - start.x;
    m_y = end.y - start.y;
}

math::vec2::vec2(const vec2& other)
{
    m_x = other.m_x;
    m_y = other.m_y;
}

float math::vec2::x() const { return m_x; }

float math::vec2::y() const { return m_y; }

float math::vec2::x(float x) {
    m_x = x;
    return m_x;
}

float math::vec2::y(float y) {
    m_y = y;
    return m_y;
}

math::vec2 math::vec2::operator+(const vec2 &other) const {
    return vec2(m_x + other.m_x, m_y + other.m_y);
}

math::vec2 math::vec2::operator-(const vec2 &other) const {
    return vec2(m_x - other.m_x, m_y - other.m_y);
}

math::vec2 math::vec2::operator*(float scalar) const { return vec2(m_x * scalar, m_y * scalar); }

math::vec2 math::vec2::operator/(float scalar) const { return vec2(m_x / scalar, m_y / scalar); }

math::vec2 &math::vec2::operator+=(const vec2 &other) {
    m_x += other.m_x;
    m_y += other.m_y;
    return *this;
}

math::vec2 &math::vec2::operator-=(const vec2 &other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    return *this;
}

math::vec2 &math::vec2::operator*=(float scalar) {
    m_x *= scalar;
    m_y *= scalar;
    return *this;
}

math::vec2 &math::vec2::operator/=(float scalar) {
    m_x /= scalar;
    m_y /= scalar;
    return *this;
}

float math::vec2::length() const { return std::sqrt(m_x * m_x + m_y * m_y); }

math::vec2 math::vec2::normalized() const {
    float len_sq = m_x * m_x + m_y * m_y;
    if (len_sq < 1e-8f)
        return vec2(0.0f, 0.0f);
    float inv_len = 1.0f / std::sqrt(len_sq);
    return vec2(m_x * inv_len, m_y * inv_len);
}

void math::vec2::normalize() {
    float len_sq = m_x * m_x + m_y * m_y;
    if (len_sq < 1e-8f) {
        m_x = 0.0f;
        m_y = 0.0f;
        return;
    }
    float inv_len = 1.0f / std::sqrt(len_sq);
    m_x *= inv_len;
    m_y *= inv_len;
}

math::vec2 math::vec2::rotated(float angle_rad) const {
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    return vec2(m_x * cos_a - m_y * sin_a, m_x * sin_a + m_y * cos_a);
}

void math::vec2::rotate(float angle_rad) {
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    float new_x = m_x * cos_a - m_y * sin_a;
    float new_y = m_x * sin_a + m_y * cos_a;
    m_x = new_x;
    m_y = new_y;
}

float math::vec2::dot_production(const vec2 &other) const {
    return m_x * other.m_x + m_y * other.m_y;
}

float math::vec2::cross_production(const vec2 &other) const {
    return m_x * other.m_y - m_y * other.m_x;
}

float math::vec2::angle_between(const vec2 &other) const {
    float dot_p = m_x * other.m_x + m_y * other.m_y;
    float len_sq_a = m_x * m_x + m_y * m_y;
    float len_sq_b = other.m_x * other.m_x + other.m_y * other.m_y;
    float len_product = std::sqrt(len_sq_a * len_sq_b);
    if (len_product < 1e-8f) {
        return 0.0f;
    }
    return std::acos(dot_p / len_product);
}

float math::vec2::distance_to(const vec2 &other) const {
    float dx = m_x - other.m_x;
    float dy = m_y - other.m_y;
    return std::sqrt(dx * dx + dy * dy);
}

float math::vec2::project_on_vector(const vec2 &other) const {
    float dot_p = m_x * other.m_x + m_y * other.m_y;
    float other_len_sq = other.m_x * other.m_x + other.m_y * other.m_y;
    if (other_len_sq < 1e-8f) {
        return 0.0f;
    }
    return dot_p / std::sqrt(other_len_sq);
}

float math::vec2::reflect(const vec2 &normal) const {
    float len_sq = normal.m_x * normal.m_x + normal.m_y * normal.m_y;
    if (len_sq < 1e-8f) {
        return 0.0f;
    }
    float inv_len = 1.0f / std::sqrt(len_sq);
    float nx = normal.m_x * inv_len;
    float ny = normal.m_y * inv_len;

    float dot = m_x * nx + m_y * ny;
    float factor = 2.0f * dot;

    return vec2(m_x - nx * factor, m_y - ny * factor).length();
}

float math::vec2::x_axis_angle() const { return std::acos(m_x / this->length()); }
float math::vec2::y_axis_angle() const { return std::acos(m_y / this->length()); }

math::vec2 math::vec2::zero() { return vec2(0.0f, 0.0f); }
math::vec2 math::vec2::one() { return vec2(1.0f, 1.0f); }

math::vec2 math::vec2::basis_i() { return vec2(1.0f, 0.0f); }
math::vec2 math::vec2::basis_j() { return vec2(0.0f, 1.0f); }

bool math::vec2::is_collinear(const vec2 &a, const vec2 &b, float &proportionality_coefficient) {
    proportionality_coefficient = a.m_x / b.m_x;
    if (a.m_y * proportionality_coefficient == b.m_y) {
        return true;
    }
    proportionality_coefficient = 0;
    return false;
}

bool math::vec2::is_orthogonal(const vec2 &a, const vec2 &b) {
    return (a.m_x * b.m_x + a.m_y * b.m_y) == 0.0f;
}

math::vec2 math::vec2::lerp(const vec2 &a, const vec2 &b, float t) {
    return vec2(a.m_x + (b.m_x - a.m_x) * t, a.m_y + (b.m_y - a.m_y) * t);
}

float math::vec2::angle_between(const vec2 &a, const vec2 &b) {
    float dot_p = a.m_x * b.m_x + a.m_y * b.m_y;
    float len_sq_a = a.m_x * a.m_x + a.m_y * a.m_y;
    float len_sq_b = b.m_x * b.m_x + b.m_y * b.m_y;
    float len_product = std::sqrt(len_sq_a * len_sq_b);
    if (len_product < 1e-8f) {
        return 0.0f;
    }
    return std::acos(dot_p / len_product);
}

float math::vec2::cross_production(const vec2 &a, const vec2 &b) {
    return a.m_x * b.m_y - a.m_y * b.m_x;
}
