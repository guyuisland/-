#include"CPU.h"
string str1[20][3];//存放整个txt文件中的内容
CPU::CPU()
{
	PC = 0;
	IB.clear();
	IR.clear();
	DR.clear();
	AR = 0;
	Z = 0;
	D = 0;
	for (int i = 0;i < 4;i++)
	{
		R[i] = 0;
	}
	PSW.C = false;
	PSW.N = false;
	PSW.V = false;
	PSW.Z = false;
	control.cache_RD = false;
	control.cache_WR = false;
	control.CLAR = false;
	control.CLDR = false;
	control.CLPC = false;
	control.CPIR = false;
	control.CPMEM = false;
	control.CPZ = false;
	control.CPD = false;
	control.IR__IB = false;
	control.PCINC = false;
	control.PC__IB = false;
	control.DR__IB = false;
	control.R_CPCR = false;
	control.R_IB = false;
	//control.R_RD = false;
	control.R_WR = false;
	control.Z_IB = false;
	control.ADD = false;
	control.SUB = false;
	//control.cache_SELECT = -1;
	control.R_SELECT = -1;
	control.PSW_CLR = false;
}

CPU::~CPU()
{
	
}

void CPU::renewCPU()
{
	if (control.PSW_CLR == true)//状态寄存器清零
	{
		PSW.C = 0;
		PSW.N = 0;
		PSW.V = 0;
		PSW.Z = 0;
	}
	if (control.PC__IB == true)//PC到总线
	{
		IB.clear();
		IB.immediate_data = PC;
	}
	if (control.PCINC == true )//PC加一
	{
		PC++;
	}
	if (control.PC__IB == false&&control.CLPC==true)//数据总线赋值到PC
	{
		PC = IB.jump_to;
	}
	if (control.CPIR == true && control.IR__IB == false)//总线到指令寄存器
	{
		IR = IB;
	}
	if (control.IR__IB == true)//指令寄存器到总线
	{
		IB = IR;
	}
	if (control.CLAR == true)//总线到AR
	{
		AR = IB.immediate_data;
	}
	if (control.CPMEM == 1 && control.cache_RD == 1)//读出内存中的指令
	{
		if (AR >= 0)
		{
			DR = cache[AR];
		}
		else
		{
			cout << AR << "AR无效的选择" << endl;
		}
	}
	//写操作未完成
	if (control.CLDR == 1&&control.DR__IB==0)//总线到数据缓存器
	{
		DR = IB;
	}
	if (control.DR__IB == 1)//数据缓存器到总线
	{
		IB = DR;
	}
	if (control.Z_IB == 1)//暂存器到总线
	{
		IB.clear();
		IB.immediate_data = Z;
	}
	if (control.CPD == 1)//总线到累加器
	{
		D = IB.immediate_data;
	}
	if (control.CPZ == 1)
	{
		if (control.ADD == 1)//相加
		{
			Z = D + IB.immediate_data;
		}
		if (control.SUB == 1)//相减
		{
			Z = D - IB.immediate_data;
		}
		if(control.ADD == 0&& control.SUB==0)
		{
			Z = D;
		}
	}
	else
	{
		if (control.ADD == 1)
		{
			if (D + IB.immediate_data == 0)//结果为零
			{
				PSW.Z = 1;
			}
			else
			{
				PSW.Z = 0;
			}
			if (D + IB.immediate_data < 0)//结果小于0
			{
				PSW.N = 1;
			}
			else
			{
				PSW.N = 0;
			}
			if (D > 0 && IB.immediate_data > 0)
			{
				if (D + IB.immediate_data < 0)//结果溢出
				{
					PSW.V = 1;
					PSW.C = 1;
				}
			}
			else if(D < 0 && IB.immediate_data < 0)
			{
				if (D + IB.immediate_data > 0)//结果溢出
				{
					PSW.V = 1;
					PSW.C = 1;
				}
			}
		}
		if (control.SUB == 1)
		{
			if (D - IB.immediate_data == 0)//结果为零
			{
				PSW.Z = 1;
			}
			else
			{
				PSW.Z = 0;
			}
			if (D - IB.immediate_data < 0)//结果小于0
			{
				PSW.N = 1;
			}
			else
			{
				PSW.N = 0;
			}
			if (D > 0 && IB.immediate_data < 0)
			{
				if (D - IB.immediate_data > 0)//结果溢出
				{
					PSW.V = 1;
					PSW.C = 1;
				}
				else
				{
					PSW.V = 0;
					PSW.C = 0;
				}
			}
			if (D < 0 && IB.immediate_data>0)
			{
				if (D - IB.immediate_data < 0)//结果溢出
				{
					PSW.V = 1;
					PSW.C = 1;
				}
				else
				{
					PSW.V = 0;
					PSW.C = 0;
				}
			}
		}
	}
	if (control.R_CPCR == 1)//写
	{
		if (control.R_SELECT >= 0)//R_SELECT=immediate_data mov 0 3
		{
			R[control.R_SELECT] = IB.immediate_data;
		}
		else
		{
			cout << "无效的选择" << endl;
		}
	}
	if (control.R_CPCR == 0 && control.R_IB == 1)//读出
	{
		if (control.R_SELECT >= 0)
		{
			IB.clear();
			IB.immediate_data = R[control.R_SELECT];
		}
		else
		{
			cout << "无效的选择" << endl;
		}
	}
}

void CPU::printstate()
{
	cout << "PC:" << PC;
	cout << "\tAR:"<<AR;
	cout << "\tZ:" << Z;
	cout << "\tD;" << D << endl;
	for (int i = 0;i < 4;i++)
	{
		cout << "R" << i << ":" << R[i] << "\t";
	}
	cout << endl;
	cout << "IB:";
	IB.show();
	cout << "IR:";
	IR.show();
	cout << "DR:";
	DR.show();
	cout << "C:" << PSW.C<<"\t" << "N:" << PSW.N <<"\t"<< "V:" << PSW.V <<"\t"<< "Z:" << PSW.Z << endl;
}

void CPU::changestate()
{
	int select = 0;
	while (select != 10)
	{
		cout << "1.PC" << endl;
		cout << "2.AR" << endl;
		cout << "3.D" << endl;
		cout << "4.Z" << endl;
		cout << "5.R0" << endl;
		cout << "6.R1" << endl;
		cout << "7.R2" << endl;
		cout << "8.R3" << endl;
		cout << "9.IB" << endl;
		cout << "10.不改变" << endl;
		cout << "请输入您的选择:";
		cin >> select;
		if (select != 10)
		{
			int idata;
			cout << "请输入改变的数据:";
			cin >> idata;
			switch (select)
			{
			case 1:
				PC = idata;
				break;
			case 2:
				AR = idata;
				break;
			case 3:
				D = idata;
				break;
			case 4:
				Z = idata;
				break;
			case 5:
				R[0] = idata;
				break;
			case 6:
				R[1] = idata;
				break;
			case 7:
				R[2] = idata;
				break;
			case 8:
				R[3] = idata;
				break;
			case 9:
				R[4] = idata;
				break;;
			default:
				break;
			}
		}
	}
	

}

DI::DI()
{
	immediate_data = -1;
	from = -1;    // -1: immediate data; 0~3: source register
	to = -1;        // 0~3: destination register
	jump_to = -1;    //
	L_number = -1;
}

void DI::clear()
{
	immediate_data = -1;
	from = -1;    // -1: immediate data; 0~3: source register
	to = -1;        // 0~3: destination register
	jump_to = -1;    //
	L_number = -1;
	instruction = nothing;
}

void DI::show()
{
	switch (instruction)
	{
	case 0:
	{
		cout << "mov\t";
		if (from == -1)
		{
			cout << immediate_data<<"  ";
		}
		else
		{
			cout << "R" << from<<"  ";
		}
		cout << "R" << to << endl;
		break;
	}
	case 1:
	{
		cout << "jmp\t";
		cout << jump_to << endl;
		break;
	}
	case 2:
	{
		cout << "add\t";
		if (from == -1)
		{
			cout << immediate_data << "  ";
		}
		else
		{
			cout << "R" << from << "  ";
		}
		cout << "R" << to << endl;
		break;
	}
	case 3:
	{
		cout << "cmp\t";
		if (from == -1)
		{
			cout << immediate_data << "  ";
		}
		else
		{
			cout << "R" << from << "  ";
		}
		cout << "R" << to << endl;
		break;
	}
	case 4:
	{
		cout << "jle\t";
		cout << jump_to << endl;
		break;
	}
	case 5:
	{
		cout << "start" << endl;
		break;
	}
	case 6:
	{
		cout << "end" << endl;
		break;
	}
	case 7:
	{
		cout << "L" << L_number<<endl;
		break;
	}
	default:
		cout << immediate_data<<endl;
		break;
	}
}
construction converse_name(string a)
{
	if (a.compare("movl") == 0) {
		return movl;
	}
	if (a.compare("jmp") == 0) {
		return jmp;
	}
	if (a.compare("addl") == 0) {
		return addl;
	}
	if (a.compare("cmpl") == 0) {
		return cmpl;
	}
	if (a.compare("jle") == 0) {
		return jle;
	}
	if (a.compare("start") == 0) {
		return start;
	}
	if (a.compare("end") == 0) {
		return _end;
	}
	if (a[0] == '.' || a[1] == 'L') {
		return L;
	}
	else {
		cout << "there is someting wrong with you input, please check that again";
		exit(9);
	}

}
int judge_lines(string str) {
	str += ':';
	cout << "***" << str << "***";
	for (int i = 0; str1[i][0].compare("end") != 0; i++) {
		if (strcmp(str1[i][0].c_str(), str.c_str()) == 0)
			return i + 1;
	}
}
void DI::init_DI(string *s, int line) {
	instruction = converse_name(s[0]);
	L_number = line;
	switch (instruction) {
	case movl:
	case addl:
	case cmpl:
		switch (s[1][0]) {
		case '$':
			immediate_data = atoi(s[1].substr(1, s[1].length() - 2).c_str());
			if (s[2].compare("R0") == 0)
				to = 0;
			if (s[2].compare("R1") == 0)
				to = 1;
			if (s[2].compare("R2") == 0)
				to = 2;
			break;
		case 'R':
			if (s[1].compare("R0,") == 0)
				from = 0;
			if (s[1].compare("R1,") == 0)
				from = 1;
			if (s[1].compare("R2,") == 0)
				from = 2;

			if (s[2].compare("R0") == 0)
				to = 0;
			if (s[2].compare("R1") == 0)
				to = 1;
			if (s[2].compare("R2") == 0)
				to = 2;
			break;
		}
		break;

	case jmp:
	case jle:
		/*immediate_data = -1;
		from = -1;
		to = -1;*/
		if (s[1].compare(".L2") == 0)
			jump_to = judge_lines(s[1]);
		if (s[1].compare(".L3") == 0)
			jump_to = judge_lines(s[1]);
		break;
	}
}


int CPU::init_cache()
{
	cache.clear();
	string str;
	//string str1[20][3];
	str.clear();
	ifstream myfile("dzy.txt");
	if (!myfile.is_open())
	{
		cout << "can not open this file" << endl;
		return 0;
	}
	char ch;
	int i, j;
	i = j = 0;
	myfile.get(ch);
	for (i = 0; i <= 12; i++, myfile.get(ch)) {
		for (j = 0; ch != '\n'; j++) {
			if (ch != ' ') {
				myfile.seekg(-1, ios::cur);
			}
			myfile >> str1[i][j];
			myfile.get(ch);
		}
	}
	i = 0;
	for (i = 0; str1[i][0].compare("end")!=0; i++) {
		class DI temp;
		temp.init_DI(str1[i], i);
		//if (temp.instruction != 7)//不是L进入指令寄存器内
		{
			cache.push_back(temp);
		}
	}
	class DI temp;
	temp.init_DI(str1[i], i);
	cache.push_back(temp);
	return 0;
}
void CPU::movl(DI instruct)
{
	if (instruct.from == -1)
	{	// 立即数mov
		control.IR__IB = true;
		renewCPU();
		control.IR__IB = false;

		control.R_CPCR = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_CPCR = false;
	}
	else
	{	// 寄存器mov
		control.R_CPCR = false;
		control.R_IB = true;
		control.R_SELECT = instruct.from;
		renewCPU();
		control.R_IB = false;

		control.R_CPCR = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_CPCR = false;
	}
}

void CPU::jmp(DI instruct)
{
	control.IR__IB = true;
	renewCPU();
	control.IR__IB = false;

	control.PC__IB = false;
	control.CLPC = true;
	renewCPU();
	control.CLPC = false;
}

void CPU::addl(DI instruct)
{
	if (instruct.from == -1)
	{	// 立即数add
		control.R_CPCR = false;
		control.R_IB = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_IB = false;

		control.CPD = true;
		renewCPU();
		control.CPD = false;

		control.IR__IB = true;
		renewCPU();
		control.IR__IB = false;

		control.ADD = true;
		control.CPZ = true;
		renewCPU();
		control.ADD = false;
		control.CPZ = false;

		control.Z_IB = true;
		renewCPU();
		control.Z_IB = false;

		control.R_CPCR = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_CPCR = false;
	}
	else
	{	// 寄存器add
		control.R_CPCR = false;
		control.R_IB = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_IB = false;

		control.CPD = true;
		renewCPU();
		control.CPD = false;

		control.R_CPCR = false;
		control.R_IB = true;
		control.R_SELECT = instruct.from;
		renewCPU();
		control.R_IB = false;

		control.ADD = true;
		control.CPZ = true;
		renewCPU();
		control.ADD = false;
		control.CPZ = false;

		control.Z_IB = true;
		renewCPU();
		control.Z_IB = false;

		control.R_CPCR = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_CPCR = false;
	}
}

void CPU::cmpl(DI instruct)
{
	if (instruct.from == -1)
	{	// 立即数cmpl
		control.R_CPCR = false;
		control.R_IB = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_IB = false;

		control.CPD = true;
		renewCPU();
		control.CPD = false;

		control.IR__IB = true;
		renewCPU();
		control.IR__IB = false;

		control.SUB = true;
		renewCPU();
		control.SUB = false;
	}
	else
	{	// 寄存器cmpl
		control.R_CPCR = false;
		control.R_IB = true;
		control.R_SELECT = instruct.to;
		renewCPU();
		control.R_IB = false;

		control.CPD = true;
		renewCPU();
		control.CPD = false;

		control.R_CPCR = false;
		control.R_IB = true;
		control.R_SELECT = instruct.from;
		renewCPU();
		control.R_IB = false;

		control.SUB = true;
		renewCPU();
		control.SUB = false;
	}
}

void CPU::jle(DI instruct)
{
	if (PSW.N != PSW.V || PSW.Z == true)
	{	// 小于等于则跳转
		control.IR__IB = true;
		renewCPU();
		control.IR__IB = false;

		control.PC__IB = false;
		control.CLPC = true;
		renewCPU();
		control.CLPC = false;
	}
	control.PSW_CLR == true;
	renewCPU();
	control.PSW_CLR == false;
}

void CPU::read_instruction()
{
	control.PC__IB = true;
	renewCPU();

	control.PC__IB = false;

	control.PCINC = true;
	renewCPU();
	control.PCINC = false;

	control.CLAR = true;
	renewCPU();
	control.CLAR = false;
	
	control.CPMEM = true;
	control.cache_RD = true;
	renewCPU();
	control.CPMEM = false;
	control.cache_RD = false;

	control.DR__IB = true;
	renewCPU();
	control.DR__IB = false;

	control.CPIR = true;
	renewCPU();
	control.CPIR = false;
}

void CPU::work() {
	for(auto p:cache)
	{
		p.show();
	}
	read_instruction();
	while (IR.instruction !=6)
	{	
		// 循环至指令为end
		if (IR.instruction == 7)
		{
			read_instruction();
			continue;
		}
		IR.show();
		switch (IR.instruction)
		{
		case 0:
			movl(IR);
			break;
		case 1:
			jmp(IR);
			break;
		case 2:
			addl(IR);
			break;
		case 3:
			cmpl(IR);
			break;
		case 4:
			jle(IR);
			break;
		default:
			break;
		}
		this->printstate();
		cout << "请问是否改变状态,y代表改变,其他代表不改变:";
		char y_or_n;
		cin >> y_or_n;
		if (y_or_n == 'y')
		{
			changestate();
		}
		read_instruction();	// PC,读指令
	}
}

