#include "vector.h"
#include <math.h>

Vec3 add(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

Vec3 subtract(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

Vec3 scalarMultiply(float s, Vec3 v) {
    Vec3 result;
    result.x = s * v.x;
    result.y = s * v.y;
    result.z = s * v.z;
    return result;
}

Vec3 scalarDivide(Vec3 v, float d) {
    Vec3 result;
    result.x = v.x / d;
    result.y = v.y / d;
    result.z = v.z / d;
    return result;
}


float dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float length2(Vec3 v) {
    return dot(v, v);
}

float length(Vec3 v) {
    return sqrt(length2(v));
}


Vec3 normalize(Vec3 v) {
    float length_ = length(v);
    return scalarDivide(v, length_);
}

float distance2(Vec3 v1, Vec3 v2) {
    return length2(subtract(v1, v2));
}

float distance(Vec3 v1, Vec3 v2) {
    return sqrt(distance2(v1, v2));
}