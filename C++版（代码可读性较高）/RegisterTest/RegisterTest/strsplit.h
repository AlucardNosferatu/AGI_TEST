#pragma once
#include<vector>
#include<string>
using namespace std;

vector<string> split(string str)//���ַ����������з�
{
	vector<size_t> pos;//�ܵ������ڵ�λ��
	vector<string> result;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '|') pos.push_back(i);
	}

	if (pos.size() > 0)
	{
		string tmp;
		if (pos.size() == 1)//ֻ��һ���ո�
		{
			tmp.assign(str, 0, pos[0]);
			result.push_back(tmp);
			tmp.assign(str, pos[0] + 1, str.size() - pos[0] - 1);
			result.push_back(tmp);
		}
		else//�����������ϵĿո�
		{
			if (pos.size() <= 2)//�������ֻ��2���ո�
			{
				tmp.assign(str, 0, pos[0]);
				result.push_back(tmp);
				tmp.assign(str, pos[0] + 1, pos[1] - pos[0] - 1);
				result.push_back(tmp);
				tmp.assign(str, pos[1] + 1, str.size() - pos[1] - 1);
				result.push_back(tmp);
			}
			else//����ո�������2
			{
				for (int i = 0; i <= pos.size() - 2; i++)
				{
					if (i == 0)
					{
						tmp.assign(str, 0, pos[0]);
						result.push_back(tmp);
						tmp.assign(str, pos[0] + 1, pos[1] - pos[0] - 1);
						result.push_back(tmp);
					}
					else
					{
						if (i < pos.size() - 2)
						{
							tmp.assign(str, pos[i] + 1, pos[i + 1] - pos[i] - 1);
							result.push_back(tmp);
						}
						else
						{
							string tmp2(str, pos[i] + 1, pos[i + 1] - pos[i] - 1);
							result.push_back(tmp2);
							tmp2.assign(str, pos[i + 1] + 1, str.size() - pos[i + 1] - 1);
							result.push_back(tmp2);
						}
					}
				}
			}
		}
	}
	else result.push_back(str);
	return result;
}
