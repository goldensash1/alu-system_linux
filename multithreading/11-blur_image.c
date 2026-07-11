#include <pthread.h>
#include "multithreading.h"

#define NB_THREADS 16

/**
 * blur_thread - Thread entry point, blurs a single portion of the image
 *
 * @arg: Pointer to the blur_portion_t describing the portion to blur
 *
 * Return: NULL
 */
static void *blur_thread(void *arg)
{
	blur_portion(arg);
	return (NULL);
}

/**
 * blur_image - Blur a whole image using a Gaussian blur and several threads
 *
 * @img_blur: Address at which the blurred image is stored
 * @img:      Pointer to the source image
 * @kernel:   Pointer to the convolution kernel to use
 *
 * The image is split into up to NB_THREADS horizontal strips, each blurred
 * by its own thread. The last strip absorbs the remaining rows.
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	pthread_t threads[NB_THREADS];
	blur_portion_t portions[NB_THREADS];
	size_t i, nb, strip, rest;

	nb = NB_THREADS;
	if (img->h < nb)
		nb = img->h;
	if (nb == 0)
		return;
	strip = img->h / nb;
	rest = img->h % nb;
	for (i = 0; i < nb; i++)
	{
		portions[i].img = img;
		portions[i].img_blur = img_blur;
		portions[i].kernel = kernel;
		portions[i].x = 0;
		portions[i].w = img->w;
		portions[i].y = i * strip;
		portions[i].h = strip + (i == nb - 1 ? rest : 0);
		pthread_create(&threads[i], NULL, blur_thread, portions + i);
	}
	for (i = 0; i < nb; i++)
		pthread_join(threads[i], NULL);
}
