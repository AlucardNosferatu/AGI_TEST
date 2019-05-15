// RegisterTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include<string>
#include<math.h>
#include<vector>
#include"strsplit.h"
using namespace std;

#define USHORT unsigned short

unsigned char binary2Byte(string bin) //�������ַ���תΪ���������ֽڣ�
{
	unsigned char result = 0;
	for (int i = 0; i < bin.size(); i++)
	{
		if(bin[i]=='1') result += pow(2, bin.size() - 1 - i);
	}
	return result;
}

string byte2Binary(unsigned char byte) //�����ֽڣ�����תΪ�������ַ���
{
	string result = "";
	unsigned char q = byte;
	unsigned char r = 0;
	while (q > 0)
	{
		r = q % 2;
		q = q / 2;
		if (r == 1) result += "1";
		else result += "0";
	}
	string reversed;
	reversed.assign(result.rbegin(), result.rend());
	return reversed;
}

unsigned short binary2Short(string bin) //�������ַ���תΪ������˫�ֽڣ�
{
	unsigned short result = 0;
	for (int i = 0; i < bin.size(); i++)
	{
		if (bin[i] == '1') result += pow(2, bin.size() - 1 - i);
	}
	cout << "�ַ���" << bin << "��תΪshort" << result << endl;;
	return result;
}

string short2Binary(unsigned short int16) //��˫�ֽڣ�����תΪ�������ַ���
{
	string result = "";
	unsigned short q = int16;
	unsigned short r = 0;
	while (q > 0)
	{
		r = q % 2;
		q = q / 2;
		if (r == 1) result += "1";
		else result += "0";
	}
	string reversed;
	reversed.assign(result.rbegin(), result.rend());
	return reversed;
}

string fill(string original, int len) //���Ȳ�������λ��0
{
	string result = "";
	for (int i = 1; i <= len - original.size(); i++)
		result += "0";
	result += original;
	return result;
}

class Register //��λ�Ĵ�����������λ������λ��
{
private:
	unsigned char value=0;
public:
	string name;
	int maxLen;
	Register(){}
	Register(string name,int maxLen)
	{
		this->name = name;
		this->maxLen = maxLen;
	}
	string getString()
	{
		return fill(byte2Binary(this->value), maxLen);
	}
	unsigned char getValue() { return this->value; }
	void setValue(unsigned char value) 
	{ 
		unsigned char temp = 255 >> (8-maxLen);
		this->value = (value&temp); 
	}
	int compareToZero()
	{
		unsigned char value = this->getValue();
		if (value == 0) return 0;
		unsigned char flag = value >> (getMaxLen() - 1);
		if (flag == 1) return -1;
		else return 1;
	}
	int getMaxLen() { return this->maxLen; }
};

class Register16 //ʮ��λ�Ĵ�������������λ�Ĵ���ƴ�Ӷ���
{
public:
	string name;
	Register* high; //��λ
	Register* low; //��λ
	Register16() {}
	Register16(string name, Register* high, Register* low)
	{
		this->name = name;
		this->high = high;
		this->low = low;
	}
	unsigned short getValue()
	{
		unsigned short result=0;
		result += ( high->getValue() << (low->maxLen) );
		result += low->getValue();
		return result;
	}
	void setValue(unsigned short value)
	{
		unsigned short highValue = value >> (low->maxLen);
		unsigned short lowValue = value - (highValue << (low->maxLen));
		//cout << "��λ" << highValue << "����λ" << lowValue << endl;
		this->high->setValue( (unsigned char)highValue );
		this->low->setValue( (unsigned char)lowValue );
	}
	int compareToZero()
	{
		unsigned short value = this->getValue();
		if (value == 0) return 0;
		unsigned short flag = value >> (getMaxLen() - 1);
		if (flag == 1) return -1;
		else return 1;
	}
	int getMaxLen() { return this->high->maxLen + this->low->maxLen; }
};

#define REG8 0
#define REG16 1
#define INTEGER 2
#define IMM 3

class Object
{
private:
	unsigned short value=0;//һ�����ڴ��������
public:
	void* ptr=0;//ָ��������������ȫ�ֱ�����ָ��
	string name;//����
	char type=-1;//����
	Object() {}
	Object(string name,unsigned short value) //�����������
	{
		this->type = IMM;
		this->name = name;
		this->value = value;
	}
	Object(string name, char type, void* ptr)
	{
		this->type = type;
		this->name = name;
		this->ptr = ptr;
	}
	unsigned short getValue()
	{
		if (type==IMM)
			return value;
		else if (type == REG8)
			return ((Register*)ptr)->getValue();
		else if (type == REG16)
			return ((Register16*)ptr)->getValue();
		else if (type == INTEGER)
			return *(USHORT*)ptr;
	}
	void setValue(unsigned short value)
	{
		if (type == IMM) this->value = value;
		else if (type == REG8)
			((Register*)ptr)->setValue(value);
		else if (type == REG16)
			((Register16*)ptr)->setValue(value);
		else if (type == INTEGER)
			*(USHORT*)ptr = value;
	}
	bool equals(Object other)
	{
		if (this->type != other.type) return false;
		else if (type == IMM) return this->value == other.value;
		else return this->ptr == other.ptr;
	}
private:
	string toString()
	{
		if (type == INTEGER)
		{
			char* charArray = "";
			sprintf(charArray, "%d", value);
			string result = charArray;
			return result;
		}
		else
			return name;
	}
};

Object C("������",1);

template<typename T>
string getName(T* obj)
{
	return ((T*)obj)->name;
}

bool expand(Register* reg) //����һλ
{
	reg->maxLen++;
	return true;
}

template<typename T>
bool left(T* reg)
{
	reg->setValue( reg->getValue() << 1 );
	return true;
}

template<typename T>
bool right(T* reg)
{
	reg->setValue(reg->getValue() >> 1);
	return true;
}

template<typename T>
bool write1(T* reg) //��1
{
	reg->setValue( reg->getValue()+ 1);
	return true;
}

template<typename T>
bool add(T* target, Object value)
{
	((T*)target)->setValue(((T*)target)->getValue() + value.getValue());
	return true;
}

template<typename T>
bool positive(T arg)
{
	return ((T)arg)->compareToZero() == 1;
}

template<typename T>
bool negative(T arg)
{
	return ((T)arg)->compareToZero() == -1;
}

template<typename T>
bool notNegative(T arg)
{
	return !negative(arg);
}

bool firstTime()
{
	return C.getValue() == 1;
}

bool notFirstTime()
{
	return C.getValue() > 1;
}

bool endWith0(Object arg)
{
	return arg.getValue() % 2 == 0;
}

bool endWith1(Object arg)
{
	return arg.getValue() % 2 == 1;
}

bool uni_negative(vector<Object> args)
{
	Object arg = args[0];
	if (arg.type == REG8)
		return negative( (Register*)(arg.ptr) );
	else if(arg.type==REG16)
		return negative((Register16*)(arg.ptr));
}

bool uni_positive(vector<Object> args)
{
	Object arg = args[0];
	if (arg.type == REG8)
		return positive((Register*)(arg.ptr));
	else if (arg.type == REG16)
		return positive((Register16*)(arg.ptr));
}

bool uni_notNegative(vector<Object> args)
{
	return !uni_negative(args);
}

bool uni_firstTime(vector<Object> args)
{
	return firstTime();
}

bool uni_notFirstTime(vector<Object> args)
{
	return !firstTime();
}

bool uni_expand(vector<Object> args)
{
	return expand((Register*)(args[0].ptr));
}

bool uni_add(vector<Object> args)
{
	Object target = args[0];
	Object value = args[1];
	if (target.type == REG8)
		return add((Register*)target.ptr, value);
	else if (target.type == REG16)
		return add((Register16*)target.ptr, value);
}

bool uni_assign(vector<Object> args)
{
	args[0].setValue(args[1].getValue());
	return true;
}

bool uni_left(vector<Object> args)
{
	if (args[0].type == REG8) ::left((Register*)(args[0].ptr));
	else if (args[0].type == REG16) ::left((Register16*)(args[0].ptr));
	else return false;

	return true;
}

bool uni_right(vector<Object> args)
{
	if(args[0].type==REG8) ::right( (Register*)(args[0].ptr) );
	else if(args[0].type==REG16) ::right((Register16*)(args[0].ptr));
	else return false;

	return true;
}

bool uni_write1(vector<Object> args)
{
	if (args[0].type == REG8) ::write1((Register*)(args[0].ptr));
	else if (args[0].type == REG16) ::write1((Register16*)(args[0].ptr));
	else return false;

	return true;
}

bool uni_endWith0(vector<Object> args)
{
	return endWith0(args[0]);
}

bool uni_endWith1(vector<Object> args)
{
	return endWith1(args[0]);
}

bool uni_xor(vector<Object> args)
{
	bool signalA =( args[0].getValue() ==1 );
	bool signalB =( args[1].getValue() ==1 );

	unsigned short value = ((signalA^signalB) ? 1:0);
	args[0].setValue(value);
	return true;
}

typedef bool(*FunctionPointer)(vector<Object>);//����ָ�� Function Pointer

#define PREDICATE 0
#define VERB 1
class Method
{
public:
	FunctionPointer ptr;
	string name;
	char type = -1;
	Method(FunctionPointer ptr, string name,char type)
	{
		this->ptr = ptr;
		this->name = name;
		this->type = type;
	}
	bool execute(vector<Object> args)
	{
		return (*ptr)(args);
	}
};

class Sentence
{
public:
	Method* verbOrPredicate;//���ʡ�ν��
	vector<Object> nouns;//�����е�����
	Sentence(){}
	Sentence(Method* v, vector<Object> nouns)
	{
		this->verbOrPredicate = v;
		this->nouns = nouns;
	}
	bool execute()
	{
		return verbOrPredicate->execute(nouns);
	}
	string toString()
	{
		string result = verbOrPredicate->name;
		if (nouns.size() > 0)
		{
			result += "|";
			for (int i = 0; i < nouns.size(); i++)
				result += nouns[i].name + (
				  (i==nouns.size()-1)?"":"|" );
		}
		return result;
	}
	bool judge() { return execute(); }
	bool equals(Sentence other)
	{
		if (this->verbOrPredicate != other.verbOrPredicate)
			return false;
		if (this->nouns.size() != other.nouns.size())
			return false;
		for (int i = 0; i < nouns.size(); i++)
			if (this->nouns[i].equals(other.nouns[i])==false)
				return false;
		return true;
	}
};

bool allTrue(vector<Sentence*> conditions)
{
	for (int i = 0; i < conditions.size(); i++)
		if (conditions[i]->execute() == false)
			return false;
	return true;
}

class IfThen
{
public:
	vector<Sentence*> conditions;//��Ҫ���������
	Sentence* command;
	IfThen(){}
	IfThen(Sentence* command)
	{
		this->command = command;
	}
	IfThen(Sentence* command, vector<Sentence*> conditions)
	{
		this->command = command;
		this->conditions = conditions;
	}
	string toString()
	{
		string str = "�����";
		for (int i = 0; i < conditions.size(); i++)
		{
			str += conditions[i]->toString();
			if (i != conditions.size() - 1)
				str += "&&";
		}
		str += "�� ��  " + command->toString();
		return str;
	}
	bool execute()
	{
		for (int i = 0; i < conditions.size(); i++)
		{
			if (conditions[i]->execute() == false)
				return false;
		}
		return command->execute();
	}
};

vector<Sentence*> intersect(vector<Sentence*> a, vector<Sentence*> b)
{
	vector<Sentence*> result;
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < b.size(); j++)
		{
			if (b[j] == a[i])
			{
				result.push_back(a[i]);
				break;//�˳�����
			}
		}
	}
	return result;
}

/*�ڴ˶���ȫ�ֶ�������*/
vector<Object*> allObjs;
Object* getObjectByName(string name)
{
	for (int i = 0; i < allObjs.size(); i++)
		if (allObjs[i]->name == name)
			return allObjs[i];
	return NULL;
}

/*
class Pointer
{
public:
	string name;
	Pointer(string name) { this->name = name; }
	Object* get()
	{
		return getObjectByName(name);
	}
};
*/

USHORT userArgValues[10]; //�û�������ȡֵ
int size_userArgs=0;
Object userArgs[10];
USHORT userTempValues[10];//�û���ʱ������ȡֵ
int size_userTemps=0;
Object userTemps[10];
USHORT answerValues[10];//�𰸵�ֵ
int size_answers = 0;
Object answers[10];

vector<USHORT> getBackupOfArgValues()
{
	vector<USHORT> results;
	for (int i = 0; i < size_userArgs; i++)
		results.push_back(userArgValues[i]);
	return results;
}

void restore(vector<USHORT> argValues)
{
	for (int i = 0; i < allObjs.size(); i++)
		allObjs[i]->setValue(0);
	for (int i = 0; i < argValues.size(); i++)
		userArgValues[i] = argValues[i];
}

void insert_userArg(string name, USHORT value)
{
	userArgValues[size_userArgs] = value;
	userArgs[size_userArgs] = *new Object(name, INTEGER, &userArgValues[size_userArgs]);
	allObjs.push_back(&userArgs[size_userArgs]);

	size_userArgs++;
}

void insert_userTemp(string name, USHORT value)
{
	userTempValues[size_userTemps] = value;
	userTemps[size_userTemps] = *new Object(name, INTEGER, &userTempValues[size_userTemps]);
	allObjs.push_back(&userTemps[size_userTemps]);

	size_userTemps++;
}

void insert_answer(string name, USHORT value)
{
	answerValues[size_answers] = value;
	answers[size_answers] = *new Object(name, INTEGER, &answerValues[size_answers]);
	allObjs.push_back(&answers[size_answers]);

	size_answers++;
}

Sentence middle_commands[10];//�м�����
int size_middleCommands = 0;
IfThen ifThens[10];

string getNameByPointer_shortOnly(USHORT* p)
{
	if (p >= userArgValues&&p <= userArgValues + size_userArgs - 1)
		return userArgs[p - userArgValues].name;
	if (p >= userTempValues&&p <= userTempValues + size_userTemps - 1)
		return userTemps[p - userTempValues].name;
	if (p >= answerValues&&p <= answerValues + size_answers - 1)
		return answers[p - answerValues].name;
}

//vector<Pointer> args;
//vector<Pointer> temps;
//vector<Pointer> answers;
vector<Method> methods;
vector<Sentence> pre_commands, end_commands;
vector<Sentence> propositions;//д��һ���Ժ󣬾Ͳ��ٱ䶯��
//vector<IfThen> ifThens;

Method* getMethodByName(string name)
{
	for (int i = 0; i < methods.size(); i++)
		if (methods[i].name == name)
			return &(methods[i]);
	return NULL;
}

/*
unsigned short getValueByName(string name)
{
	return getObjectByName(name)->getValue();
}
*/

void createNewArgs(vector<string> names)
{
	for (int i = 0; i < names.size(); i++)
		::insert_userArg(names[i], 0);
	
}

void createNewTemps(vector<string> names)
{
	for (int i = 0; i < names.size(); i++)
		::insert_userTemp(names[i], 0);
}

void createNewAnswers(vector<string> names)
{
	for (int i = 0; i < names.size(); i++)
		::insert_answer(names[i], 0);
}

Sentence createSentence(string str)
{
	vector<string> str_arr = split(str);
	string methodName = str_arr[0];
	Method* method = getMethodByName(methodName);
	vector<Object> args_arr;
	if (str_arr.size() > 1)
	{
		for (int i = 1; i < str_arr.size(); i++)
			args_arr.push_back(*getObjectByName(str_arr[i]));
	}
	Sentence sentence = *new Sentence(method, args_arr);
	return sentence;
}

void init_createPropositions() //��ʼ����ö������״̬����
{
	for (int i = 0; i < methods.size(); i++)
	{
		if (methods[i].type == VERB) continue;
		if (methods[i].name == "�״�����" || methods[i].name == "�����״�����")
		{
			propositions.push_back(*new Sentence(&methods[i], *new vector<Object>));
			continue;
		}

		for (int j = 0; j < allObjs.size(); j++)
		{
			bool isArg = false;
			for (int k = 0; k < size_userArgs; k++)
				if (userArgs[k].name == allObjs[j]->name) { isArg = true; break; }
			bool isAnswer = false;
			for (int k = 0; k < size_answers; k++)
				if (answers[k].name == allObjs[j]->name) { isAnswer = true; break; }

			if (isArg||isAnswer) continue;
			vector<Object> nouns;
			nouns.push_back(*allObjs[j]);
			propositions.push_back(*new Sentence(&(methods[i]), nouns));
			//cout << propositions[propositions.size()-1].toString()<< endl;
		}
	}
}

vector<Sentence*> selectTrueStatements()
{
	vector<Sentence*> results;
	for (int i = 0; i < propositions.size(); i++)
		if(propositions[i].execute()==true)
			results.push_back(&(propositions[i]));
	return results;
}

IfThen* findIfThenByCommand(Sentence command)
{
	//cout << "���ԣ�����ͨ��Command����IfThen...�ܳ���Ϊ" << size_middleCommands<<endl;
	for (int i = 0; i < size_middleCommands; i++)
		if (ifThens[i].command->equals(command))
			return &ifThens[i];
	return NULL;
}

IfThen* findIfThenByCommand(Sentence* command)
{
	if (command >= middle_commands && command <= middle_commands + size_middleCommands - 1)
		return &ifThens[command - middle_commands];
	else
		return NULL;

}

bool insertNewCommand(Sentence command)//�ظ�����false������ɹ�����true
{
	//cout << "���ԣ�size_middleCommands=" << size_middleCommands << endl;
	for (int i = 0; i < size_middleCommands; i++)
	{
		if (command.equals(middle_commands[i]))
			return false;//�ظ�����false
	}
	middle_commands[size_middleCommands]=command;
	ifThens[size_middleCommands] = *new IfThen(
		&middle_commands[size_middleCommands]);

	size_middleCommands++;
	return true;
}

bool createConnections(Sentence command)
{
	bool isNew = insertNewCommand(command);
	//cout << "���ԣ���仰���µ���" << isNew << endl;
	IfThen* ifThen = NULL;
	if (isNew) ifThen = &ifThens[size_middleCommands - 1];
	else ifThen = findIfThenByCommand(command);
	//cout << "���ԣ�findIfThenByCommand(command)=" << ifThen << endl;
	
	if (isNew) ifThen->conditions = selectTrueStatements();
	else ifThen->conditions = intersect(ifThen->conditions, selectTrueStatements());

	return isNew;
}

Sentence* executeSuitableCommand()
{
	for (int i = 0; i < size_middleCommands; i++)
	{
		if (allTrue(ifThens[i].conditions))
		{
			ifThens[i].command->execute();
			return ifThens[i].command;
		}
	}
	return NULL;
}

int numOfBit;//λ��
Register A, Q;
Register16 AQ;

void clear()
{
	C.setValue(1);//ѭ������������Ϊ1
	A.setValue(0);//�Ĵ�������
	Q.setValue(0);//ͬ��

	for (int i = 0; i < allObjs.size(); i++)
		allObjs[i]->setValue(0);
}

void executeAll()
{
	for (int i = 0; i < pre_commands.size(); i++)
	{
		cout << pre_commands[i].toString() << endl;
		pre_commands[i].execute();
	}

	for (int i = 0; i < size_middleCommands; i++)
	{
		Sentence* command = executeSuitableCommand();
		if (command != NULL)
			cout << command->toString() << endl;
	}

	for (int i = 0; i < end_commands.size(); i++)
	{
		cout << end_commands[i].toString() << endl;
		end_commands[i].execute();
	}
}

bool check(vector<Object*> returnings, vector<unsigned short> values)
{
	for (int i = 0; i < returnings.size(); i++)
		if (values[i] != returnings[i]->getValue())
			return false;
	return true;
}

void trySolution(vector<Sentence*> solution)
{
	for (int i = 0; i < pre_commands.size(); i++)
		pre_commands[i].execute();

	for (int i = 0; i < solution.size(); i++)
		solution[i]->execute();

	for (int i = 0; i < end_commands.size(); i++)
		end_commands[i].execute();
}

void IO_learn_pre()
{
	cout << "������Ԥ��Ҫִ�е�ָ���ʼ��ָ�����NEXT������\n";
	while (true)
	{
		string input;
		cin >> input;
		if (input == "NEXT") break;
		Sentence command = createSentence(input);
		::pre_commands.push_back(command);
	}
}

void IO_learn_end()
{
	cout << "��������ĩβ��ִ�е�ָ���ʼ��ָ�����NEXT������\n";
	while (true)
	{
		string input;
		cin >> input;
		if (input == "NEXT") break;
		Sentence command = createSentence(input);
		::end_commands.push_back(command);
	}
}

template<typename T>
void swap(vector<T>* array, int a, int b)
{
	T temp = array->at(a);
	array->at(a) = array->at(b);
	array->at(b) = temp;
}

template<typename T>
void perm(vector<vector<T>>* solutions, vector<T>* list, int k, int m)//����
{
	int i;
	if (k>m)
	{
		vector<T> solution;
		for (i = 0; i <= m; i++)
			solution.push_back(list->at(i));
		solutions->push_back(solution);
		m++;
	}
	else
	{
		for (i = k; i <= m; i++)
		{
			swap(list, k, i);//������k��Ԫ�غ͵�i��Ԫ��
			perm(solutions, list, k + 1, m);
			swap(list, k, i);
		}
	}
}

template<typename T>
void perm(vector<vector<T>>* solutions, vector<T>* list)//ȫ����
{
	perm(solutions, list, 0, list->size() - 1);
}

vector<vector<IfThen*>> getAllArrangements()
{
	vector<vector<IfThen*>> arrangements;
	vector<IfThen*> pointerArray;
	for (int i = 0; i < size_middleCommands; i++)
		pointerArray.push_back(&ifThens[i]);
	perm(&arrangements, &pointerArray);
	return arrangements;
}

#define OBJ_A objs[0]
#define OBJ_Q objs[1]
#define OBJ_AQ objs[2]

void showAllValues()
{
	string result = "";
	for (int i = 0; i < allObjs.size(); i++)
	{
		result += allObjs[i]->name + "=";
		if (allObjs[i]->type == INTEGER) //allObjs���ǲ���������������
			result += ::short2Binary(allObjs[i]->getValue());
		else
		{
			int maxLen;
			if (allObjs[i]->type == REG8)
				maxLen=((Register*)(allObjs[i]->ptr))->maxLen;
			else
				maxLen = ((Register16*)(allObjs[i]->ptr))->getMaxLen();
			result += ::fill(short2Binary(allObjs[i]->getValue()), maxLen);
		}
		result += "    ";
	}
	cout << result <<"  C="<<C.getValue()<< endl;
}

void showAllValues2()
{
	//string result = "";
	for (int i = 0; i < allObjs.size(); i++)
	{
		cout << allObjs[i]->name<<"="<<allObjs[i]->getValue() << "  ";
	}
	cout << endl;
}

void IO_setValueForAllArgs()
{
	cout << "���������\n";
	for (int i = 0; i < size_userArgs; i++)
	{
		cout << userArgs[i].name << "=";
		string inputBinary;
		cin >> inputBinary;
		userArgs[i].setValue(binary2Short(inputBinary));
	}
}

vector<USHORT> IO_setGoals()//����Ŀ��
{
	vector<USHORT> goals;
	for (int i = 0; i < size_answers; i++)
	{
		cout << answers[i].name << "=";
		USHORT goalValue;
		cin >> goalValue;
		goals.push_back(goalValue);
	}
	return goals;
}

bool checkIfReachTheGoals(vector<USHORT> goals)
{
	for (int i = 0; i < size_answers; i++)
		if (goals[i] != answers[i].getValue())
			return false;
	return true;
}

class Solution
{
public:
	vector<short> steps;
	bool execute(bool showDetails)
	{
		for (int i = 0; i < steps.size(); i++)
		{
			short index = steps[i];
			bool succeed = ifThens[index].execute();
			if (showDetails&&succeed)
				cout << ifThens[index].command->toString() << endl;
		}
		return true;
	}
	bool execute() { return execute(false); }
	string toString()
	{
		string result = "";
		for (int i = 0; i < steps.size(); i++)
		{
			short index = steps[i];
			result += ifThens[index].toString() + "\n";
		}
		return result;
	}
    Solution(){}
	Solution(vector<short> steps) { this->steps = steps; }
	bool equals(Solution other)
	{
		if (this->steps.size() != other.steps.size()) return false;
		for (int i = 0; i < steps.size(); i++)
		{
			if (this->steps[i] != other.steps[i])
				return false;
		}
		return true;
	}
};

vector<Solution> intersect_solution(vector<Solution> a, vector<Solution> b)
{
	if (a.size() == 0 || b.size() == 0)
		return *new vector<Solution>();

	vector<Solution> intersection;
	for (int i = 0; i < a.size(); i++)
	{
		bool found = false;
		for (int j = 0; j < b.size(); j++)
		{
			if (a[i].equals(b[i]))
			{
				found = true;
				break;
			}
		}
		if (found) intersection.push_back(a[i]);
	}
	return intersection;
}

vector<Solution> solutions;

vector<short> convertIntoBitArray(int originalNumber, int radix, int size)
{
	//��longNumberת��Ϊ����Ϊsize��radix�����������Ȳ������λ����
	vector<short> result;
	int q = originalNumber;
	int r = 0;
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

bool hasSameDigit(vector<short> arr)
{
	for (int i = 0; i < arr.size()-1; i++)
	{
		for (int j = i + 1; j < arr.size(); j++)
			if (arr[i] == arr[j])//����2λ��ͬ
				return true;
	}
	return false;
}

void enumerate()
{
	int min = 0;
	int max = pow(size_middleCommands, size_middleCommands);
	for (int i = min; i < max; i++)
	{
		vector<short> bitArr = ::convertIntoBitArray(i, size_middleCommands, size_middleCommands);
		if (hasSameDigit(bitArr) == false)
			solutions.push_back(*new Solution(bitArr));
	}
}

bool executeComplete(int index,bool showDetails) //������ִ��һ��
{
	for (int i = 0; i < ::pre_commands.size(); i++)
		pre_commands[i].execute();

	Solution solution = solutions[index];
	for (int i = 1; i <= Q.maxLen; i++)
	{
		C.setValue(i);
		if (showDetails == true) {
			showAllValues();
		}
		bool succeed_middle = solution.execute(showDetails);
		
	}

	for (int i = 0; i < ::end_commands.size(); i++)
		end_commands[i].execute();
	if (showDetails) showAllValues();

	return true;
}

bool executeComplete(bool showDetails) { return executeComplete(0,showDetails); }

void resort(Solution *sol,vector<int> indexes)
{
	vector<short> bak = sol->steps;
	for (int i = 0; i < sol->steps.size(); i++)
		sol->steps[i] = bak[indexes[i]];
}

void IO_resort(Solution *sol)
{
	vector<int> indexes;
	for (int i = 0; i < sol->steps.size(); i++)
	{
		int index;
		cin >> index;
		indexes.push_back(index);
	}
	resort(sol, indexes);
}

int main()
{
	methods.push_back(*new Method(&uni_expand, "��չ1λ", VERB));
	methods.push_back(*new Method(&uni_add, "��",VERB));
	methods.push_back(*new Method(&uni_assign, "��ֵ", VERB));
	methods.push_back(*new Method(&uni_write1, "��1", VERB));
	methods.push_back(*new Method(&uni_left, "����", VERB));
	methods.push_back(*new Method(&uni_right, "����", VERB));
	methods.push_back(*new Method(&uni_assign, "��ֵ", VERB));
	methods.push_back(*new Method(&uni_firstTime, "�״�����",PREDICATE));
	methods.push_back(*new Method(&uni_notFirstTime, "�����״�����", PREDICATE));
	methods.push_back(*new Method(&uni_negative, "����", PREDICATE));
	methods.push_back(*new Method(&uni_positive, "����", PREDICATE));
	methods.push_back(*new Method(&uni_notNegative, "�Ǹ�", PREDICATE));
	methods.push_back(*new Method(&uni_endWith0, "��0��β", PREDICATE));
	methods.push_back(*new Method(&uni_endWith1, "��1��β", PREDICATE));

	
	// A��Q�Ĵ���Ĭ�ϳ���Ϊ5
	A = *new Register("A", 5);
	Q = *new Register("Q", 5);
	AQ = *new Register16("AQ", &A, &Q);
	Object A_obj = *new Object("A", REG8, &A);
	Object Q_obj = *new Object("Q", REG8, &Q);
	Object AQ_obj = *new Object("AQ", REG16, &AQ);

	allObjs.push_back(&A_obj);
	allObjs.push_back(&Q_obj);
	allObjs.push_back(&AQ_obj);

	cout << "���������������NEXT����\n";
	vector<string> namesOfArgs;
	while (true)
	{
		string name;
		cin >> name;
		if ("NEXT" == name) break;
		namesOfArgs.push_back(name);
	}
	createNewArgs(namesOfArgs);

	cout << "��������ʱ����������NEXT����\n";
	vector<string> namesOfTempVars;
	while (true)
	{
		string name;
		cin >> name;
		if ("NEXT" == name) break;
		namesOfTempVars.push_back(name);
	}
	createNewTemps(namesOfTempVars);

	cout << "������𰸣�����ֵ������������NEXT����\n";
	vector<string> namesOfAnswerVars;
	while (true)
	{
		string name;
		cin >> name;
		if ("NEXT" == name) break;
		namesOfAnswerVars.push_back(name);
	}
	createNewAnswers(namesOfAnswerVars);

	/*����״̬����*/
	init_createPropositions();

	for (int s = 1;;s++)//���ѭ�� s��ʾsample
	{
		clear();//��������

		cout << "���濪ʼѧϰ����\n";
		int input_numOfBits;//AQ�Ĵ�����λ��
		cout << "������Q�Ĵ�����λ�������븺���ɽ�������ѧϰ���̣�";
		cin >> input_numOfBits;
		if (input_numOfBits < 0) break;
		Q.maxLen = input_numOfBits;
		A.maxLen = input_numOfBits + 1;

		IO_setValueForAllArgs();//�������ֵ

		showAllValues();

		string userInput;
		
		if (s == 1) IO_learn_pre();//ѧϰԤ��ָ��
		for (int i = 0; i < ::pre_commands.size(); i++)//ȫ��ִ��һ��
		{
			pre_commands[i].execute();
			showAllValues();
		}
		
		for (int c = 1;c<=input_numOfBits; c++)
		{
			cout << "��ǰѭ��������C=" << c;
			cout << ",������һϵ�������NEXT��β��\n";
			while (true) //��ѭ��������������ָ��
			{
				string inputSentence;
				cin >> inputSentence;
				if ("NEXT" == inputSentence) break;
				Sentence command = createSentence(inputSentence);
				::createConnections(command);
				command.execute();
				showAllValues();
				cout << findIfThenByCommand(command)->toString() << endl;
			}
			C.setValue(C.getValue() + 1);
		}

		if (s == 1) IO_learn_end();//ѧϰĩβָ��
		for (int i = 0; i < ::end_commands.size(); i++)//ȫ��ִ��һ��
		{
			end_commands[i].execute();
			showAllValues();
		}
	}

	vector<short> temp_steps;
	for (int i = 0; i < size_middleCommands; i++)
		temp_steps.push_back(i);
	Solution finalSolution = *new Solution(temp_steps);
	cout << finalSolution.toString();
	cout << "�������������ȷ���𣿣�Y/N��";
	string YOrN;cin >> YOrN;
	if (YOrN == "Y")
	{
		solutions.push_back(finalSolution);
		goto calc;
	}
	else
	{
		cout << "����������:";
		IO_resort(&finalSolution);
		solutions.push_back(finalSolution);
	}

	/*
	//�������ˡ�����
	enumerate();

	while (true) //���������Լ���
	{
		IO_setValueForAllArgs();
		vector<USHORT> goals = IO_setGoals();
		vector<Solution> succeeedOnes;//ͨ������ķ���

		vector<USHORT> bak = ::getBackupOfArgValues();
		for (int x = 0; x < solutions.size(); x++)
		{
			executeComplete(true);//��Ҫ�������

			if (checkIfReachTheGoals(goals))
			{
				succeeedOnes.push_back(solutions[x]);
				cout << "�˷�������Ҫ��\n";
			}

			cout << "_________________________\n";
			::restore(bak);
			system("pause");
		}
		solutions = intersect_solution(succeeedOnes, solutions);
		cout << "����ɸѡ�������·�������Ҫ��\n";
		for (int x = 0; x < solutions.size(); x++)
			cout << x << "��\n" + solutions[x].toString();
		cout << "�Ƿ����ѵ������Y/N��";
		string finishOrNot; cin >> finishOrNot;
		if (finishOrNot == "Y") break;
	}
	*/

calc:
	cout << "\n\n���г˷����������\n";
	while (true)
	{
		clear();//����
		cout << "������Q�Ĵ�����λ����";
		int maxLen_Q; cin >> maxLen_Q;
		Q.maxLen = maxLen_Q;
		A.maxLen = maxLen_Q + 1;
		IO_setValueForAllArgs();
		executeComplete(true);
		showAllValues();

		system("pause");
		system("cls");
	}

	return 0;
}
