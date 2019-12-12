// SPDX-License-Identifier: LGPL-2.1
// Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
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
EXPORT motorcape motorcape_init (uint8_t addr)
{
	pthread_mutexattr_t lock_attr;
	struct motorcape_t *han;
	unsigned long i;

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

	for (i = 0; i < NUM_ELEM (han->ports); i++)
		han->ports [i] = PORT_UNUSED;

	pthread_mutexattr_init (&lock_attr);
	pthread_mutexattr_setprotocol (&lock_attr, PTHREAD_PRIO_INHERIT);
	pthread_mutexattr_setrobust (&lock_attr, PTHREAD_MUTEX_ROBUST);
	pthread_mutex_init (&han->lock, &lock_attr);
	pthread_mutexattr_destroy(&lock_attr);

	if (motorcape_set_pwm (han, DEFAULT_FREQ))
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
EXPORT int motorcape_close (motorcape han)
{
	unsigned long i;
	int ret = 0;

	if (!han)
	{
		errno = EINVAL;
		return -1;
	}

	pthread_mutex_lock (&han->lock);
	for (i = 0; i < NUM_ELEM (han->ports); i++)
	{
		if (han->ports [i])
		{
			errno = EBUSY;
			ret = -1;
			break;
		}
	}
	pthread_mutex_unlock (&han->lock);
	if (ret)
		return -1;

	ret = pthread_mutex_destroy (&han->lock);
	if (ret)
	{
		errno = ret;
		return -1;
	}

	if (gpio_release (han))
		return -1;

	return close (han->i2c_fd);
}

/**
 * set PWM frequency
 * @param han motor bridge cape handler
 * @param freq PWM frequency in Hz
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_set_pwm (motorcape han, uint32_t freq)
{
	if (!han)
	{
		errno = EINVAL;
		return -1;
	}

	if (i2c_write_u16 (han->i2c_fd, CONFIG_TB_PWM_FREQ, freq))
		return -1;

	usleep (GUARD_TIME);
	han->freq = freq;

	return 0;
}

/** @} */
