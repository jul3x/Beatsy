//
// Created by julex on 03.11.2020.
//

#ifndef BEATSY_UTILS_H
#define BEATSY_UTILS_H

class Vec3 {
public:
    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(Vec3 const &obj) {
        return Vec3(x + obj.x, y + obj.y, z + obj.z);
    }

    Vec3 operator-(Vec3 const &obj) {
        return Vec3(x - obj.x, y - obj.y, z - obj.z);
    }

    float operator*(Vec3 const &obj) {
        return x * obj.x + y * obj.y + z * obj.z;
    }

    Vec3 operator*(float scalar) {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator/(float scalar) {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    float x{0.f}, y{0.f}, z{0.f};
};


#endif //BEATSY_UTILS_H
