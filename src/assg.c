#include <stdio.h>
#include <stdlib.h>
#include "spheres.h"
#include "vector.h"
#include "float.h"
#include "math.h"
#include "color.h"


void write_ppm(const char *filename, int width, int height, unsigned char *data) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp,"P3\n%d %d\n255\n", width, height);
    for (int i = 0; i < width * height * 3; i += 3) {
        fprintf(fp,"%d %d %d ", data[i], data[i + 1], data[i + 2]);
        if ((i / 3 + 1) % width == 0) {
            fprintf(fp, "\n");
        }
    }
    fflush(fp);
    fclose(fp);
}

int compareSphereColor(const void *a, const void *b) {
    Sphere *sphereA = *(Sphere **)a;
    Sphere *sphereB = *(Sphere **)b;

    return compareColor(&sphereA->color, &sphereB->color);
}

int main(int argc, char *argv[]) {

    int width, height;
    float viewport_height;
    float focal_length;
    Vec3 light_position;
    float light_brightness;
    int m;
    int background_color_index;
    int n;
    Sphere **spheres;

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    
    fscanf(fp,"%d %d", &width, &height);
    fscanf(fp,"%f", &viewport_height);
    fscanf(fp,"%f", &focal_length);
    fscanf(fp,"%f %f %f", &light_position.x, &light_position.y, &light_position.z);
    fscanf(fp,"%f", &light_brightness);
    fscanf(fp,"%d", &m);
    int *colorsHex = malloc(m * sizeof(int));
    Vec3 *colors = malloc(m * sizeof(Vec3));

    for (int i = 0; i < m; i++) {
        int color;
        fscanf(fp,"%x", &color);
        colorsHex[i] = color;
        // TODO: Add color to color array
        // Vec3 spherecolor = unpackRGB(color);
        // colors[i].x = spherecolor.x;
        // colors[i].y = spherecolor.y;
        // colors[i].z = spherecolor.z;

    } 


    qsort(colorsHex, m, sizeof(int), compareColor);

    for (int i = 0; i < m; i++) {
        int color = colorsHex[i];
        Vec3 spherecolor = unpackRGB(color);
        colors[i].x = spherecolor.x;
        colors[i].y = spherecolor.y;
        colors[i].z = spherecolor.z;

    } 




    fscanf(fp,"%d", &background_color_index);
    fscanf(fp,"%d", &n);
    spheres = malloc(n * sizeof(Sphere));
    World world;
    worldInit(&world);

    for (int i = 0; i < n; i++) {
        float x, y, z, r;
        int color_index;
        fscanf(fp,"%f %f %f %f", &x, &y, &z, &r);
        fscanf(fp,"%x", &color_index);
        // unpack hex color

        Vec3 position;
        position.x = x;
        position.y = y;
        position.z = z;
        Sphere* new_sphere = createSphere(r, position, colors[color_index]);

        addSphere(&world, new_sphere);

    }

    fclose(fp);

    fp = fopen("faltu", "w");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", argv[2]);
        return 1;
    }
    
    Vec3 background_color = colors[background_color_index];

    // Vec3 results = add(background_color, light_position);

    // fprintf(fp,"(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
    //     background_color.x, background_color.y, background_color.z,
    //     light_position.x, light_position.y, light_position.z,
    //     results.x, results.y, results.z);

    // results = subtract(background_color, light_position);
    // fprintf(fp,"(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
    //     background_color.x, background_color.y, background_color.z,
    //     light_position.x, light_position.y, light_position.z,
    //     results.x, results.y, results.z);

    float viewport_width = (float)(viewport_height) * ((float)(width) / (float)height);
    // results = scalarMultiply(viewport_width, light_position);
    // fprintf(fp,"%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
    //     viewport_width, light_position.x, light_position.y, light_position.z,
    //     results.x, results.y, results.z);
    // results = normalize(light_position);
    // fprintf(fp,"normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
    //     light_position.x, light_position.y, light_position.z,
    //     results.x, results.y, results.z);

    // fprintf(fp,"\n");

    for (int i = 0; i < world.size; i++) {
        Sphere *sphere = world.spheres[i];
        fprintf(fp,"(%.1f, %.1f, %.1f) / %.1f = (%.1f, %.1f, %.1f)\n",
            sphere->color.x, sphere->color.y, sphere->color.z,
            sphere->r,
            sphere->color.x / sphere->r, sphere->color.y / sphere->r, sphere->color.z / sphere->r);
        fprintf(fp,"dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
            light_position.x, light_position.y, light_position.z,
            sphere->pos.x, sphere->pos.y, sphere->pos.z,
            dot(sphere->pos, light_position));
            
        fprintf(fp,"distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
            light_position.x, light_position.y, light_position.z,
            sphere->pos.x, sphere->pos.y, sphere->pos.z,
            distance(sphere->pos, light_position));
        fprintf(fp,"length(%.1f, %.1f, %.1f) = %.1f\n",
            sphere->pos.x, sphere->pos.y, sphere->pos.z,
            length(sphere->pos));
        if (i != world.size - 1) {
            fprintf(fp,"\n");
        }
    }

    Vec3 ray;
    Vec3 cameraPosition = {0, 0, 0};

    // Allocate memory for the image data
    unsigned char *data = (unsigned char *)malloc(3 * width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vec3 colormixer = {0, 0, 0};

            for (int c = -1; c <= 1; c++) {
                for (int d = -1; d <= 1; d++) {
                    float newX = (d / 3.0f); 
                    float newY = (c / 3.0f); 
                    float i = (x - width / 2.0f + newX) * viewport_width / width;
                    float j = (y - height / 2.0f + newY) * viewport_height / height;
                    ray.x = i;
                    ray.y = j;
                    ray.z = -focal_length;
                    // int idx = ((height - y - 1) * width + x) * 3; // Calculate pixel index
                    Vec3 rayDir = normalize(ray); // Normalize ray direction

                    float closest_t = FLT_MAX;
                    int closest_sphere_index = -1;

                    for (int s = 0; s < world.size; s++) {
                        float t;
                        if (doesIntersect(world.spheres[s], cameraPosition, rayDir, &t) && t < closest_t) {
                            closest_t = t;
                            closest_sphere_index = s;
                        }
                    }

                    if (closest_sphere_index != -1) {
                        Sphere *sphere = world.spheres[closest_sphere_index];
                        Vec3 intersectionPoint = add(cameraPosition, scalarMultiply(closest_t, rayDir));
                        Vec3 normal = normalize(subtract(intersectionPoint, sphere->pos));
                        Vec3 lightDir = normalize(subtract(light_position, intersectionPoint));
                        float light_distance = length(subtract(light_position, intersectionPoint));

                        int isShadowed = 0;
                        for (int s = 0; s < world.size; s++) {
                            if (s == closest_sphere_index) {
                                continue; // Skip the current sphere
                            }

                            float shadow_t;
                            if (doesIntersect(world.spheres[s], intersectionPoint, lightDir, &shadow_t) &&
                                shadow_t > 1e-3 && shadow_t < light_distance) {
                                isShadowed = 1;
                                break;
                            }
                        }

                        float light_intensity = 0.04f;
                        if (!isShadowed) {
                            float dotProduct = fmax(0.0f, dot(normal, lightDir));
                            float distance_squared = light_distance * light_distance;
                            light_intensity = fmin(1.0f, (light_brightness * dotProduct) / distance_squared);
                        }

                        Vec3 surfaceLightingColor = scalarMultiply(light_intensity, sphere->color);
                        colormixer = add(colormixer, surfaceLightingColor); // mix my colors
                    } else {
                        colormixer = add(colormixer, background_color);
                    }
                }
            }

            // Average the colors from all 9 samples
            Vec3 finalColor = scalarMultiply(1.0f / 9.0f, colormixer);

            // Store the averaged color in the image data
            int idx = ((height - y - 1) * width + x) * 3;
            data[idx] = (unsigned char)(fmin(255.0f, finalColor.x * 255));
            data[idx + 1] = (unsigned char)(fmin(255.0f, finalColor.y * 255));
            data[idx + 2] = (unsigned char)(fmin(255.0f, finalColor.z * 255));
        }
    }   


    write_ppm(argv[2], width, height, data);
    
    free(colors);
    free(colorsHex);
    fflush(fp);
    fclose(fp);
    free(spheres);
    free(data);
    freeWorld(&world);
    return 0;
}