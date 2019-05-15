// StringConverter.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"reflect.h"
#include"strsplit.h"
#include<afx.h>
using namespace std;

template<typename T>
int getIndex(vector<T> arr, T obj) 
{
	if (arr.size() == 0) return -1;
	for (int i = 0; i < arr.size(); i++)
		if (obj == arr[i])
			return i;
	return -1;
}

template<typename T>
bool arrEquals(vector<T> &a, vector<T> &b)
{
	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); i++)
		if (a[i] != b[i]) return false;
	return true;
}

template<typename K, typename V>
vector<V> getValuesForKey(multimap<K, V> &_map, K key)
{
	vector<V> results;
	auto begin = _map.lower_bound(key);
	auto end = _map.upper_bound(key);
	for (auto it = begin; it != end; ++it)
		results.push_back(it->second);
	return results;
}

#define getCondition() second
#define getActionOrConsequence() first
#define getAction() first
#define getConsequene() first


typedef const char* CharArray;

vector<string> temp_string;
vector<CString> temp_CString;
vector<CharArray> temp_CharArray;
vector<int> temp_int;

void* argPointer = NULL;
#define SECONDARY &argPointer

vector<FunctionPointer> funcs;
multimap<FunctionPointer, FunctionPointer> relationships;
map<FunctionPointer, bool> funcTypes;
map<FunctionPointer, string> names;

FunctionPointer getFunction(string name)
{
	for (FunctionPointer f : funcs)
		if (names[f] == name)
			return f;
	return NULL;
}

typedef vector<FunctionPointer> ComplexFunction;
#define complexFunctionEquals arrEquals<FunctionPointer>

int count_complexFunctions = 0;
ComplexFunction* complexFunctions=new ComplexFunction[5];

#define declareUniversalComplexFunction(id) void* uni_complexFunction##id##(vector<void*> args){void* returning = NULL;\
                   for (FunctionPointer f : complexFunctions[##id##])returning = f(args);return returning; }

declareUniversalComplexFunction(0);
declareUniversalComplexFunction(1);
declareUniversalComplexFunction(2);
declareUniversalComplexFunction(3);
declareUniversalComplexFunction(4);

FunctionPointer uni_complexFunctions[5] = { &uni_complexFunction0, &uni_complexFunction1, &uni_complexFunction2, &uni_complexFunction3, &uni_complexFunction4 };

string toString_route(vector<FunctionPointer> route)
{
	string str = "";
	for (int i = 0; i < route.size(); i++)
	{
		str += names[route[i]]; 
		if (i != route.size() - 1) str += "����>";
	}
	return str;
}

vector<FunctionPointer> removePredicates(vector<FunctionPointer> original)
{
	vector<FunctionPointer> result;
	for (FunctionPointer f : original)
		if (funcTypes[f] == VERB)
			result.push_back(f);
	return result;
}

vector<vector<FunctionPointer>> getAllValidSequences(vector<FunctionPointer> route)
{
	vector<vector<FunctionPointer>> sequences;
	for (int i = 1; i < route.size(); i++)
	{
		sequences.push_back(*new vector<FunctionPointer>(route.begin(), route.begin() + i + 1));
	}
	return sequences;
}

vector<vector<FunctionPointer>> getAllValidSequences_backwards(vector<FunctionPointer> originalRoute)
{
	vector<FunctionPointer> route;
	for (int i = originalRoute.size() - 1; i >= 0; i--)
		route.push_back(originalRoute[i]);

	vector<vector<FunctionPointer>>  temp=getAllValidSequences(route);
	vector<vector<FunctionPointer>> sequences;
	for (vector<FunctionPointer> one : temp)
	{
		vector<FunctionPointer> reversed;
		for (int i = one.size() - 1; i >= 0; i--)
			reversed.push_back(one[i]);
		sequences.push_back(reversed);
	}
	return sequences;
}

vector<pair<FunctionPointer,ComplexFunction>> detectRelationships_conditionAndFunction(vector<FunctionPointer> route)
{
	vector<pair<FunctionPointer, ComplexFunction>> returning;
	for (int i = 0; i < route.size()-1; i++)
	{
		FunctionPointer f = route[i];
		if (funcTypes[f] == VERB) continue;//ǰ�������ν��
		vector<FunctionPointer> rest = *new vector<FunctionPointer>(route.begin() + i + 1, route.end());
		vector<ComplexFunction> seqs=getAllValidSequences(rest);
		vector<ComplexFunction> sifted;//ɸѡ���
		for (ComplexFunction seq : seqs)
		{
			ComplexFunction seq_predicatesRemoved = removePredicates(seq);
			if (seq_predicatesRemoved.size() < 2) continue;//����Ϊ0��1��û����
			bool exist = false;
			for (ComplexFunction one_sifted : sifted)
			{
				if (arrEquals(one_sifted, seq_predicatesRemoved))
				{
					exist = true;
					break;
				}
			}
			if (!exist)
				sifted.push_back(seq_predicatesRemoved);
		}
		for (ComplexFunction one_sifted : sifted)
			returning.push_back(*new pair<FunctionPointer, ComplexFunction>(route[i], one_sifted));
	}
	return returning;
}

vector<pair<ComplexFunction,FunctionPointer>> detectRelationships_functionAndEffect(vector<FunctionPointer> route)
{
	vector<pair<ComplexFunction, FunctionPointer>> returning;
	for (int i = 2; i < route.size(); i++)
	{
		FunctionPointer f = route[i];
		if (funcTypes[f] == VERB) continue;//���������ν��
		vector<FunctionPointer> rest = *new vector<FunctionPointer>(route.begin(), route.begin()+i);//ע���������ұ�
		vector<ComplexFunction> seqs = getAllValidSequences_backwards(rest);
		vector<ComplexFunction> sifted;//ɸѡ���
		for (ComplexFunction seq : seqs)
		{
			ComplexFunction seq_predicatesRemoved = removePredicates(seq);
			if (seq_predicatesRemoved.size() < 2) continue;//����Ϊ0��1��û����
			bool exist = false;
			for (ComplexFunction one_sifted : sifted)
			{
				if (arrEquals(one_sifted, seq_predicatesRemoved))
				{
					exist = true;
					break;
				}
			}
			if (!exist)
				sifted.push_back(seq_predicatesRemoved);
		}
		for (ComplexFunction one_sifted : sifted)
			returning.push_back(*new pair<ComplexFunction, FunctionPointer>(one_sifted,route[i]));
	}
	return returning;
}

bool exist_complexFunction(ComplexFunction complexFunction)
{
	for (int i = 0; i < count_complexFunctions; i++)
		if (complexFunctionEquals(complexFunctions[i], complexFunction))
			return true;
	return false;
}

vector<ComplexFunction> extractComplexFunctions(vector<pair<FunctionPointer, ComplexFunction>> pairs_conditionAndFunction,
	vector<pair<ComplexFunction, FunctionPointer>> pairs_functionAndEffect)
{
	vector<ComplexFunction> results;
	for (auto it = pairs_conditionAndFunction.begin(); it != pairs_conditionAndFunction.end(); it++)
		results.push_back((*it).second);
	for (auto it = pairs_functionAndEffect.begin(); it != pairs_functionAndEffect.end(); it++)
		results.push_back((*it).first);
	return results;
}


void updateRelationships(vector<pair<FunctionPointer, ComplexFunction>> pairs_conditionAndFunction,
	vector<pair<ComplexFunction, FunctionPointer>> pairs_functionAndEffect)
{
	for (auto thisPair : pairs_conditionAndFunction)
	{
		for (int i = 0; i < count_complexFunctions; i++)
		{
			ComplexFunction storedFunction = complexFunctions[i];
			if (complexFunctionEquals(storedFunction, thisPair.second))
			{
				relationships.insert(*new pair<FunctionPointer, FunctionPointer>(uni_complexFunctions[i], thisPair.first));//���Ӻ���<��������
				break;
			}
		}
	}

	for (auto thisPair : pairs_functionAndEffect)
	{
		for (int i = 0; i < count_complexFunctions; i++)
		{
			ComplexFunction storedFunction = complexFunctions[i];
			if (complexFunctionEquals(storedFunction, thisPair.first))
			{
				relationships.insert(*new pair<FunctionPointer, FunctionPointer>(thisPair.second,uni_complexFunctions[i]));//Ӱ��<�������Ӻ���
				break;
			}
		}
	}
}

void saveFunction(string name, ComplexFunction steps)
{
	complexFunctions[count_complexFunctions] = steps;
	funcs.push_back(uni_complexFunctions[count_complexFunctions]);
	names.insert(*new pair<FunctionPointer, string>(uni_complexFunctions[count_complexFunctions], name));
	funcTypes.insert(*new pair<FunctionPointer, bool>(uni_complexFunctions[count_complexFunctions], VERB));
	count_complexFunctions++;
}

void IO_saveFunction(ComplexFunction complexFunction)
{
	cout << "�������ݣ�" << toString_route(complexFunction) << endl;
	cout << "��Ϊ�˺���ȡ����";
	string name;
	cin >> name;
	if (name != "XX")
		saveFunction(name, complexFunction);
}

void IO_saveFunctions(vector<ComplexFunction> complexFunctions)
{
	for (ComplexFunction func : complexFunctions)
	{
		if (exist_complexFunction(func) == false)
			IO_saveFunction(func);
	}
}

void* executeUni(FunctionPointer func, void* arg)
{
	vector<void*> args;
	args.push_back(arg);
	return func(args);
}

vector<FunctionPointer> getTruePredicates(void* arg)
{
	vector<FunctionPointer> predicates;
	for (FunctionPointer func : funcs)
	{
		if (funcTypes[func] == VERB) continue;
		if (executeUni(func, arg))
			predicates.push_back(func);
	}
	return predicates;
}

bool executeAndRecordRelationships(FunctionPointer verb, void *arg)
{
	vector<FunctionPointer> conditions = getTruePredicates(arg);
	if (executeUni(verb, arg))
	{
		if (funcTypes[verb] == VERB)
		{
			for (FunctionPointer condition : conditions)
				relationships.insert(*new pair<FunctionPointer, FunctionPointer>(verb, condition));

			vector<FunctionPointer> effects = getTruePredicates(arg);
			for (FunctionPointer effect : effects)
				if (getIndex<FunctionPointer>(conditions, effect) == -1)
					relationships.insert(*new pair<FunctionPointer, FunctionPointer>(effect, verb));
		}

		return true;
	}
	else
		return false;
}

int showMsgBox(CString content)
{
	return MessageBox(NULL, content, L"������Ҫ��ϵͳ��ʾ����Ϣ", 1);
}

bool isStd(void* ptr)
{
	if (ptr == SECONDARY) return isStd(*SECONDARY);
	if (temp_string.size() == 0) return false;
	else
	{
		void* head = &(temp_string[0]);
		void *tail = &(temp_string[temp_string.size() - 1]);
		return head <= ptr && tail >= ptr;
	}
}
createSimpleTypeVerb_singleArg(isStd, void*);

bool isCString(void* ptr)
{
	if (ptr == SECONDARY) return isCString(  *SECONDARY );
	if (temp_CString.size() == 0) return false;
	else
	{
		void* head = &(temp_CString[0]);
		void *tail = &(temp_CString[temp_CString.size()-1]);
		return head <= ptr && tail >= ptr;
	}
}
createSimpleTypeVerb_singleArg(isCString, void*);

bool isCharArray(void* ptr)
{
	if (ptr == SECONDARY) return isCharArray(*SECONDARY);
	if (temp_CharArray.size() == 0) return false;
	else
	{
		void* head = &(temp_CharArray[0]);
		void *tail = &(temp_CharArray[temp_CharArray.size() - 1]);
		return head <= ptr && tail >= ptr;
	}
}
createSimpleTypeVerb_singleArg(isCharArray, void*);

bool isInteger(void* ptr)
{
	if (ptr == SECONDARY) return isInteger(*SECONDARY);
	if (temp_int.size() == 0) return false;
	else
	{
		void* head = &(temp_int[0]);
		void *tail = &(temp_int[temp_int.size() - 1]);
		return head <= ptr && tail >= ptr;
	}
}
createSimpleTypeVerb_singleArg(isInteger, void*);

void* uni_showMsgBox(vector<void*> args)
{
	if (args[0] == SECONDARY)
	{
		vector<void*> temp;
		temp.push_back( *SECONDARY );
		return uni_showMsgBox(temp);
	}

	argPointer = args[0];
	if (isCString(args[0]))
	{
		showMsgBox(*(CString*)args[0]);
		return (void*)true;
	}
	else
		return (void*)false;
}

CharArray std2CharArray(string str)
{
	CharArray charArray = str.c_str();
	return charArray;
}

CString CharArray2CString(CharArray charArray)
{
	return *new CString(charArray);
}

CString int2CString(int integer)
{
	CString str;
	str.Format(_T("%d"), integer);
	return str;
}

void* uni_std2CharArray(vector<void*> args)
{
	if (isStd(args[0]) == false) return NULL;
	if (args[0] == SECONDARY)
	{
		args[0] = *SECONDARY;
		return uni_std2CharArray(args);
	}
	CharArray result = ((string*)args[0])->c_str();
	temp_CharArray.push_back(result);
	argPointer = &temp_CharArray[temp_CharArray.size() - 1];
	return argPointer;
}

void* uni_CharArray2CString(vector<void*> args)
{
	if (isCharArray(args[0]) == false) return NULL;
	if (args[0] == SECONDARY)
	{
		args[0] = *SECONDARY;
		return uni_CharArray2CString(args);
	}
	CString result = CharArray2CString(*(CharArray*)args[0]);
	temp_CString.push_back(result);
	argPointer = &temp_CString[temp_CString.size() - 1];
	return argPointer;
}

void* uni_int2CString(vector<void*> args)
{
	if (isInteger(args[0]) == false) return NULL;
	if (args[0] == SECONDARY)
	{
		args[0] = *SECONDARY;
		return uni_int2CString(args);
	}
	CString result = int2CString(*(int*)args[0]);
	temp_CString.push_back(result);
	argPointer = &temp_CString[temp_CString.size() - 1];
	return argPointer;
}

bool findCluesBackwards(FunctionPointer currentGoal,void* arg,vector<FunctionPointer> *route)
{
	//if (funcTypes[currentGoal] == VERB) 
		route->push_back(currentGoal);

	if (executeUni(currentGoal, arg))
	{
		if (funcTypes[currentGoal] == PREDICATE)
			cout << "�жϣ�" << names[currentGoal] << "��" << "��" << endl;
		else
			cout << "���ԣ�" << names[currentGoal] << "��" << "�ɹ�" << endl;
		return true;
	}
	else
	{
		if (funcTypes[currentGoal] == PREDICATE)
			cout << "�жϣ�" << names[currentGoal] << "��" << "��" << endl;
		else
			cout << "���ԣ�" << names[currentGoal] << "��" << "ʧ��" << endl;
	}

	vector<FunctionPointer> conditions = getValuesForKey<FunctionPointer, FunctionPointer>(relationships,currentGoal);
	if (conditions.size() == 0) return false;

	for (FunctionPointer condition:conditions)
	{
		bool returning = executeUni(condition, arg);
		
		if (returning == true)
		{
			if (funcTypes[condition] == PREDICATE)
				cout << "�жϣ�" << names[condition] << "��" << (returning ? "��" : "��") << endl;
			else
				cout << "���ԣ�" << names[condition] << "��" << (returning ? "�ɹ�" : "ʧ��") << endl;

			route->push_back(condition);
			return true;
		}
		else
		{
			vector<FunctionPointer> innerRoute = *route;
			bool returning= findCluesBackwards(condition, arg,&innerRoute);
			if (returning == true)
			{
				*route = innerRoute;
				return true;
			}
		}
	}

	return false;
}

void initFunctions()
{
	funcs.push_back(&uni_isCharArray);
	funcs.push_back(&uni_isCString);
	funcs.push_back(&uni_isInteger);
	funcs.push_back(&uni_isStd);
	funcs.push_back(&uni_showMsgBox);
	funcs.push_back(&uni_std2CharArray);
	funcs.push_back(&uni_CharArray2CString);
	funcs.push_back(&uni_int2CString);
	
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_isCharArray, PREDICATE));
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_isCString, PREDICATE));
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_isInteger, PREDICATE));
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_isStd, PREDICATE));
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_showMsgBox, VERB));
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_std2CharArray, VERB));
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_CharArray2CString, VERB));
	funcTypes.insert(*new pair<FunctionPointer, bool>(&uni_int2CString, VERB));
	names.insert(*new pair<FunctionPointer, string>(&uni_isCharArray, "���ַ�����"));
	names.insert(*new pair<FunctionPointer, string>(&uni_isCString, "��CString���ַ���"));
	names.insert(*new pair<FunctionPointer, string>(&uni_isInteger, "������"));
	names.insert(*new pair<FunctionPointer, string>(&uni_isStd, "�Ǳ�׼�ַ���"));
	names.insert(*new pair<FunctionPointer, string>(&uni_showMsgBox, "�����Ի���"));
	names.insert(*new pair<FunctionPointer, string>(&uni_std2CharArray, "��׼�ַ���תΪ�ַ�����"));
	names.insert(*new pair<FunctionPointer, string>(&uni_CharArray2CString, "�ַ�����תΪCString"));
	names.insert(*new pair<FunctionPointer, string>(&uni_int2CString, "����תΪCString"));
}

void initRules()
{
	relationships.insert(*new pair<FunctionPointer, FunctionPointer>(&uni_showMsgBox, &uni_isCString));
	relationships.insert(*new pair<FunctionPointer, FunctionPointer>(&uni_isCString, &uni_CharArray2CString));
	relationships.insert(*new pair<FunctionPointer, FunctionPointer>(&uni_isCString, &uni_int2CString));//������
	relationships.insert(*new pair<FunctionPointer, FunctionPointer>(&uni_CharArray2CString, &uni_isCharArray));
	relationships.insert(*new pair<FunctionPointer, FunctionPointer>(&uni_isCharArray, &uni_std2CharArray));
	relationships.insert(*new pair<FunctionPointer, FunctionPointer>(&uni_std2CharArray, &uni_isStd));

	/*
	vector<FunctionPointer> values = getValuesForKey<FunctionPointer, FunctionPointer>(relationships,&uni_isCString);
	for (FunctionPointer it:values)
		cout << names[it] << endl;
	*/
}

//Ԫ�����Ƕ��ʣ�������涨ÿ��Ԫ�������������з���ֵ������0��NULL����ʾִ��ʧ�ܣ����ط���ֵ��ʾ�ɹ���
//ν�ʽ��������ж����������ԣ�����Բ�����������κ�Ӱ�졣�������Ĳ����������ĳ���ԣ��򷵻�true�����򷵻�false��
//���еĶ��ʡ�ν�ʾ�ֻ����һ��������

//��ʵ�����漰�ġ���ϵ��ֻ�����֣�
//һ���ǡ�ִ�еĶ������롾��ɵ�Ӱ�졿֮�����ϵ��ǰ����һ���ɶ��ʺͲ������ɵ������������ν�ʺͲ������ɵ����⡣
//���硾���ַ�����תΪCString����ִ�еĶ�������ʹ�ó�ΪCString���ַ���������ɵ�Ӱ�죬
//����һ�ֹ�ϵ�ǡ�ִ�еĶ������롾ִ�д˶�������Ҫ�����ǰ��������֮�����ϵ��
//���硾���ַ�����תΪCString���������㴫��Ĳ���������������ַ����飬������������������Ὣ�ö���ǿ��ת�ͳ��ַ����鵼���쳣��
//����������ַ����顿����ִ�������ǰ����������Ҳ��һ����ν�ʺͲ������ɵ����⡣
//������Щ�������������ɳ���ԱԤ�ȱ�д�õģ���Ҫͨ�������Լ�ѧϰ�õ���


//����Ŀ��ģʽ
//�ڳ���ģʽ�£�ִ��һ��������ʧ���ˣ���ϵͳ�ͻ�ֱ������Ļ�������ʧ�ܡ���
//��������Ŀ��ģʽ�£����ִ������ʧ���ˣ�ϵͳ��˼�������Ҫʹ�������ִ�гɹ�����Ҫ����ʲô������Ȼ�������뷽�跨ʹ��ǰ������������
//�������һ����׼���ַ�������ϵͳ�ڶԻ�������ʾ����ַ��������ڲ������Ͳ���CString�ͣ�ִ��ʧ�ܡ�
//ϵͳ�뷽�跨����������ת��ΪCString�ͣ����˽⵽���á����ַ�����תΪCString�����Դﵽ��һЧ����
//�����������˸ú�����������ڲ��������ַ����飬����ִ��ʧ���ˡ�
//Ȼ��ϵͳ���뷨�跨�ò�������ַ����飬�����֡�����׼�ַ���תΪ�ַ����顿����������һ���󣬸�����ִ�гɹ���
//����ϵͳ��¼��ȫ���̣�����׼�ַ���תΪ�ַ����顪�����ַ�����תΪCString���������Ի���
//���ִ����Щ������ճɹ����ڶԻ����������ԭ��׼�ַ��������ݡ�

//��¼�����飨ʧ�ܾ��飩
//����Ԫ�����ĺ�������д�����쳣������룬���硾���ַ�����תΪCString������������ָ������������ָ���ַ�����ģ�
//��ֱ�ӷ���NULL������ִ�к���Ĵ��롣���������һ���쳣�������ע�͵��أ�����Ȼ���������ֱ�ӱ�������ֹ���С�
//���ǿ�������һ���������¼���ƣ���ʱ��̵������ݿ����棨��Ҫ��¼���ڴ����棬��Ϊ��������Ժ��ڴ�����ݾ�û�ˣ�
//��¼ϵͳִ�е�����C���Լ�ִ������֮ǰ������״̬P�������������쳣�˳������´����г����ʱ�򣬶�ȡ���ݿ�����������ݡ�
//����¼�����������顪���ڲ�������״̬P��ʱ��ִ������C���ᵼ�³���������´���ִ������C��ʱ��ϵͳ��������Ƿ����״̬P��
//������㣬��ֱ�ӷ���NULL������ǿ��ת�͵��³�������������ѵ���Ժ󣬳���ͻ�ϰ��һ���쳣������ơ�




string toString_Relationship(pair<FunctionPointer, FunctionPointer> relationship)
{
	return names[relationship.first] + " <---- " + names[relationship.second];
}

void showAllRelationships()
{
	for (FunctionPointer func : funcs)
	{
		vector<FunctionPointer> conditions=
			getValuesForKey<FunctionPointer,FunctionPointer>(relationships, func);
		for (FunctionPointer condition : conditions)
			cout << names[func] << " <---- " << names[condition] << endl;
	}
}

#define STD 1
#define CHARARRAY 2
#define CSTRING 3
#define INT 4

void IO_processInputCmd(string cmd, int type,bool simplyExecute)
{
	string funcName = split(cmd, '|')[0];
	string argString = split(cmd, '|')[1];
	FunctionPointer func = getFunction(funcName);
	switch (type)
	{
	case STD:
	{
		temp_string.push_back(argString); 
		argPointer = &temp_string[temp_string.size()-1]; 
		break; 
	}
	case CHARARRAY:
	{
		temp_CharArray.push_back(argString.c_str()); 
		argPointer = &temp_CharArray[temp_CharArray.size() - 1];
		break; 
	}
	case CSTRING:
	{
		temp_CString.push_back(*new CString(argString.c_str()));
		argPointer = &temp_CString[temp_CString.size() - 1];
		break;
	}
	case INT:
	{
		temp_int.push_back(atoi(argString.c_str()));
		argPointer = &temp_int[temp_int.size() - 1];
		break;
	}
	default:break;
	}

	if (simplyExecute)
	{
		bool returning=executeAndRecordRelationships(func, SECONDARY);
		if (funcTypes[func] == VERB) cout << (returning?"�ɹ�\n":"ʧ��\n");
		else cout << (returning?"��\n":"��\n");
	}
	else
	{
		FunctionPointer initialStatus = getTruePredicates(SECONDARY)[0];
		vector<FunctionPointer> route;//���ڴ洢·��
		findCluesBackwards(func, SECONDARY, &route);
		route.push_back(initialStatus);//�ѳ�ʼ״̬�ŵ����
		auto backup = route;//���ݵ���·������������ת��
		route.clear();//�Ȱ�ԭ�������
		for (int i = backup.size() - 1; i >= 0; i--)//����ת��
			route.push_back(backup[i]);

		auto relationships_conditionAndFunction = detectRelationships_conditionAndFunction(route);
		auto relationships_functionAndEffect = detectRelationships_functionAndEffect(route);

		auto appearedComplexFunctions=extractComplexFunctions(relationships_conditionAndFunction, relationships_functionAndEffect);

		IO_saveFunctions(appearedComplexFunctions);

		updateRelationships(relationships_conditionAndFunction, relationships_functionAndEffect);
	}
}

void IO_inputCmdAndArgType(bool simplyExecute)
{
	string cmd;
	cout << "���������";
	cin >> cmd;
	int argType;
	cout << "������������ͣ�1����׼�ַ�����2���ַ����飻3.CString���ַ�����4.������\n";
	cin >> argType;
	IO_processInputCmd(cmd, argType, simplyExecute);
}

void showComplexFunctions()
{
	for (int i = 0; i < count_complexFunctions; i++)
		cout << (i + 1) << ". " << names[uni_complexFunctions[i]] << endl << toString_route(complexFunctions[i]) << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	initFunctions();
	while (true)
	{
		string taskName;
		cin >> taskName;
		if ("ִ������" == taskName)
			IO_inputCmdAndArgType(true);
		else if ("ʵ��Ŀ��" == taskName)
			IO_inputCmdAndArgType(false);
		else if ("�鿴�����ϵ" == taskName)
			showAllRelationships();
		else if ("�鿴���Ӻ���" == taskName)
			showComplexFunctions();
	}
	
	string any;
	
	//initRules();


	temp_string.push_back("�Ĵ�����"); temp_string.push_back("�¼���"); temp_string.push_back("�й�"); temp_string.push_back("�¹�");
	temp_CharArray.push_back("����"); temp_CharArray.push_back("����"); temp_CharArray.push_back("����");
	temp_CString.push_back(L"Chemistry"); temp_CString.push_back(L"Math"); temp_CString.push_back(L"History");

	/*
	vector<string> part = *new vector<string>(temp_string.begin() + 1, temp_string.begin() + 3);
	for (string str : part)
		cout << str << "  ";
	cout << endl;
	*/

	argPointer = &temp_string[1];//���¼��¡�

	executeAndRecordRelationships(&uni_std2CharArray, SECONDARY);
	showAllRelationships();
	//cin >> any;
	executeAndRecordRelationships(&uni_CharArray2CString, SECONDARY);
	showAllRelationships();
	//cin >> any;
	executeAndRecordRelationships(&uni_showMsgBox, SECONDARY);
	showAllRelationships();
	//cin >> any;
	temp_int.push_back(347);
	argPointer = &temp_int[0];
	executeAndRecordRelationships(&uni_int2CString, SECONDARY);
	showAllRelationships();
	//cin >> any;

	/*
	cout<<executeUni(&uni_showMsgBox, SECONDARY)<<endl;
	cout << executeUni(&uni_std2CharArray, SECONDARY) << endl;
	cout << executeUni(&uni_CharArray2CString, SECONDARY) << endl;
	cout << executeUni(&uni_showMsgBox, SECONDARY) << endl;
	*/

	argPointer = &temp_string[2];
	FunctionPointer initialStatus = getTruePredicates(SECONDARY)[0];
	vector<FunctionPointer> route;
	cout << executeUni(&uni_isCString, SECONDARY) << endl;
	cout << findCluesBackwards(&uni_isCString, SECONDARY, &route) << endl;
	cout << executeUni(&uni_isCString, SECONDARY) << endl;
	route.push_back(initialStatus);

	if (isStd(SECONDARY)) cout << "��׼\n";
	else if (isCString(SECONDARY)) cout << "CString\n";
	else if (isCharArray(SECONDARY)) cout << "�ַ�����\n";
	else if (isInteger(SECONDARY)) cout << "����\n";
	else cout << "����\n";


	for (int i = route.size() - 1; i >= 0; i--)
	{
		executeUni(route[i], SECONDARY);
	}
	
	auto backup = route;
	route.clear();
	for (int i = backup.size() - 1; i >= 0; i--)
	{
		route.push_back(backup[i]);
	}

	//route = *new vector<FunctionPointer>(route.begin(), route.end());
	for (int i =0; i<route.size(); i++)
	{
		cout << names[route[i]];
		if (i != route.size()-1) cout << "����>";
	}

	cout << endl;

	if (isStd(SECONDARY)) cout << "��׼\n";
	else if (isCString(SECONDARY)) cout << "CString\n";
	else if (isCharArray(SECONDARY)) cout << "�ַ�����\n";
	else if (isInteger(SECONDARY)) cout << "����\n";
	else cout << "����\n";


	for (pair<FunctionPointer, ComplexFunction> thisPair : detectRelationships_conditionAndFunction(route))
	{
		cout << "��" << toString_route(thisPair.second) << "��" ;
		cout << "<����" << names[thisPair.first] << endl;
	}

	//�û���������
	//1.��ʾϵͳ���ֵ����������ϵ
	//2.��ʾϵͳ��ȡ�����ĺ���
	//3.���û������������Щ��������������ظ��Ͳ�������
	//4.���½�������ʾ�����ϵ

	return 0;
}

