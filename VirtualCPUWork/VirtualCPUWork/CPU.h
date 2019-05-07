#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;
/*typedef struct immediate_data_Instruction
{
	int immediate_data1;
	int immediate_data2;
	string addr1;
	string addr2;
	string instruction;
	 
}DI;*/
enum construction
{
	movl, jmp, addl, cmpl, jle, start, _end, L,nothing
};
construction converse_name(string a);
class DI {
public:
	construction instruction;
	DI();
	void init_DI(string *s, int line);
	int immediate_data;
	int from;
	int to;
	int jump_to;
	int L_number;
	void clear();
	void show();
	DI &operator=(const DI& a)
	{
		this->from = a.from;
		this->to = a.to;
		this->immediate_data = a.immediate_data;
		this->jump_to = a.jump_to;
		this->instruction = a.instruction;
		this->L_number = a.instruction;
		return *this;
	}
};
struct ProgramStatusWord
{
	bool C;//进位标志
	bool V;//溢出标志
	bool Z;//零标志
	bool N;//负标志
};
struct ControlSignal
{
	bool PCINC;//pc++信号
	bool cache_WR;//cache写入信号
	bool cache_RD;//cache读入信号
	bool R_WR;//R写入信号
	bool R_RD;//R读入信号 
	bool R_CPCR;//R读写使能
	bool CLPC;//写入脉冲
	bool CPIR;//写入脉冲
	bool CPZ;//写入脉冲
	bool CPD;//写入脉冲
	bool CLAR;//写入脉冲
	bool CLDR;//写入脉冲
	bool CPMEM;//写入读出脉冲
	bool Z_IB;//Z到IB的三态门使能
	bool R_IB;//R到IB的三态门使能
	bool PC__IB;//PC到IB的双向三态门使能
	bool IR__IB;//IR到IB的双向三态门使能
	bool DR__IB;//
	bool ADD;//加法使能
	bool SUB;//减法使能
	bool PSW_CLR;//状态寄存器清零使能
	int R_SELECT;//R字节选择
	int cache_SELECT;//cache字节选择

};
class CPU
{
public:
	CPU();
	~CPU();
	void renewCPU();
	void printstate();
	void changestate();
	
	void read_instruction();
	//void condition(construction instruction, int from = 0, int to = 0);
	void work();
	void movl(DI);
	void jmp(DI);
	void addl(DI);
	void cmpl(DI);
	void jle(DI);

	int init_cache();
private:
	/*各类器件的定义*/
	vector<DI> cache;//高速缓存存放指令
	int PC;//程序计数器
	DI IB;//内部总线
	DI IR;//指令寄存器
	DI DR;//数据缓冲寄存器
	int AR;//数据地址寄存器
	int Z;//暂存器
	int D;//累加器
	int R[4];//通用寄存器R0-R4
	ProgramStatusWord PSW; //状态寄存器
	ControlSignal control;
	/*------------------------------------------------------------*/
};



