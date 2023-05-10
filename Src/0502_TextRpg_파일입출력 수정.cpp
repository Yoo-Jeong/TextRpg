#include <iostream>
#include <crtdbg.h>
#include <ctime>

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

}INFO;

enum STATE
{
	LOSE,
	WIN = 1,
	RUN = 1,
	END
};

INFO*	Select_Job();
void	Create_Object(INFO** ppInfo, const char* pName, const char* pJob, int _iHp, int _iAtk, int _iLevel, int _iExp);
void	Main_Game(INFO* pPlayer);
void	Render(INFO* pinfo);
void	Field(INFO* pPlayer);
void	Move(INFO* pPlayer, int _iInput);
STATE	Battle(INFO* pPlayer, INFO* pMonster);

void	Write_Object(INFO* pInfo);
void	Load_Data(INFO* pInfo);

#pragma region 맵 관련 함수 선언
int		Create_Rand(void);
void	Input_Array(int _iArray[], int _iSize);
void	Shuffle(int _iArray[]);
void	Print_Origin(int _iArray[]);
#pragma endregion

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

INFO*	Select_Job()
{
	INFO* pInfo = nullptr;
	char	cName[32];

	int		iInput = 0;
	bool	bSelect = false;

	FILE* pReadFile = NULL;		// 파일 스트림
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");
	// fread(입력할 메모리의 주소, 입력할 메모리 사이즈, 입력할 메모리 개수, 개방 스트림);

	if (0 == err)
	{
		INFO* ppInfo = new INFO;
		fread(ppInfo, sizeof(INFO), 1, pReadFile);
		fclose(pReadFile);
		pInfo = &(*ppInfo);
		cout << "파일 불러오기 성공 : 플레이어 정보를 읽어옵니다..." << endl;
		Render(pInfo);
		bSelect = true;

		system("pause");
		system("cls");

		return pInfo;
	}
	else
	{
		cout << "파일 불러오기 실패 : 캐릭터를 생성합니다." << endl;
		system("pause");
		system("cls");
		bSelect = false;
	}


	while (!bSelect) {

		cout << "닉네임을 입력해주세요 : ";
		cin >> cName;

		cout << "직업을 선택하세요" << endl
			<< "1. 전사 2. 마법사 3. 도적 4. 종료 : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			//tTemp = { "전사", 100, 10 };
			Create_Object(&pInfo, cName, "전사", 100, 10, 1, 0);
			bSelect = true;
			break;

		case 2:
			//tTemp = { "마법사", 100, 10 };
			Create_Object(&pInfo, cName, "마법사", 100, 10, 1, 0);
			bSelect = true;
			break;

		case 3:
			//tTemp = { "도적", 100, 10 };
			Create_Object(&pInfo, cName, "도적", 100, 10, 1, 0);
			bSelect = true;
			break;

		case 4:
			bSelect = true;
			break;
		}
	}
	Write_Object(pInfo);
	return pInfo;
}
void	Create_Object(INFO** ppInfo, const char* pName, const char* pJob, int _iHp, int _iAtk, int _iLevel, int _iExp)
{
	*ppInfo = new INFO;
	strcpy_s((*ppInfo)->cName, sizeof((*ppInfo)->cName), pName);
	strcpy_s((*ppInfo)->cJob, sizeof((*ppInfo)->cJob), pJob);
	(*ppInfo)->iHp = _iHp;
	(*ppInfo)->iAtk = _iAtk;
	(*ppInfo)->iLevel = _iLevel;
	(*ppInfo)->iExp = _iExp;
}
void	Main_Game(INFO* pPlayer)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(pPlayer);
		cout << "1. 사냥터 2. 종료 : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			Field(pPlayer);
			break;
		case 2:
			Write_Object(pPlayer);
			return;
		case 3:
			return;

		default:
			break;
		}
	}
}
void	Render(INFO* pinfo)
{
	cout << "===========================================" << endl
		<< "이름 : " << pinfo->cName << endl
		<< "직업 : " << pinfo->cJob << "\t레벨 : " << pinfo->iLevel << "\t경험치: " << pinfo->iExp << endl
		<< "체력 : " << pinfo->iHp << "\t공격력 : " << pinfo->iAtk << endl;
}
void	Field(INFO* pPlayer)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(pPlayer);
		cout << "1. 초급 사냥터 2. 중급 사냥터 3. 고급 사냥터 4. 전 단계 : ";
		cin >> iInput;

		if ((0 >= iInput) || (4 < iInput))
			continue;

		else if (4 == iInput)
			return;

		else if (4 > iInput)
		{
			Move(pPlayer, iInput);
		}
	}
}
void	Move(INFO* pPlayer, int _iInput)
{
	int			iMyArray[25] = {};
	int			iSize = (sizeof(iMyArray)) / (sizeof(int));
	int			iLocation = 0;

	char		cInput = 'z';
	const char*	cMonsterLevle = "";
	const char* cMonsterName = "";

	STATE eState;

	INFO*	pMonster = NULL;

	if (1 == _iInput)
	{
		cMonsterName = "초급 몬스터";
		cMonsterLevle = "초급";
	}
	else if (2 == _iInput)
	{
		cMonsterName = "중급 몬스터";
		cMonsterLevle = "중급";
	}
	else if (3 == _iInput)
	{
		cMonsterName = "고급 몬스터";
		cMonsterLevle = "고급";
	}

	Input_Array(iMyArray, iSize);

	while (true)
	{
		system("cls");
		Render(pPlayer);
		cout << endl << "\t" << "================" << cMonsterLevle << " 사냥터 ================" << endl << endl;

		// 5 * 5 출력
		Print_Origin(iMyArray);

		cout << "q 전 단계" << endl << endl
			<< "\tw 위" << endl
			<< "a 왼쪽\t\td 오른쪽" << endl
			<< "\ts 아래" << endl
			<< " : ";
		cin >> cInput;

		// 1 자리 찾기.
		for (int i = 0; i < iSize; ++i)
		{
			if (8 == iMyArray[i])
				iLocation = i;
		}

		//s 아래 이동
		if ('s' == cInput)
		{
			if (iLocation < (iSize - 5))
			{
				int iTemp = 0;

				iTemp = iMyArray[iLocation];
				iMyArray[iLocation] = iMyArray[iLocation + 5];
				iMyArray[iLocation + 5] = iTemp;

				//	교환 자리의 값이 15 이상이면 그 자리에 몬스터 생성
				//	1값의 자리와 15이상인 자리와 교환되면 전투 발생
				if (15 <= (iMyArray[iLocation]))
				{
					cout << "\t==============몬스터 등장!==============" << endl;
					system("pause");

					Create_Object(&pMonster, cMonsterName, cMonsterLevle, (30 * _iInput), (3 * _iInput), (3 * _iInput), (30 * _iInput));
					eState = Battle(pPlayer, pMonster);

					if (LOSE == eState)
					{
						pPlayer->iHp = 100;

						if (10 <= (pPlayer->iExp))
							pPlayer->iExp -= 10;
						else
							pPlayer->iExp = 0;

						return;
					}

					if (RUN == eState || WIN == eState)
					{
						if (NULL != pMonster)
						{
							delete pMonster;
							pMonster = NULL;
						}
					}
					iMyArray[iLocation] = 0;
				}

			}
		}


		//a 왼쪽 이동
		if ('a' == cInput)
		{
			int iTemp = 0;
			if (0 < iLocation)
			{
				// (iLocation - 1)이 (4, 9, 14, 19, 24)가 되면 안됨.
				if (4 != ((iLocation - 1) % 5))
				{
					iTemp = iMyArray[iLocation];
					iMyArray[iLocation] = iMyArray[iLocation - 1];
					iMyArray[iLocation - 1] = iTemp;

					if (15 <= (iMyArray[iLocation]))
					{
						cout << "\t==============몬스터 등장!==============" << endl;
						system("pause");

						Create_Object(&pMonster, cMonsterName, cMonsterLevle, (30 * _iInput), (3 * _iInput), (3 * _iInput), (30 * _iInput));
						eState = Battle(pPlayer, pMonster);

						if (LOSE == eState)
						{
							pPlayer->iHp = 100;

							if (10 <= (pPlayer->iExp))
								pPlayer->iExp -= 10;
							else
								pPlayer->iExp = 0;

							return;
						}

						if (RUN == eState || WIN == eState)
						{
							if (NULL != pMonster)
							{
								delete pMonster;
								pMonster = NULL;
							}
						}
						iMyArray[iLocation] = 0;
					}
				}
			}
		}

		//d 오른쪽 이동
		if ('d' == cInput)
		{
			int iTemp = 0;
			if (iLocation < (iSize - 1))
			{
				// (iLocation + 1)이 5의 배수가 되면 안됨.(0, 5, 10, 15, 20)
				if (0 != ((iLocation + 1) % 5))
				{

					iTemp = iMyArray[iLocation];
					iMyArray[iLocation] = iMyArray[iLocation + 1];
					iMyArray[iLocation + 1] = iTemp;

					if (15 <= (iMyArray[iLocation]))
					{
						cout << "\t==============몬스터 등장!==============" << endl;
						system("pause");

						Create_Object(&pMonster, cMonsterName, cMonsterLevle, (30 * _iInput), (3 * _iInput), (3 * _iInput), (30 * _iInput));
						eState = Battle(pPlayer, pMonster);

						if (LOSE == eState)
						{
							pPlayer->iHp = 100;

							if (10 <= (pPlayer->iExp))
								pPlayer->iExp -= 10;
							else
								pPlayer->iExp = 0;

							return;
						}

						if (RUN == eState || WIN == eState)
						{
							if (NULL != pMonster)
							{
								delete pMonster;
								pMonster = NULL;
							}
						}
						iMyArray[iLocation] = 0;
					}
				}
			}
		}

		//w 위로 이동
		if ('w' == cInput)
		{
			int iTemp = 0;
			if (0 < iLocation)
			{

				iTemp = iMyArray[iLocation];
				iMyArray[iLocation] = iMyArray[iLocation - 5];
				iMyArray[iLocation - 5] = iTemp;

				if (15 <= (iMyArray[iLocation]))
				{
					cout << "\t==============몬스터 등장!==============" << endl;
					system("pause");

					Create_Object(&pMonster, cMonsterName, cMonsterLevle, (30 * _iInput), (3 * _iInput), (3 * _iInput), (30 * _iInput));
					eState = Battle(pPlayer, pMonster);

					if (LOSE == eState)
					{
						pPlayer->iHp = 100;

						if (10 <= (pPlayer->iExp))
							pPlayer->iExp -= 10;
						else
							pPlayer->iExp = 0;

						return;
					}

					if (RUN == eState || WIN == eState)
					{
						if (NULL != pMonster)
						{
							delete pMonster;
							pMonster = NULL;
						}
					}
					iMyArray[iLocation] = 0;
				}
			}
		}

		if ('q' == cInput)
			return;
	}
}
STATE	Battle(INFO* pPlayer, INFO* pMonster)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(pPlayer);
		Render(pMonster);
		cout << "1. 공격 2. 도망 : ";
		cin >> iInput;

		if (1 == iInput) // 1. 공격 이면
		{
			// 플레이어와 몬스터 체력 계산
			pPlayer->iHp -= (pMonster->iAtk);
			pMonster->iHp -= (pPlayer->iAtk);

			// 플레이어의 체력이 0이하면 플레이어 사망
			if (0 >= pPlayer->iHp)
			{
				//(_pPlayer->iHP) = 100;
				cout << endl << "플레이어 사망..." << endl << "경험치 - 10" << endl;
				system("pause");
				
				return LOSE;
			}

			// 플레이어의 체력이 0이상이고, 몬스터의 체력이 0이하면 승리
			if ((0 <= pPlayer->iHp) && (0 >= pMonster->iHp))
			{
				pPlayer->iExp += pMonster->iExp; // 경험치 획득
				cout << endl << "승리" << endl;

				// 경험치가 100이상이면 레벨업
				if (100 <= pPlayer->iExp)
				{
					(pPlayer->iLevel)++;
					pPlayer->iExp -= 100;
					cout << endl << "레벨업!" << endl;

					if (0 == (pPlayer->iLevel) % 3) // 3레벨 올릴때마다 공격력 5증가
					{
						pPlayer->iAtk += 5;
						cout <<  "+ 공격력 5증가" << endl << endl;

						if (80 <= (pPlayer->iAtk)) // 공격력은 80까지만 증가
						{
							pPlayer->iAtk = 80;
						}
					}

				}
				system("pause");
				return WIN;
			}
		}
		else if (2 == iInput)
			return RUN;
	}
}

void Write_Object(INFO* pInfo)
{
	// 파일 스트림
	FILE* pFile = NULL;
	// 1. 파일 스트림 생성(쓰기 전용 텍스트 모드)
	errno_t err = fopen_s(&pFile, "./Data/PlayerData.txt", "wb");

	if (0 == err)
	{
		//	// 2. 파일에 쓰거나 읽기 함수를 호출
		fwrite(pInfo->cName, sizeof(pInfo->cName), 1, pFile);
		fwrite(pInfo->cJob, sizeof(pInfo->cJob), 1, pFile);
		fwrite(&pInfo->iHp, sizeof(int), 1, pFile);
		fwrite(&pInfo->iAtk, sizeof(int), 1, pFile);
		fwrite(&pInfo->iLevel, sizeof(int), 1, pFile);
		fwrite(&pInfo->iExp, sizeof(int), 1, pFile);
		cout << "캐릭터 정보 저장 성공" << endl;
		system("pause");
		fclose(pFile);	//3. 파일을 소멸
	}
	else
	{
		cout << "캐릭터 정보 저장 실패" << endl;
		system("pause");
	}
}
void Load_Data(INFO* pInfo)
{
	FILE* pReadFile = NULL;		// 파일 스트림
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");
	// fread(입력할 메모리의 주소, 입력할 메모리 사이즈, 입력할 메모리 개수, 개방 스트림);

	if (0 == err)
	{
		INFO* ppInfo = new INFO;
		//cout << sizeof(&pInfo) << "\t" << sizeof(*ppInfo) << "\t" << endl;
		//cout << sizeof(*ppInfo) << "\t" << sizeof(pInfo) << "\t" << sizeof(&pInfo) << "\t" << sizeof(INFO) << endl;
		fread(ppInfo, sizeof(INFO), 1, pReadFile);
		fclose(pReadFile);
		pInfo = &(*ppInfo);
		cout << "파일 불러오기 성공 : 캐릭터 정보를 읽어옵니다..." << endl;
		Render(pInfo);

		system("pause");
		system("cls");

	}
	else
	{
		cout << "파일 불러오기 실패" << endl;
		system("pause");
		system("cls");
	}
}


#pragma region 맵 관련 함수 구현

void Input_Array(int _iArray[], int _iSize)	// 배열에 값을 넣는다.
{
	for (int i = 0; i < _iSize; ++i)
	{
		_iArray[i] = i + 1;
	}
	Shuffle(_iArray); // 자리 섞기
}
void Print_Origin(int _iArray[])	// 5 * 5 사이즈 출력 : 1(플레이어)가 아니면 *로 출력한다.
{
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int iIndex = (i * 5) + j;

			if (0 == (_iArray[iIndex]))
			{
				cout << "\t" << "    " << "0";
			}
			else if (8 != (_iArray[iIndex]))
			{
				cout << "\t" << "    " << "*";
				//cout << "\t" << "    " << (_iArray[iIndex]);
			}
			else
			{
				cout << "\t" << "    " << (_iArray[iIndex]);
			}
		}
		cout << endl << endl;
	}
}
int Create_Rand(void)	// 랜덤값 리턴
{
	return rand() % 25 + 1;
}
void Shuffle(int _iArray[])	// 자리 섞기
{
	for (int i = 0; i < 50; ++i)
	{
		int iDst = Create_Rand();
		int iSrc = Create_Rand();

		int iTemp = _iArray[iDst - 1];
		_iArray[iDst - 1] = _iArray[iSrc - 1];
		_iArray[iSrc - 1] = iTemp;
	}
}

#pragma endregion
