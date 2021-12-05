#include "put.h"
#include "sched.h"
#define uint64_t unsigned long long
int count = 0;
void handler_s(uint64_t cause,uint64_t* ptr_epc)
{
	if (cause >> 63)
	{ // interrupt
		if (((cause << 1) >> 1) == 5)
		{ // supervisor timer interrupt
			asm volatile("ecall");
			do_timer();
			count++;
		}
	}
	else
	{
		puts("Exeception has occured");
		puts(", caused by: 0x");
		putullHex(cause);
		puts(", at 0x");
		putullHex(*ptr_epc);
		puts("\n");
		if (cause == 12)
		{
			puts("instruction page fault\n");
		}
		else if (cause == 13)
		{
			puts("load page fault\n");
		}
		else if (cause == 15)
		{
			puts("store page fault\n");
		}
		(*ptr_epc)+=4;
	}
	return;
}