
# Barnsley-Vicsek Fractal Algorithm

This document explains the algorithm used in the `barnsley_vicsek_fractal.c` file to generate a fractal image combining the Barnsley Fern and Vicsek Fractal.

## Overview

The algorithm generates a fractal image by iteratively applying transformations to a point. The transformations are chosen randomly between those defining the Barnsley Fern and the Vicsek Fractal.

## Steps

1. **Initialization**:
    - Define the image dimensions (`WIDTH` and `HEIGHT`).
    - Set the number of iterations (`ITERATIONS`).
    - Initialize the starting point (`Point p = {0, 0}`).

2. **Drawing Points**:
    - The `draw_point` function maps the fractal coordinates to image coordinates and colors the corresponding pixel.

3. **Barnsley Fern Transformation**:
    - The `barnsley_fern` function applies one of four affine transformations to the point based on a random number.

4. **Vicsek Fractal Transformation**:
    - The `vicsek_fractal` function applies one of five transformations to the point based on a random number.

5. **Combining Transformations**:
    - The `barnsley_vicsek_fractal` function randomly chooses between the Barnsley Fern and Vicsek Fractal transformations and applies it to the point.

6. **Main Loop**:
    - Iterate `ITERATIONS` times, applying the combined transformation and drawing the resulting point on the image.

7. **Saving the Image**:
    - Use the `stb_image_write` library to save the generated image as a PNG file.

## Functions

### `draw_point`

Maps the fractal coordinates to image coordinates and colors the corresponding pixel.

### `barnsley_fern`

Applies one of four affine transformations to the point based on a random number.

### `vicsek_fractal`

Applies one of five transformations to the point based on a random number.

### `barnsley_vicsek_fractal`

Randomly chooses between the Barnsley Fern and Vicsek Fractal transformations and applies it to the point.

## Example

The following code snippet shows the main loop where the transformations are applied and points are drawn:

```c
Point p = { 0, 0 };
for (int i = 0; i < ITERATIONS; i++) {
    barnsley_vicsek_fractal(&p);
    draw_point(image, p, color);
}
```

This loop iterates `ITERATIONS` times, applying the combined transformation and drawing the resulting point on the image.

## Conclusion

The Barnsley-Vicsek Fractal algorithm combines the beauty of the Barnsley Fern and the Vicsek Fractal to create a unique fractal image. By iteratively applying random transformations, the algorithm generates complex and intricate patterns.
