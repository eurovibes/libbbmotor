// SPDX-License-Identifier: LGPL-2.1
// Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <unistd.h>

#include "bbmotor.h"
#include "com.h"
#include "internal.h"

/**
 * @defgroup libbbmotor_servo libbbmotor servo functions
 * @ingroup libbbmotor_public
 * @brief servo functions
 * @{
 */

/**
 * initialize servo port
 * @param han motor bridge cape handler
 * @param port servo port
 * @param freq servo frequency
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_servo_init (motorcape han, uint8_t port, uint16_t freq)
{
	uint8_t reg;

	if (!han || --port > 5)
	{
		errno = EINVAL;
		return -1;
	}

	reg = SVM1_STATE + (SVM2_STATE - SVM1_STATE)*port;
	port = SVM1_FREQ + (SVM2_FREQ - SVM1_FREQ)*port;

	if (i2c_write_u16 (han->i2c_fd, port, freq))
		return 1;

	usleep (GUARD_TIME);


	if (i2c_write_u8 (han->i2c_fd, reg, SVM_ENABLE))
		return -1;

	usleep (GUARD_TIME);

	return 0;
}

/**
 * set servo rotating angle
 * @param han motor bridge cape handler
 * @param port servo port
 * @param angle rotating angle
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_servo_angle (motorcape han, uint8_t port, uint16_t angle)
{
	if (!han || --port > 5)
	{
		errno = EINVAL;
		return -1;
	}

	port = SVM1_ANGLE + (SVM2_ANGLE - SVM1_ANGLE)*port;

	if (i2c_write_u8 (han->i2c_fd, port, angle))
		return -1;

	usleep (GUARD_TIME);

	return 0;
}

/** @} */
