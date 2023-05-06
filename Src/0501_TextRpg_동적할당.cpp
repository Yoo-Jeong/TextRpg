#include <iostream>

#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

using namespace std;

typedef struct INFO
{
	char cName[32];
	char cJob[32];
	int iHp;
	int iAtk;
	int iLevel;
	int iExp;
	int iDeath;

}INFO, * PINFO;


INFO Select_Job();
void Render(INFO* _pinfo);
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
		system("cls");
		Render(&tPlayer);
		cout << "1. 사냥터 2. 종료 : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Field(&tPlayer);
			break;
		case 2:
			return;

		default:
			break;
		}
	}
}

void Render(INFO* _pStat)
{
	cout << "===========================================" << endl
		<< "이름 : " << _pStat->cName << endl
		<< "직업 : " << _pStat->cJob << "\t레벨 : " << _pStat->iLevel << "\t경험치: "<< _pStat->iExp << endl
		<< "체력 : " << _pStat->iHp << "\t공격력 : " << _pStat->iAtk << endl;
}

INFO Select_Job()
{
	INFO tTemp = {};
	char cName[32];

	int iInput = 0;

	cout << "닉네임을 입력해주세요 : ";
	cin >> cName;
	strcpy_s(tTemp.cName, sizeof(tTemp.cName), cName);

	cout << "직업을 선택하세요(1. 전사 2. 마법사 3. 도적) : ";
	cin >> iInput;

	switch (iInput)
	{
	case 1:
		//tTemp = { "전사", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "전사");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	case 2:
		//tTemp = { "마법사", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "마법사");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	case 3:
		//tTemp = { "도적", 100, 10 };
		strcpy_s(tTemp.cJob, sizeof(tTemp.cJob), "도적");
		tTemp.iHp = 100;
		tTemp.iAtk = 10;
		break;

	}

	tTemp.iLevel = 1;
	tTemp.iExp = 0;
	tTemp.iDeath = 0;

	return tTemp;
}


void Field(INFO* _pPlayer)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		cout << "1. 초급 2. 중급 3. 고급 4. 전 단계 : ";
		cin >> iInput;

		if ((0 >= iInput) || (4 < iInput))
			continue;

		else if (4 == iInput)
			return;

		else if (4 > iInput)
		{
			PINFO pMonster = new INFO[3]{};
			Create_Object(pMonster);
			Battle(_pPlayer, &pMonster[iInput - 1]);

			delete[] pMonster;		// 동적 배열에 해당하는 heap공간을 모두 해제
			pMonster = nullptr;
		}
	}
}

void Create_Object(INFO _pMonster[])
{
	strcpy_s(_pMonster[0].cName, sizeof(_pMonster[0].cName), "초급 몬스터");
	strcpy_s(_pMonster[1].cName, sizeof(_pMonster[1].cName), "중급 몬스터");
	strcpy_s(_pMonster[2].cName, sizeof(_pMonster[2].cName), "고급 몬스터");

	strcpy_s(_pMonster[0].cJob, sizeof(_pMonster[0].cJob), "초급");
	strcpy_s(_pMonster[1].cJob, sizeof(_pMonster[1].cJob), "중급");
	strcpy_s(_pMonster[2].cJob, sizeof(_pMonster[2].cJob), "고급");

	for (int i = 0; i < 3; ++i)
	{
		_pMonster[i].iHp = (1 + i) * 30;
		_pMonster[i].iAtk = (1 + i) * 3;
		_pMonster[i].iLevel = (1 + i) * 3;
		_pMonster[i].iExp = (1 + i) * 30;
		_pMonster[i].iDeath = 0;
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
		cout << "1. 공격 2. 도망 : ";
		cin >> iInput;

		if (1 == iInput) // 1. 공격 이면
		{
			// 플레이어와 몬스터 체력 계산
			_pPlayer->iHp -= (_pMonster->iAtk);
			_pMonster->iHp -= (_pPlayer->iAtk);

			// 플레이어의 체력이 0이하면 플레이어 사망
			if (0 >= _pPlayer->iHp)
			{
				(_pPlayer->iHp) = 100;
				cout << endl << "플레이어 사망" << endl;
				system("pause");
				break;
			}

			// 플레이어의 체력이 0이상이고, 몬스터의 체력이 0이하면 승리
			if ((0 <= _pPlayer->iHp) && (0 >= _pMonster->iHp))
			{
				_pPlayer->iExp += _pMonster->iExp; // 경험치 획득
				cout << endl << "승리" << endl;

				// 경험치가 100이상이면 레벨업
				if (100 <= _pPlayer->iExp)
				{
					(_pPlayer->iLevel)++; 
					_pPlayer->iExp -= 100;
					cout << endl << "레벨업!" << endl;
				}
				system("pause");
				break;
			}
		}
		else if (2 == iInput)
			return;
	}
}


