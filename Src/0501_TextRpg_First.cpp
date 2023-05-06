#include <iostream>

#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

using namespace std;

typedef struct INFO
{
	char cJob[32];
	int iHp;
	int iAtk;

}INFO, * PINFO;


INFO Select_Job();
void Render(INFO *_pinfo);
void Field(INFO* _pPlayer);
void Create_Object(INFO _pMonster[]);
void Battle(INFO* _pPlayer, INFO* _pMonster);

void main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	INFO tPlayer = {};
	int iInput = 0;

	tPlayer = Select_Job();

	while (true)
	{
		Render(&tPlayer);
		cout << "1. ����� 2. ���� : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Field(&tPlayer);

		case 2:
			return;

		default:
			break;
		}
	}
}

void Render(INFO* _pStat)
{
	cout << "=====================================" << endl
		<< "�̸� : " << _pStat->cJob << endl
		<< "ü�� : " << _pStat->iHp << "\t���ݷ� : " << _pStat->iAtk << endl;
}

INFO Select_Job()
{
	INFO tTemp = {};
	int iInput = 0;

	cout << "������ �����ϼ���(1. ���� 2. ������ 3. ����) : ";
	cin >> iInput;

	switch (iInput)
	{
	case 1:
		//tTemp = { "����", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "����");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	case 2:
		//tTemp = { "������", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "������");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	case 3:
		//tTemp = { "����", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "����");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	default:
		break;
	}
	return tTemp;
}


void Field(INFO* _pPlayer)
{
	int iInput = 0;
	INFO tMonster[3] = {};

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		cout << "1. �ʱ� 2. �߱� 3. ��� 4. �� �ܰ� : ";
		cin >> iInput;

		if ((0 >= iInput) || (4 < iInput))
			continue;
		else if (4 > iInput)
		{
			Create_Object(tMonster);
			Battle(_pPlayer, &tMonster[iInput - 1]);
		}
		else if (4 == iInput)
			return;
		else
			return;
	}
}

void Create_Object(INFO _pMonster[])
{
	strcpy_s(_pMonster[0].cJob, sizeof(_pMonster[0].cJob), "�ʱ�");
	strcpy_s(_pMonster[1].cJob, sizeof(_pMonster[1].cJob), "�߱�");
	strcpy_s(_pMonster[2].cJob, sizeof(_pMonster[2].cJob), "���");

	for (int i = 0; i < 3; ++i)
	{
		_pMonster[i].iHp = (1 + i) * 30;
		_pMonster[i].iAtk = (1 + i) * 3;
	}
}

void Battle(INFO* _pPlayer, INFO* _pMonster)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		Render(_pMonster);
		cout << "1. ���� 2. ���� : ";
		cin >> iInput;

		if (1 == iInput) // 1. ���� �̸�
		{
			// �÷��̾�� ���� ü�� ���
			_pPlayer->iHp  -= (_pMonster->iAtk);
			_pMonster->iHp -= (_pPlayer->iAtk);

			// �÷��̾��� ü���� 0�̻��̰�, ������ ü���� 0���ϸ� �¸�
			if ((0 <= _pPlayer->iHp) && (0 >= _pMonster->iHp))
			{
				cout << endl << "�¸�" << endl;
				system("pause");
				break;
			}

			// �÷��̾��� ü���� 0���ϸ� �÷��̾� ���
			if (0 >= _pPlayer->iHp)
			{
				(_pPlayer->iHp) = 100;
				cout << endl << "�÷��̾� ���" << endl;
				system("pause");
				break;
			}
		}
		else if (2 == iInput)
		{
			return;
		}
	}
}