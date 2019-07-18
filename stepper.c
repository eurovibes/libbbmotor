#ifdef CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/mman.h>

#include "bbmotor.h"

static uint32_t sec_steps [3] = {6, 8, 6};
static int stop;

void sig_han(int sig)
{
	stop = sig;
}

int main (void)
{
	struct sched_param param;
	struct timespec next;
	motorcape han;
	int i, ret = 1;

	mlockall (MCL_CURRENT | MCL_FUTURE);
	param.sched_priority = 90;

	if (sched_setscheduler (0, SCHED_FIFO, &param) < 0)
	{
		perror ("set priority failed");
		return 1;
	}

	stop = 0;
	signal (SIGHUP, sig_han);
	signal (SIGINT, sig_han);
	signal (SIGQUIT, sig_han);

	han = motorcape_init (0x4b);
	if (!han)
	{
		perror ("motorcape init failed.");
		return 1;
	}

	if (motorcape_set_pwm (han, 10000))
	{
		perror ("PWM initialization failed.");
		goto out;
	}

	for (i = 1; i < 3; i++)
	{
		if (motorcape_stepper_init (han, i, 1000))
		{
			perror ("stepper initialization failed.");
			goto out;
		}

		if (motorcape_stepper_speed (han, i, 2000))
		{
			perror ("Init stepper A speed failed.");
			goto out;
		}

		if (motorcape_stepper_dir (han, i, 1))
		{
			perror ("Init stepper A dir failed.");
			goto out;
		}
	}

	if (clock_gettime (CLOCK_MONOTONIC, &next))
	{
		perror ("get time failed.");
		goto out;
	}

	next.tv_sec += 2;
	next.tv_nsec = 0;

	while (!stop)
	{
		clock_nanosleep (CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);

		if (motorcape_stepper_steps (han, 1,
					     sec_steps [next.tv_sec %3]))
		{
			perror ("Init stepper A steps failed.");
			goto out;
		}

		usleep (500000);
		if (motorcape_stepper_steps (han, 2,
					     sec_steps [next.tv_sec %3]))
		{
			perror ("Init stepper A steps failed.");
			goto out;
		}
		fprintf (stderr, "*");
		next.tv_sec++;
	}

	ret = 0;
out:
	motorcape_close (han);

	return ret;
}
