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

#pragma region �� ���� �Լ� ����
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

	FILE* pReadFile = NULL;		// ���� ��Ʈ��
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");
	// fread(�Է��� �޸��� �ּ�, �Է��� �޸� ������, �Է��� �޸� ����, ���� ��Ʈ��);

	if (0 == err)
	{
		INFO* ppInfo = new INFO;
		fread(ppInfo, sizeof(INFO), 1, pReadFile);
		fclose(pReadFile);
		pInfo = &(*ppInfo);
		cout << "���� �ҷ����� ���� : �÷��̾� ������ �о�ɴϴ�..." << endl;
		Render(pInfo);
		bSelect = true;

		system("pause");
		system("cls");

		return pInfo;
	}
	else
	{
		cout << "���� �ҷ����� ���� : ĳ���͸� �����մϴ�." << endl;
		system("pause");
		system("cls");
		bSelect = false;
	}


	while (!bSelect) {

		cout << "�г����� �Է����ּ��� : ";
		cin >> cName;

		cout << "������ �����ϼ���" << endl
			<< "1. ���� 2. ������ 3. ���� 4. ���� : ";
		cin >> iInput;

		switch (iInput)
		{
		case 1:
			//tTemp = { "����", 100, 10 };
			Create_Object(&pInfo, cName, "����", 100, 10, 1, 0);
			bSelect = true;
			break;

		case 2:
			//tTemp = { "������", 100, 10 };
			Create_Object(&pInfo, cName, "������", 100, 10, 1, 0);
			bSelect = true;
			break;

		case 3:
			//tTemp = { "����", 100, 10 };
			Create_Object(&pInfo, cName, "����", 100, 10, 1, 0);
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
		cout << "1. ����� 2. ���� : ";
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
		<< "�̸� : " << pinfo->cName << endl
		<< "���� : " << pinfo->cJob << "\t���� : " << pinfo->iLevel << "\t����ġ: " << pinfo->iExp << endl
		<< "ü�� : " << pinfo->iHp << "\t���ݷ� : " << pinfo->iAtk << endl;
}
void	Field(INFO* pPlayer)
{
	int iInput = 0;

	while (true)
	{
		system("cls");
		Render(pPlayer);
		cout << "1. �ʱ� ����� 2. �߱� ����� 3. ��� ����� 4. �� �ܰ� : ";
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
		cMonsterName = "�ʱ� ����";
		cMonsterLevle = "�ʱ�";
	}
	else if (2 == _iInput)
	{
		cMonsterName = "�߱� ����";
		cMonsterLevle = "�߱�";
	}
	else if (3 == _iInput)
	{
		cMonsterName = "��� ����";
		cMonsterLevle = "���";
	}

	Input_Array(iMyArray, iSize);

	while (true)
	{
		system("cls");
		Render(pPlayer);
		cout << endl << "\t" << "================" << cMonsterLevle << " ����� ================" << endl << endl;

		// 5 * 5 ���
		Print_Origin(iMyArray);

		cout << "q �� �ܰ�" << endl << endl
			<< "\tw ��" << endl
			<< "a ����\t\td ������" << endl
			<< "\ts �Ʒ�" << endl
			<< " : ";
		cin >> cInput;

		// 1 �ڸ� ã��.
		for (int i = 0; i < iSize; ++i)
		{
			if (8 == iMyArray[i])
				iLocation = i;
		}

		//s �Ʒ� �̵�
		if ('s' == cInput)
		{
			if (iLocation < (iSize - 5))
			{
				int iTemp = 0;

				iTemp = iMyArray[iLocation];
				iMyArray[iLocation] = iMyArray[iLocation + 5];
				iMyArray[iLocation + 5] = iTemp;

				//	��ȯ �ڸ��� ���� 15 �̻��̸� �� �ڸ��� ���� ����
				//	1���� �ڸ��� 15�̻��� �ڸ��� ��ȯ�Ǹ� ���� �߻�
				if (15 <= (iMyArray[iLocation]))
				{
					cout << "\t==============���� ����!==============" << endl;
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


		//a ���� �̵�
		if ('a' == cInput)
		{
			int iTemp = 0;
			if (0 < iLocation)
			{
				// (iLocation - 1)�� (4, 9, 14, 19, 24)�� �Ǹ� �ȵ�.
				if (4 != ((iLocation - 1) % 5))
				{
					iTemp = iMyArray[iLocation];
					iMyArray[iLocation] = iMyArray[iLocation - 1];
					iMyArray[iLocation - 1] = iTemp;

					if (15 <= (iMyArray[iLocation]))
					{
						cout << "\t==============���� ����!==============" << endl;
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

		//d ������ �̵�
		if ('d' == cInput)
		{
			int iTemp = 0;
			if (iLocation < (iSize - 1))
			{
				// (iLocation + 1)�� 5�� ����� �Ǹ� �ȵ�.(0, 5, 10, 15, 20)
				if (0 != ((iLocation + 1) % 5))
				{

					iTemp = iMyArray[iLocation];
					iMyArray[iLocation] = iMyArray[iLocation + 1];
					iMyArray[iLocation + 1] = iTemp;

					if (15 <= (iMyArray[iLocation]))
					{
						cout << "\t==============���� ����!==============" << endl;
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

		//w ���� �̵�
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
					cout << "\t==============���� ����!==============" << endl;
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
		cout << "1. ���� 2. ���� : ";
		cin >> iInput;

		if (1 == iInput) // 1. ���� �̸�
		{
			// �÷��̾�� ���� ü�� ���
			pPlayer->iHp -= (pMonster->iAtk);
			pMonster->iHp -= (pPlayer->iAtk);

			// �÷��̾��� ü���� 0���ϸ� �÷��̾� ���
			if (0 >= pPlayer->iHp)
			{
				//(_pPlayer->iHP) = 100;
				cout << endl << "�÷��̾� ���..." << endl << "����ġ - 10" << endl;
				system("pause");
				
				return LOSE;
			}

			// �÷��̾��� ü���� 0�̻��̰�, ������ ü���� 0���ϸ� �¸�
			if ((0 <= pPlayer->iHp) && (0 >= pMonster->iHp))
			{
				pPlayer->iExp += pMonster->iExp; // ����ġ ȹ��
				cout << endl << "�¸�" << endl;

				// ����ġ�� 100�̻��̸� ������
				if (100 <= pPlayer->iExp)
				{
					(pPlayer->iLevel)++;
					pPlayer->iExp -= 100;
					cout << endl << "������!" << endl;

					if (0 == (pPlayer->iLevel) % 3) // 3���� �ø������� ���ݷ� 5����
					{
						pPlayer->iAtk += 5;
						cout <<  "+ ���ݷ� 5����" << endl << endl;

						if (80 <= (pPlayer->iAtk)) // ���ݷ��� 80������ ����
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
	// ���� ��Ʈ��
	FILE* pFile = NULL;
	// 1. ���� ��Ʈ�� ����(���� ���� �ؽ�Ʈ ���)
	errno_t err = fopen_s(&pFile, "./Data/PlayerData.txt", "wb");

	if (0 == err)
	{
		//	// 2. ���Ͽ� ���ų� �б� �Լ��� ȣ��
		fwrite(pInfo->cName, sizeof(pInfo->cName), 1, pFile);
		fwrite(pInfo->cJob, sizeof(pInfo->cJob), 1, pFile);
		fwrite(&pInfo->iHp, sizeof(int), 1, pFile);
		fwrite(&pInfo->iAtk, sizeof(int), 1, pFile);
		fwrite(&pInfo->iLevel, sizeof(int), 1, pFile);
		fwrite(&pInfo->iExp, sizeof(int), 1, pFile);
		cout << "ĳ���� ���� ���� ����" << endl;
		system("pause");
		fclose(pFile);	//3. ������ �Ҹ�
	}
	else
	{
		cout << "ĳ���� ���� ���� ����" << endl;
		system("pause");
	}
}
void Load_Data(INFO* pInfo)
{
	FILE* pReadFile = NULL;		// ���� ��Ʈ��
	errno_t err = fopen_s(&pReadFile, "./Data/PlayerData.txt", "rb");
	// fread(�Է��� �޸��� �ּ�, �Է��� �޸� ������, �Է��� �޸� ����, ���� ��Ʈ��);

	if (0 == err)
	{
		INFO* ppInfo = new INFO;
		//cout << sizeof(&pInfo) << "\t" << sizeof(*ppInfo) << "\t" << endl;
		//cout << sizeof(*ppInfo) << "\t" << sizeof(pInfo) << "\t" << sizeof(&pInfo) << "\t" << sizeof(INFO) << endl;
		fread(ppInfo, sizeof(INFO), 1, pReadFile);
		fclose(pReadFile);
		pInfo = &(*ppInfo);
		cout << "���� �ҷ����� ���� : ĳ���� ������ �о�ɴϴ�..." << endl;
		Render(pInfo);

		system("pause");
		system("cls");

	}
	else
	{
		cout << "���� �ҷ����� ����" << endl;
		system("pause");
		system("cls");
	}
}


#pragma region �� ���� �Լ� ����

void Input_Array(int _iArray[], int _iSize)	// �迭�� ���� �ִ´�.
{
	for (int i = 0; i < _iSize; ++i)
	{
		_iArray[i] = i + 1;
	}
	Shuffle(_iArray); // �ڸ� ����
}
void Print_Origin(int _iArray[])	// 5 * 5 ������ ��� : 1(�÷��̾�)�� �ƴϸ� *�� ����Ѵ�.
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
int Create_Rand(void)	// ������ ����
{
	return rand() % 25 + 1;
}
void Shuffle(int _iArray[])	// �ڸ� ����
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
