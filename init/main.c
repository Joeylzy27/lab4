#include "put.h"
#include "sched.h"

int start_kernel()
{
	const char *msg = "ZJU OS LAB 4     Student1: 3190101094 朱理真 Student2:3190101931 李兆渊\n";
	const char *msg_protection_test[2]={
		"Try to write on rodata section: \n",
		"Try to execute on data section: \n"
	};
	puts(msg);
	puts(msg_protection_test[0]);
	extern unsigned long long rodata_start;
	rodata_start=0;

	puts(msg_protection_test[1]); // executable test is almost unrecoverable in each run, uncommented them carefully
	extern unsigned long long data_start;
	// putullHex((uint64_t)(&start_kernel)); //0x0478
	// putullHex((uint64_t)&data_start); //0x3000
	// void (*p)(void) = (void(*)(void))(&data_start);
	// (*p)();
	
	task_init();
	dead_loop();
	return 0;
}