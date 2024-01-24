#include "include.h"
#include "task_manage.h"

int main()
{	
	do_create_task_start();
	return 0;
}

int fputc(int ch, FILE *f)
{  
    SEGGER_RTT_PutChar(0, ch);
 	return ch;
}

