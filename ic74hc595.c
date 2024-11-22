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

#include "ic74hc595.h"

#define SETPIN(PIN)     gpio_set_level(PIN, 1)
#define CLRPIN(PIN)     gpio_set_level(PIN, 0)
#define _DELAY_US(x)    ets_delay_us(x)

int8_t ic74hc595_init(shift_reg_config_t *shft)
{
        if (shft == NULL)
                return 1;

        if (shft->reg_value == NULL)
                return 1;

        // Start all registers as 0
	memset(shft->reg_value, 0, shft->num_reg * sizeof(shft->reg_value[0]));

	gpio_config_t io_conf;

	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;

	//bit mask of the pins that you want to set,e.g.GPIO18/19
        uint32_t buf32_0 = 0;
        uint32_t buf32_1 = 0;
        uint64_t result = 0;

        if (shft->pin.clk >= 32)
                buf32_1 |= 1 << (shft->pin.clk - 32);
        else
                buf32_0 |= 1 << shft->pin.clk;

        if (shft->pin.latch >= 32)
                buf32_1 |= 1 << (shft->pin.latch - 32);
        else
                buf32_0 |= 1 << shft->pin.latch;

        if (shft->pin.signal >= 32)
                buf32_1 |= 1 << (shft->pin.signal - 32);
        else
                buf32_0 |= 1 << shft->pin.signal;

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

int8_t ic74hc595_send(uint8_t *data, uint8_t len, shift_reg_config_t *shft)
{
        if (shft == NULL)
                return 1;

        if (shft->reg_value == NULL)
                return 1;

        if (data == NULL)
                return 1;

	if (len > shft->num_reg)
                return 1;

	for (uint8_t i = 0; i < len; i++) {
		ic74hc595_send8bits(data[i], shft);
		shft->reg_value[i] = data[i];
	}

	return 0;
}

int8_t ic74hc595_send8bits(uint8_t data, shift_reg_config_t *shft)
{
        if (shft == NULL)
                return 1;

	for (int8_t i = 7; i >= 0; i--) {
		if ((data >> i) & 1) {
			SETPIN(shft->pin.signal);
		} else {
			CLRPIN(shft->pin.signal);
		}

		SETPIN(shft->pin.clk);
		_DELAY_US(1);
		CLRPIN(shft->pin.clk);
		_DELAY_US(1);
	}

	return 0;
}

int8_t ic74hc595_latch(shift_reg_config_t *shft)
{
        if (shft == NULL)
                return 1;

	SETPIN(shft->pin.latch);
	_DELAY_US(1);
	CLRPIN(shft->pin.latch);
	_DELAY_US(1);

	return 0;
}
