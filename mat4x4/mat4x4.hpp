#ifndef MAT4X4_HPP
#define MAT4X4_HPP

#include "../vec4/vec4.hpp"

#include <iostream>
#include <optional>

namespace math {
class mat4x4 {
  public:
    mat4x4();
    mat4x4(const float (&elements)[4][4]);
    mat4x4(const mat4x4 &other);

    mat4x4 &operator=(const mat4x4 &other);

    mat4x4 operator+(const mat4x4 &other) const;
    mat4x4 operator-(const mat4x4 &other) const;
    mat4x4 operator*(const mat4x4 &other) const;
    mat4x4 &operator+=(const mat4x4 &other);
    mat4x4 &operator-=(const mat4x4 &other);
    mat4x4 &operator*=(const mat4x4 &other);

    mat4x4 operator+(const float scalar) const;
    mat4x4 operator-(const float scalar) const;
    mat4x4 operator*(const float scalar) const;
    mat4x4 &operator+=(const float scalar);
    mat4x4 &operator-=(const float scalar);
    mat4x4 &operator*=(const float scalar);

    vec4 operator*(const vec4& vector) const;

    float &at(int row, int col);
    const float &at(int row, int col) const;

    mat4x4 transpose() const;
    mat4x4 inverse() const;
    double determinant() const;

    static mat4x4 identity();
    static mat4x4 zero();

    static mat4x4 translation(float tx, float ty, float tz);
    static mat4x4 scaling(float sx, float sy, float sz);
    static mat4x4 rotation_x(float angle_rad);
    static mat4x4 rotation_y(float angle_rad);
    static mat4x4 rotation_z(float angle_rad);
    static mat4x4 rotation_axis_angle_intrinsic(std::optional<float> angle_rad_x,
        std::optional<float> angle_rad_y,
        std::optional<float> angle_rad_z);
    static mat4x4 rotation_axis_angle_extrinsic(std::optional<float> angle_rad_x,
        std::optional<float> angle_rad_y,
        std::optional<float> angle_rad_z);

    static mat4x4 make_model_matrix(
        const mat4x4& translation, const mat4x4& rotation, const mat4x4& scaling);

    // static mat4x4 perspective(float fov_rad, float aspect, float near, float far);
    // static mat4x4 orthographic(float left, float right, float bottom, float top, float near,
    // float far);
    // static mat4x4 look_at(const float eye[3], const float center[3], const float
    // up[3]);
    // static mat4x4 frustum(float left, float right, float bottom, float top, float near,
    // float far);
    // static mat4x4 viewport(int x, int y, int width, int height, float min_depth,
    // float max_depth);

private:
    alignas(32) float m_matrix[4][4];
};

std::ostream &operator<<(std::ostream &os, const math::mat4x4 &matrix);

inline float from_degrees_to_radians(float degrees)
{
    return degrees * (3.14159265358979323846f / 180.0f);
}

} // namespace math

#endif // MAT4X4_HPP