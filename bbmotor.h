#ifndef _BBMOTOR_H_
#define _BBMOTOR_H_

struct motorcape_t;
typedef struct motorcape_t *motorcape;

motorcape motorcape_init (uint8_t addr);
int motorcape_close (motorcape han);

int motorcape_set_pwm (motorcape han, uint32_t freq);

int motorcape_stepper_init (motorcape han, uint8_t port, uint16_t duty);
int motorcape_stepper_dir (motorcape han, uint8_t port, uint8_t dir);
int motorcape_stepper_speed (motorcape han, uint8_t port, uint32_t speed);
int motorcape_stepper_steps (motorcape han, uint8_t port, uint32_t steps);

#endif /* _BBMOTOR_H_ */
