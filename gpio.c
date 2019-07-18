#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <gpiod.h>
#include <unistd.h>

#include "bbmotor.h"
#include "internal.h"

int gpio_init (struct motorcape_t *han)
{
	struct gpiod_chip *chip1, *chip3;

	if (!han)
	{
		errno = EINVAL;
		return -1;
	}

	/* NRST - GPIO1_17 */
	chip1 = gpiod_chip_open_by_number (1);

	han->nrst = gpiod_chip_get_line (chip1, 17);
	if (!han->nrst)
		return -1;

	if (gpiod_line_request_output (han->nrst, PACKAGE, 0) < 0)
		goto out;

	/* BOOT0 - GPIO3_21 */
	chip3 = gpiod_chip_open_by_number (3);

	han->boot0 = gpiod_chip_get_line (chip3, 21);
	if (!han->boot0)
		goto out_release;

	if (gpiod_line_request_output (han->boot0, PACKAGE, 0) < 0)
		goto out_close;

	gpiod_line_set_value (han->boot0, 0);
	gpiod_line_set_value (han->nrst, 0);
	usleep (50000);
	gpiod_line_set_value (han->nrst, 1);

	return 0;

out_close:
	gpiod_chip_close (chip3);
out_release:
	gpiod_line_release (han->nrst);
out:
	gpiod_chip_close (chip1);

	return -1;
}

int gpio_release (struct motorcape_t *han)
{
	if (!han)
	{
		errno = EINVAL;
		return -1;
	}

	gpiod_line_release (han->boot0);
	gpiod_line_release (han->nrst);

	return 0;
}
