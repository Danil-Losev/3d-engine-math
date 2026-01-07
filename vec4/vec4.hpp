#ifndef VEC4_HPP
#define VEC4_HPP

#include "../vec3/vec3.hpp"

namespace math {

class vec4 {
  public:
    vec4();
    vec4(float x, float y, float z, float w);
    vec4(const vec3 &v, float w);
    float x() const;
    float y() const;
    float z() const;
    float w() const;

    float x(float x);
    float y(float y);
    float z(float z);
    float w(float w);

    vec4 operator+(const vec4 &other) const;
    vec4 operator-(const vec4 &other) const;
    vec4 operator*(float scalar) const;
    vec4 operator/(float scalar) const;

    vec4 &operator+=(const vec4 &other);
    vec4 &operator-=(const vec4 &other);
    vec4 &operator*=(float scalar);
    vec4 &operator/=(float scalar);

    float length() const;
    vec4 normalized() const;
    void normalize();

    float angle_between(const vec4 &other) const;

    float x_axis_angle() const;
    float y_axis_angle() const;
    float z_axis_angle() const;
    float w_axis_angle() const;

    vec4 project_on_vector(const vec4 &other) const;

    vec3 to_vec3_orthographic() const;
    vec3 to_vec3_perspective(float focal_length = 1.0f) const;

    float dot_production(const vec4 &other) const;
    float distance_to(const vec4 &other) const;

    vec4 lerp(const vec4 &other, float t) const;
    vec4 reflect(const vec4 &normal) const;

    static vec4 zero();
    static vec4 one();

    static vec4 basis_i();
    static vec4 basis_j();
    static vec4 basis_k();
    static vec4 basis_l();

    static float dot_production(const vec4 &a, const vec4 &b);
    static vec4 cross_production_exp(const vec4 &a, const vec4 &b, const vec4 &c);

  private:
    float m_x, m_y, m_z, m_w;
};

} // namespace math

#endif // VEC4_HPP
