# Ray Tracing Sphere Renderer

This project is a simple ray tracing implementation written in C that renders a 3D scene featuring spheres. The output is saved as a PPM image file, showcasing the rendered scene with shading and colors.

## Features

- Implements a basic ray tracing algorithm.
- Renders spheres in a 3D world with realistic shading and color computation.
- Outputs the rendered scene in the PPM image format.

## How It Works

1. **Ray Tracing Algorithm**: The program casts rays from the camera into the scene to detect intersections with objects (spheres).
2. **Shading and Colors**: Calculates the color of each pixel based on object properties and lighting.
3. **PPM Output**: Generates the image in the portable pixmap format, which can be viewed using image viewers supporting `.ppm`.

## Getting Started

### Prerequisites

- A C compiler (e.g., GCC or Clang)
- Basic familiarity with compiling and running C programs

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/raytracing.git
   cd raytracing

