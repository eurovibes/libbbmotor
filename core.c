// SPDX-License-Identifier: LGPL-2.1
// Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <linux/i2c-dev.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "bbmotor.h"
#include "com.h"
#include "internal.h"

/**
 * @mainpage
 *
 * This manual documents the libbbmotor C API.
 */

/**
 * @defgroup libbbmotor_public libbbmotor public available functions
 * @ingroup libbbmotor
 * @brief public functions
 * @{
 */

/**
 * initialize motor bridge cape
 * @param addr I2C address of the cape
 * @returns handler on success or NULL on failure with errno set
 */
motorcape motorcape_init (uint8_t addr)
{
	struct motorcape_t *han;

	if (!addr)
		addr = 0x4b;

	if (addr > 0x7f)
	{
		errno = EINVAL;
		return NULL;
	}

	han = calloc (1, sizeof (*han));
	if (!han)
		return NULL;

	if (gpio_init (han))
		goto out;

	han->i2c_fd = open ("/dev/i2c-2", O_RDWR);
	if (han->i2c_fd < 0)
		goto out_release;

	if (ioctl(han->i2c_fd, I2C_SLAVE, addr) < 0)
		goto out_close;

	return han;

out_close:
	close (han->i2c_fd);

out_release:
	gpio_release (han);

out:
	free (han);
	return NULL;
}

/**
 * close motor bridge cape
 * @param han motor bridge cape handler
 * @returns 0 on success or -1 on failure with errno set
 */
int motorcape_close (motorcape han)
{
	if (!han)
	{
		errno = EINVAL;
		return -1;
	}

	if (gpio_release (han))
		return -1;

	return close (han->i2c_fd);
}

/**
 * set PWM frequency
 * @param han motor bridge cape handler
 * @param freq PWM frequency
 * @returns 0 on success or -1 on failure with errno set
 */
int motorcape_set_pwm (motorcape han, uint32_t freq)
{
	if (!han)
	{
		errno = EINVAL;
		return -1;
	}

	if (i2c_write_u16 (han->i2c_fd, CONFIG_TB_PWM_FREQ, freq))
		return 1;

	usleep (5000);

	return 0;
}

/** @} */
