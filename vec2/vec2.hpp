#ifndef VEC2_HPP
#define VEC2_HPP

namespace math {

struct point2 {
    float x;
    float y;
};

class vec2 {
  public:
    vec2();
    vec2(float x, float y);
    vec2(const point2 &point);
    vec2(const point2 &start, const point2 &end);
    vec2(const vec2 &other);

    float x() const;
    float y() const;

    float x(float x);
    float y(float y);

    vec2 operator+(const vec2 &other) const;
    vec2 operator-(const vec2 &other) const;
    vec2 operator*(float scalar) const;
    vec2 operator/(float scalar) const;

    vec2 &operator+=(const vec2 &other);
    vec2 &operator-=(const vec2 &other);
    vec2 &operator*=(float scalar);
    vec2 &operator/=(float scalar);

    float length() const;
    vec2 normalized() const;
    void normalize();

    vec2 rotated(float angle_rad) const;
    void rotate(float angle_rad);

    float dot_production(const vec2 &other) const;
    float cross_production(const vec2 &other) const;

    float angle_between(const vec2 &other) const;

    float distance_to(const vec2 &other) const;

    float project_on_vector(const vec2 &other) const;
    float reflect(const vec2 &normal) const;

    float x_axis_angle() const;
    float y_axis_angle() const;

    static vec2 zero();
    static vec2 one();

    static vec2 basis_i();
    static vec2 basis_j();

    static bool is_collinear(const vec2 &a, const vec2 &b, float &proportionality_coefficient);
    static bool is_orthogonal(const vec2 &a, const vec2 &b);

    static vec2 lerp(const vec2 &a, const vec2 &b, float t);

    static float angle_between(const vec2 &a, const vec2 &b);
    static float cross_production(const vec2 &a, const vec2 &b);

  private:
    float m_x, m_y;
};

} // namespace math

#endif // VEC2_HPP
