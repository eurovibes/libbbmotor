/* SPDX-License-Identifier: LGPL-2.1 */
/* Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de> */

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
