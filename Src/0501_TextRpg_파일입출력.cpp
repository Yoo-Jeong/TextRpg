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

}INFO, * PINFO;

int Create_Rand(void);
void Shuffle(int _iArray[]);
void Input_Array(int _iArray[], int _iSize); // 배열에 값을 넣는다.
void Print_Origin(int _iArray[]); // 5 * 5 출력
void Move(INFO* _pPlayer, int _iInput);

INFO Select_Job();
void Render(INFO* _pinfo);
void Field(INFO* _pPlayer);
void Create_Object(INFO _pMonster[]);
void Battle(INFO* _pPlayer, INFO* _pMonster);

void Write_Object(INFO* _tObject);
void Read_Object(INFO* _tObject);

void main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	srand(unsigned(time(NULL)));

	INFO tPlayer = {};
	int iInput = 0;

	FILE* pReadFile = NULL;		// 파일 스트림
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");

	if (0 == err)
	{
		// fread(입력할 메모리의 주소, 입력할 메모리 사이즈, 입력할 메모리 개수, 개방 스트림);
		fread(&tPlayer, sizeof(tPlayer), 1, pReadFile);
		fclose(pReadFile);
		cout << "파일 불러오기 성공 : 캐릭터 정보를 읽어옵니다..." << endl;
		Render(&tPlayer);
		system("pause");

	}
	else
	{
		cout << "파일 불러오기 실패 : 캐릭터를 생성합니다." << endl;
		system("pause");
		tPlayer = Select_Job();
	}

	//Read_Object(&tPlayer);

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
			Write_Object(&tPlayer);
			return;
		case 3: 

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
		<< "직업 : " << _pStat->cJob << "\t레벨 : " << _pStat->iLevel << "\t경험치: " << _pStat->iExp << endl
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

	Write_Object(&tTemp);

	return tTemp;
}


void Field(INFO* _pPlayer)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		cout << "1. 초급 사냥터 2. 중급 사냥터 3. 고급 사냥터 4. 전 단계 : ";
		cin >> iInput;

		if ((0 >= iInput) || (4 < iInput))
			continue;

		else if (4 == iInput)
			return;

		else if (4 > iInput)
		{
			Move(_pPlayer, iInput);
			/*		
			PINFO pMonster = new INFO[3]{};
			Create_Monster(pMonster);
			Battle(_pPlayer, &pMonster[iInput - 1]);

			delete[] pMonster;		// 동적 배열에 해당하는 heap공간을 모두 해제
			pMonster = nullptr;
			*/
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
				//(_pPlayer->iHP) = 100;
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


void Move(INFO* _pPlayer, int _iInput)
{
	int iMyArray[25] = {};
	int iSize = (sizeof(iMyArray)) / (sizeof(int));
	int iLocation = 0;

	char iInput = 'z';

	Input_Array(iMyArray, iSize);

	while (true)
	{
		system("cls");
		Render(_pPlayer);
		cout << endl << "\t================= 사냥터 =================" << endl << endl;

		// 5 * 5 출력
		Print_Origin(iMyArray);
	
		cout << "q 전 단계" << endl << endl
			<< "\tw 위" << endl
			<< "a 왼쪽\t\td 오른쪽" << endl
			<< "\ts 아래" << endl
			<< " : ";
		cin >> iInput;
	
		// 1 자리 찾기.
		for (int i = 0; i < iSize; ++i)
		{
			if (8 == iMyArray[i])
				iLocation = i;
		}

		//2번 아래 이동
		if ('s' == iInput)
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

					PINFO pMonster = new INFO[3]{};
					Create_Object(pMonster);
					Battle(_pPlayer, &pMonster[_iInput - 1]);

					delete[] pMonster;		// 동적 배열에 해당하는 heap공간을 모두 해제
					pMonster = nullptr;

					iMyArray[iLocation] = 0;

					if (0 >= _pPlayer->iHp)
					{
						(_pPlayer->iHp) = 100;
						return;
					}
				}

			}
		}


		//4번 왼쪽 이동
		if ('a' == iInput)
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
						PINFO pMonster = new INFO[3]{};
						Create_Object(pMonster);
						Battle(_pPlayer, &pMonster[_iInput - 1]);

						delete[] pMonster;
						pMonster = nullptr;

						iMyArray[iLocation] = 0;

						if (0 >= _pPlayer->iHp)
						{
							(_pPlayer->iHp) = 100;
							return;
						}
					}
				}
			}
		}

		//6번 오른쪽 이동
		if ('d' == iInput)
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
						PINFO pMonster = new INFO[3]{};
						Create_Object(pMonster);
						Battle(_pPlayer, &pMonster[_iInput - 1]);

						delete[] pMonster;
						pMonster = nullptr;

						iMyArray[iLocation] = 0;

						if (0 >= _pPlayer->iHp)
						{
							(_pPlayer->iHp) = 100;
							return;
						}
					}
				}
			}
		}

		//8번 위로 이동
		if ('w' == iInput)
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
					PINFO pMonster = new INFO[3]{};
					Create_Object(pMonster);
					Battle(_pPlayer, &pMonster[_iInput - 1]);

					delete[] pMonster;
					pMonster = nullptr;

					iMyArray[iLocation] = 0;

					if (0 >= _pPlayer->iHp)
					{
						(_pPlayer->iHp) = 100;
						return;
					}
				}
			}
		}

		if ('q' == iInput)
			return;
	}
}


// 배열에 값을 넣는다.
void Input_Array(int _iArray[], int _iSize)
{
	for (int i = 0; i < _iSize; ++i)
	{
		_iArray[i] = i + 1;
	}
	Shuffle(_iArray); // 자리 섞기
}

// 5 * 5 사이즈 출력 : 1(플레이어)가 아니면 *로 출력한다.
void Print_Origin(int _iArray[])
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

// 랜덤값 리턴
int Create_Rand(void)
{
	return rand() % 25 + 1;
}

// 자리 섞기
void Shuffle(int _iArray[])
{
	for (int i = 0; i < 50; ++i)
	{
		int iDst = Create_Rand();
		int iSrc = Create_Rand();

		int iTemp = _iArray[iDst - 1];
		_iArray[iDst- 1] = _iArray[iSrc-1];
		_iArray[iSrc-1] = iTemp;
	}
}

void Write_Object(INFO* _Object)
{
	// 파일 스트림
	FILE* pFile = NULL;
	// 1. 파일 스트림 생성(쓰기 전용 텍스트 모드)
	errno_t err = fopen_s(&pFile, "./Data/PlayerData.txt", "wb");

	if (0 == err)
	{
		//	// 2. 파일에 쓰거나 읽기 함수를 호출
		fwrite(_Object->cName, sizeof(_Object->cName), 1, pFile);
		fwrite(_Object->cJob, sizeof(_Object->cJob), 1, pFile);
		fwrite(&_Object->iHp, sizeof(int), 1, pFile);
		fwrite(&_Object->iAtk, sizeof(int), 1, pFile);
		fwrite(&_Object->iLevel, sizeof(int), 1, pFile);
		fwrite(&_Object->iExp, sizeof(int), 1, pFile);
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

void Read_Object(INFO* _tObject)
{
	FILE* pReadFile = NULL;		// 파일 스트림
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");
	// fread(입력할 메모리의 주소, 입력할 메모리 사이즈, 입력할 메모리 개수, 개방 스트림);

	if (0 == err)
	{
		fread(&_tObject, sizeof(_tObject), 1, pReadFile);
		fclose(pReadFile);
		cout << "파일 불러오기 성공" << endl;
		Render(_tObject);
		system("pause");

	}
	else
	{
		cout << "파일 불러오기 실패" << endl;
		system("pause");
	}
}
