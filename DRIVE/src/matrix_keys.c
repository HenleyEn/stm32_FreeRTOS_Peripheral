#include "matrix_keys.h"
#include "include.h"

#ifdef MATRIX_KEYS_MODULE

#define MATRIX_KEYS_CLK			RCC_APB2Periph_GPIOA
#define MATRIX_KEYS_ROW_PORT	GPIOA
#define MATRIX_KEYS_COL_PORT 	GPIOA
#define ROWS_PINS				GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define COLS_PINS				GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3

void matrix_keys_row_scan_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(MATRIX_KEYS_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = COLS_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MATRIX_KEYS_COL_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = ROWS_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MATRIX_KEYS_ROW_PORT, &GPIO_InitStructure);	
}

void matrix_keys_col_scan_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(MATRIX_KEYS_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = COLS_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MATRIX_KEYS_COL_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = ROWS_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MATRIX_KEYS_ROW_PORT, &GPIO_InitStructure);	
}

uint8_t matrix_keys_get_key_num(void)
{
	uint8_t col;
	uint8_t row;
	uint8_t key_num;
	uint16_t read_value;
	
	matrix_keys_col_scan_init();
	GPIO_ResetBits(MATRIX_KEYS_COL_PORT, COLS_PINS);
	GPIO_SetBits(MATRIX_KEYS_ROW_PORT, ROWS_PINS);

	read_value = GPIO_ReadInputData(MATRIX_KEYS_ROW_PORT);
	read_value &= 0x00FF;

	if(read_value != 0x00F0)
	{
		vTaskDelay(10);
		
		read_value = GPIO_ReadInputData(MATRIX_KEYS_ROW_PORT);
		if(read_value != 0x00F0)
		{
			read_value &= 0x00F0;
			col = read_value;

			matrix_keys_row_scan_init();	
			GPIO_ResetBits(MATRIX_KEYS_ROW_PORT, ROWS_PINS);
			GPIO_SetBits(MATRIX_KEYS_COL_PORT, COLS_PINS);
		
			vTaskDelay(50);
			
			read_value = GPIO_ReadInputData(MATRIX_KEYS_COL_PORT);
			read_value &= 0x000F;
			row = read_value;
			
			key_num = col | row;

			return key_num;
		}
	}
	return NULL;
}

#define keys_num	16
uint8_t keys_value[keys_num] = {0x77, 0x7B, 0x7D, 0x7E, 
									0xB7, 0xBB, 0xBD, 0xBE, 
									0xD7, 0xDB, 0xDD, 0xDE, 
									0xE7, 0xEB, 0xED,0xED};

void matrix_keys_operate(void)
{
	uint8_t key_value;
	int i;
	key_value = matrix_keys_get_key_num();

	if(key_value != NULL)
	{
//		printf("key_num = %x\r\n", key_num);
		for(i = 0; i < keys_num; i++)
		{
			if(key_value == keys_value[i])
			{
				printf("input %d\r\n", i);
			}
		}
		
	}
}

void matrix_keys_task(void *param)
{

	while(1)
	{
//		matrix_keys_get_key_num();
		matrix_keys_operate();
		vTaskDelay(3);
	}
}

void do_matrix_keys_task(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(matrix_keys_task, "matrix_task", 64, "MATRIX_KEYS_MODULE", 1, NULL);

	if(pdPASS == xReturn)	
	{
		printf("do_matrix_keys_task is create\r\n");
	}
	else
	{
		printf("do_matrix_keys_task is not create\r\n");
	}
}


#endif /* MATRIX_KEYS_MODULE */


