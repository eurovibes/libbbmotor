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
 * @defgroup libbbmotor_dcm libbbmotor DC motor functions
 * @ingroup libbbmotor_public
 * @brief DC motor functions
 * @{
 */

/**
 * initialize DC motor port
 * @param han motor bridge cape handler
 * @param index DC motor port
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_dcm_init (motorcape han, uint8_t index)
{
	int ret = 0;

	if (!han || --index > 3)
	{
		errno = EINVAL;
		return -1;
	}

	pthread_mutex_lock (&han->lock);
	if (han->ports [index])
	{
		errno = EBUSY;
		ret = -1;
	}
	else
		han->ports [index] = PORT_DCM;
	pthread_mutex_unlock (&han->lock);

	if (ret)
		return -1;

	if (reset_port (han, index))
		return -1;

	return 0;
}

/**
 * release DC motor port
 * @param han motor bridge cape handler
 * @param index DC motor port
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_dcm_release (motorcape han, uint8_t index)
{
	int ret = 0;

	if (!han || --index > 3)
	{
		errno = EINVAL;
		return -1;
	}

	pthread_mutex_lock (&han->lock);
	if (han->ports [index] != PORT_DCM)
	{
		errno = EINVAL;
		ret = -1;
	}
	else
		han->ports [index] = PORT_UNUSED;
	pthread_mutex_unlock (&han->lock);

	if (ret)
		return -1;

	if (motorcape_dcm_stop (han, index + 1))
		return -1;

	return 0;
}

static uint8_t idx2dir (uint8_t index)
{
	switch (index)
	{
	case 0:
		return TB_1B_DIR;
	case 1:
		return TB_1A_DIR;
	case 2:
		return TB_2B_DIR;
	case 3:
		return TB_2A_DIR;
	}

	/* should not be reached */
	return TB_1B_DIR;
}

static uint8_t idx2duty (uint8_t index)
{
	switch (index)
	{
	case 0:
		return TB_1B_DUTY;
	case 1:
		return TB_1A_DUTY;
	case 2:
		return TB_2B_DUTY;
	case 3:
		return TB_2A_DUTY;
	}

	/* should not be reached */
	return TB_1B_DUTY;
}

/**
 * set DM motor movement
 * @param han motor bridge cape handler
 * @param index DC motor port
 * @param dir rotating direction
 * @param speed DC motor speed in 1/1000
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_dcm_move (motorcape han, uint8_t index,
			       uint8_t dir, uint16_t speed)
{
	uint32_t tmp;

	if (!han || --index > 3 || speed > 1000)
	{
		errno = EINVAL;
		return -1;
	}

	dir = (dir) ? TB_CW : TB_CCW;
	if (i2c_write_u8 (han->i2c_fd, idx2dir (index), dir))
		return -1;
	usleep (GUARD_TIME);

	tmp = speed*han->freq/1000;
	speed = tmp & 0xffff;

	if (i2c_write_u16 (han->i2c_fd, idx2duty (index), speed))
		return -1;
	usleep (GUARD_TIME);

	return 0;
}

/**
 * stop DC motor
 * @param han motor bridge cape handler
 * @param index DC motor port
 * @returns 0 on success or -1 on failure with errno set
 */
EXPORT int motorcape_dcm_stop (motorcape han, uint8_t index)
{
	if (!han || --index > 3)
	{
		errno = EINVAL;
		return -1;
	}

	if (i2c_write_u8 (han->i2c_fd, idx2dir (index), TB_STOP))
		return -1;
	usleep (GUARD_TIME);

	return 0;
}
/** @} */
