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

static ssize_t i2c_write_u8 (int fd, uint8_t reg, uint8_t val)
{
	uint8_t buf [3];

	buf [0] = 1;
	buf [1] = reg;
	buf [2] = val;

	return (write (fd, buf, 3) != 3);
}

static ssize_t i2c_write_u16 (int fd, uint8_t reg, uint16_t val)
{
	uint8_t buf [4];
	uint16_t *t = (uint16_t *) (buf + 2);

	buf [0] = 1;
	buf [1] = reg;
	*t = val;

	return (write (fd, buf, 4) != 4);
}

static ssize_t i2c_write_u32 (int fd, uint8_t reg, uint32_t val)
{
	uint8_t buf [6];
	uint32_t *t = (uint32_t *) (buf + 2);

	buf [0] = 1;
	buf [1] = reg;
	*t = val;

	return (write (fd, buf, 6) != 6);
}

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

int motorcape_stepper_init (motorcape han, uint8_t port, uint16_t duty)
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

	usleep (5000);

	if (i2c_write_u16 (han->i2c_fd, reg, duty))
		return 1;

	usleep (5000);

	return 0;
}

int motorcape_stepper_dir (motorcape han, uint8_t port, uint8_t dir)
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

	usleep (5000);

	return 0;
}

int motorcape_stepper_speed (motorcape han, uint8_t port, uint32_t speed)
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
		return 1;

	usleep (5000);

	return 0;
}

int motorcape_stepper_steps (motorcape han, uint8_t port, uint32_t steps)
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
		return 1;

	usleep (5000);

	return 0;
}
