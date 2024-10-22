/* Copyright 2024 Grug Huhler.  License SPDX BSD-2-Clause.
 *
 * The PWM controller contains a counter that counts at the system clock
 * speed from 0 to the value of the COUNTER_LIMIT register - 1.  The PWM
 * output is on whenever the counter < the value of the ON_LIMIT register.
 *
 * Example: Set COUNTER_LIMIT to the system clock frequency and ON_LIMIT
 * to 1/2 that value to blink an LED once per second with a 50% duty cycle.
 */

#ifndef _PWM_CONTROLLER_H
#define _PWM_CONTROLLER_H

extern void pwm_set_counter_limit(const unsigned int val);
extern void pwm_set_on_limit(const unsigned int val);

#endif

