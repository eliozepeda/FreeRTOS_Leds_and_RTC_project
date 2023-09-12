/*
 * Led_effect.c
 *
 *  Created on: Aug 8, 2023
 *      Author: ezepedasanchez
 */


#include "main.h"



int flag_led = 0;

void led_effect_stop(void)
{
	for(int i = 0 ; i<4 ; i++)
	{
		xTimerStop(handle_led_timer[i], portMAX_DELAY);
	}
	flag_led = 0;
}

void led_effect(int n)
{
	/*The next line stop the last led effect*/
	led_effect_stop();
	xTimerStart(handle_led_timer[n-1], portMAX_DELAY);
}

void turn_off_all_leds(void)
{
	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
}

void turn_on_all_leds(void)
{
	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_SET);
}

void LED_effect1(void)
{
	static int flag = 1;
	(flag ^= 1) ? turn_off_all_leds() : turn_on_all_leds();
}

void turn_off_two_leds(void)
{
	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_SET);
}

void turn_on_two_leds(void)
{
	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
}

void LED_effect2(void)
{
	static int flag = 1;
	(flag ^= 1) ? turn_off_two_leds() : turn_on_two_leds();
}


void turn_off_up_leds(void)
{
    if(flag_led == 1)
    {
        /*"led 2 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
        flag_led++;
    }
    if(flag_led == 3)
    {
        /*"led 4 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_SET);
        flag_led=0;
    }
}

void turn_on_up_leds(void)
{
    if(flag_led == 0)
    {
        /*"led 1 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
        flag_led++;
    }
    if(flag_led == 2)
    {
        /*"led 3 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
        flag_led++;
    }
}

void LED_effect3(void)
{
	static int flag = 1;
	(flag ^= 1) ? turn_off_up_leds() : turn_on_up_leds();
}


void turn_off_down_leds(void)
{
    if(flag_led == 1)
    {
        /*"led 3 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
        flag_led++;
    }
    if(flag_led == 3)
    {
        /*"led 1 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
        flag_led=0;
    }
}

void turn_on_down_leds(void)
{
    if(flag_led == 0)
    {
        /*"led 4 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_SET);
        flag_led++;
    }
    if(flag_led == 2)
    {
        /*"led 2 on */
    	HAL_GPIO_WritePin(GPIOA, Led_1_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_2_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOB, Led_3_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOB, Led_4_Pin, GPIO_PIN_RESET);
        flag_led++;
    }
}

void LED_effect4(void)
{
	static int flag = 1;
	(flag ^= 1) ? turn_off_down_leds() : turn_on_down_leds();
}




