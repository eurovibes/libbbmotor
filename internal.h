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

ssize_t i2c_write_u8 (int fd, uint8_t reg, uint8_t val);
ssize_t i2c_write_u16 (int fd, uint8_t reg, uint16_t val);
ssize_t i2c_write_u32 (int fd, uint8_t reg, uint32_t val);

#endif /* INTERNAL_H */
