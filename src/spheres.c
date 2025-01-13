#include "spheres.h"

#include <stdlib.h>

#include "math.h"

void worldInit(World *world) {
    world->spheres = NULL;
    world->size = 0;
    world->capacity = 0;
}

void freeWorld(World *world) {
    for (int i = 0; i < world->size; i++) {
        free(world->spheres[i]);
    }
    free(world->spheres);
}

void addSphere(World *world, Sphere *sphere) {
    if (world->capacity == 0) {
        world->capacity = 1;
        world->spheres = malloc(world->capacity * sizeof(Sphere));
    }
    if (world->size == world->capacity) {
        world->capacity *= 2;
        world->spheres = realloc(world->spheres, world->capacity * sizeof(Sphere));
    }
    world->spheres[world->size++] = sphere;
}

Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *sphere = malloc(sizeof(Sphere));
    sphere->r = radius;
    sphere->pos = position;
    sphere->color = color;
    return sphere;
}

int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
Vec3 V = subtract(rayPos, sphere->pos);
    float a = dot(rayDir, rayDir);
    float b = 2 * dot(rayDir, V);
    float c = dot(V, V) - sphere->r * sphere->r;
    float discriminant = b * b - 4 * a * c;

    // Check the discriminant to determine intersection
    if (discriminant < 0) {
        return 0; // No intersection
    }

    // Compute both roots of the quadratic equation
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2 * a);
    float t2 = (-b + sqrtDiscriminant) / (2 * a);

    // Determine the smallest positive root (if any)
    if (t1 > 0 && t2 > 0) {
        *t = (t1 < t2) ? t1 : t2;
        return 1; // Intersection at the closest point
    }
    if (t1 > 0) {
        *t = t1;
        return 1; // Intersection at t1
    }
    if (t2 > 0) {
        *t = t2;
        return 1; // Intersection at t2
    }

    // Both roots are negative (intersection behind the ray)
    return 0;
}