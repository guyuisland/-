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
	bool C;//��λ��־
	bool V;//�����־
	bool Z;//���־
	bool N;//����־
};
struct ControlSignal
{
	bool PCINC;//pc++�ź�
	bool cache_WR;//cacheд���ź�
	bool cache_RD;//cache�����ź�
	bool R_WR;//Rд���ź�
	bool R_RD;//R�����ź� 
	bool R_CPCR;//R��дʹ��
	bool CLPC;//д������
	bool CPIR;//д������
	bool CPZ;//д������
	bool CPD;//д������
	bool CLAR;//д������
	bool CLDR;//д������
	bool CPMEM;//д���������
	bool Z_IB;//Z��IB����̬��ʹ��
	bool R_IB;//R��IB����̬��ʹ��
	bool PC__IB;//PC��IB��˫����̬��ʹ��
	bool IR__IB;//IR��IB��˫����̬��ʹ��
	bool DR__IB;//
	bool ADD;//�ӷ�ʹ��
	bool SUB;//����ʹ��
	bool PSW_CLR;//״̬�Ĵ�������ʹ��
	int R_SELECT;//R�ֽ�ѡ��
	int cache_SELECT;//cache�ֽ�ѡ��

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
	/*���������Ķ���*/
	vector<DI> cache;//���ٻ�����ָ��
	int PC;//���������
	DI IB;//�ڲ�����
	DI IR;//ָ��Ĵ���
	DI DR;//���ݻ���Ĵ���
	int AR;//���ݵ�ַ�Ĵ���
	int Z;//�ݴ���
	int D;//�ۼ���
	int R[4];//ͨ�üĴ���R0-R4
	ProgramStatusWord PSW; //״̬�Ĵ���
	ControlSignal control;
	/*------------------------------------------------------------*/
};



