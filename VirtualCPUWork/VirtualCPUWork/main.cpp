#include"CPU.h"


int main(void)
{
	
	CPU cpu;
	/*1.���ڴ��е�ָ������cache��*/
	cpu.init_cache();
	/*2.ȡ��ָ��,ִ��ָ��*/
	cpu.work();
	
	return 0;
}