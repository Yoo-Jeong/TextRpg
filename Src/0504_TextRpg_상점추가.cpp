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

		cout << "TextRpg에 오신걸 환영 합니다." << endl
			<< "직업을 선택하세요" << endl
			<< "1. 전사 2. 마법사 3. 도적 4. 불러오기 5. 종료 : ";
		cin >> iInput;

		if (4 > iInput)
		{
			cout << "닉네임을 입력해주세요 : ";
			cin >> cName;
		}

		switch (iInput)
		{
		case 1:
			Create_Object(&pInfo, cName, "전사", 100, 10, 1, 0, 0);
			bSelect = true;
			break;

		case 2:
			Create_Object(&pInfo, cName, "마법사", 100, 10, 1, 0, 0);
			bSelect = true;
			break;

		case 3:
			Create_Object(&pInfo, cName, "도적", 100, 10, 1, 0, 0);
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
		strcpy_s((*ppInfo)->tItem[i].szName, sizeof((*ppInfo)->tItem[i].szName), "없음");
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
		cout << "1. 사냥터 2. 상점 3. 종료(저장) : ";
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
		<< "이름 : " << pInfo->szName << endl
		<< "직업 : " << pInfo->szJob << "\t체력 : " << pInfo->iHp << " / " << pInfo->iMaxHp << endl
		<< "레벨 : " << pInfo->iLevel << "\t공격력 : " << pInfo->iAtk << endl;

	if (bSelect)
	{
		cout << "경험치: " << pInfo->iExp << " / " << pInfo->iMaxExp << endl
			<< "골드 : " << pInfo->iMoney << endl
			<< "인벤토리 :\t";

		for (int i = 0; i < iInven; ++i)
		{
			if (!strcmp(pInfo->tItem[i].szName, "없음") || (0 == pInfo->tItem[i].iCount))
			{
				cout << "□\t";
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

		cout << "1. 초급 2. 중급 3. 고급 4. 저장하기 5. 전 단계 : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Create_Object(&pMonster, "초급 몬스터", "초급", (30 * iInput), (3 * iInput), (3 * iInput), (30 * iInput), (100 * iInput));
			break;

		case 2:
			Create_Object(&pMonster, "중급 몬스터", "중급", (30 * iInput), (3 * iInput), (3 * iInput), (30 * iInput), (100 * iInput));
			break;

		case 3:
			Create_Object(&pMonster, "고급 몬스터", "고급", (30 * iInput), (3 * iInput), (3 * iInput), (30 * iInput), (100 * iInput));
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

		cout << "1. 공격 2. 아이템사용 3. 도망 : ";
		cin >> iInput;

		if (1 == iInput) // 1. 공격 이면
		{
			// 플레이어와 몬스터 체력 계산
			pPlayer->iHp -= (pMonster->iAtk);
			pMonster->iHp -= (pPlayer->iAtk);

			// 플레이어의 체력이 0이하면 플레이어 사망
			if (0 >= pPlayer->iHp)
			{
				cout << "플레이어 사망..." << endl;
				system("pause");

				return LOSE;
			}

			// 몬스터의 체력이 0이하면 승리
			if (0 >= pMonster->iHp)
			{
				cout << "승리" << endl;

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
	pPlayer->iExp += _iExp; // 경험치 획득

	// 경험치가 100이상이면 레벨업
	if (pPlayer->iMaxExp <= pPlayer->iExp)
	{
		if (!strcmp(pPlayer->szJob, "전사"))
		{
			pPlayer->iAtk += 10;
			pPlayer->iMaxHp += 20;
			pPlayer->iMaxExp += 50;
		}
		else if (!strcmp(pPlayer->szJob, "마법사"))
		{
			pPlayer->iAtk += 25;
			pPlayer->iMaxHp += 10;
			pPlayer->iMaxExp += 50;

		}
		else if (!strcmp(pPlayer->szJob, "도적"))
		{
			pPlayer->iAtk += 15;
			pPlayer->iMaxHp += 15;
			pPlayer->iMaxExp += 50;
		}

		(pPlayer->iLevel)++;			// 레벨업
		pPlayer->iHp = pPlayer->iMaxHp; // 체력 풀 회복
		pPlayer->iExp = 0;				// 현재 경험치 0

		cout << "레벨업!" << endl;
	}
}

void	Get_Money(INFO* pPlayer, int _iMoney)
{
	pPlayer->iMoney += _iMoney;		// 돈 획득

	cout << _iMoney << "골드 획득" << endl;
}


void	Shop(INFO* pPlayer)
{
	ITEM* pItem = NULL;

	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(pPlayer, true);

		cout << endl << "상점에 오신 걸 환영합니다." << endl << endl;
		cout << "1. HP포션(300G) 4. 전 단계 : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Create_Item(&pItem, "HP포션", 20, 300, 1);
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
		pPlayer->tItem[iIndex].iCount += pItem->iCount;	// 개수 기존에서+
		pPlayer->tItem[iIndex].iMaxCount = pItem->iMaxCount;

		cout << "구매 성공" << endl;
		system("pause");

		Render(pPlayer, true);
	}
	else
	{
		cout << "소지금이 부족합니다." << endl;
		system("pause");
	}

}

void	Use_Item(INFO* pPlayer)
{
	int iInput = 0;

	cout << endl << "1. HP포션(체력 20회복)" << endl
		<< "2. 아이템 사용 취소" << endl
		<< "어떤 아이템을 사용하시겠습니까? : ";

	cin >> iInput;

	switch (iInput)
	{

	case 1:
		if ((!strcmp(pPlayer->tItem[0].szName, "HP포션")) && (0 < pPlayer->tItem[0].iCount))
		{
			pPlayer->iHp += pPlayer->tItem[0].iEffect;
			pPlayer->tItem[0].iCount -= 1;

			if (pPlayer->iHp > pPlayer->iMaxHp)
			{
				pPlayer->iHp = pPlayer->iMaxHp;
			}

			cout << "체력이 회복되었습니다." << endl;
			system("pause");

		}
		else
		{
			cout << "아이템이 없습니다." << endl;
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

	FILE* pFile = nullptr;	// 파일 스트림

	errno_t err = fopen_s(&pFile, "./Data/Save.txt", "wb");

	if (0 == err)
	{
		// 2. 파일에 쓰거나 읽기 함수를 호출
		fwrite(pPlayer, sizeof(INFO), 1, pFile);

		cout << "캐릭터 정보 저장 성공" << endl;

		fclose(pFile);

		system("pause");

		return true;
	}
	else
		cout << "캐릭터 정보 저장 실패" << endl;

	system("pause");

	return false;

}

bool	Load_Data(INFO** ppPlayer)
{
	(*ppPlayer) = new INFO;

	FILE* pFile = nullptr;		// 파일 스트림

	errno_t err = fopen_s(&pFile, "./Data/Save.txt", "rb");

	if (0 == err)
	{
		// fread(입력할 메모리의 주소, 입력할 메모리 사이즈, 입력할 메모리 개수, 개방 스트림);
		fread((*ppPlayer), sizeof(INFO), 1, pFile);

		cout << "파일 불러오기 성공 : 캐릭터 정보를 읽어옵니다..." << endl;
		fclose(pFile);

		system("pause");

		return true;
	}
	else
	{
		cout << "파일 불러오기 실패" << endl;
		system("pause");

		return false;
	}

	return false;
}