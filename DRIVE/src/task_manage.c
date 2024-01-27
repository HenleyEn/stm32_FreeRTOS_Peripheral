#include "include.h"

#ifdef LED_MODULE
#include "led_module.h"
#endif

#ifdef MATRIX_KEYS_MODULE
#include "matrix_keys.h"
#endif

#ifdef SEG_DISPLAY_MODULE
#include "seg_display.h"
#endif

TaskHandle_t task_start_handle;

void task_start(void* param)
{
	taskENTER_CRITICAL();
	
#ifdef LED_MODULE
	do_create_led_task();
#endif

#ifdef MATRIX_KEYS_MODULE
	do_matrix_keys_task();
#endif

#ifdef SEG_DISPLAY_MODULE
	do_create_seg_display_task();
#endif

	taskEXIT_CRITICAL();
	
	vTaskDelete(task_start_handle); 					
}

void do_create_task_start(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(task_start, "task_start", 1024, NULL, 1, &task_start_handle);

	if(pdPASS == xReturn)	
	{
		printf("do_create_task_start is create\r\n");
		vTaskStartScheduler();
	}
	else
	{
		printf("do_create_task_start is not create\r\n");
	}
}


