#ifndef VEC3_HPP
#define VEC3_HPP

#include "../vec2/vec2.hpp"

namespace math {
class vec3 {
  public:
    vec3();
    vec3(float x, float y, float z, bool normalize = false);
    vec3(const vec2 &xy, float z, bool normalize = false);

    float x() const;
    float y() const;
    float z() const;

    float x(float x);
    float y(float y);
    float z(float z);

    vec3 &operator=(const vec3 &other);

    vec3 &operator+=(const vec3 &other);
    vec3 &operator-=(const vec3 &other);
    vec3 operator+(const vec3 &other) const;
    vec3 operator-(const vec3 &other) const;

    vec3 &operator*=(float scalar);
    vec3 &operator/=(float scalar);
    vec3 operator*(float scalar) const;
    vec3 operator/(float scalar) const;

    float dot_production(const vec3 &other) const;
    vec3 cross_production(const vec3 &other) const;

    float distance_to(const vec3 &other) const;

    float length() const;
    vec3 normalized() const;
    void normalize();
    vec3 reflect(const vec3 &normal) const;
    float angle_between(const vec3 &other) const;

    float x_axis_angle() const;
    float y_axis_angle() const;
    float z_axis_angle() const;

    vec3 project_on_vector(const vec3 &other) const;

    vec3 rotated_around_axis(const vec3 &axis, float angle_rad) const;
    void rotate_around_axis(const vec3 &axis, float angle_rad);

    vec2 to_vec2_orthographic() const;
    vec2 to_vec2_perspective(float focal_length = 1.0f) const;

    static vec3 zero();
    static vec3 one();

    static vec3 basis_i();
    static vec3 basis_j();
    static vec3 basis_k();

    static bool is_collinear(const vec3 &a, const vec3 &b, float &proportionality_coefficient);
    static bool is_coplanar(const vec3 &a, const vec3 &b, const vec3 &c);

    static bool is_orthogonal(const vec3 &a, const vec3 &b);
    static bool is_parallel(const vec3 &a, const vec3 &b);

    static vec3 lerp(const vec3 &a, const vec3 &b, float t);

    static float angle_between(const vec3 &a, const vec3 &b);

    static vec3 cross_production(const vec3 &a, const vec3 &b);
    static float dot_production(const vec3 &a, const vec3 &b);
    static float mixed_production(const vec3 &a, const vec3 &b, const vec3 &c);

    static vec3 project_vector(const vec3 &source, const vec3 &on_target);
    static vec3 reflect_vector(const vec3 &falling, const vec3 &normal);

  private:
    float m_x, m_y, m_z;
};

vec3 l_inf_normalize(const vec3 &v);
vec3 l_one_normalize(const vec3 &v);
vec3 static_normalize(const vec3 &v);

} // namespace math
#endif // VEC3_HPP
