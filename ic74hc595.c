/**
 * @file ic74hc595.c
 * @author Jaime Albuquerque (jaime.albq@gmail.com)
 * @brief Output shifter register library
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <unistd.h>

#include "ic74hc595.h"

int8_t ic74hc595_init(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL)
                return 1;

        const gpio_num_t pins[] = {
                ic74hc595->signal_pin,
                ic74hc595->clock_pin,
                ic74hc595->latch_pin
        };

        for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
                gpio_set_direction(pins[i], GPIO_MODE_OUTPUT);
                gpio_set_pull_mode(pins[i], GPIO_PULLDOWN_ONLY);
                gpio_set_level(pins[i], 0);
        }

	return 0;
}

int8_t ic74hc595_send(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL)
                return 1;

        if (ic74hc595->bits == NULL)
                return 1;

	for (uint8_t i = 0; i < ic74hc595->count; i++) {
		ic74hc595_send8bits(ic74hc595, ic74hc595->bits[i]);
	}

        ic74hc595_latch(ic74hc595);

	return 0;
}

int8_t ic74hc595_send8bits(ic74hc595_t *ic74hc595, uint8_t data)
{
        if (ic74hc595 == NULL)
                return 1;

	for (int8_t i = 7; i >= 0; i--) {
		uint8_t bit = (data >> i) & 1;

                gpio_set_level(ic74hc595->signal_pin, bit);
                gpio_set_level(ic74hc595->clock_pin, 1);
		usleep(1);
                gpio_set_level(ic74hc595->signal_pin, 0);
                gpio_set_level(ic74hc595->clock_pin, 0);
		usleep(1);
	}

	return 0;
}

int8_t ic74hc595_latch(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL)
                return 1;

        gpio_set_level(ic74hc595->latch_pin, 1);
	usleep(1);
        gpio_set_level(ic74hc595->latch_pin, 0);
	usleep(1);

	return 0;
}
