// ģ��ѧϰ��֤����.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<vector>
#include<set>
#include<string>
#include<iostream>
#include<sstream>
#include<time.h>
using namespace std;

typedef bool(*funcp)();//ָ��bool������ָ�� 
char alphabet1 = ' ', alphabet2 = ' ';
bool LastExcuted = false;//�Ƿ�ִ���ˡ��Ⱥ��ұ߳��Զ�����һ����
int LastRemainder = 0;//�Ⱥ��ұ߳��Զ��󣬵õ�������
bool finished = false;

void init()
{
	alphabet1 =alphabet2= ' ';
	LastExcuted = false;
	LastRemainder = 0;
	finished = false;
}

string int2str(int i)
{
	stringstream ss;
	string str;
	ss << i;
	ss >> str;
	return str;
}

class item //������
{
public:
	int coef=1;//ϵ��
	bool var1=false;
	bool var2 = false;
	item(){}
	item(int coef)
	{
		this->coef = coef;
	}
	item(int coef, bool v1, bool v2)
	{
		this->coef = coef;
		var1 = v1; var2 = v2;
	}
	string tostring()
	{
		string result;
		if (var1 || var2)
		{
			if (coef > 1) result += int2str(coef);
		}
		else
		{
			if(coef!=0) result += int2str(coef);
		}

		if (var1) result += alphabet1;
		if (var2) result += alphabet2;
		return result;
	}
	void display()
	{
		cout << this->tostring();
	}
	item div2()
	{
		item result(coef / 2, var1, var2);
		return result;
	}
};
class expression //���ʽ
{
public:
	vector<item> items; //���ʽ�������������ÿһ��֮���üӺ�����
	expression() {}
	expression(item it) { this->items.push_back(it); }
	expression(item it1, item it2)
	{
		this->items.push_back(it1);
		this->items.push_back(it2);
	}
	bool available() //�Ƿ���Ч
	{
		return (items.size()>0);//һ��������������ı��ʽ����Ч��
	}
	void add(item it) //���������
	{
		this->items.push_back(it);
	}
	string tostring()
	{
		if (items.size() == 0) return "";
		else
		{
			string result;
			for (int i = 0; i < items.size(); i++)
			{
				result += items[i].tostring();//�����ǰ����������
				if (i != items.size() - 1)//�������һ��
				{
					if (items[i+1].coef || items[i+1].var1 || items[i+1].var2)//�����һ�Ϊ��
						result += "+";//���ڵ�ǰ�ź�������Ӻ�
				}
			}
			return result;
		}
	}
	void display()
	{
		cout << this->tostring();
	}
	int mod2()
	{
		int remainder = 0;//����
		if (items.size() == 0) return 0;
		for (int i = 0; i < items.size(); i++)
		{
			remainder += items[i].coef % 2;
		}
		return remainder;
	}
	expression div2()
	{
		if (items.size() == 0) return *this;
		else
		{
			expression result = *this;
			for (int i = 0; i < result.items.size(); i++)
			{
				result.items[i].coef /= 2;
			}
			return result;
		}
	}
};

item ItemMul(item it1, item it2) //�˷�
{
	item result;
	result.coef = it1.coef*it2.coef;
	result.var1 = it1.var1 || it2.var1;
	result.var2 = it1.var2 || it2.var2;
	return result;
}

expression ExprMul(expression ex1, expression ex2) //�˷�
{
	expression result;
	for (int i = 0; i < ex1.items.size(); i++)
		for (int j = 0; j < ex2.items.size(); j++)
			result.items.push_back(ItemMul(ex1.items[i], ex2.items[j]));
	return result;
}

class formula //
{
public:
	bool odd[3] = { false,false,false };
	expression exprs[3];
	formula(){}
	formula(bool odd0, bool odd1, bool odd2)
	{
		odd[0] = odd0; odd[1] = odd1; odd[2] = odd2;
	}
	bool equals(formula f)
	{
		return f.odd[0] == this->odd[0] && f.odd[1] == this->odd[1] && f.odd[2] == this->odd[2];
	}
	void display()
	{
		cout << (odd[0] ? "����" : "ż��") << "*" << (odd[1] ? "����" : "ż��") <<
			"=" << (odd[2] ? "����" : "ż��");
	}
};

formula CurrentFormula;
item _2m(2,true,false);
item _2n(2, false, true);
item _1(1, false, false);
expression _EVEN0(_2m);
expression _EVEN1(_2n);
expression _ODD0(_2m,_1);
expression _ODD1(_2n, _1);

//���������ж�
bool ProblemOdd0()//�Ⱥ���ߵ�һ������������
{
	return CurrentFormula.odd[0];
}
bool ProblemOdd1()//�Ⱥ���ߵڶ�������������
{
	return CurrentFormula.odd[1];
}
bool ProblemOdd2()
{
	return CurrentFormula.odd[2];
}
bool ProblemEven0()//�Ⱥ���ߵ�һ��������ż��
{
	return !CurrentFormula.odd[0];
}
bool ProblemEven1()//�Ⱥ���ߵڶ���������ż��
{
	return !CurrentFormula.odd[1];
}
bool ProblemEven2()
{
	return !CurrentFormula.odd[2];
}

funcp ProblemOdd[3] = { ProblemOdd0,ProblemOdd1,ProblemOdd2 };
funcp ProblemEven[3] = { ProblemEven0,ProblemEven1,ProblemEven2 };
//����״̬�����ж�
bool ResultRightOdd() //��ʽ�Ⱥ��ұߣ���������Ϊ����
{
	if (!CurrentFormula.exprs[2].available())
	{
		//cout << "�٣���ʽ�Ⱥ��ұߣ���������Ϊ����\n";
		return false;
	}
	else
	{
		//if (CurrentFormula.exprs[2].mod2() == 1&&LastExcuted) cout << "�棺";
		//else cout << "�٣�";
		//cout << "��ʽ�Ⱥ��ұߣ���������Ϊ����\n";
		return CurrentFormula.exprs[2].mod2() == 1 && LastExcuted;
	}
}
bool Remainder0()
{
	//cout << ((LastExcuted && (LastRemainder == 0)) ? "��" : "��") << "������Ϊ��\n";
	return LastExcuted&&(LastRemainder == 0);
}
bool Remainder1()
{
	//cout << ((LastExcuted && (LastRemainder == 1)) ? "��" : "��") << "������Ϊһ\n";
	return LastExcuted&&(LastRemainder == 1);
}

//����ִ�к���
bool Odd0Represent()
{
	if (CurrentFormula.odd[0])
	{
		CurrentFormula.exprs[0] = _ODD0;
		cout << "��ߵ�һ���ʾΪ" << CurrentFormula.exprs[0].tostring() << endl;
		return true;
	}
	else
	{
		cout << "������ߵ�һ���������\n";
		return false;
	}
}
bool Odd1Represent()
{
	if (CurrentFormula.odd[1])
	{
		CurrentFormula.exprs[1] = _ODD1;
		cout << "��ߵڶ����ʾΪ" << CurrentFormula.exprs[1].tostring() << endl;
		return true;
	}
	else
	{
		cout << "������ߵڶ����������\n";
		return false;
	}
}
bool Even0Represent()
{
	if (!CurrentFormula.odd[0])
	{
		CurrentFormula.exprs[0] = _EVEN0;
		cout << "��ߵ�һ���ʾΪ" << CurrentFormula.exprs[0].tostring() << endl;
		return true;
	}
	else
	{
		cout << "������ߵ�һ���ż����\n";
		return false;
	}
}
bool Even1Represent()
{
	if (!CurrentFormula.odd[1])
	{
		CurrentFormula.exprs[1] = _EVEN1;
		cout << "��ߵڶ����ʾΪ" << CurrentFormula.exprs[1].tostring() << endl;
		return true;
	}
	else
	{
		cout << "������ߵڶ����ż����\n";
		return false;
	}
}

//ͨ��ִ�к������û�����һ�����־�ִ�У������жϣ�
bool FormulaMultiply()
{
	if (CurrentFormula.exprs[0].available()&& CurrentFormula.exprs[1].available())
	{
		CurrentFormula.exprs[2] = ExprMul(CurrentFormula.exprs[0], CurrentFormula.exprs[1]);
		cout << "�����㣬�Ⱥ��ұߵ���" << CurrentFormula.exprs[2].tostring() << endl;
		return true;
	}
	else
	{
		if (!CurrentFormula.exprs[0].available())
			cout << "�㻹û���ô���ʽ��ʾ��ߵ�һ��\n";
		if (!CurrentFormula.exprs[1].available())
			cout << "�㻹û���ô���ʽ��ʾ��ߵڶ���\n";
		return false;
	}
}
bool Mod2()
{
	if (CurrentFormula.exprs[2].available())
	{
		int remainder = CurrentFormula.exprs[2].mod2();
		expression quotient = CurrentFormula.exprs[2].div2();//��
		cout << "�Ⱥ��ұ߳���2����" << quotient.tostring() << "������" << remainder << endl;
		LastExcuted = true;
		LastRemainder = remainder;
		return true;
	}
	else
	{
		cout << "�Ⱥ��ұ�û�б���ʾ�ɴ���ʽ��\n";
		return false;
	}
}
bool Remainder1Odd()//��1��Ϊ����
{
	if (LastExcuted)
	{
		if (LastRemainder == 1)
		{
			cout << "���ԵȺ��ұ�Ϊ������\n";
			cout << "֤�����\n";
			finished = true;
			return true;
		}
		else
		{
			cout << "������������1��\n";
			return false;
		}
	}
	else
	{
		cout << "��û�н��й��������㣡\n";
		return false;
	}
}
bool Remainder0Even()//��1��Ϊ����
{
	if (LastExcuted)
	{
		if (LastRemainder == 0)
		{
			cout << "���ԵȺ��ұ�Ϊż����\n";
			cout << "֤�����\n";
			finished = true;
			return true;
		}
		else
		{
			cout << "������������0��\n";
			return false;
		}
	}
	else
	{
		cout << "��û�н��й��������㣡\n";
		return false;
	}
}

//���ڼ�¼�ı��������������������±�ƽ�й�ϵ
vector<funcp> commands;//�û����������
vector<vector<funcp>> conditions;//ÿ��ָ���ִ�ж�����һ��ǰ�������ģ�ǰ��������ֹһ����

vector<funcp> GetTrueConditions(int VarIndex) //���ؽ��Ϊtrue��������������������ǰ���ڲ����ı������±�
{
	vector<funcp> returning;
	if (VarIndex >= 0 && VarIndex <= 2)
	{
		if (ProblemOdd[VarIndex]()) returning.push_back(ProblemOdd[VarIndex]);
		if (ProblemEven[VarIndex]()) returning.push_back(ProblemEven[VarIndex]);
	}
	if (ResultRightOdd()) returning.push_back(&ResultRightOdd);
	if (Remainder0()) returning.push_back(&Remainder0);
	if (Remainder1()) returning.push_back(&Remainder1);
	return returning;
}

bool ConditionsAllTrue(int index) //�жϣ����б�������������ϣ���ǰ����==ԭ��������
{
	if (conditions[index].size() == 0)
		return true;
	else
	{
		for (int i = 0; i < conditions[index].size(); i++)
		{
			if (conditions[index][i]() == false)
			{
				//cout << conditions[index][i] << "��Ϊ��\n";
				return false;
			}
		}
	}
	return true;
}

int GetCommandIndex(funcp cmd) //��ȡ�����±�
{
	if (commands.size() == 0) return -1;
	else
	{
		for (int i = 0; i < commands.size(); i++)
		{
			if (commands[i] == cmd) return i;
		}
	}
	return -1;
}

//��¼����
bool RecordCommand(int VarIndex,funcp cmd)
{
	bool exist = false;
	if (commands.size() > 0)
	{
		for (int i = 0; i < commands.size(); i++)
		{
			if (commands[i] == cmd)
			{
				exist = true; return false;
			}
		}
	}
	if (!exist)
	{
		commands.push_back(cmd);
		conditions.push_back(GetTrueConditions(VarIndex));
		return true;
	}
}

bool Var0Represent()//���û�����һ������ʽ����ʾ��ߵ�һ���ż�Բ�ȷ����
{
	string input;
	while (true)
	{
		cout << "����һ������ʽ����ʾ��ߵ�һ���ʽ�磺2X �� 2X+1����";
		cin >> input;
		if (input.length() != 2 && input.length() != 4)
		{
			cout << "��ʽ����\n";
		}
		else
		{
			alphabet1 = input[1];
			if (input.length() == 2) //ż��
			{
				if (Even0Represent() == false)//��ʱ�������ʾ������Ϣ����ߵ�һ���ż��
					continue;//���û���ȥ������
				else
				{
					//��¼֮
					RecordCommand(0, &Even0Represent);
				}
			}
			else //����
			{
				if (Odd0Represent() == false)
					continue;
				else
				{
					//��¼֮
					RecordCommand(0, &Odd0Represent);
				}
			}
			break;
		}
	}
	return true;
}
bool Var1Represent()//���û�����һ������ʽ����ʾ��ߵڶ����ż�Բ�ȷ����
{
	string input;
	while (true)
	{
		cout << "����һ������ʽ����ʾ��ߵڶ����ʽ�磺2X �� 2X+1����";
		cin >> input;
		if (input.length() != 2 && input.length() != 4)
		{
			cout << "��ʽ����\n";
		}
		else
		{
			alphabet2 = input[1];
			if (input.length() == 2)
			{
				if (Even1Represent() == false) continue;
				else
				{
					//��¼֮
					RecordCommand(1, &Even1Represent);
				}
			}
			else
			{
				if (Odd1Represent() == false) continue;
				else
				{
					RecordCommand(1, &Odd1Represent);
				}
			}
			break;
		}
	}
	return true;
}

//������ͣ���ִ������ǰ�ȼ�¼
bool interpret(string command)
{
	funcp cmd;//Ҫִ�е�����
	bool cmdSucceed = false;//�����Ƿ�ִ�гɹ�
	int VarIndex = -1;
	if (command == "�ô���ʽ��ʾ��ߵ�һ��") cmd=&Var0Represent;
	else if (command == "�ô���ʽ��ʾ��ߵڶ���") cmd = &Var1Represent;
	else if (command == "���") cmd = &FormulaMultiply;
	else if (command == "�Ⱥ��ұ߳��Զ�") cmd = &Mod2;
	else if (command == "��һ��Ϊ����") cmd = &Remainder1Odd;
	else if (command == "������Ϊż��") cmd = &Remainder0Even;
	else cmd = 0;


	if (cmd)//���ָ��bool�ͺ�����ָ�벻Ϊ��
	{
		bool HasRecorded = false;
		if(GetCommandIndex(cmd)==-1 && //����������û�б���¼��
			cmd != &Var0Represent&&cmd != Var1Represent)
			HasRecorded=RecordCommand(-1, cmd);//�ȼ�¼

		cmdSucceed = cmd();//��ִ�к���

		if (cmdSucceed==false&& //���ִ��ʧ��
			HasRecorded)//���Ҹղż�¼��
		{
			commands.pop_back();//�ͰѸղż�¼��������¡�����
			conditions.pop_back();
		}
	}
	return cmdSucceed;
}


int main()
{
	formula formula1(true, true, true);//��ʽ1������X����=����
	formula formula2(false, false, false);//��ʽ2��ż��Xż��=ż��
	formula formula3(true, false, false);//��ʽ3������Xż��=ż����Ҫ�������Լ�֤��
	CurrentFormula = formula1;
	
start_prove:
	system("cls");
	cout << "���ڿ�ʼ֤����ʽ��";
	CurrentFormula.display(); cout << "\n";
	while (finished==false)
	{
		string input;
		cin >> input;
		interpret(input);
	}
	if (CurrentFormula.equals(formula1))
	{
		CurrentFormula = formula2;
		init();
		system("pause");
		goto start_prove;
	}

	system("pause"); init(); system("cls");
	//�Զ�֤����ʽ3
	char RandomAlphabet1=alphabet1 = rand() % 24 + 'a';
	char RandomAlphabet2 = alphabet2 = alphabet1 + 1;
	CurrentFormula = formula3;
	cout << "����֤�����̰���һЩ����Ҫ�Ĳ������Ժ�����Թ��̽��м�\n";
	cout << "���ڿ�ʼ֤����ʽ��";
	CurrentFormula.display(); cout << "\n";
	set<int> AlreadyExcuted;//�Ѿ�ִ�й�����������Ӧ���±�
	vector<int> procedure;//֤������
	while (!finished)
	{
		for (int i = 0; i < commands.size(); i++)
		{
			set<int>::iterator it = AlreadyExcuted.find(i);//��ֹ�ظ�ִ��
			if (ConditionsAllTrue(i)&&it==AlreadyExcuted.end())
			{
				if (commands[i]()) //���ִ�гɹ�
				{
					AlreadyExcuted.insert(i);//��¼�±꣬�����ظ�ִ��
					procedure.push_back(i);
				}
			}
		}
		//system("pause");
	}
	system("pause");
	system("cls"); cout << "�����Ǽ򻯰汾��\n";
	init();
	alphabet1 = RandomAlphabet1;
	alphabet2 = RandomAlphabet2;
	for (int i = 0; i < procedure.size(); i++)
		commands[procedure[i]]();
	system("pause");
	return 0;
}

