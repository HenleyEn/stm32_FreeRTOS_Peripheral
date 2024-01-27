#include "seg_display.h"

#ifdef SEG_DISPLAY_MODULE

#define gpio_pins_set	GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define select_bits_gpio_pins	GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 

#define seg_cnt	8
#define seg_display_cnt	16
static const uint8_t seg_num[seg_display_cnt] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
												0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};	
										
static const seg_def seg8bits_def = 
{
	.seg_clk = RCC_APB2Periph_GPIOA,
	.seg_port = GPIOA,
	.seg_gpio_pins = gpio_pins_set,

#ifdef DYNAMIC_74HC138
	.select_bit_clk = RCC_APB2Periph_GPIOA,
	.select_bit_port = GPIOA,
	.select_bit_gpio_pins = select_bits_gpio_pins,
#endif /* DYNAMIC_74HC138 */

};

seg_drv seg8bit_dev =
{
	.def = &seg8bits_def,
	.seg_counts = seg_cnt,
	.seg_display_num = seg_num,
};

static void seg_display_init(seg_drv *seg_device)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(seg_device->def->seg_clk, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = seg_device->def->seg_gpio_pins;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(seg_device->def->seg_port, &GPIO_InitStructure);	
	
#ifdef DYNAMIC_74HC138
	RCC_APB2PeriphClockCmd(seg_device->def->select_bit_clk, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = seg_device->def->select_bit_gpio_pins;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(seg_device->def->select_bit_port, &GPIO_InitStructure);
#endif /* DYNAMIC_74HC138 */
}

#ifdef DYNAMIC_74HC138
static int seg_port_set(seg_drv *seg_device, uint8_t num)
{
	uint8_t bit[5] = {0};
	uint8_t i = 0;
	
	if (num > 7)
		return -1;	
	
	while (num > 0) 
	{
		bit[i] = num % 2;
		i++;
		num  = num / 2;
	}
	
	GPIO_WriteBit(seg_device->def->select_bit_port, GPIO_Pin_8, (BitAction)bit[0]);
	GPIO_WriteBit(seg_device->def->select_bit_port, GPIO_Pin_9, (BitAction)bit[1]);
	GPIO_WriteBit(seg_device->def->select_bit_port, GPIO_Pin_10,( BitAction)bit[2]);
	
	return 0;
}
#endif /* DYNAMIC_74HC138 */

void seg_display(seg_drv *seg_device)
{
//	uint8_t wait_time = 1;
	int i;
	for(i = 0; i < seg_device->seg_counts; i++)
	{
		GPIO_Write(seg_device->def->seg_port, (uint16_t)(seg_device->seg_display_num[i]));
#ifdef DYNAMIC_74HC138
		seg_port_set(seg_device, i);
		vTaskDelay(1);
		GPIO_Write(seg_device->def->seg_port, (uint16_t)0x00);//clear seg_display
#else
		vTaskDelay(500);
#endif /* DYNAMIC_74HC138 */
	}
}

static void seg_display_control(seg_drv *seg_device)
{
	if(seg_device != NULL)
	{
		seg_display(seg_device);
	}
	else
	{
		printf("seg_device is err\r\n");
	}
}

void seg_display_task(void *param)
{	
	seg_display_init(param);

	while(1)
	{
		seg_display_control(param);
		vTaskDelay(3);
	}
}

void do_create_seg_display_task(void)
{	
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(seg_display_task, "seg_display_task", 64, (void *)&seg8bit_dev, 1, NULL);

	if(pdPASS == xReturn)	
	{
		printf("do_create_seg_display_task is create\r\n");
	}
	else
	{
		printf("do_create_seg_display_task is not create\r\n");
	}
	
}

#endif /* SEG_DISPLAY_MODULE */


