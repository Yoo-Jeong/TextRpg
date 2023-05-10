#include <iostream>
#include <crtdbg.h>
#include <ctime>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

using namespace std;


const int iInven = 5;

typedef struct tagItem
{
	char	szName[32];
	int		iEffect;
	int		iPrice;
	int		iCount;
	int		iMaxCount;

}ITEM;

typedef struct tagInfo
{
	char	szName[32];
	char	szJob[32];
	int		iHp;
	int		iMaxHp;
	int		iAtk;
	int		iLevel;
	int		iExp;
	int		iMaxExp;
	int		iMoney;

	ITEM	tItem[iInven];

}INFO;


enum STATE
{
	LOSE,
	WIN = 1,
	RUN = 1,
	END
};

INFO* Select_Job();
void	Create_Object(INFO** ppInfo, const char* pName, const char* pJob, int _iMaxHp, int _iAtk, int _iLevel, int _iExp, int _iMoney);
void	Main_Game(INFO* pPlayer);
void	Render(INFO* pInfo, bool bSelect = false);
void	Field(INFO* pPlayer);
STATE	Fight(INFO* pPlayer, INFO* pMonster);
void	Level_Up(INFO* pPlayer, int _iExp);
void	Get_Money(INFO* pPlayer, int _iMoney);

void	Create_Item(ITEM** ppItem, const char* pName, int _iEffect, int _iPrice, int _iCount);
void	Shop(INFO* pPlayer);
void	Buy_Item(INFO* pPlayer, ITEM* pItem, int _iInput);
void	Use_Item(INFO* pPlayer);

bool	Save_Data(INFO* pPlayer);
bool	Load_Data(INFO** ppPlayer);



void main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	srand(unsigned(time(NULL)));

	INFO* pPlayer = nullptr;

	pPlayer = Select_Job();

	if (NULL == pPlayer)
		return;

	Main_Game(pPlayer);

	if (NULL != pPlayer)
	{
		delete pPlayer;
		pPlayer = NULL;
	}

}

INFO* Select_Job()
{
	INFO* pInfo = nullptr;
	int		iInput = 0;
	bool	bSelect = false;
	char	cName[32];

	while (!bSelect) {

		system("cls");

		cout << "TextRpg�� ���Ű� ȯ�� �մϴ�." << endl
			<< "������ �����ϼ���" << endl
			<< "1. ���� 2. ������ 3. ���� 4. �ҷ����� 5. ���� : ";
		cin >> iInput;

		if (4 > iInput)
		{
			cout << "�г����� �Է����ּ��� : ";
			cin >> cName;
		}

		switch (iInput)
		{
		case 1:
			Create_Object(&pInfo, cName, "����", 100, 10, 1, 0, 0);
			bSelect = true;
			break;

		case 2:
			Create_Object(&pInfo, cName, "������", 100, 10, 1, 0, 0);
			bSelect = true;
			break;

		case 3:
			Create_Object(&pInfo, cName, "����", 100, 10, 1, 0, 0);
			bSelect = true;
			break;

		case 4:
			bSelect = Load_Data(&pInfo);
			break;

		case 5:
			bSelect = false;
			break;
		}
	}
	return pInfo;
}
void	Create_Object(INFO** ppInfo, const char* pName, const char* pJob, int _iMaxHp, int _iAtk, int _iLevel, int _iExp, int _iMoney)
{
	*ppInfo = new INFO;
	strcpy_s((*ppInfo)->szName, sizeof((*ppInfo)->szName), pName);
	strcpy_s((*ppInfo)->szJob, sizeof((*ppInfo)->szJob), pJob);
	(*ppInfo)->iMaxHp = _iMaxHp;
	(*ppInfo)->iHp = (*ppInfo)->iMaxHp;
	(*ppInfo)->iAtk = _iAtk;
	(*ppInfo)->iLevel = _iLevel;
	(*ppInfo)->iExp = _iExp;
	(*ppInfo)->iMaxExp = 100;
	(*ppInfo)->iMoney = _iMoney;

	for (int i = 0; i < iInven; ++i)
	{
		strcpy_s((*ppInfo)->tItem[i].szName, sizeof((*ppInfo)->tItem[i].szName), "����");
		(*ppInfo)->tItem[i].iEffect = 0;
		(*ppInfo)->tItem[i].iPrice = 0;
		(*ppInfo)->tItem[i].iCount = 0;
		(*ppInfo)->tItem[i].iMaxCount = 99;
	}
}
void Create_Item(ITEM** ppItem, const char* pName, int _iEffect, int _iPrice, int _iCount)
{
	*ppItem = new ITEM;
	strcpy_s((*ppItem)->szName, sizeof((*ppItem)->szName), pName);
	(*ppItem)->iEffect = _iEffect;
	(*ppItem)->iPrice = _iPrice;
	(*ppItem)->iCount = _iCount;
	(*ppItem)->iMaxCount = 99;

}
void	Main_Game(INFO* pPlayer)
{
	int iInput = 0;

	while (true)
	{
		system("cls");

		Render(pPlayer, true);
		cout << "1. ����� 2. ���� 3. ����(����) : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Field(pPlayer);
			break;

		case 2:
			Shop(pPlayer);
			break;

		case 3:
			Save_Data(pPlayer);
			return;

		default:
			break;
		}
	}
}
void	Render(INFO* pInfo, bool bSelect)
{
	cout << "===========================================" << endl
		<< "�̸� : " << pInfo->szName << endl
		<< "���� : " << pInfo->szJob << "\tü�� : " << pInfo->iHp << " / " << pInfo->iMaxHp << endl
		<< "���� : " << pInfo->iLevel << "\t���ݷ� : " << pInfo->iAtk << endl;

	if (bSelect)
	{
		cout << "����ġ: " << pInfo->iExp << " / " << pInfo->iMaxExp << endl
			<< "��� : " << pInfo->iMoney << endl
			<< "�κ��丮 :\t";

		for (int i = 0; i < iInven; ++i)
		{
			if (!strcmp(pInfo->tItem[i].szName, "����") || (0 == pInfo->tItem[i].iCount))
			{
				cout << "��\t";
			}
			else
			{
				cout << pInfo->tItem[i].szName << "*" << pInfo->tItem[i].iCount << "\t";
			}
		}
	}

	cout << endl;
}
void	Field(INFO* pPlayer)
{
	int iInput = 0;

	INFO* pMonster = NULL;

	bool	bSave = false;

	while (true)
	{
		system("cls");
		Render(pPlayer, true);

		cout << "1. �ʱ� 2. �߱� 3. ���� 4. �����ϱ� 5. �� �ܰ� : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Create_Object(&pMonster, "�ʱ� ����", "�ʱ�", (30 * iInput), (3 * iInput), (3 * iInput), (30 * iInput), (100 * iInput));
			break;

		case 2:
			Create_Object(&pMonster, "�߱� ����", "�߱�", (30 * iInput), (3 * iInput), (3 * iInput), (30 * iInput), (100 * iInput));
			break;

		case 3:
			Create_Object(&pMonster, "���� ����", "����", (30 * iInput), (3 * iInput), (3 * iInput), (30 * iInput), (100 * iInput));
			break;

		case 4:
			bSave = Save_Data(pPlayer);
			break;

		case 5:
			return;
		}

		if (bSave)
		{
			bSave = false;
			continue;
		}

		STATE	eState = Fight(pPlayer, pMonster);

		if (LOSE == eState)
		{
			pPlayer->iHp = pPlayer->iMaxHp;
		}

		if (RUN == eState || WIN == eState)
		{
			if (NULL != pMonster)
			{
				delete pMonster;
				pMonster = NULL;
			}
		}
	}
}

STATE	Fight(INFO* pPlayer, INFO* pMonster)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(pPlayer, true);
		Render(pMonster);

		cout << "1. ���� 2. �����ۻ�� 3. ���� : ";
		cin >> iInput;

		if (1 == iInput) // 1. ���� �̸�
		{
			// �÷��̾�� ���� ü�� ���
			pPlayer->iHp -= (pMonster->iAtk);
			pMonster->iHp -= (pPlayer->iAtk);

			// �÷��̾��� ü���� 0���ϸ� �÷��̾� ���
			if (0 >= pPlayer->iHp)
			{
				cout << "�÷��̾� ���..." << endl;
				system("pause");

				return LOSE;
			}

			// ������ ü���� 0���ϸ� �¸�
			if (0 >= pMonster->iHp)
			{
				cout << "�¸�" << endl;

				Level_Up(pPlayer, pMonster->iExp);
				Get_Money(pPlayer, pMonster->iMoney);
				system("pause");

				return WIN;
			}
		}
		else if (2 == iInput)
		{
			Use_Item(pPlayer);
		}

		else if (3 == iInput)
			return RUN;
	}
}

void	Level_Up(INFO* pPlayer, int _iExp)
{
	pPlayer->iExp += _iExp; // ����ġ ȹ��

	// ����ġ�� 100�̻��̸� ������
	if (pPlayer->iMaxExp <= pPlayer->iExp)
	{
		if (!strcmp(pPlayer->szJob, "����"))
		{
			pPlayer->iAtk += 10;
			pPlayer->iMaxHp += 20;
			pPlayer->iMaxExp += 50;
		}
		else if (!strcmp(pPlayer->szJob, "������"))
		{
			pPlayer->iAtk += 25;
			pPlayer->iMaxHp += 10;
			pPlayer->iMaxExp += 50;

		}
		else if (!strcmp(pPlayer->szJob, "����"))
		{
			pPlayer->iAtk += 15;
			pPlayer->iMaxHp += 15;
			pPlayer->iMaxExp += 50;
		}

		(pPlayer->iLevel)++;			// ������
		pPlayer->iHp = pPlayer->iMaxHp; // ü�� Ǯ ȸ��
		pPlayer->iExp = 0;				// ���� ����ġ 0

		cout << "������!" << endl;
	}
}

void	Get_Money(INFO* pPlayer, int _iMoney)
{
	pPlayer->iMoney += _iMoney;		// �� ȹ��

	cout << _iMoney << "��� ȹ��" << endl;
}


void	Shop(INFO* pPlayer)
{
	ITEM* pItem = NULL;

	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(pPlayer, true);

		cout << endl << "������ ���� �� ȯ���մϴ�." << endl << endl;
		cout << "1. HP����(300G) 4. �� �ܰ� : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Create_Item(&pItem, "HP����", 20, 300, 1);
			Buy_Item(pPlayer, pItem, iInput);

			delete pItem;
			pItem = NULL;

			break;

		case 2:

			break;

		case 3:

			break;

		case 4:
			return;

		}

	}

}

void	Buy_Item(INFO* pPlayer, ITEM* pItem, int _iInput)
{
	int iIndex = _iInput - 1;

	if (pPlayer->iMoney >= pItem->iPrice)
	{
		pPlayer->iMoney -= pItem->iPrice;

		strcpy_s(pPlayer->tItem[_iInput - 1].szName, pItem->szName);
		pPlayer->tItem[iIndex].iEffect = pItem->iEffect;
		pPlayer->tItem[iIndex].iPrice = pItem->iPrice;
		pPlayer->tItem[iIndex].iCount += pItem->iCount;	// ���� ��������+
		pPlayer->tItem[iIndex].iMaxCount = pItem->iMaxCount;

		cout << "���� ����" << endl;
		system("pause");

		Render(pPlayer, true);
	}
	else
	{
		cout << "�������� �����մϴ�." << endl;
		system("pause");
	}

}

void	Use_Item(INFO* pPlayer)
{
	int iInput = 0;

	cout << endl << "1. HP����(ü�� 20ȸ��)" << endl
		<< "2. ������ ��� ���" << endl
		<< "� �������� ����Ͻðڽ��ϱ�? : ";

	cin >> iInput;

	switch (iInput)
	{

	case 1:
		if ((!strcmp(pPlayer->tItem[0].szName, "HP����")) && (0 < pPlayer->tItem[0].iCount))
		{
			pPlayer->iHp += pPlayer->tItem[0].iEffect;
			pPlayer->tItem[0].iCount -= 1;

			if (pPlayer->iHp > pPlayer->iMaxHp)
			{
				pPlayer->iHp = pPlayer->iMaxHp;
			}

			cout << "ü���� ȸ���Ǿ����ϴ�." << endl;
			system("pause");

		}
		else
		{
			cout << "�������� �����ϴ�." << endl;
			system("pause");
		}

		break;

	case 2:
		return;

	default:
		break;
	}
}

bool Save_Data(INFO* pPlayer)
{

	FILE* pFile = nullptr;	// ���� ��Ʈ��

	errno_t err = fopen_s(&pFile, "./Data/Save.txt", "wb");

	if (0 == err)
	{
		// 2. ���Ͽ� ���ų� �б� �Լ��� ȣ��
		fwrite(pPlayer, sizeof(INFO), 1, pFile);

		cout << "ĳ���� ���� ���� ����" << endl;

		fclose(pFile);

		system("pause");

		return true;
	}
	else
		cout << "ĳ���� ���� ���� ����" << endl;

	system("pause");

	return false;

}

bool	Load_Data(INFO** ppPlayer)
{
	(*ppPlayer) = new INFO;

	FILE* pFile = nullptr;		// ���� ��Ʈ��

	errno_t err = fopen_s(&pFile, "./Data/Save.txt", "rb");

	if (0 == err)
	{
		// fread(�Է��� �޸��� �ּ�, �Է��� �޸� ������, �Է��� �޸� ����, ���� ��Ʈ��);
		fread((*ppPlayer), sizeof(INFO), 1, pFile);

		cout << "���� �ҷ����� ���� : ĳ���� ������ �о�ɴϴ�..." << endl;
		fclose(pFile);

		system("pause");

		return true;
	}
	else
	{
		cout << "���� �ҷ����� ����" << endl;
		system("pause");

		return false;
	}

	return false;
}