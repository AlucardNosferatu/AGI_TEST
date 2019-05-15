// �����ź�.cpp : �������̨Ӧ�ó������ڵ㡣
//
//�����У��Ĵ������ڴ������������������
//�Ĵ����϶�Ҫ��ָ�룬�ڴ����ҲҪ��ָ�루��list������ģ���ڴ棩������ͳһΪvoid*
//������ֱ����һ��Object����Ϳ�����
//����ȫ������һ��ֻ�ܴ洢short��������һ���洢Object�������󣬸ö���ָ��һ��short����

#include "stdafx.h"
#include<vector>
#include<string>
#include<iostream>
#include"strsplit.h"
#include"getIndex.h"
#include<list>

using namespace std;

#define SHORTARRAY vector<short>
#define LEFT_IS_MEMORY true
#define LEFT_IS_REG false

class Solution
{
public:
	SHORTARRAY steps;
	bool equals(Solution other)
	{
		if (this->steps.size() != other.steps.size()) return false;
		for (int i = 0; i < steps.size(); i++)
			if (this->steps[i] != other.steps[i]) return false;
		return true;
	}
	string toString();
	bool execute();
};

vector<short> memory;//�ڴ棬ֻ������������������
vector<short> enumResults;

class Register
{
public:
	string name="NULL";
	short value=0;
	Register(){}
	Register(string name) { this->name = name; }
	void toNull() { name = "NULL"; value = 0; }
	bool isNull() { return "NULL" == name; }
	string toString() 
	{
		string str_value;
		char temp[10] = {};
		_itoa(value, temp, 10);
		str_value = temp;
		return name + "=" + str_value;
	}
};

Register regPtr;

#define MAR_CODE 0
#define MDR_CODE 1
#define ALU_CODE 2
#define Y_CODE 3
#define Z_CODE 4
#define R1_CODE 5
#define R2_CODE 6
#define IR1_CODE 7
#define IR2_CODE 8
#define REGPTR_CODE 9





void addEnum()
{
	short lastVerbCode = enumResults[enumResults.size() - 1] / 100;
	short min = 100 * (lastVerbCode + 1);
	short max = min + 99;
	for (short i = min; i <= max; i++)
	{
		short target = i % 100 / 10;//ʮλ��Ŀ��Ĵ������
		short source = i % 10;//��λ��Դ�Ĵ������
		if (target == source) continue;//Ŀ��Ĵ��������Ժ�Դ�Ĵ���һ��
		if (target == IR1_CODE || target==IR2_CODE) continue;//���ܸĶ�IR1��IR2�������Ĵ���
		
		enumResults.push_back(i);//��ӵ�ö��������
	}
}

Register MAR=*new Register("MAR");
Register MDR = *new Register("MDR");
Register ALU = *new Register("ALU");
Register Y = *new Register("Y");
Register Z = *new Register("Z");
Register R1 = *new Register("R1");
Register R2 = *new Register("R2");
Register IR1= *new Register("IR1");
Register IR2 = *new Register("IR2");
Register* registers[10] = { &MAR,&MDR,&ALU,&Y,&Z,&R1,&R2,&IR1,&IR2,&regPtr};
int getIndexOfRegister(string name)
{
	for (int i = 0; i<REGPTR_CODE; i++)
		if (registers[i]->name == name)
			return i;
	return -1;
}
int getIndexOfRegister(Register* reg)
{
	for (int i = 0; i <REGPTR_CODE; i++)
		if (registers[i]==reg)
			return i;
	return -1;
}

void clear()
{
	regPtr.toNull();
	IR1.value = 0;
	IR2.value = 0;
}

void refresh_passive()
{
	if ("NULL" == regPtr.name)
		regPtr.value = 0;
	else
		regPtr = *registers[getIndexOfRegister(regPtr.name)];
}

void refresh_active()
{
	if (regPtr.name != "NULL")
		*registers[getIndexOfRegister(regPtr.name)] = regPtr;
}

vector<Register> getRegisterArray() //��������Register[]
{
	vector<Register> returning;
	for (int i = 0; i < 10; i++)
		returning.push_back(*registers[i]);
	return returning;
}

void setRegisters(vector<Register> regs)
{
	//cout << "���ԣ��������мĴ���\n";
	for (int i = 0; i < 10; i++)
		*registers[i] = regs[i];
	//cout << "���ԣ��������\n";
}

class Equation
{
public:
	int left_index;
	short right_value;
	bool leftIsMemory;
	bool judge()
	{
		if (leftIsMemory==false)
			return registers[left_index]->value == right_value;
		else
			return memory[left_index] == right_value;
	}
	Equation(){}
	Equation(int left_index, short right_value, bool leftIsMemory)
	{
		this->left_index = left_index;
		this->right_value = right_value;
		this->leftIsMemory = leftIsMemory;
	}
	void toBeTrue()
	{
		if (leftIsMemory==false)//�ǼĴ���
		{
			registers[left_index]->value = right_value;
			if (left_index == REGPTR_CODE) refresh_active();
			else refresh_passive();
		}
		else
			memory[left_index] = right_value;
	}
};

bool RM(Register* a, Register* b)//���ڴ�Read Memory
{
	if (memory.size() - 1 < MAR.value)//�ڴ������±�Խ��
		return false;
	else
	{
		MDR.value = memory[MAR.value];//����ַΪMAR���ڴ浥Ԫ�����ݷŵ��ݴ���MDR��
		return true;
	}
}

bool WM(Register* a, Register* b) //д�ڴ�
{
	if (memory.size() - 1 < MAR.value) //�±�Խ��
		return false;
	else
	{
		memory[MAR.value] = MDR.value;//���ݴ���MDR�е�����д����ַΪMAR���ڴ浥Ԫ��
		return true;
	}
}

bool ALU_plus(Register* a, Register* b) //ALU�ӷ������ź�
{
	Z.value = ALU.value + Y.value;
	return true;
}

bool ALU_minus(Register* a, Register* b) //ALU���������ź�
{
	if (ALU.value >= Y.value)
	{
		Z.value = ALU.value - Y.value; //ǿ�ƹ涨������>=����
		return true;
	}
	else
		return false;
}

bool mov(Register* a, Register* b)
{
	if (a->isNull() || b->isNull()) return false;//�Ĵ���ָ������ǿյģ��ͷ��ء�ʧ�ܡ�

	a->value = b->value;//��Ĵ���a��ֵ���ڼĴ���b��ֵ

	if (a == &regPtr) refresh_active();//ָ�������޸ļĴ�����ֵ
	return true;
}


#define FUNCTION_NOARG_LENGTH 4
#define FUNCTION_NOARG_LASTINDEX 3
#define FUNCTION_DOUBLEARG_FIRSTINDEX 4
#define BASICFUNCTION_LENGTH 5
#define BASICFUNCTION_LASTINDEX 4
#define USERFUCTION_FIRSTINDEX BASICFUNCTION_LENGTH

typedef bool(*FunctionPointer)(Register*,Register*);//����ָ��
FunctionPointer basicFunctions[BASICFUNCTION_LENGTH] = { &RM,&WM,&ALU_minus,&ALU_plus,&mov };
string names_basicFunctions[BASICFUNCTION_LENGTH] = { "RM","WM","PLUS","MINUS","MOV"};

string getNameOfBasicFunction(FunctionPointer fp)
{
	for (int i = 0; i <= FUNCTION_NOARG_LASTINDEX; i++)
		if (fp == basicFunctions[i])
			return names_basicFunctions[i];
}

int getIndexOfBasicFunction(string name)
{
	for (int i = 0; i <= BASICFUNCTION_LASTINDEX; i++)
		if (names_basicFunctions[i] == name)
			return i;
	return -1;
}

bool executeCode_basic(short code)
{
	if ((code/100) > BASICFUNCTION_LASTINDEX)
		cout << "���ԣ������ⲻ��һ������������code="<<code<<endl;
	short verb = code / 100;
	short target = code % 100 / 10;
	short source = code % 10;
	return basicFunctions[verb](registers[target], registers[source]);
}

class Function
{
public:
	string name;
	vector<short> sentences;
	bool targetIsMemory = false;
	Function() { }
	Function(string name) { this->name = name; }
	bool execute(Register* target, Register* source);
};
vector<Function> userFunctions;

bool Function::execute(Register* target, Register* source)
{
	if (target->isNull() || source->isNull()) return false;
	vector<short> bak_memory = ::memory;//�����ڴ�
	vector<Register> bak_regs = ::getRegisterArray();//���ݼĴ���
	Register bak_regPtr = *registers[REGPTR_CODE];//���ݼĴ���ָ��

	if (targetIsMemory) //����Ŀ�����ڴ浥Ԫ
	{
		regPtr.toNull();//�Ĵ���ָ�����
		IR1.value = target->value;//��ߵ�������������
	}
	else
	{
		regPtr = *target;//ָ��ָ��Ŀ��Ĵ���
		IR1.value = 0;//����Ҫ��������������
	}

	IR2.value = source->value;//���ܺ����Ƿ������ڴ棬�ұ���Զ��������������

	bool whole_succeed = true;//����������������䣩�Ƿ�ִ�гɹ�
	for (int i = 0; i < sentences.size(); i++)
	{
		bool current_succeed = true;//��ǰ����Ƿ�ִ�гɹ�
		if (sentences[i] / 100 <= BASICFUNCTION_LASTINDEX)
			current_succeed = executeCode_basic(sentences[i]);
		else
		{
			Function f = userFunctions[sentences[i] / 100 - USERFUCTION_FIRSTINDEX];
			Register* _target = registers[sentences[i] % 100 / 10];
			Register* _source = registers[sentences[i] % 10];
			current_succeed = f.execute(_target, _source);
			if (_target == &regPtr) {
				/*
				cout << "Ŀ���ǼĴ���ָ�룬��Դ��" << _source->name << "��";
				cout << "ˢ��ǰREGPTR=" << regPtr.name << "=" << regPtr.value
					<<"��ָ��ļĴ�����ֵ��"<<
					registers[getIndexOfRegister(regPtr.name)]->value<< endl;
				*/
				refresh_active();//����ˢ��
				/*
				cout << "ˢ��ǰREGPTR=" << regPtr.name << "=" << regPtr.value
					<< "��ָ��ļĴ�����ֵ��" <<
					registers[getIndexOfRegister(regPtr.name)]->value << endl;
				*/
			}
		}
		if (current_succeed == false) {
			whole_succeed = false;
			break;//�������������
		}
	}

	//�ָ�ԭ��
	if (targetIsMemory) //�����������ڴ�
	{
		::setRegisters(bak_regs);//�Ĵ���ȫ���ָ�ԭֵ��ָ��Ҳ��һ��
	}
	else //���������ǼĴ���
	{
		::memory = bak_memory;//�ָ��ڴ�
		for (int i = 0; i <= REGPTR_CODE-1; i++)
		{
			if (registers[i]->name == target->name) continue;
			else *registers[i] = bak_regs[i];
		}
		
		if (target == &regPtr) //�������������ǼĴ���ָ��
			refresh_active();//����ˢ��
		else
			refresh_passive();//����

		//ע���ˣ�����ط���Σ�գ�����
		regPtr.name = bak_regPtr.name;//�Ĵ���ָ�뵥������
		refresh_passive();

	}
	return whole_succeed;
}

vector<short> convertIntoBitArray(long longNumber,int radix,int size)
{
	//��longNumberת��Ϊ����Ϊsize��radix�����������Ȳ������λ����
	vector<short> result;
	long q = longNumber;
	long r = 0;
	while (q > 0)
	{
		r = q % radix;
		q = q / radix;
		result.push_back(r);
	}

	if (result.size() < size)
		for (int i = 1; i <= size - result.size(); i++)
			result.push_back(0); //��λ����

	vector<short> reversed;
	for (int a = (result.size() - 1); a >= 0; a--) //������
	{
		if (a < 0) break;
		reversed.push_back(result[a]);
	}
	return reversed;
}

bool executeCode(short code)
{
	short verb = code / 100;//���������ʣ����
	short target = code % 100 / 10; //Ŀ��Ĵ������
	short source = code % 10; //Դ�Ĵ������
	if (verb <= BASICFUNCTION_LASTINDEX) return executeCode_basic(code);
	else 
	{
		bool succeed=
			userFunctions[verb - USERFUCTION_FIRSTINDEX].execute(registers[target], registers[source]);
		
		return succeed;
	}
}

bool Solution::execute()
{
	for (int i = 0; i < this->steps.size(); i++)
	{
		bool succeed=executeCode(this->steps[i]);
		if (succeed == false) return false;
	}
	return true;
}

bool executeByIndex(short index) //ִ�С�ö�ٽ�����������±�Ϊindex������
{
	return executeCode(enumResults[index]);
}

bool executeByIndexes(vector<short> indexes) //ִ�ж������
{
	for (int i = 0; i < indexes.size(); i++)
	{
		bool succeed = executeByIndex(indexes[i]);
		if (succeed == false) return false;
	}
	return true;
}

bool executeLongNumber(long longNumber, int radix,int size)
{
	vector<short> bitArray = convertIntoBitArray(longNumber,radix,size);
	//cout << "bitArray�ĳ���" << bitArray.size() << endl;
	/*�������
	for (int i = 0; i < bitArray.size(); i++)
		cout << bitArray[i] << " , ";
	cout << endl;
	*/
	//return true;
	return executeByIndexes(bitArray);
}

Solution createSolutionByLongNumber(long longNumber, int radix,int size)
{
	vector<short> bitArray = convertIntoBitArray(longNumber, radix,size);
	Solution sol = *new Solution();
	for (int i = 0; i < bitArray.size(); i++)
		sol.steps.push_back(enumResults[bitArray[i]]);
	
	return sol;
}

int getIndexOfUserFunction(string name)
{
	for (int i = 0; i <userFunctions.size(); i++)
		if (userFunctions[i].name == name)
			return i;
	return -1;
}

string getNameOfFunction(short code)
{
	if (code <= 4)
		return names_basicFunctions[code];
	else
		return userFunctions[code - 5].name;
}

class Sentence
{
public:
	short verb;
	short target=0;
	short source=0;
	Sentence(){}
	Sentence(short code)
	{
		verb = code / 100;
		target = code % 100 / 10;
		source = code % 10;
	}
	Sentence(short verbCode, short targetCode, short sourceCode)
	{
		verb = verbCode;
		target = targetCode;
		source = sourceCode;
	}
	string toString()
	{
		if (verb <= FUNCTION_NOARG_LASTINDEX) return ::names_basicFunctions[verb];
		else {
			string leftName;
			if (target == REGPTR_CODE) leftName = "REGPTR";
			else leftName = registers[target]->name;
			string rightName = registers[source]->name;
			return getNameOfFunction(verb) + "|" + leftName + "|" + rightName;
		}
	}
	string toString_value()
	{
		if (verb <= FUNCTION_NOARG_LASTINDEX) return ::names_basicFunctions[verb];
		else {
			string leftName="";
			char temp[10] = {};
			if (target == IR1_CODE) {
				_itoa(IR1.value, temp, 10);
				leftName = temp;
			}
			else if (target == IR2_CODE) {
				_itoa(IR2.value, temp, 10);
				leftName = temp;
			}
			else {
				/*
				if (target == REGPTR_CODE)
					cout << "������ʾ�������" << target << endl;
				if (registers[target]->isNull())
					cout << "������ʾ����߼Ĵ���ָ���ǿյ�" << endl;
				*/
				leftName = registers[target]->name;
			}

			string rightName="";
			char temp2[10] = {};
			if (source == IR1_CODE) {
				_itoa(IR1.value, temp, 10);
				rightName = temp;
			}
			else if (source == IR2_CODE) {
				_itoa(IR2.value, temp, 10);
				rightName = temp;
			}
			else rightName = registers[source]->name;

			return getNameOfFunction(verb) + "|" + leftName + "|" + rightName;
		}

	}
	bool execute()
	{
		if (verb <= 4)
		{
			bool succeed=basicFunctions[verb](registers[target], registers[source]);
			if (target == REGPTR_CODE) refresh_active();//�����ı�
			else refresh_passive();//ָ���ֵ�������
		}
		else
		{
			//return false;
			return userFunctions[verb - 5].execute(registers[target], registers[source]);
		}
	}
};

string Solution::toString()
{
	string str = "";
	for (int i = 0; i < this->steps.size(); i++)
	{
		Sentence sentence = *new Sentence(steps[i]);
		string str_sentence = sentence.toString();
		str += str_sentence + "\n";
	}
	return str;
}

Sentence createSentence(string str)
{
	vector<string> strArr=split(str, '|');

	if (strArr.size() == 3)
	{
		//cout << "�зֺ�" + strArr[0] + "  " + strArr[1] + "  " + strArr[2] + "\n";
		int verbCode = getIndexOfBasicFunction(strArr[0]);
		if (verbCode == -1) verbCode = getIndexOfUserFunction(strArr[0]) + USERFUCTION_FIRSTINDEX;
		int target = getIndexOfRegister(strArr[1]);
		int source = getIndexOfRegister(strArr[2]);
		//cout << "target=" << target << "  source=" << source << endl;

		if (target == -1) {
			IR1.value = atoi((char*)(strArr[1].c_str()));
			regPtr.toNull(); 
		}
		else {
			IR1.value = 0;
			regPtr = *registers[target];
		}

		if (source == -1) {
			//cout << "���ԣ�IR2������ֵΪ" << atoi((char*)(strArr[2].c_str())) << endl;
			IR2.value = atoi((char*)(strArr[2].c_str()));//����
			//cout << "���ԣ�if(source==1)��ִ֧�����\n";
		}
		else {
			IR2.value = registers[source]->value;
		}

		short leftCode, rightCode;
		if (regPtr.isNull()) leftCode = IR1_CODE;
		else leftCode = getIndexOfRegister(regPtr.name);
		rightCode = IR2_CODE;

		//cout << "����newһ��Sentence������verbCode��leftCode��rightCode�ֱ���" <<
		//	verbCode << " " << leftCode << " " << rightCode << endl;;
		Sentence sentence = *new Sentence(verbCode,leftCode,rightCode);
		return sentence;
	}
	else //if(strArr.size()==1)
	{
		short verbCode = getIndexOfBasicFunction(strArr[0]);
		Sentence sentence = *new Sentence(verbCode, 0, 0);
		return sentence;
	}
}

string getNameOfRegister(Register* reg)
{
	if (NULL==reg) return "NULL";
	else return reg->name;
}

void showAllRegisters()
{
	for (int i = 0; i < REGPTR_CODE; i++)
		cout << registers[i]->toString() << "��";
	cout << "REGPTR=" + regPtr.toString() << endl;
}

void showMemory()
{
	for (int i = 0; i < memory.size(); i++)
	{
		cout << "[" << i << "]=" << memory[i];
		if (i != memory.size() - 1) cout << "��";
	}
	cout << endl;
}

void modifyRegister()
{
	string name;
	cout << "������Ҫ�޸ĵļĴ��������ƣ�";
	cin >> name;
	cout << "��" + name << "��ֵΪ��";
	short value;
	cin >> value;
	registers[getIndexOfRegister(name)]->value = value;
	showAllRegisters();
}

void modifyMemory()
{
	int address;
	cout << "�������ڴ��ַ��";
	cin >> address;
	cout << "����ַΪ" << address << "���ڴ浥Ԫ�������޸�Ϊ��";
	short value;
	cin >> value;
	memory[address] = value;
	showMemory();
}

void insertDataIntoMemory()
{
	cout << "���ڵ�ַΪ" << memory.size() << "���ڴ浥Ԫ�������ݣ�";
	short value;
	cin >> value;
	memory.push_back(value);
	showMemory();
}

vector<Solution> enumAndTry(int len,int percent,Equation goal) //ö��&�Դ�
{
	long min = pow(enumResults.size(), len - 1);
	long max = pow(enumResults.size(), len) - 1;
	long total = max - min+1;
	long total_actual;
	if (percent == 100) total_actual = total;
	else total_actual= ((double)percent) / 100 * ((double)total);
	long max_actual = min + total_actual - 1;

	SHORTARRAY bak_mem = ::memory;
	vector<Register> bak_regs = ::getRegisterArray();
	cout << "��ʼ״̬:\n";
	::showAllRegisters(); ::showMemory();
	//string what; cin >> what;

	cout << "min=" << min <<"��";
	cout << "max_actual=" << max_actual <<"��";
	cout << "�ܹ�Ҫ����" << max_actual - min << "�ַ���\n";

	vector<Solution> solutions;
	for (long i = min; i <= max_actual; i++)
	{
		Solution currentSolution;
		executeLongNumber(i, enumResults.size(), len);

		//cout << "���ԣ�����MDR=" << MDR.value << endl;
		if (goal.judge()==true) //����Ҫ��
		{
			vector<short> bitArray = convertIntoBitArray(i, enumResults.size(), len);
			
			for (int j = 0; j < bitArray.size(); j++)
			{
				//cout << "�±�" << bitArray[j] << "��Ӧ������";cout<<enumResults[bitArray[j]] << "  ";
				currentSolution.steps.push_back(enumResults[bitArray[j]]);
			}

			memory = bak_mem;
			setRegisters(bak_regs);
			solutions.push_back(currentSolution);
		}
		else
		{
			//�Ĵ����ͱ���ȫ���ָ�ԭֵ
			memory = bak_mem;
			setRegisters(bak_regs);
		}
		
	}
	return solutions;
}

vector<Solution> eliminate(vector<Solution> oldOnes,Equation goal)
{
	cout << "��ʼ��̭ԭ����...���ѣ���ʼ״̬������ʾ��\n";
	showMemory(); showAllRegisters();
	vector<Solution> intersection;
	for (int i = 0; i < oldOnes.size(); i++)
	{
		SHORTARRAY bak_memory = ::memory;
		vector<Register> bak_regs = ::getRegisterArray();

		oldOnes[i].execute();
		if (goal.judge() == true) intersection.push_back(oldOnes[i]);

		memory = bak_memory;
		setRegisters(bak_regs);
	}
	return intersection;
}

void IO_selfProgram()
{
	bool targetIsMemory = false;
	cout << "������Ŀ�����ڴ棨M�����ǼĴ�����R���� ";
	string inp_target;cin >> inp_target;
	if (inp_target == "M") targetIsMemory = true;

	Equation goal;

	if (targetIsMemory)
	{
		cout << "�������ڴ��ַ��Ŀ��ֵ���Կո�ָ���";
		int address; int value;
		cin >> address >> value;
		goal = *new Equation(address, value, LEFT_IS_MEMORY);
		regPtr.toNull();//����Ҫ�Ĵ���ָ��
		IR1.value = address; 
		//IR2.value = value;
	}
	else
	{
		cout << "������Ĵ�������Ŀ��ֵ���Կո�ָ���";
		string nameOfReg; int value;
		cin >> nameOfReg >> value;
		goal = *new Equation(::getIndexOfRegister(nameOfReg), value, LEFT_IS_REG);
		regPtr = *registers[::getIndexOfRegister(nameOfReg)];
		IR1.value = 0;
		//IR2.value = value;
	}

	int numOfSteps;
	cout << "�����ò�������"; cin >> numOfSteps;

	vector<Solution> solutions=::enumAndTry(numOfSteps, 100, goal);
	if (solutions.size() == 0) cout << "û���ҵ�����\n";
	else
	{
		while (solutions.size()>0)
		{
			cout << "����������Դ��ҵ�����Щ������\n";
			for (int i = 0; i < solutions.size(); i++)
			{
				cout << "���" << i << "��\n";
				cout << solutions[i].toString() << endl;
			}
			cout << "�Ƿ񱣴淽������Y/N��:";
			string YorN; cin >> YorN;
			if (YorN == "Y")
			{
				cout << "�����뷽����ţ�";
				int indexOfSolution; cin >> indexOfSolution;
				Solution chosenSolution = solutions[indexOfSolution];
				cout << "��Ϊ�÷�����������ȡ�����֣�";
				string fName; cin >> fName;
				Function f = *new Function(fName);
				f.sentences = chosenSolution.steps;
				f.targetIsMemory = targetIsMemory;
				userFunctions.push_back(f);
				::addEnum();
				return;
			}
			else
			{
				while (true)
				{
					cout << "Ϊ�˽�һ��ɸѡ���������������Ĵ�����ֵ���޸��ڴ������\n";
					cout << "�޸��ڴ�������1����������������2���޸ļĴ���������3��";
					cout << "�鿴�ڴ�������4���鿴�Ĵ���������5�����������6" << endl;
					int choice=-1;
					cin >> choice;
					if (choice == 1) modifyMemory();
					else if (choice == 2) insertDataIntoMemory();
					else if (choice == 3) modifyRegister();
					else if (choice == 4) showMemory();
					else if (choice == 5) showAllRegisters();
					else if (choice == 6) break;
				}

				//��ʼ�趨Ŀ��
				if (targetIsMemory)
				{
					cout << "�������ڴ��ַ��Ŀ��ֵ���Կո�ָ���";
					int address; int value;
					cin >> address >> value;
					goal = *new Equation(address, value, LEFT_IS_MEMORY);
					regPtr.toNull();//����Ҫ�Ĵ���ָ��
					IR1.value = address; 
					//IR2.value = value;
				}
				else
				{
					cout << "������Ĵ�������Ŀ��ֵ���Կո�ָ���";
					string nameOfReg; int value;
					cin >> nameOfReg >> value;
					goal = *new Equation(::getIndexOfRegister(nameOfReg), value, LEFT_IS_REG);
					regPtr = *registers[::getIndexOfRegister(nameOfReg)];
					//IR2.value = value;
				}

				//��Ͽ�ʼɸѡ
				solutions = ::eliminate(solutions, goal);
				//ת��ѭ���Ŀ�ͷ�������û��Լ�ѡ�񷽰�
				if (solutions.size() == 0) cout << "û�к��ʵķ���\n";
			}
		}
	}
}

void IO_interpreter()
{
	cout << "���ѽ��������ģʽ������EXIT���˳���\n";
	system("color 1e");
	string input;
	while (input != "EXIT")
	{
		cin >> input;
		if (input == "�޸��ڴ�") modifyMemory();
		else if (input == "�޸ļĴ���") modifyRegister();
		else if (input == "EXIT") {
			system("color 0f");
			cout << "�����˳�������ģʽ\n";
			return;
		}
		else
		{
			Sentence sentence = createSentence(input);
			sentence.execute();
			
			showMemory(); showAllRegisters();
		}
	}
}

void IO_main() //��Ҫ�����
{
	while (true)
	{
		string cmdInput;
		cin >> cmdInput;
		if (cmdInput == "�鿴�ڴ�") showMemory();
		else if (cmdInput == "�鿴�Ĵ���") showAllRegisters();
		else if (cmdInput == "�޸��ڴ�") modifyMemory();
		else if (cmdInput == "��������") insertDataIntoMemory();
		else if (cmdInput == "�޸ļĴ���") modifyRegister();
		else if (cmdInput == "������ģʽ") IO_interpreter();
		else if (cmdInput == "�Ա��") IO_selfProgram();
		else if (cmdInput == "�˳�") break;
	}
}

int main()
{
	//functions.push_back(*new Function());
	memory.push_back(133);//[0]
	memory.push_back(2041);//[1]
	memory.push_back(2007);//[2]
	memory.push_back(1900);//[3]
	memory.push_back(3);//[4]
	enumResults.push_back(0);
	enumResults.push_back(100);
	enumResults.push_back(200);
	enumResults.push_back(300);
	::addEnum();
	
	IO_main();
	
	//int size = enumResults.size();
	/*
	int min = enumResults.size();
	int max = pow(enumResults.size(), 3) - 1;
	cout << "��" << min << "��" << max << endl;
	for (int i = min; i <= max; i++)
	{
		SHORTARRAY bitArray = convertIntoBitArray(i, enumResults.size(),3);
		for (int j = 0; j < bitArray.size(); j++)
			cout << bitArray[j] << "  ";
		cout << endl;
		cout << createSolutionByLongNumber(i, enumResults.size(),3).toString() << endl;
	}
	*/
	//IR2.value = 1;

	/*
	R1.value = 37; IR2.value = 1;
	regPtr = R1;

	vector<Solution> ���Ѱַ = ::enumAndTry(3,19 ,*new Equation(R1_CODE,2041,LEFT_IS_REG));

	regPtr = R2;
	���Ѱַ = ::eliminate(���Ѱַ, *new Equation(R2_CODE, 2041, LEFT_IS_REG));

	if (���Ѱַ.size() != 0)
	{
		cout << "�ܹ���" << ���Ѱַ.size() << "������\n";
		for (int x = 0; x < ���Ѱַ.size(); x++)
		{
			cout << x << "\n" + ���Ѱַ[x].toString() << endl;
		}
	}
	else
		cout << "û�к��ʷ���\n";

	Function f_���Ѱַ = *new Function("��ֵ_������");
	f_���Ѱַ.targetIsMemory = false;
	f_���Ѱַ.sentences = ���Ѱַ[0].steps;
	userFunctions.push_back(f_���Ѱַ);
	::addEnum();


	string anyStr; cin >> anyStr;

	R1.value = 0; regPtr = R2;
	IR2.value = 4;
	showMemory();
	showAllRegisters();
	vector<Solution> �������Ѱַ = ::enumAndTry(2, 100, 
		*new Equation(R2_CODE, 1900, LEFT_IS_REG));
	if (�������Ѱַ.size() == 0) cout << "û�п��з���\n";
	else
	{
		for (int i = 0; i < �������Ѱַ.size(); i++)
		{
			cout << i << ":\n";
			cout << �������Ѱַ[i].toString() << endl;
		}
	}

	cin >> anyStr;
	/*
	Sentence ���Ѱַ��� = createSentence("��ֵ_������|R1|2");
	���Ѱַ���.execute();
	showAllRegisters(); showMemory();
	*/
	/*
	R1.value = 0; regPtr = R1;
	IR2.value = 4;
	showMemory();
	showAllRegisters();

	Solution ����ָ��Ѱַ = *new Solution();
	����ָ��Ѱַ.steps.push_back(500 + MAR_CODE * 10 + IR2_CODE);
	����ָ��Ѱַ.steps.push_back(500 + REGPTR_CODE * 10 + MAR_CODE);
	
	Function f_����ָ�� = *new Function("����ָ��Ѱַ");
	f_����ָ��.targetIsMemory = false;
	f_����ָ��.sentences = ����ָ��Ѱַ.steps;
	f_����ָ��.execute(&R2, &IR2);

	

	cout << "ִ������Ժ�\n";
	showAllRegisters();

	//bool ��һ�仰�Ƿ�ɹ�=executeCode(����ָ��Ѱַ.steps[0]);
	//cout << "��һ�仰�Ƿ�ɹ�=" << ��һ�仰�Ƿ�ɹ� << endl;
	//showAllRegisters();
	//IR2.value = MAR.value;
	//userFunctions[0].execute(registers[REGPTR_CODE], &MAR);
	//bool �ڶ��仰�Ƿ�ɹ� = executeCode(����ָ��Ѱַ.steps[1]);
	//cout << "�ڶ��仰�Ƿ�ɹ�=" << �ڶ��仰�Ƿ�ɹ� << endl;
	//showAllRegisters();

	//����ָ��Ѱַ.execute();
	//cout << "ִ����\n" + ����ָ��Ѱַ.toString() << endl;
	//showMemory(); showAllRegisters();
	/*
	vector<Solution> ����ָ��Ѱַ = ::enumAndTry(2, 100, 
		*new Equation(R1_CODE, 1900, LEFT_IS_REG));
	for (int i = 0; i < ����ָ��Ѱַ.size(); i++)
	{
		cout << "����" << i << endl;
		cout << ����ָ��Ѱַ[i].toString() << endl;
	}
	*/

	/*
	cin >> anyStr;

	R1.value = 34;
	if (registers[REGPTR_CODE]->isNull())
		cout << "��һ�仰����֮ǰ����Ĵ����ǿ�ֵ\n";
	
	Sentence testSentence=createSentence("MOV|R1|23");
	cout <<
		testSentence.toString() +
		"  " + testSentence.toString_value() + " "
		+ IR1.toString() + " " + IR2.toString() + " " + R1.toString()
		<< "��REGPTR=" + regPtr.toString() << endl;

	testSentence.target = REGPTR_CODE;
	if (NULL==registers[testSentence.target])
		cout << "����֮ǰ����Ĵ����ǿ�ֵ\n";
	cout <<
		testSentence.toString() +
		"  " + testSentence.toString_value() +" "
		+IR1.toString()+" "+IR2.toString()+" "+R1.toString()
		<<"��REGPTR=" + regPtr.toString() << endl;

	string any; cin >> any;

	testSentence.execute();
	cout << "ִ������\n";
	cout <<
		testSentence.toString() +
		"  " + testSentence.toString_value() + " "
		+ IR1.toString() + " " + IR2.toString() + " " + R1.toString()
		<< "��REGPTR=" + regPtr.toString() << endl;

	cin >> any;

	cout << "������Ҫ�ı�regPtr1������" << endl;
	regPtr.name = "MDR";
	cout << "������Ҫˢ���ˣ�\n";
	refresh_passive();//����
	cout <<
		testSentence.toString() +
		"  " + testSentence.toString_value() +
		"��IR1=" << IR1.value << "��IR2=" << IR2.value <<
		"��REGPTR=" + regPtr.toString() +"��"
		<<registers[getIndexOfRegister(regPtr.name)]->toString()
		<< endl;
	cin >> any;
	testSentence.execute();
	cout << "�޸ĺ����䣬ִ��һ��:\n";
	cout <<
		testSentence.toString() +
		"  " + testSentence.toString_value() +
		"��IR1=" << IR1.value << "��IR2=" << IR2.value <<
		"��REGPTR=" + regPtr.toString() + "��"
		<< registers[getIndexOfRegister(regPtr.name)]->toString()
		<< endl;
	
	cout << "�����������������...";
	cin >> any;

	clear();
	showAllRegisters();
	while (true)
	{
		string input;
		cin >> input;
		if (input == "EXIT") break;
		Sentence userSentence = createSentence(input);
		cout << "���ղ������ˣ�" + userSentence.toString() + "  " + userSentence.toString_value() + "\n";
		userSentence.execute();
		showAllRegisters();
		clear();
	}
	*/
	return 0;
}

//��ֵ_���ַ��5,R1
//IR1=5��IR2=34
//IR1��MAR  5
//IR2��MDR  34
//WM

//��ֵ_�ҵ�ַ��R1���ɻ�������Ĵ�����,7
//IR1=0,IR2=7  REGPTR=R1
//IR2��MAR
//WM
//MDR��REGPTR ��ָ�������޸�ԭ�Ĵ�����ֵ��