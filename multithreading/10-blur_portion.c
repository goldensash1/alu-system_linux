#include "multithreading.h"

/**
 * blur_pixel - Compute the blurred value of a single pixel
 *
 * @portion: Description of the portion being blurred
 * @px:      X coordinate of the pixel in the image
 * @py:      Y coordinate of the pixel in the image
 *
 * The convolution kernel is centered on the pixel. Contributions coming
 * from pixels outside the image are ignored and the accumulated weight is
 * adjusted accordingly so borders stay normalized.
 */
static void blur_pixel(blur_portion_t const *portion, size_t px, size_t py)
{
	img_t const *img = portion->img;
	kernel_t const *kernel = portion->kernel;
	size_t kx, ky, half = kernel->size / 2;
	long ix, iy;
	double r = 0, g = 0, b = 0, weight = 0, k;
	pixel_t *dst;

	for (ky = 0; ky < kernel->size; ky++)
		for (kx = 0; kx < kernel->size; kx++)
		{
			ix = (long)(px + kx) - (long)half;
			iy = (long)(py + ky) - (long)half;
			if (ix < 0 || iy < 0 || (size_t)ix >= img->w ||
				(size_t)iy >= img->h)
				continue;
			k = kernel->matrix[ky][kx];
			r += img->pixels[iy * img->w + ix].r * k;
			g += img->pixels[iy * img->w + ix].g * k;
			b += img->pixels[iy * img->w + ix].b * k;
			weight += k;
		}
	dst = portion->img_blur->pixels + (py * img->w + px);
	dst->r = (uint8_t)(r / weight);
	dst->g = (uint8_t)(g / weight);
	dst->b = (uint8_t)(b / weight);
}

/**
 * blur_portion - Blur a rectangular portion of an image using a Gaussian blur
 *
 * @portion: Pointer to the structure describing the portion to blur
 */
void blur_portion(blur_portion_t const *portion)
{
	size_t px, py;

	for (py = portion->y; py < portion->y + portion->h; py++)
		for (px = portion->x; px < portion->x + portion->w; px++)
			blur_pixel(portion, px, py);
}
