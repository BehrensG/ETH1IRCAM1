/*
 * LED.h
 *
 *  Created on: Feb 14, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_LED_H_
#define BSP_INC_LED_H_


typedef enum led_select
{
	RED = 0x1,
	GREEN = 0x2,
	BLUE = 0x4
}led_select_t;

#define LED_ALL (RED | GREEN | BLUE)

void LED_Control(led_select_t LEDs, uint8_t enable);
void LED_Toggle(led_select_t LEDs, uint32_t time_on, uint32_t time_off);

#endif /* BSP_INC_LED_H_ */
