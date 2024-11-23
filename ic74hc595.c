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

#define SETPIN(PIN)     gpio_set_level(PIN, 1)
#define CLRPIN(PIN)     gpio_set_level(PIN, 0)
#define _DELAY_US(x)    usleep(x)

int8_t ic74hc595_init(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL)
                return 1;

	gpio_config_t io_conf;

	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;

	//bit mask of the pins that you want to set,e.g.GPIO18/19
        uint32_t buf32_0 = 0;
        uint32_t buf32_1 = 0;
        uint64_t result = 0;

        if (ic74hc595->clock_pin >= 32)
                buf32_1 |= 1 << (ic74hc595->clock_pin - 32);
        else
                buf32_0 |= 1 << ic74hc595->clock_pin;

        if (ic74hc595->pin.latch >= 32)
                buf32_1 |= 1 << (ic74hc595->latch_pin - 32);
        else
                buf32_0 |= 1 << ic74hc595->latch_pin;

        if (ic74hc595->signal_pin >= 32)
                buf32_1 |= 1 << (ic74hc595->signal_pin - 32);
        else
                buf32_0 |= 1 << ic74hc595->signal_pin;

        result = ((uint64_t)buf32_1 << 32) | ((uint64_t)buf32_0 << 0);
        io_conf.pin_bit_mask = result;

	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

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
		if ((data >> i) & 1) {
			SETPIN(ic74hc595->signal_pin);
		} else {
			CLRPIN(ic74hc595->signal_pin);
		}

		SETPIN(ic74hc595->clock_pin);
		_DELAY_US(1);
		CLRPIN(ic74hc595->clock_pin);
		_DELAY_US(1);
	}

	return 0;
}

int8_t ic74hc595_latch(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL)
                return 1;

	SETPIN(ic74hc595->latch_pin);
	_DELAY_US(1);
	CLRPIN(ic74hc595->latch_pin);
	_DELAY_US(1);

	return 0;
}
