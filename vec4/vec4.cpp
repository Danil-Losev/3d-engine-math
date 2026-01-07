#include "vec4.hpp"
#include <cmath>

math::vec4::vec4() : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_w(0.0f) {}
math::vec4::vec4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

math::vec4::vec4(const vec3 &v, float w) : m_x(v.x()), m_y(v.y()), m_z(v.z()), m_w(w) {}

float math::vec4::x() const { return m_x; }

float math::vec4::y() const { return m_y; }

float math::vec4::z() const { return m_z; }

float math::vec4::w() const { return m_w; }

float math::vec4::x(float x) {
    m_x = x;
    return m_x;
}

float math::vec4::y(float y) {
    m_y = y;
    return m_y;
}
float math::vec4::z(float z) {
    m_z = z;
    return m_z;
}

float math::vec4::w(float w) {
    m_w = w;
    return m_w;
}

math::vec4 math::vec4::operator+(const vec4 &other) const {
    return vec4(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

math::vec4 math::vec4::operator-(const vec4 &other) const {
    return vec4(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

math::vec4 math::vec4::operator*(float scalar) const {
    return vec4(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
}

math::vec4 math::vec4::operator/(float scalar) const {
    return vec4(m_x / scalar, m_y / scalar, m_z / scalar, m_w / scalar);
}

math::vec4 &math::vec4::operator+=(const vec4 &other) {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    m_w += other.m_w;
    return *this;
}

math::vec4 &math::vec4::operator-=(const vec4 &other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;
    m_w -= other.m_w;
    return *this;
}

math::vec4 &math::vec4::operator*=(float scalar) {
    m_x *= scalar;
    m_y *= scalar;
    m_z *= scalar;
    m_w *= scalar;
    return *this;
}

math::vec4 &math::vec4::operator/=(float scalar) {
    m_x /= scalar;
    m_y /= scalar;
    m_z /= scalar;
    m_w /= scalar;
    return *this;
}

float math::vec4::length() const {
    return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

math::vec4 math::vec4::normalized() const {
    float len_sq = m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
    if (len_sq == 0.0f) [[unlikely]] {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    float invlen = 1.0f / std::sqrt(len_sq);
    return vec4(m_x * invlen, m_y * invlen, m_z * invlen, m_w * invlen);
}

void math::vec4::normalize() {
    float len_sq = m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
    if (len_sq == 0.0f) [[unlikely]] {
        m_x = 0.0f;
        m_y = 0.0f;
        m_z = 0.0f;
        m_w = 0.0f;
        return;
    }
    float invlen = 1.0f / std::sqrt(len_sq);
    m_x *= invlen;
    m_y *= invlen;
    m_z *= invlen;
    m_w *= invlen;
}

float math::vec4::angle_between(const vec4 &other) const {
    float dot = this->dot_production(other);
    float len_sq = m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
    float other_len_sq = other.m_x * other.m_x + other.m_y * other.m_y + other.m_z * other.m_z +
                         other.m_w * other.m_w;
    float len_product = std::sqrt(len_sq * other_len_sq);
    if (len_product == 0.0f) [[unlikely]] {
        return 0.0f;
    }
    float cos_angle = dot / len_product;
    return std::acos(cos_angle);
}

float math::vec4::x_axis_angle() const { return std::acos(m_x / this->length()); }

float math::vec4::y_axis_angle() const { return std::acos(m_y / this->length()); }

float math::vec4::z_axis_angle() const { return std::acos(m_z / this->length()); }

float math::vec4::w_axis_angle() const { return std::acos(m_w / this->length()); }

math::vec4 math::vec4::project_on_vector(const vec4 &other) const {
    float other_len_sq = other.m_x * other.m_x + other.m_y * other.m_y + other.m_z * other.m_z +
                         other.m_w * other.m_w;
    if (other_len_sq == 0.0f) [[unlikely]] {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    float dot = this->dot_production(other);
    float scalar = dot / other_len_sq;
    return other * scalar;
}

math::vec3 math::vec4::to_vec3_orthographic() const { return vec3(m_x, m_y, m_z); }

math::vec3 math::vec4::to_vec3_perspective(float focal_length) const {
    if (m_w == 0.0f) [[unlikely]] {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float inv_w = focal_length / m_w;
    return vec3(m_x * inv_w, m_y * inv_w, m_z * inv_w);
}

float math::vec4::dot_production(const vec4 &other) const {
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
}

float math::vec4::distance_to(const vec4 &other) const {
    float dx = m_x - other.m_x;
    float dy = m_y - other.m_y;
    float dz = m_z - other.m_z;
    float dw = m_w - other.m_w;
    return std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

math::vec4 math::vec4::lerp(const vec4 &other, float t) const {
    return vec4(m_x + (other.m_x - m_x) * t, m_y + (other.m_y - m_y) * t,
                m_z + (other.m_z - m_z) * t, m_w + (other.m_w - m_w) * t);
}

math::vec4 math::vec4::reflect(const vec4 &normal) const {
    float len_sq = normal.m_x * normal.m_x + normal.m_y * normal.m_y + normal.m_z * normal.m_z +
                   normal.m_w * normal.m_w;
    if (len_sq == 0.0f) [[unlikely]] {
        return *this;
    }
    float inv_len = 1.0f / std::sqrt(len_sq);
    float nx = normal.m_x * inv_len;
    float ny = normal.m_y * inv_len;
    float nz = normal.m_z * inv_len;
    float nw = normal.m_w * inv_len;

    float dot = m_x * nx + m_y * ny + m_z * nz + m_w * nw;
    float factor = 2.0f * dot;

    return vec4(m_x - factor * nx, m_y - factor * ny, m_z - factor * nz, m_w - factor * nw);
}

math::vec4 math::vec4::zero() { return vec4(0.0f, 0.0f, 0.0f, 0.0f); }

math::vec4 math::vec4::one() { return vec4(1.0f, 1.0f, 1.0f, 1.0f); }

math::vec4 math::vec4::basis_i() { return vec4(1.0f, 0.0f, 0.0f, 0.0f); }

math::vec4 math::vec4::basis_j() { return vec4(0.0f, 1.0f, 0.0f, 0.0f); }

math::vec4 math::vec4::basis_k() { return vec4(0.0f, 0.0f, 1.0f, 0.0f); }

math::vec4 math::vec4::basis_l() { return vec4(0.0f, 0.0f, 0.0f, 1.0f); }

float math::vec4::dot_production(const vec4 &a, const vec4 &b) {
    return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z + a.m_w * b.m_w;
}

math::vec4 math::vec4::cross_production_exp(const vec4 &a, const vec4 &b, const vec4 &c) {
    float x = a.m_y * (b.m_z * c.m_w - c.m_z * b.m_w) - a.m_z * (b.m_y * c.m_w - c.m_y * b.m_w) +
              a.m_w * (b.m_y * c.m_z - c.m_y * b.m_z);
    float y = a.m_x * (c.m_z * b.m_w - b.m_z * c.m_w) - a.m_z * (c.m_x * b.m_w - b.m_x * c.m_w) +
              a.m_w * (c.m_x * b.m_z - b.m_x * c.m_z);
    float z = a.m_x * (b.m_y * c.m_w - c.m_y * b.m_w) - a.m_y * (b.m_x * c.m_w - c.m_x * b.m_w) +
              a.m_w * (b.m_x * c.m_y - c.m_x * b.m_y);
    float w = a.m_x * (c.m_y * b.m_z - b.m_y * c.m_z) - a.m_y * (c.m_x * b.m_z - b.m_x * c.m_z) +
              a.m_z * (c.m_x * b.m_y - b.m_x * c.m_y);
    return vec4(x, y, z, w);
}