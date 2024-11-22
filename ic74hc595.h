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
#include "esp32/rom/ets_sys.h"

typedef struct {
	uint8_t num_reg;	// Number of shift registers
	uint8_t *reg_value;	// Last value of all registers

	// GPIO
	struct pin {
		gpio_num_t clk;
		gpio_num_t signal;
		gpio_num_t latch;
	} pin;
} shift_reg_config_t;

/**
 * @brief Initialize the microcontroller to do the output
 * 
 * @param shft 
 * @return < 0 means error
 */
int8_t ic74hc595_init(shift_reg_config_t *shft);

/**
 * @brief Send the whole data
 * 
 * @param data 
 * @param len 
 * @param shft 
 * @return -1 = data longer than number of registers; 1 = successfully sent
 */
int8_t ic74hc595_send(uint8_t *data, uint8_t len, shift_reg_config_t *shft);

/**
 * @brief Send 1 byte at a time
 * 
 * @param data 
 * @param shft 
 * @return int8_t 
 */
int8_t ic74hc595_send8bits(uint8_t data, shift_reg_config_t *shft);

/**
 * @brief Latch the registers
 * 
 * @param shft 
 * @return int8_t 
 */
int8_t ic74hc595_latch(shift_reg_config_t *shft);

#endif // _IC74HC595_H_
