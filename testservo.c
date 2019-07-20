// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de>

#ifdef CONFIG_H
#include "config.h"
#endif

#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

#include <sys/mman.h>

#include "bbmotor.h"

static int stop;

void sig_han(int sig)
{
	stop = sig;
}

uint16_t angles [] = {0, 5, 10, 15, 30, 60, 90, 180, 360};

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

	for (i = 1; i < 6; i++)
	{
		if (motorcape_servo_init (han, i, 50))
		{
			perror ("servo initialization failed.");
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

		for (i = 1; i < 6; i++)
		{
			if (motorcape_servo_angle (han, i,
						   angles [next.tv_sec %9]))
			{
				perror ("Set servo angle failed.");
				goto out;
			}
		}

		fprintf (stderr, "angle: %d\n", angles [next.tv_sec %9]);
		next.tv_sec++;
	}

	ret = 0;
out:
	motorcape_close (han);

	return ret;
}
