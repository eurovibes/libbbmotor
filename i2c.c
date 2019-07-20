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
 * @internal
 * @defgroup libbbmotor_internal libbbmotor internal helper functions
 * @ingroup libbbmotor
 * @brief libbbmotor helper functions
 */

/**
 * @internal
 * @defgroup libbbmotor_i2c libbbmotor internal i2c helper functions
 * @ingroup libbbmotor_internal
 * @brief i2c helper functions
 * @{
 */

/**
 * write one byte to the motor cape configuration area
 * @internal
 * @param fd I2C file descriptor
 * @param reg motor cape configuration register
 * @param val register value
 */
ssize_t i2c_write_u8 (int fd, uint8_t reg, uint8_t val)
{
	uint8_t buf [3];

	buf [0] = 1;
	buf [1] = reg;
	buf [2] = val;

	return (write (fd, buf, 3) != 3);
}

/**
 * write one word to the motor cape configuration area
 * @internal
 * @param fd I2C file descriptor
 * @param reg motor cape configuration register
 * @param val register value
 */
ssize_t i2c_write_u16 (int fd, uint8_t reg, uint16_t val)
{
	uint8_t buf [4];
	uint16_t *t = (uint16_t *) (buf + 2);

	buf [0] = 1;
	buf [1] = reg;
	*t = val;

	return (write (fd, buf, 4) != 4);
}

/**
 * write one long word to the motor cape configuration area
 * @internal
 * @param fd I2C file descriptor
 * @param reg motor cape configuration register
 * @param val register value
 */
ssize_t i2c_write_u32 (int fd, uint8_t reg, uint32_t val)
{
	uint8_t buf [6];
	uint32_t *t = (uint32_t *) (buf + 2);

	buf [0] = 1;
	buf [1] = reg;
	*t = val;

	return (write (fd, buf, 6) != 6);
}

/** @} */
