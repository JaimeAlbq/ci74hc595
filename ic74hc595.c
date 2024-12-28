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

#include <errno.h>

#include "ic74hc595.h"

int8_t ic74hc595_init(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL) {
                errno = EFAULT;
                return -1;
        }

        const gpio_num_t pins[] = {
                ic74hc595->signal_pin,
                ic74hc595->clock_pin,
                ic74hc595->latch_pin
        };

        for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
                esp_err_t retval;

                retval = gpio_set_direction(pins[i], GPIO_MODE_OUTPUT);
                if (retval == ESP_ERR_INVALID_ARG) {
                        errno = EINVAL;
                        return -1;
                }

                retval = gpio_set_pull_mode(pins[i], GPIO_PULLDOWN_ONLY);
                if (retval == ESP_ERR_INVALID_ARG) {
                        errno = EINVAL;
                        return -1;
                }

                retval = gpio_set_level(pins[i], 0);
                if (retval == ESP_ERR_INVALID_ARG) {
                        errno = EINVAL;
                        return -1;
                }
        }

	return 0;
}

int ic74hc595_send(ic74hc595_t *ic74hc595, uint8_t *data, size_t len)
{
        int8_t retval;

        if (ic74hc595 == NULL) {
                errno = EFAULT;
                return -1;
        }

        if (data == NULL) {
                errno = EFAULT;
                return -1;
        }

	for (size_t i = 0; i < len; i++) {
		retval = ic74hc595_send8bits(ic74hc595, data[i]);
                if (retval == -1) {
                        return -1;
                }
	}

        if (len > 0) {
		retval = ic74hc595_latch(ic74hc595);
                if (retval == -1) {
                        return -1;
                }
        }

	return len;
}

int8_t ic74hc595_send8bits(ic74hc595_t *ic74hc595, uint8_t data)
{
        if (ic74hc595 == NULL) {
                errno = EFAULT;
                return -1;
        }

	for (int8_t i = 0; i < 8; i++) {
		uint8_t bit;

                if (ic74hc595->send_mode == IC74HC595_SEND_MODE_LSB_FIRST) {
                        bit = ((0x01U & (data >> i)) == 0x01U); // LSB first
                } else { // default
                        bit = ((0x80U & (data << i)) == 0x80U); // MSB first
                }

                struct {
                        gpio_num_t pin;
                        uint32_t level;
                } sequence[4] = {
                        { pin: ic74hc595->signal_pin, level: bit },
                        { pin: ic74hc595->clock_pin, level: 1 },
                        { pin: ic74hc595->signal_pin, level: 0 },
                        { pin: ic74hc595->clock_pin, level: 0 }
                };

                for (uint8_t j = 0; j < sizeof(sequence) / sizeof(sequence[0]); j++) {
                        esp_err_t retval;

                        retval = gpio_set_level(sequence[j].pin, sequence[j].level);
                        if (retval == ESP_ERR_INVALID_ARG) {
                                errno = EINVAL;
                                return -1;
                        }
                }
	}

	return 0;
}

int8_t ic74hc595_latch(ic74hc595_t *ic74hc595)
{
        if (ic74hc595 == NULL) {
                errno = EFAULT;
                return -1;
        }

        struct {
                gpio_num_t pin;
                uint32_t level;
        } sequence[2] = {
                { pin: ic74hc595->latch_pin, level: 1 },
                { pin: ic74hc595->latch_pin, level: 0 }
        };

        for (uint8_t j = 0; j < sizeof(sequence) / sizeof(sequence[0]); j++) {
                esp_err_t retval;

                retval = gpio_set_level(sequence[j].pin, sequence[j].level);
                if (retval == ESP_ERR_INVALID_ARG) {
                        errno = EINVAL;
                        return -1;
                }
        }

	return 0;
}

