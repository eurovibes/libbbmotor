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
 * @defgroup libbbmotor_stepper libbbmotor stepper functions
 * @ingroup libbbmotor_public
 * @brief stepper functions
 * @{
 */

/**
 * initialize stepper port
 * @param han motor bridge cape handler
 * @param port stepper port
 * @param duty stepper motor duty cycle in us
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_stepper_init (motorcape han, uint8_t port, uint16_t duty)
{
	uint8_t reg;

	if (!han || --port > 1)
	{
		errno = EINVAL;
		return -1;
	}

	if (!port)
	{
		port = TB_1A_MODE;
		reg = TB_1A_DUTY;
	}
	else
	{
		port = TB_2A_MODE;
		reg = TB_2A_DUTY;
	}

	if (i2c_write_u8 (han->i2c_fd, port, TB_SPM))
		return -1;

	usleep (GUARD_TIME);

	if (i2c_write_u16 (han->i2c_fd, reg, duty))
		return -1;

	usleep (GUARD_TIME);

	return 0;
}

/**
 * set stepper rotating direction
 * @param han motor bridge cape handler
 * @param port stepper port
 * @param dir rotating direction
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_stepper_dir (motorcape han, uint8_t port, uint8_t dir)
{
	if (!han || --port > 1)
	{
		errno = EINVAL;
		return -1;
	}

	if (!port)
		port = TB_1A_DIR;
	else
		port = TB_2A_DIR;

	if (i2c_write_u8 (han->i2c_fd, port, (dir) ? TB_CW : TB_CCW))
		return -1;

	usleep (GUARD_TIME);

	return 0;
}

/**
 * set stepper motor speed
 * @param han motor bridge cape handler
 * @param port stepper port
 * @param speed stepper motor cycle speed in Hz
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_stepper_speed (motorcape han, uint8_t port, uint32_t speed)
{
	if (!han || --port > 1)
	{
		errno = EINVAL;
		return -1;
	}

	if (!port)
		port = TB_1A_SPM_SPEED;
	else
		port = TB_2A_SPM_SPEED;

	if (i2c_write_u32 (han->i2c_fd, port, speed))
		return -1;

	usleep (GUARD_TIME);

	return 0;
}

/**
 * perform stepper motor steps
 * @param han motor bridge cape handler
 * @param port stepper port
 * @param steps number of stepper motor steps to perform
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_stepper_steps (motorcape han, uint8_t port, uint32_t steps)
{
	if (!han || --port > 1)
	{
		errno = EINVAL;
		return -1;
	}

	if (!port)
		port = TB_1A_SPM_STEP;
	else
		port = TB_2A_SPM_STEP;

	if (i2c_write_u32 (han->i2c_fd, port, steps))
		return -1;

	usleep (GUARD_TIME);

	return 0;
}

/** @} */
