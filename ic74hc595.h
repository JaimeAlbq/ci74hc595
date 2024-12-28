/**
 * @file ic74hc595.h
 * @author Jaime Albuquerque (jaime.albq@gmail.com)
 * @brief Output shifter register library
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef IC74HC595_H
#define IC74HC595_H

#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IC74HC595_SEND_MODE_MSB_FIRST 0
#define IC74HC595_SEND_MODE_LSB_FIRST 1

typedef struct {
        gpio_num_t clock_pin;
        gpio_num_t signal_pin;
        gpio_num_t latch_pin;

        uint8_t send_mode;
} ic74hc595_t;

int8_t ic74hc595_init(ic74hc595_t *ic74hc595);

int ic74hc595_send(ic74hc595_t *ic74hc595, uint8_t *data, size_t len);

int8_t ic74hc595_send8bits(ic74hc595_t *ic74hc595, uint8_t data);

int8_t ic74hc595_latch(ic74hc595_t *ic74hc595);

#ifdef __cplusplus
}
#endif

#endif /* IC74HC595_H */

