#ifndef _SEG_DISPLAY_H
#define _SEG_DISPLAY_H

#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DYNAMIC_74HC138

typedef struct 
{
	uint32_t		seg_clk;
	GPIO_TypeDef 	*seg_port;
	uint32_t		seg_gpio_pins;

#ifdef DYNAMIC_74HC138
	uint32_t		select_bit_clk;
	GPIO_TypeDef 	*select_bit_port;
	uint32_t		select_bit_gpio_pins;	
#endif /* DYNAMIC_74HC138 */
	
}seg_def;

typedef struct 
{
	const seg_def *def;
	uint8_t seg_counts;
	const uint8_t *seg_display_num;

	
}seg_drv;


//void seg_display_init(seg_drv *dev);

void seg_display_task(void *param);
void do_create_seg_display_task(void);

#ifdef __cplusplus
}
#endif


#endif /* _SEG_DISPLAY_H */

