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

#if !defined(_IC74HC595_H_)
#define _IC74HC595_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "driver/gpio.h"

typedef struct {
	// GPIO
        gpio_num_t clock_pin;
        gpio_num_t signal_pin;
        gpio_num_t latch_pin;
} ic74hc595_t;

/**
 * @brief Initialize the microcontroller to do the output
 * 
 * @param shft 
 * @return < 0 means error
 */
int8_t ic74hc595_init(ic74hc595_t *ic74hc595);

/**
 * @brief Send the whole data
 * 
 * @param data 
 * @param len 
 * @param shft 
 * @return -1 = data longer than number of registers; 1 = successfully sent
 */
int8_t ic74hc595_send(ic74hc595_t *ic74hc595, uint8_t *data, size_t len);

/**
 * @brief Send 1 byte at a time
 * 
 * @param data 
 * @param shft 
 * @return int8_t 
 */
int8_t ic74hc595_send8bits(ic74hc595_t *ic74hc595, uint8_t data);

/**
 * @brief Latch the registers
 * 
 * @param shft 
 * @return int8_t 
 */
int8_t ic74hc595_latch(ic74hc595_t *ic74hc595);

#endif // _IC74HC595_H_
