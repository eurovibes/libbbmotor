#ifndef INTERNAL_H
#define INTERNAL_H

#include <gpiod.h>

struct motorcape_t
{
	int i2c_fd;
	struct gpiod_line *boot0;
	struct gpiod_line *nrst;
};

int gpio_init (struct motorcape_t *han);
int gpio_release (struct motorcape_t *han);

#endif /* INTERNAL_H */
