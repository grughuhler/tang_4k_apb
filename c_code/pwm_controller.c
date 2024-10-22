/* Copyright 2024 Grug Huhler.  License SPDX BSD-2-Clause. */

#include "pwm_controller.h"

#define PWM_BASE 0x40002500

#define COUNTER_LIMIT (*((volatile unsigned int *) (PWM_BASE + 0x00)))
#define ON_LIMIT (*((volatile unsigned int *) (PWM_BASE + 0x04)))

void pwm_set_counter_limit(const unsigned int val)
{
  COUNTER_LIMIT = val;
}

void pwm_set_on_limit(const unsigned int val)
{
  ON_LIMIT = val;
}

