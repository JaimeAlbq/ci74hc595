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

int8_t ic74hc595_send(ic74hc595_t *ic74hc595, uint8_t *data, size_t len)
{
        if (ic74hc595 == NULL)
                return 1;

        if (data == NULL)
                return 1;

        if (len < 1)
                return 1;

	for (size_t i = 0; i < len; i++) {
		ic74hc595_send8bits(ic74hc595, data[i]);
	}

        ic74hc595_latch(ic74hc595);

	return 0;
}

int8_t ic74hc595_send8bits(ic74hc595_t *ic74hc595, uint8_t data)
{
        if (ic74hc595 == NULL)
                return 1;

	for (int8_t i = 0; i < 8; i++) {
		uint8_t bit;

                if (ic74hc595->send_mode == IC74HC595_SEND_MODE_LSB_FIRST) {
                        bit = ((0x01U & (data >> i)) == 0x01U); // LSB first
                } else { // default
                        bit = ((0x80U & (data << i)) == 0x80U); // MSB first
                }

                gpio_set_level(ic74hc595->signal_pin, bit);
                gpio_set_level(ic74hc595->clock_pin, 1);
                gpio_set_level(ic74hc595->signal_pin, 0);
                gpio_set_level(ic74hc595->clock_pin, 0);
	}

	return 0;
}

int8_t ic74hc595_latch(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL)
                return 1;

        gpio_set_level(ic74hc595->latch_pin, 1);
        gpio_set_level(ic74hc595->latch_pin, 0);

	return 0;
}
