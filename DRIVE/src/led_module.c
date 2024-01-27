#include "include.h"
#include "led_module.h"

/*
* func:
* 	1. blink
* 	2. water control
* data: 2024/01/24
*/

#ifdef LED_MODULE

/* GPIOA */
#define LED_PORT			GPIOA
#define LED_CLOCK_CONFIG 	RCC_APB2Periph_GPIOA
#define LED_0				GPIO_Pin_0
#define LED_1				GPIO_Pin_1
#define LED_2				GPIO_Pin_2
#define LED_ALL 			LED_0 | LED_1 | LED_2

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_CLOCK_CONFIG, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_ALL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);	

	GPIO_SetBits(LED_PORT, LED_ALL);
}

void single_led_off(uint16_t GPIO_Pin)
{
	GPIO_SetBits( LED_PORT, GPIO_Pin);
}

void single_led_on(uint16_t GPIO_Pin)
{
	GPIO_ResetBits(LED_PORT, GPIO_Pin);
}

void blink_control(uint16_t GPIO_Pin)
{
	TickType_t wait_time = 500;
	
	single_led_on(GPIO_Pin);
	vTaskDelay(wait_time);
	single_led_off(GPIO_Pin);
	vTaskDelay(wait_time);
	
}
void light_water_control(uint8_t min, uint8_t max, uint16_t GPIO_Pin)
{
	uint8_t cnt = 0;
	TickType_t wait_time = 100;
	
	for(cnt = min; cnt < max; cnt++)
	{
		GPIO_Pin &= 0x01;
		single_led_on(GPIO_Pin << cnt);
		vTaskDelay(wait_time);

		single_led_off(GPIO_Pin << cnt);		
		vTaskDelay(wait_time);
	}

}
void leds_task(void *param)
{
#define MIN	0
#define MAX 3

	led_init();
	
	while(1)
	{
//		blink_control(LED_0);
		light_water_control(MIN, MAX, LED_0);
		vTaskDelay(3);
	}
}

void do_create_led_task(void)
{	
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(leds_task, "led_task", 64, "LED_MODULE", 1, NULL);

	if(pdPASS == xReturn)	
	{
		printf("do_create_led_task is create\r\n");
	}
	else
	{
		printf("do_create_led_task is not create\r\n");
	}
	
}

#endif /* LED_MODULE */

