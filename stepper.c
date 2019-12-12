// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <argp.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/mman.h>

#include "bbmotor.h"

#if ENABLE_NLS
#include <libintl.h>
#define _(Text) gettext (Text)
#else
#define textdomain(Domain)
#define _(Text) Text
#endif
#define N_(Text) Text

static uint32_t sec_steps [3] = {6, 8, 6};
static int stop;

static error_t parse_opt (int key, char *arg, struct argp_state *state);
static void show_version (FILE *stream, struct argp_state *state);

/* argp option keys */
enum {DUMMY_KEY = 129};

/* Option flags and variables.  These are initialized in parse_opt.  */

int duty;			/* --duty */
int prio;			/* --priority */
int want_verbose;		/* --verbose */

static struct argp_option options [] =
{
	{"duty", 'd', "<val>", 0, N_("Set duty to <val> milliseconds"), 0},
	{"prioity", 'p', "<prio>", 0, N_("Set SCHED_FIFO priority <prio>"), 0},
	{"verbose", 'v', NULL, 0, N_("Print more information"), 0},
	{NULL, 0, NULL, 0, NULL, 0}
};

/* The argp functions examine these global variables.  */
const char *argp_program_bug_address = "https://github.com/eurovibes/libbbmotor/issues";
void (*argp_program_version_hook) (FILE *, struct argp_state *) = show_version;

static struct argp argp =
{
	options, parse_opt, NULL,
	N_("Test Beaglebone Motor Cape stepper motors."),
	NULL, NULL, NULL
};

/* Parse a single option.  */
static error_t parse_opt (int key, char *arg __attribute__((unused)),
			  struct argp_state *state)
{
	switch (key)
	{
	case ARGP_KEY_INIT:
		/* Set up default values.  */
		duty = 500;
		prio = 90;
		want_verbose = 0;
		break;

	case 'd':			/* --duty */
		duty = atoi (arg);
		if (duty < 1 || duty > 999)
		{
			fprintf (stderr,
				 _("Duty %i out of range [1:999].\n"
				   "Reset to 500 ms.\n"),
				 duty);
			duty = 500;
		}
		break;

	case 'p':			/* --priority */
		prio = atoi (arg);
		if (prio < 1 || prio > 99)
		{
			fprintf (stderr,
				 _("Priority %i out of range [1:99].\n"
				   "Reset to 90.\n"),
				 prio);
			prio = 90;
		}
		break;

	case 'v':			/* --verbose */
		want_verbose = 1;
		break;

	case ARGP_KEY_ARG:
		argp_usage (state);
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}
void sig_han(int sig)
{
	stop = sig;
}

int main (int argc, char **argv)
{
	struct sched_param param;
	struct timespec next;
	motorcape han;
	int i, ret = 1;

	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	argp_parse (&argp, argc, argv, 0, NULL, NULL);

	if (want_verbose)
	{
		printf (_("%s paraleter list:\n"), argv [0]);
		printf (_("Duty: %d ms\n"), duty);
		printf (_("Priority: %d\n"), prio);
	}

	/* convert duty to microseconds */
	duty *= 1000;

	mlockall (MCL_CURRENT | MCL_FUTURE);
	param.sched_priority = prio;

	if (sched_setscheduler (0, SCHED_FIFO, &param) < 0)
	{
		perror (_("Set priority failed. Abort."));
		return 1;
	}

	stop = 0;
	signal (SIGHUP, sig_han);
	signal (SIGINT, sig_han);
	signal (SIGQUIT, sig_han);

	han = motorcape_init (0x4b);
	if (!han)
	{
		perror (_("The motorcape initialization failed. Abort."));
		return 1;
	}

	if (motorcape_set_pwm (han, 10000))
	{
		perror (_("The PWM initialization failed. Abort."));
		goto out;
	}

	for (i = 1; i < 3; i++)
	{
		if (motorcape_stepper_init (han, i, 1000))
		{
			perror (_("The stepper initialization failed. Abort."));
			goto out;
		}

		if (motorcape_stepper_speed (han, i, 2000))
		{
			perror (_("Cannot set stepper A speed. Abort."));
			goto out;
		}

		if (motorcape_stepper_dir (han, i, 1))
		{
			perror (_("Cannot set stepper A dir. Abort."));
			goto out;
		}
	}

	if (clock_gettime (CLOCK_MONOTONIC, &next))
	{
		perror (_("Get current time failed. Abort"));
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
			perror (_("Stepper A failed to step. Abort."));
			goto out;
		}

		usleep (duty);
		if (motorcape_stepper_steps (han, 2,
					     sec_steps [next.tv_sec %3]))
		{
			perror (_("Stepper B failed to step. Abort."));
			goto out;
		}
		fprintf (stderr, N_("*"));
		next.tv_sec++;
	}

	ret = motorcape_stepper_release (han, 1);
	if (ret)
	{
		perror (_("Stepper A release failed."));
		goto out;
	}

	ret = motorcape_stepper_release (han, 2);
	if (ret)
		perror (_("Stepper B release failed."));

out:
	motorcape_close (han);

	return ret;
}

/* Show the version number and copyright information.  */
static void show_version (FILE *stream, struct argp_state *state)
{
	(void) state;

	fputs (PACKAGE" "VERSION"\n", stream);
	fprintf (stream, _("Written by %s.\n\n"), "Benedikt Spranger");
	fprintf (stream, _("Copyright (C) %s %s\n"), "2019", "Benedikt Spranger");
	fputs(_("\
This program is free software; you may redistribute it under the terms of\n\
the GNU General Public License.  This program has absolutely no warranty.\n"),
	      stream);
}
