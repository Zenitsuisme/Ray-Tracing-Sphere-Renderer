#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "color.h"

Vec3 unpackRGB(unsigned int packedRGB) {
    Vec3 color;
    color.x = ((packedRGB & 0xFF0000) >> 16) / 255.0f;
    color.y = ((packedRGB & 0x00FF00) >> 8) / 255.0f;
    color.z = (packedRGB & 0x0000FF) / 255.0f;
    return color;
}

void writeColour(FILE *ppmFile, Vec3 color) {
    // Ensure color values are clamped to the [0, 1] range to avoid overflow
    float r = fmax(0.0f, fmin(1.0f, color.x));
    float g = fmax(0.0f, fmin(1.0f, color.y));
    float b = fmax(0.0f, fmin(1.0f, color.z));

    // Scale the color to the [0, 255] range for PPM format
    int ir = (int)(r * 255.0f);
    int ig = (int)(g * 255.0f);
    int ib = (int)(b * 255.0f);

    // Write the color in "R G B" format
    fprintf(ppmFile, "%d %d %d ", ir, ig, ib);
}

int compareColor(const void *a, const void *b) {

    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++)
    {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }
    
    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}