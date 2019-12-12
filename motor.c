// SPDX-License-Identifier: LGPL-2.1
// Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <pthread.h>
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

static int lock_motor_ports (motorcape han, uint8_t port)
{
	int ret = 0;
	if (port > 1)
	{
		errno = EINVAL;
		return -1;
	}

	pthread_mutex_lock (&han->lock);
	if (han->ports [port*2] || han->ports [port*2 + 1])
	{
		errno = EBUSY;
		ret = -1;
	}
	else
	{
		han->ports [port*2] = PORT_STM;
		han->ports [port*2 + 1] = PORT_STM;
	}
	pthread_mutex_unlock (&han->lock);

	return ret;
}

static int unlock_motor_ports (motorcape han, uint8_t port)
{
	int ret = 0;
	if (port > 1)
	{
		errno = EINVAL;
		return -1;
	}

	pthread_mutex_lock (&han->lock);
	if (han->ports [port*2] != PORT_STM ||
	    han->ports [port*2 + 1] != PORT_STM)
	{
		errno = EINVAL;
		ret = -1;
	}
	else
	{
		han->ports [port*2] = PORT_UNUSED;
		han->ports [port*2 + 1] = PORT_UNUSED;
	}
	pthread_mutex_unlock (&han->lock);

	return ret;
}

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

	if (lock_motor_ports (han, port))
		return -1;

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
 * reset motor port to defaults
 * @param han motor bridge cape handler
 * @param index motor port (0 to 3)
 * @returns 0 on success or -1 on failure with errno set
 */
static int reset_port (motorcape han, uint8_t index)
{
	uint8_t port, dir;

	switch (index)
	{
	case 0:
		port = TB_1A_MODE;
		dir = TB_1A_DIR;
		break;

	case 1:
		port = TB_1A_MODE;
		dir = TB_1A_DIR;
		break;

	case 2:
		port = TB_1A_MODE;
		dir = TB_1A_DIR;
		break;

	case 3:
		port = TB_1A_MODE;
		dir = TB_1A_DIR;
		break;

	default:
		errno = EINVAL;
		return -1;
	}

	if (i2c_write_u8 (han->i2c_fd, dir, TB_STOP))
		return -1;
	usleep (GUARD_TIME);

	if (i2c_write_u8 (han->i2c_fd, port, TB_DCM))
		return -1;
	usleep (GUARD_TIME);

	return 0;
}

/**
 * release stepper port
 * @param han motor bridge cape handler
 * @param port stepper port
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_stepper_release (motorcape han, uint8_t port)
{
	int ret;

	if (!han || --port > 1)
	{
		errno = EINVAL;
		return -1;
	}

	if (unlock_motor_ports (han, port))
		return -1;

	if (!port)
	{
		ret = reset_port (han, 0);
		if (ret)
			return ret;

		ret = reset_port (han, 1);
		if (ret)
			return ret;
	}
	else
	{
		ret = reset_port (han, 2);
		if (ret)
			return ret;

		ret = reset_port (han, 3);
		if (ret)
			return ret;

	}

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
