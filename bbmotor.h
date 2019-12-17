/* SPDX-License-Identifier: LGPL-2.1 */
/* Copyright (C) 2019 Benedikt Spranger <b.spranger@linutronix.de> */

#ifndef _BBMOTOR_H_
#define _BBMOTOR_H_

#include <stdint.h>

struct motorcape_t;
typedef struct motorcape_t *motorcape;

motorcape motorcape_init (uint8_t addr);
int motorcape_close (motorcape han);

int motorcape_set_pwm (motorcape han, uint32_t freq);

int motorcape_servo_init (motorcape han, uint8_t port, uint16_t freq);
int motorcape_servo_angle (motorcape han, uint8_t port, uint16_t angle);

int motorcape_stepper_init (motorcape han, uint8_t port, uint16_t duty);
int motorcape_stepper_release (motorcape han, uint8_t port);
int motorcape_stepper_dir (motorcape han, uint8_t port, uint8_t dir);
int motorcape_stepper_speed (motorcape han, uint8_t port, uint32_t speed);
int motorcape_stepper_steps (motorcape han, uint8_t port, uint32_t steps);

int motorcape_dcm_init (motorcape han, uint8_t port);
int motorcape_dcm_release (motorcape han, uint8_t port);
int motorcape_dcm_move (motorcape han, uint8_t index,
			uint8_t dir, uint16_t speed);
int motorcape_dcm_stop (motorcape han, uint8_t index);

#endif /* _BBMOTOR_H_ */
