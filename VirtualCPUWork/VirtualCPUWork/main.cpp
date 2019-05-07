#include"CPU.h"


int main(void)
{
	
	CPU cpu;
	/*1.将内存中的指令移至cache中*/
	cpu.init_cache();
	/*2.取出指令,执行指令*/
	cpu.work();
	
	return 0;
}