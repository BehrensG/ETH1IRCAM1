/*
 * LED.c
 *
 *  Created on: Feb 14, 2024
 *      Author: grzegorz
 */


#include "main.h"
#include "LED.h"
#include "types.h"
#include "cmsis_os.h"

void LED_Control(led_select_t LEDs, uint8_t enable)
{
	if (LEDs & RED) {
		(TRUE == enable) ? (LL_GPIO_ResetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin)) : (LL_GPIO_SetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin));
	}
	if (LEDs & GREEN) {
		(TRUE == enable) ? (LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin)) : (LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin));
	}
	if (LEDs & BLUE) {
		(TRUE == enable) ? (LL_GPIO_ResetOutputPin(LED_BLUE_GPIO_Port, LED_BLUE_Pin)) : (LL_GPIO_SetOutputPin(LED_BLUE_GPIO_Port, LED_BLUE_Pin));
	}
}

void LED_Toggle(led_select_t LEDs, uint32_t time_on, uint32_t time_off)
{
	LED_Control(LEDs, 1);
	osDelay(pdMS_TO_TICKS(time_on));
	LED_Control(LEDs, 0);
	osDelay(pdMS_TO_TICKS(time_off));
}
